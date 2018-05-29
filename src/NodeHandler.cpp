#include <QDebug>
#include <cstddef>
#include <iostream>
#include <unistd.h>

#include <uavcan/protocol/param/GetSet.hpp>
#include <uavcan/protocol/param/ExecuteOpcode.hpp>

#include "NodeHandler.h"
#include "PlatformLinux.h"
#include "CanWorker.h"

const uavcan::NodeStatusProvider::NodeName DEFAULT_NODE_NAME = "morus.can.node";

/**
 * Convenience function for blocking service calls.
 */
template <typename T>
std::pair<int, typename T::Response> performBlockingServiceCall(
		uavcan::INode& node,
        uavcan::NodeID remote_node_id,
        const typename T::Request& request)
{
    bool success = false;

    // Generated types have zero-initializing constructors, always.
    typename T::Response response;

    uavcan::ServiceClient<T> client(node);
    client.setCallback([&](const uavcan::ServiceCallResult<T>& result)
        {
            success = result.isSuccessful();
            response = result.getResponse();
        });

    const int call_res = client.call(remote_node_id, request);
    qDebug() << "performBlockingServiceCall() - "
    		"Call res: " << call_res;
    if (call_res >= 0)
    {
        while (client.hasPendingCalls())
        {
            const int spin_res = node.spin(
            		uavcan::MonotonicDuration::fromMSec(10));

            qDebug() << "performBlockingServiceCall() - "
            		"Spin res: " << spin_res;
            if (spin_res < 0)
            {
                return {spin_res, response};
            }
        }
        qDebug() << "performBlockingServiceCall() - "
        		"Success res: " << success;
        return {success ? 0 : -uavcan::ErrFailure, response};
    }
    return {call_res, response};
}

NodeHandler::NodeHandler(CanWorker& worker)
{
	this->canWorker_ = &worker;
	this->opcodeClient_ = NULL;
}

NodeHandler::~NodeHandler()
{
	qDebug() << "NodeHandler::~NodeHandler() "
			"- destructor called.";

	delete canNode_;
	delete fileServer_;
	delete fileServerBackend_;
	delete updateNodeInfoRetriever_;
	delete updateTrigger_;
}

int NodeHandler::createNewNode(std::string ifaceName, int nodeID)
{
	qDebug() << "NodeHandler::createNewNode() "
			"- Started node creation.";

	/*
     * Node initialization.
     * Node ID and name are required; otherwise, the node will refuse to start.
     * Version info is optional.
     */
	canNode_ = &getCanNode(ifaceName);
	canNode_->setNodeID(nodeID);
	canNode_->setName(DEFAULT_NODE_NAME);

	qDebug() << "NodeHandler::createNewNode() "
			"- Node successfully created.";

	/*
     * Start the node.
     * All returnable error codes are listed in the
     * header file uavcan/error.hpp.
     */
    const int nodeStartRes = canNode_->start();
    if (nodeStartRes < 0)
    {
    	qDebug() << "NodeHandler::createNewNode() "
    			"- Unable to start the CAN node.";
        return nodeStartRes;
    }

    /*
	 * Setup this node as a file server.
	 */
	setupNodeFileServer();

	// Initialize opcode client
	opcodeClient_ = new uavcan::ServiceClient
			<param_ns::ExecuteOpcode>(*canNode_);
	const int opcodeRes = opcodeClient_->init();
	if (opcodeRes < 0)
	{
		qDebug() << "NodeHandler::createNewNode() - Failed to initialize "
				"opcode client";
		return opcodeRes;
	}

	// Initialize hardware reset client
	restartClient_ = new uavcan::ServiceClient
			<uavcan::protocol::RestartNode>(*canNode_);
	const int res = restartClient_->init();
	if (res < 0)
	{
		qDebug() << "NodeHandler::createNewNode() - Failed to initialize "
				"restart client.";
	}
	// TODO (lmark): Setup callback and tmeout

    /*
     * Informing other nodes that we're ready to work.
     * Default mode is INITIALIZING.
     */
    canNode_->setModeOperational();
    nodeWorking_ = true;

    qDebug() << "NodeHandler::createNewNode() "
    		"- Successfully created the node.";

    return nodeStartRes;
}

