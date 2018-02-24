#include <node_worker.h>

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
		pCan_node_handler->create_new_node(iface_name, node_id);
		node_handler_initialized = true;
	}

	while (worker_started) {

		pCan_node_handler->start_current_node(1000);
	}
}
