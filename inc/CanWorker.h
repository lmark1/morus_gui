#ifndef CAN_WORKER_H
#define CAN_WORKER_H

#include <QObject>
#include <QEventLoop>

#include "NodeHandler.h"

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
	 *
	 * iface_name - CAN interface name
	 * node_id - new CAN node I
	 */
    explicit CanWorker(
			std::string ifaceName,
			int nodeID,
			QObject *parent = 0);
    ~CanWorker();

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
     * Stops the node_worker.
     */
    void stopWorker();

public slots:

	/**
	 * Whole workload is done in this method.
	 * Creating CAN node, spinning CAN node etc.
	 */
    void process();
 
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
    void nodeInformationFound(NodeInfo_t *nodeInfo);

private:

    /**
     * CAN interface name.
     */
    std::string ifaceName;

    /**
     * New CAN node ID.
     */
    int nodeID;

    /**
     *	Flag indicating worker started processing.
     */
    bool working = false;

    /**
     * Flag indicating that node handler was initialized.
     */
    bool nodeHandlerInitialized = false;

    /**
     * Node handler used for CAN node operations.
     */
    NodeHandler *canNodeHandler = NULL;
};

#endif //CAN_WORKER_H
