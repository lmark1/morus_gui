#include <QDebug>

#include <cstddef>
#include <iostream>

#include "NodeHandler.h"
#include "PlatformLinux.h"
#include "CanWorker.h"

const uavcan::NodeStatusProvider::NodeName DEFAULT_NODE_NAME = "morus.can.node";

NodeHandler::NodeHandler(CanWorker& worker)
{
	this->canWorker_ = &worker;
}

NodeHandler::~NodeHandler()
{
	qDebug() << "NodeHandler::~NodeHandler() "
			"- destructor called.";

	delete canNode_;
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
	qDebug() << "NodeHandler::spinCurrentNode() "
				"- spin node.\n";

	// Spin the node
	const int res = canNode_->spin(
			uavcan::MonotonicDuration::fromMSec(timeout_ms));

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


