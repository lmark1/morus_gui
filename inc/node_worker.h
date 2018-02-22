#ifndef NODE_WORKER_H
#define NODE_WORKER_H

#include <QObject>

/**
 *	This class is used as a wrapper for node_handler. 
 *	Implemented for use in QThreads.
 */
class node_worker : public QObject {
    Q_OBJECT
 
public:
    
    explicit node_worker(QObject *parent = 0);
    ~node_worker();
 
public slots:
    void process();
 
signals:
    void finished();
    void error(QString err);
 
private:
    // add your variables here
};

#endif //NODE_WORKER_H