int NodeHandler::spinCurrentNode(int timeout_ms)
{
	/*
	 * If there's nothing to do, the thread blocks inside the driver's
	 * method select() until the timeout expires or an error occurs
	 * All error codes are listed in the header uavcan/error.hpp.
	 */

	// If node is not created return -1
	if (!nodeWorking_)
	{
		qDebug() << "NodeHandler::spinCurrentNode() "
				"- Unable to spin node. No longer working.\n";
		return -1;
	}

	/*
	 * No longer using this nod to collect information about other
	 * nodes.
	 */

	//qDebug() << "NodeHandler::spinCurrentNode() "
	//			"- spin node.\n";
	// Spin the node
	int res = canNode_->spin(
			uavcan::MonotonicDuration::fromMSec(timeout_ms));

	// Read all parameters if requested
	if (readParametersFlag_) { readAllParameters(); }

	// Update parameters if requested
	if (updateParametersFlag_) { updateParameters(); }

	// Store parameters if requested
	if (storeParametersFlag_) { storeParameters(); }

	// Erase parameters if requested
	if (eraseParametersFlag_) { eraseParameters(); }

	// Restart node request
	if (restarNodeFlag_) { restartNode(); }

	return res;
}

void NodeHandler::setupNodeFileServer()
{
	// Try starting the node info retriever
	qDebug() << "NodeHandler::setupNodeFileServer() "
			"- Starting node info retriever.";
	updateNodeInfoRetriever_ = new NodeInfoRetriever(*canNode_);
	const int recieverRes = updateNodeInfoRetriever_->start();
	if (recieverRes < 0)
	{
		throw std::runtime_error(
				"Failed to start the node info retriever: "
				+ std::to_string(recieverRes));
	}

	// Initialize and start the update trigger
	qDebug() << "NodeHandler::setupNodeFileServer() "
			"- Starting versionChecker / trigger.";
	versionChecker_ = new CanFirmwareVersionChecker();
	updateTrigger_ = new FirmwareUpdateTrigger(*canNode_, *versionChecker_);
	const int triggerRes = updateTrigger_->start(*updateNodeInfoRetriever_);
	if (triggerRes < 0)
	{
		throw std::runtime_error("Failed to start the firmware update trigger:"
				+ std::to_string(triggerRes));
	}

	/*
	 * Initializing the file server.
	 * It is not necessary to run the file server on the same node
	 * with the firmware update trigger
	 * (this is explained in the specification),
	 * but this use case is the most common case.
	 */
	qDebug() << "NodeHandler::setupFileServer "
			"- Starting server / backend.";
	fileServerBackend_ = new uavcan_posix::BasicFileServerBackend(*canNode_);
	fileServer_ = new FileServer(*canNode_, *fileServerBackend_);
	const int fileServerRes = fileServer_->start();
	if (fileServerRes < 0)
	{
	   throw std::runtime_error(
			   "Failed to start the file server: "
			   + std::to_string(fileServerRes));
	}
}

void NodeHandler::destroyCurrentNode()
{
	qDebug() << "NodeHandler::destroyCurrentNode()"
			"- Current node is destroyed.";
	canNode_ = NULL;
}

void NodeHandler::stopCurrentNode()
{
	qDebug() << "NodeHandler::stopCurrentNode() "
			"- Current node is stopped.";
	nodeWorking_ = false;
}

CustomNode_t& getCanNode(std::string iface_name)
{
    static CustomNode_t node(getCanDriver(iface_name), getSystemClock());
    return node;
}


void NodeHandler::readAllParameters()
{
	/*
	 * Reading all params from the remote node (access by index);
	 * printing request/response to stdout in YAML format.
	 * Note that access by index should be used only to list params,
	 * not to get or set them.
	 */
	readParametersFlag_ = false;
	std::vector<param_ns::GetSet::Response> remoteParams;

	qDebug() << "NodeHandler::readAllParameters() - "
				"start reading parameters.";
	while (true)
	{
		qDebug() << "NodeHandler::readAllParameters() - "
				"do request.";
		param_ns::GetSet::Request paramRequest;
		paramRequest.index = remoteParams.size();
		std::cout << "Param GET request:\n" << paramRequest
				<< std::endl << std::endl;
		auto res = performBlockingServiceCall
				<param_ns::GetSet>(
						*canNode_,
						externalNodeId_,
						paramRequest);
		cout << res.first << endl;
		if (res.first < 0)
		{
			throw std::runtime_error(
					"Failed to get param: " + std::to_string(res.first));
		}

		// Empty name means no such param, which means we're finished
		if (res.second.name.empty())
		{
			std::cout << "Param read done!\n\n" << std::endl;
			break;
		}

		std::cout << "Response:\n" << res.second << std::endl << std::endl;
		        remoteParams.push_back(res.second);
	}
	qDebug() << "NodeHandler::readAllParameters() - "
				"finished reading parameters.";

	// Start propagating parameters towards CanWorker -> MorusMainWindow
	canWorker_->updateParametersCallback(remoteParams);

	// Reset read parameter flags
	externalNodeId_ = -1;
}

