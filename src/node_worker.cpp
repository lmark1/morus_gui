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

	worker_started = true;
}
 
node_worker::~node_worker() {

}

void node_worker::process() {

	// Initialize node_handler here
	if (! node_handler_initialized) {

		pCan_node_handler = new node_handler();
		const int res_id = pCan_node_handler->create_new_node(iface_name, node_id);

		//If initialization failed stop processing
		if (res_id < 0) {
			worker_started = false;
		}

		node_handler_initialized = true;
	}

	// Do work.. spin the nodes
	while (worker_started) {

		const int res_id = pCan_node_handler->spin_current_node(NODE_TIMEOUT);

		// If something went wrong stop doing work.
		if (res_id < 0) {
			worker_started = false;
		}
	}

	emit finished();
}
