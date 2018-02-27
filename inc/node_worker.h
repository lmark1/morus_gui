#ifndef NODE_WORKER_H
#define NODE_WORKER_H

#include "node_handler.h"

#include <QObject>
#include <QEventLoop>

/**
 * Timeout in milliseconds. Used when spinning nodes.
 */
extern const int NODE_TIMEOUT;

/**
 *	This class is used as a wrapper for node_handler. 
 *	Implemented for use with QThreads.
 */
class node_worker : public QObject {
    Q_OBJECT
 
public:
    
	/**
	 * Explicit node_worker constructor.
	 *
	 * iface_name - CAN interface name
	 * node_id - new CAN node I
	 */
    explicit node_worker(
			std::string iface_name,
			int node_id,
			QObject *parent = 0);
    ~node_worker();
 
public slots:
    void process();
 
signals:
    void finished();
    void error(QString err);
 
private:

    /**
     * CAN interface name.
     */
    std::string iface_name;

    /**
     * New CAN node ID.
     */
    int node_id;

    /**
     *	Flag indicating worker started processing.
     */
    bool worker_started = false;

    /**
     * Flag indicating that node handler was initialized.
     */
    bool node_handler_initialized = false;

    /**
     * Node handler used for CAN node operations.
     */
    node_handler *pCan_node_handler = NULL;
};

#endif //NODE_WORKER_H
