#include <node_worker.h>

// --- CONSTRUCTOR ---
node_worker::node_worker(QObject *parent) : 
	QObject(parent) {
    // you could copy data from constructor arguments to internal variables here.
}
 
// --- DECONSTRUCTOR ---
node_worker::~node_worker() {
    // free resources
}

// --- PROCESS ---
// Start processing data.
void node_worker::process() {
    // allocate resources using new here
    qDebug("Hello World!");
    emit finished();
}
