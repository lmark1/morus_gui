#ifndef CAN_WORKER_H
#define CAN_WORKER_H

#include <QObject>
#include <QMutex>
#include <QTreeWidgetItem>

#include <uavcan/protocol/param/GetSet.hpp>
#include <uavcan/protocol/param/ExecuteOpcode.hpp>
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

		/**
		 * This method will be called from the node handler with all
		 * the parameters found in the node configuration.
		 * Parameters will be further propagated to MorusMainWindow where
		 * they can be updated.
		 *
		 * @param params
		 */
		void updateParametersCallback(
				std::vector
				<uavcan::protocol::param::GetSet::Response> params);

		/**
		 * This method is called when user requests to update parameters.
		 *
		 * @param changedItems - All parameters that changed
		 * @param nodeId
		 */
		void updateParametersRequest(
				std::vector<QTreeWidgetItem> changedItems,
				int nodeId);

		/**
		 * This method is called when user requests to store parameters
		 * to the flash memory. The parameters that will be stored are the
		 * ones currently in the firmware.
		 *
		 * @param nodeId
		 */
		void storeParametersRequest(int nodeId);

		/**
		 * This method is called when user requests to erase parameters
		 * from the flash memory. The parameters will be returned to their
		 * default values.
		 */
		void eraseParametersRequest(int nodeId);

	public slots:

		/**
		 * Whole workload is done in this method.
		 * Creating CAN node, spinning CAN node etc.
		 */
		void process();

		/**
		 * Firmware update is requested. Notify firmwareVersionChecker
		 * in node handler to check if update is allowed.
		 *
		 * @param firmwareFilePath
		 * @param nodeId
		 */
		//TODO(lmark): Change this slot to a method when doing Firmware update
		void firmwareUpdateRequested(std::string firmwareFilePath, int nodeId);

	signals:

		/**
		 * Emit this signal when worker is done.
		 */
		void finishedSignal();

		/**
		 * Emit this signal when something goes wrong.
		 */
		void errorSignal(QString err);

		/**
		 * Emit this signal when new node information is found.
		 * In current implementation this will be emitted from inside the
		 * NodeHandler object where the NodeInfoCollector is found.
		 */
		void nodeInfoFoundSignal(std::vector<NodeInfo_t> *activeNodeInfo);

		/**
		 * This signal will be emitted when node handler has found
		 * node parameter configuration.
		 *
		 * @param params
		 */
		void updateParametersSignal(
				std::vector
				<uavcan::protocol::param::GetSet::Response> params);

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
		 * Attempts to read all parameters from node with given paramNodeID_.
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
