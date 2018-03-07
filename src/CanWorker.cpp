#include "CanWorker.h"
#include "NodeHandler.h"

const int NODE_TIMEOUT = 1000;

CanWorker::CanWorker(
		std::string iface_name,
		int node_id,
		QObject *parent) : QObject(parent) {

	this->nodeID_ = node_id;
	this->ifaceName_ = iface_name;

	// Avoid initializing anything in this constructor.
	// Object created here remain in the main thread and
	// not in the QThread as needed.

	working_ = true;
}
 
CanWorker::~CanWorker() {

	delete canNodeHandler_;
}

void CanWorker::process() {

	const int node_init_res = initializeNodeHandler();

	// Stop worker if initialization failed
	if (node_init_res < 0) {
		stopWorker();
	}

	// Do the work... spin the nodes
	while (working_) {

		std::cout << "Processing";
		const int res_id = runNodeHandler();

		// If something went wrong stop doing work.
		if (res_id < 0) {
			stopWorker();
		}
	}

	emit finished();
}

int CanWorker::initializeNodeHandler() {

	canNodeHandler_ = new NodeHandler(*this);

	try {

		// Try creating new node.
		const int res_id = canNodeHandler_->
				createNewNode(ifaceName_, nodeID_);
		return res_id;

	}  catch (const std::exception &ex) {

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

int CanWorker::runNodeHandler() {

	try {

		// Try spinning the current node
		const int res_id = canNodeHandler_->spinCurrentNode(NODE_TIMEOUT);
		return res_id;

	} catch (const std::exception& ex) {

		// Emit error message
		std::string error_message(ex.what());
		emit error(
			QString::fromStdString(
				 "Error occurred while spinning node.\n" +
				 error_message
				 )
			);

		return -1;
	}

}

void CanWorker::stopWorker() {

	//TODO(lmark): add mutex for outside stopping
	working_ = false;
	canNodeHandler_->destroyCurrentNode();
}
