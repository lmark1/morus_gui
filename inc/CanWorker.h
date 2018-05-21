#ifndef CAN_WORKER_H
#define CAN_WORKER_H

#include <QObject>
#include <QMutex>

#include "NodeInfo.h"

// Forward declaration of the NodeHandler class.
class NodeHandler;

/**
 * Timeout in milliseconds. Used when spinning nodes.
 */
extern const int NODE_TIMEOUT;

/**
 *	This class is used as a wrapper for node_handler.
 *	Implemented for use with QThreads.
 *
 *	- 	Added node collecting functionality through the
 *		node handler. It is implemented here in order to communicate
 *		Node information to the main(UI) thread via signals.
 */
class CanWorker : public QObject {
    Q_OBJECT
 
	public:

		/**
		 * Explicit node_worker constructor.
		 */
		explicit CanWorker(QObject *parent = 0);
		~CanWorker();

		/**
		 * Initialize CanWorker object. Unable to run the worker without
		 * calling this method.
		 *
		 * iface_name - CAN interface name
		 * node_id - new CAN node id
		 */
		void initializeWorker(std::string ifaceName, int nodeId);

		/**
		 * Stops the node_worker.
		 */
		void stopWorker();

		/**
		 * Returns true if canWorker is running, otherwise false.
		 */
		bool isRunning();

		/**
		 * Pause monitor worker.
		 */
		void pauseWorker();

		/**
		 * Resume monitor worker.
		 */
		void resumeWorker();

		/**
		 * Signals to the Can Worker to start reading parameters
		 * from the node with given ID.
		 *
		 * @param nodeID
		 */
		void readParameterSignal(int nodeID);

	public slots:

		/**
		 * Whole workload is done in this method.
		 * Creating CAN node, spinning CAN node etc.
		 */
		void process();

		/**
		 * Firmware update is requested. Notify firmwareVersionChecker
		 * in node handler to check if update is allowed.
		 */
		void firmwareUpdateRequested(std::string firmwareFilePath, int nodeId);

	signals:

		/**
		 * Emit this signal when worker is done.
		 */
		void finished();

		/**
		 * Emit this signal when something goes wrong.
		 */
		void error(QString err);

		/**
		 * Emit this signal when new node information is found.
		 * In current implementation this will be emitted from inside the
		 * NodeHandler object where the NodeInfoCollector is found.
		 */
		void nodeInformationFound(std::vector<NodeInfo_t> *activeNodeInfo);

	private:

		/**
		 * Initialize node_handler object. If it fails,
		 * exception will be thrown and error message emitted.
		 */
		int initializeNodeHandler();

		/**
		 * Start running the node handler. If unable to run,
		 * exception will be thrown and error message emitted.
		 */
		int runNodeHandler();

		/**
		 * Attempts to read all parameters from node with paramNodeID_.
		 */
		void readAllParameters();

		/**
		 * CAN interface name.
		 */
		std::string ifaceName_;

		/**
		 * New CAN node ID.
		 */
		int workerID_ = -1;

		/**
		 * ID of the node whose parameters are being read.
		 */
		int paramNodeID_ = -1;

		/**
		 * True if parameters will be read, false otherwise.
		 */
		bool readParametersFlag_ = false;

		/**
		 *	Flag indicating worker started processing.
		 */
		bool working_ = false;

		/**
		 * Flag indicating that node handler was initialized.
		 */
		bool nodeHandlerInitialized_ = false;

		/**
		 * Node handler used for CAN node operations.
		 */
		NodeHandler *canNodeHandler_ = NULL;

		/**
		 * Mutex used for managing processing and calls from other threads.
		 */
		QMutex mutex_;

		/**
		 * True if node is paused, otherwise false.
		 */
		bool paused_ = false;
};

#endif //CAN_WORKER_H