void NodeHandler::storeParameters()
{
	storeParametersFlag_ = false;

	// Store parameters
	param_ns::ExecuteOpcode::Request storeRequest;
	storeRequest.opcode = param_ns::ExecuteOpcode::Request::OPCODE_SAVE;

	int callRes = opcodeClient_->call(externalNodeId_, storeRequest);
	if (callRes < 0)
	{
		qDebug() << "NodeHandler::storeParameters() - Request call failed.";
		return;
	}

	qDebug() << "NodeHandler::storeParameters() - Parameters stored.";
}

void NodeHandler::eraseParameters()
{
	eraseParametersFlag_ = false;

	param_ns::ExecuteOpcode::Request eraseRequest;
	eraseRequest.opcode = param_ns::ExecuteOpcode::Request::OPCODE_ERASE;

	int callRes = opcodeClient_->call(externalNodeId_, eraseRequest);
	if (callRes < 0)
	{
		qDebug() << "NodeHandler::eraseParameters() - Erase call failed.";
		return;
	}

	qDebug() << "NodeHandler::eraseParameters() - Parameters erased.";
}

void NodeHandler::restartNode()
{
	restarNodeFlag_ = false;

	 uavcan::protocol::RestartNode::Request restartRequest;
	 restartRequest.magic_number = restartRequest.MAGIC_NUMBER;
	 const int restartRes = restartClient_->
			 call(externalNodeId_, restartRequest);
	 if (restartRes < 0)
	 {
		 qDebug() << "NodeHandler::restartNode() - "
				 "failed.";
		 return;
	 }

	 qDebug() << "NodeHandler::restartNode() - successful.";
}

void NodeHandler::updateParameters()
{
	qDebug() << "NodeHandler::storeParameters() - " << nodeParameters_.size();

	for (uint32_t index = 0; index < nodeParameters_.size(); index++)
	{
		param_ns::GetSet::Request storeRequest;

		// Set name
		storeRequest.name.operator +=(
				nodeParameters_[index].text(NAME_INDEX).toStdString().c_str());

		// Check type
		// INTEGER
		if (QString::compare(
				nodeParameters_[index].text(TYPE_INDEX),
				QSTRING_INT) == 0)
		{
			storeRequest.
			value.to<param_ns::Value::Tag::integer_value>() =
					nodeParameters_[index].text(VALUE_INDEX).toInt();
		}
		// FLOAT
		else if (QString::compare(
					nodeParameters_[index].text(TYPE_INDEX),
					QSTRING_FLOAT) == 0)
		{
			storeRequest.
			value.to<param_ns::Value::Tag::real_value>() =
					nodeParameters_[index].text(VALUE_INDEX)
					.replace(",", ".")
					.toDouble();
		}
		// STRING
		else if (QString::compare(
				nodeParameters_[index].text(TYPE_INDEX),
				QSTRING_STRING) == 0)
		{
			storeRequest.
			value.to<param_ns::Value::Tag::string_value>() =
					nodeParameters_[index].text(VALUE_INDEX).
					toStdString().
					c_str();
		}
		// UINT8
		else if (QString::compare(
				nodeParameters_[index].text(TYPE_INDEX),
				QSTRING_UINT8) == 0)
		{
			storeRequest.
			value.to<param_ns::Value::Tag::boolean_value>() =
					nodeParameters_[index].text(VALUE_INDEX).toUInt();
		}

		// Print store request
		cout << storeRequest << endl;
		auto res = performBlockingServiceCall
				<param_ns::GetSet>(
						*canNode_,
						externalNodeId_,
						storeRequest);
		if (res.first < 0)
		{
			qDebug() << "NodeHandler::storeParameters() - "
					"Not able to set param: "
					<< nodeParameters_[index].text(NAME_INDEX)
					<< " ... skipping.";
		}

	}

	updateParametersFlag_ = false;
	externalNodeId_ = -1;
}


