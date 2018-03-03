#include <node_worker.h>

const int NODE_TIMEOUT = 1000;

node_worker::node_worker(
		std::string iface_name,
		int node_id,
		QObject *parent) : QObject(parent) {

	this->node_id = node_id;
	this->iface_name = iface_name;

	// Avoid initializing anything in this constructor.
	// Object created here remain in the main thread and
	// not in the QThread as needed.

	working = true;
}
 
node_worker::~node_worker() {

}

void node_worker::process() {

	int node_init_res = initialize_node_handler();

	// Stop worker if initialization failed
	if (node_init_res < 0) {
		stop_worker();
	}

	// Do the work... spin the nodes
	while (working) {

		const int res_id = pCan_node_handler->spin_current_node(NODE_TIMEOUT);

		// If something went wrong stop doing work.
		if (res_id < 0) {
			working = false;
		}
	}

	emit finished();
}

int node_worker::initialize_node_handler() {

	pCan_node_handler = new node_handler();

	try {

		// Try creating new node.
		const int res_id = pCan_node_handler->create_new_node(iface_name, node_id);
		return res_id;

	}  catch (const runtime_error &ex) {

		// Emit error message
		std::string error_message(ex.what());
        emit error(
            QString::fromStdString(
           		 "Error occurred while creating a new node.\n" +
				 error_message
				 )
            );

        return -1;
	}

}

void node_worker::stop_worker() {

	//TODO: add mutex for outside stopping
	working = false;
}
