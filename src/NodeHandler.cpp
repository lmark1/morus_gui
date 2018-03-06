#include <cstddef>
#include <iostream>

#include "NodeHandler.h"
#include "PlatformLinux.h"
#include "CanWorker.h"
#include "NodeInfoCollector.h"

const uavcan::NodeStatusProvider::NodeName DEFAULT_NODE_NAME = "morus.can.node";

NodeHandler::NodeHandler(CanWorker& worker) {

	this->canWorker = &worker;
}

NodeHandler::~NodeHandler() {

	delete canNode;
}

int NodeHandler::createNewNode(std::string ifaceName, int nodeID) {

	/*
     * Node initialization.
     * Node ID and name are required; otherwise, the node will refuse to start.
     * Version info is optional.
     */
	canNode = &getCanNode(ifaceName);
	canNode->setNodeID(nodeID);
	canNode->setName(DEFAULT_NODE_NAME);

	/*
     * Start the node.
     * All returnable error codes are listed in the
     * header file uavcan/error.hpp.
     */
    const int nodeStartRes = canNode->start();
    if (nodeStartRes < 0) {
        return nodeStartRes;
    }

    // Initialize the node info retriever object
    uavcan::NodeInfoRetriever retriever(*canNode);

    /*
	 * Registering our collector against the retriever object.
	 * The retriever class may keep the pointers to listeners in the
	 * dynamic memory pool, therefore the operation may fail if the
	 * node runs out of memory in the pool.
	 */
	const int addListenerRes = retriever.addListener(collector);
	if (addListenerRes < 0)
	{
		throw std::runtime_error(
				"Failed to add listener; error: " +
				std::to_string(addListenerRes));
	}

    /*
     * Informing other nodes that we're ready to work.
     * Default mode is INITIALIZING.
     */
    canNode->setModeOperational();
    nodeCreated = true;

    return nodeStartRes;
}

int NodeHandler::spinCurrentNode(int timeout_ms) {

	/*
	 * If there's nothing to do, the thread blocks inside the driver's
	 * method select() until the timeout expires or an error occurs
	 * All error codes are listed in the header uavcan/error.hpp.
	 */

	// If node is not created return -1
	if (!nodeCreated) {
		return -1;
	}

	// Collect available node information
	collectNodeInformation();

	const int res = canNode->spin(
			uavcan::MonotonicDuration::fromMSec(timeout_ms));

	return res;
}

void NodeHandler::collectNodeInformation() {

	// Clear currently active nodes
	activeNodesInfo.clear();

	NodeInfo_t tempNodeInfo;
	for (uint8_t i = 1; i <= uavcan::NodeID::Max; i++) {

		// Try to get new node information
		if (auto p = collector->getNodeInfo(i)) {

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

			activeNodesInfo.push_back(tempNodeInfo);
		}
	}

	emit canWorker->nodeInformationFound(&activeNodesInfo);
}

void NodeHandler::destroyCurrentNode() {

	canNode = NULL;
	nodeCreated = false;
}


CustomNode_t& getCanNode(std::string iface_name)
{
    static CustomNode_t node(getCanDriver(iface_name), getSystemClock());
    return node;
}


