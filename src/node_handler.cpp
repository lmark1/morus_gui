#include "node_handler.h"
#include "platform_linux.h"
#include <cstddef>

const uavcan::NodeStatusProvider::NodeName DEFAULT_NODE_NAME = "morus.can.node";

node_handler::node_handler() {
	// Initialize node handler...
}

node_handler::~node_handler() {
	// Destroy node handler...
}

int node_handler::create_new_node(std::string iface_name, int node_id) {

	/*
     * Node initialization.
     * Node ID and name are required; otherwise, the node will refuse to start.
     * Version info is optional.
     */
	cout <<
        "Starting node initialization with iface_name: " <<
        iface_name << " and node_id: " << node_id << "\n";

	can_node = &getCanNode(iface_name);
	can_node->setNodeID(node_id);
	can_node->setName(DEFAULT_NODE_NAME);

	/*
     * Start the node.
     * All returnable error codes are listed in the header file uavcan/error.hpp.
     */
    const int node_start_res = can_node->start();
    if (node_start_res < 0)
    {
        generateDialog(
            "Unable to start CAN node with error: " +
            std::to_string(node_start_res));
        return 1;
    }

    /*
     * Informing other nodes that we're ready to work.
     * Default mode is INITIALIZING.
     */
    can_node->setModeOperational();
    node_created = true;

    cout <<
        "Node initialization successful.\n";
    return 0;
}

int node_handler::start_current_node(int timeout_ms) {

    while (true) {

    	/*
		 * If there's nothing to do, the thread blocks inside the driver's
		 * method select() until the timeout expires or an error occurs (e.g. driver failure).
		 * All error codes are listed in the header uavcan/error.hpp.
		 */
		const int res = can_node->spin(
				uavcan::MonotonicDuration::fromMSec(timeout_ms));
		if (res < 0)
		{
			std::cerr << "Transient failure: " << res << std::endl;
		}
    }
}


Node& getCanNode(std::string iface_name)
{
    static Node node(getCanDriver(iface_name), getSystemClock());
    return node;
}

void generateDialog(std::string message) {

    // Display message box
    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString(message));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}
