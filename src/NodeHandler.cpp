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

	delete nodeInfoRetriever_;
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

    // Run node collector setup
	setupNodeInfoCollector();

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


void NodeHandler::setupNodeInfoCollector()
{
	// Initialize the node info retriever and collector
	nodeInfoRetriever_ = new NodeInfoRetriever(*canNode_);

	// Try to start the retriever
	const int retrieverRes = nodeInfoRetriever_->start();
	if (retrieverRes < 0)
	{
		qDebug() << "NodeHandler::setupNodeInfoCollector() "
		"- Failed to start the retriever.";

		throw std::runtime_error(
				"Failed to start the retriever; error: "
				+ std::to_string(retrieverRes));
	}
	qDebug() << "NodeHandler::setupNodeInfoCollector() "
			"- Successfully started retriever.";

	/*
	 * Registering our collector against the retriever object.
	 * The retriever class may keep the pointers to listeners in the
	 * dynamic memory pool, therefore the operation may fail if the
	 * node runs out of memory in the pool.
	 */
	const int addListenerRes = nodeInfoRetriever_
			->addListener(&nodeInfoCollector_);
	if (addListenerRes < 0)
	{
		qDebug() << "NodeHandler::setupNodeInfoCollector() "
				"- Unable to start the NodeListener.";
		throw std::runtime_error(
				"Failed to add listener; error: " +
				std::to_string(addListenerRes));
	}
	qDebug() << "NodeHandler::setupNodeInfoCollector() "
			"- Added node listener.";

	// Check if retriever is running properly
	qDebug() << "NodeHandler::setupNodeInfoCollector() "
			"- Number of listeners "
			<< nodeInfoRetriever_->getNumListeners();

//	qDebug() << "NodeHandler::setupNodeInfoCollector() "
//			"- isRunning() "
//			<< nodeInfoRetriever_->isRunning();

	qDebug() << "NodeHandler::setupNodeInfoCollector() "
			"- isRetrieving() "
			<< nodeInfoRetriever_->isRetrievingInProgress();
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

	qDebug() << "NodeHandler::spinCurrentNode() "
				"- spin node.\n";

	// Spin the node
	const int res = canNode_->spin(
			uavcan::MonotonicDuration::fromMSec(timeout_ms));

	// Collect available node information
	collectNodeInformation();

	return res;
}

void NodeHandler::collectNodeInformation()
{
	qDebug() << "NodeHandler::collectNodeInformation "
			"- Starting to collect information about nodes.";

	// Clear currently active nodes
	activeNodesInfo_.clear();

	NodeInfo_t tempNodeInfo;
	for (uint8_t i = 0; i <= uavcan::NodeID::Max; i++)
	{
		// Try to get new node information
		if (auto p = nodeInfoCollector_.getNodeInfo(i))
		{
			qDebug() << "NodeHandler::collectNodeInformation "
					"- Found node!";

			// Collect node information
			tempNodeInfo.id = int(i);
			tempNodeInfo.nodeName = std::string(p->name.c_str());
			tempNodeInfo.softwareVersionMinor = p->software_version.minor;
			tempNodeInfo.softwareVersionMajor = p->software_version.major;
			tempNodeInfo.hardwareVersionMinor = p->hardware_version.minor;
			tempNodeInfo.hardwareVersionMajor = p->hardware_version.major;
			tempNodeInfo.uptime = p->status.mode;
			tempNodeInfo.health = p->status.health;
			tempNodeInfo.vendorSpecificStatusCode = p->status
					.vendor_specific_status_code;

			activeNodesInfo_.push_back(tempNodeInfo);
		}
	}

	// Emit new node information to the main thread
	emit canWorker_->nodeInformationFound(&activeNodesInfo_);
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


