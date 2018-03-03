#include "node_handler.h"
#include "platform_linux.h"
#include <cstddef>

const uavcan::NodeStatusProvider::NodeName DEFAULT_NODE_NAME = "morus.can.node";

node_handler::node_handler() {
	// Initialize node handler...
}

node_handler::~node_handler() {
	delete pCan_node;
}

int node_handler::create_new_node(std::string iface_name, int node_id) {

	/*
     * Node initialization.
     * Node ID and name are required; otherwise, the node will refuse to start.
     * Version info is optional.
     */
	pCan_node = &getCanNode(iface_name);
	pCan_node->setNodeID(node_id);
	pCan_node->setName(DEFAULT_NODE_NAME);

	/*
     * Start the node.
     * All returnable error codes are listed in the header file uavcan/error.hpp.
     */
    const int node_start_res = pCan_node->start();
    if (node_start_res < 0) {
        return node_start_res;
    }

    /*
     * Informing other nodes that we're ready to work.
     * Default mode is INITIALIZING.
     */
    pCan_node->setModeOperational();
    node_created = true;

    return node_start_res;
}

int node_handler::spin_current_node(int timeout_ms) {

	/*
	 * If there's nothing to do, the thread blocks inside the driver's
	 * method select() until the timeout expires or an error occurs (e.g. driver failure).
	 * All error codes are listed in the header uavcan/error.hpp.
	 */


	const int res = pCan_node->spin(
			uavcan::MonotonicDuration::fromMSec(timeout_ms));

	return res;
}

void node_handler::destroy_current_node() {
	pCan_node = NULL;
	node_created = false;
}


Node& getCanNode(std::string iface_name)
{
    static Node node(getCanDriver(iface_name), getSystemClock());
    return node;
}


