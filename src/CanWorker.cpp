#include <QDebug>

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

	qDebug() << "CanWorker::~CanWorker() "
			"- destructor called!";
	// Destroy CAN node handled by the node_handler
	canNodeHandler_->stopCurrentNode();
	canNodeHandler_->destroyCurrentNode();

	delete canNodeHandler_;
}

void CanWorker::process() {

	qDebug() << "CanWorker::process() "
			"- Initializing node handler";
	const int node_init_res = initializeNodeHandler();

	// Stop worker if initialization failed
	if (node_init_res < 0) {
		qCritical() << "CanWorker::process() "
				"- NodeHandler initialization failed.";
		stopWorker();
	}

	if (working_) {
		qDebug() << "CanWorker::process() "
				"- Starting work.";
	}

	// Do the work... spin the nodes
	while (working_) {

		qDebug() << "CanWorker::process() "
				"- Doing work.";
		const int res_id = runNodeHandler();

		// If something went wrong stop doing work.
		if (res_id < 0) {
			qCritical() << "CanWorker::process() "
					"- Something went wrong while running the node.";
			stopWorker();
		}
	}

	qDebug() << "CanWorker::process() "
			"- worker finished.";
	emit finished();
}

int CanWorker::initializeNodeHandler() {

	canNodeHandler_ = new NodeHandler(*this);

	try {

		qDebug() << "CanWorker::initializeNodeHandler() "
				"- trying to initialize node handler.";
		// Try creating new node.
		const int res_id = canNodeHandler_->
				createNewNode(ifaceName_, nodeID_);
		return res_id;

	}  catch (const std::exception &ex) {

		qCritical() << "CanWorker::initializeNodeHandler() "
				"- Failed to initialized node handler.";
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

		qCritical() << "Failed to run node handler.";

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

	qDebug() << "CanWorker::stopWorker() "
			"- Stopping CAN worker.";
	mutex_.lock();
	working_ = false;
	mutex_.unlock();
}