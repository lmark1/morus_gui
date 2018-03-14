#include <QDebug>

#include <string>

#include "MorusMainWindow.h"
#include "UiMorusMainWindow.h"
#include "CanWorker.h"
#include "NodeHandler.h"
#include "MonitorWorker.h"

const QString DEFAULT_IFACE_NAME = "can0";
const int DEFAULT_NODE_ID = 127;

MorusMainWindow::MorusMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MorusMainWindow)
{
    ui_->setupUi(this);
    
    // Set initial node-id and node interface values
    ui_->localNodeIDSpinBox->setValue(DEFAULT_NODE_ID);
    ui_->localNodeIDSpinBox->setMaximum(DEFAULT_NODE_ID);
    ui_->canIfaceNamePlainTextEdit->setPlainText(DEFAULT_IFACE_NAME);

    // Initialize monitor thread
	monitorWorkerThread_ = new QThread();

	// Initialize monitorWorker
	monitorWorker_ = new MonitorWorker();

	// Setup QThread object
	monitorWorker_->moveToThread(monitorWorkerThread_);

	// Setup signal / slot connections
	setupMonitorThreadConnections();

	// Start monitor
	std::string iface(DEFAULT_IFACE_NAME.toStdString());
	monitorWorker_->initializeWorker(iface, 1);
	monitorWorkerThread_->start();
}

MorusMainWindow::~MorusMainWindow()
{
	qDebug() << "MorusMainWindow::~MorusMainWindow "
				"- Stopping CanWorker.";
	if (canNodeWorker_ != NULL && canNodeWorker_->isRunning()) {
		canNodeWorker_->stopWorker();
	}

	qDebug() << "MorusMainWindow::~MorusMainWindow "
			"- Stopping monitorWorker";
	if (monitorWorker_ != NULL && monitorWorker_->isRunning()) {
		monitorWorker_->stopWorker();
	}

	monitorWorkerThread_->terminate();
	monitorWorkerThread_->wait();

	if (canWorkerThread_)
	{
		canWorkerThread_->terminate();
		canWorkerThread_->wait();
	}

	qDebug() << "MorusMainWindow::~MorusMainWindow "
				"- deleting UI.";
	delete canNodeWorker_;
	delete monitorWorker_;
	delete ui_;
}

void MorusMainWindow::on_startLocalNodeButton_clicked()
{
	// Get node id and interface name from GUI
    int node_id = ui_->localNodeIDSpinBox->value();
    std::string iface_name = ui_->
    		canIfaceNamePlainTextEdit->
			toPlainText().toStdString();

    /*
     * Can worker needs to be initialized inside the start button
     * callback. Both canWorker and canThread get deleted each time
     * they finish working.
     */
    canWorkerThread_ = new QThread();
    canNodeWorker_ = new CanWorker();
    canNodeWorker_->moveToThread(canWorkerThread_);
    setupCanThreadConnections();

    // Initialize node worker
    canNodeWorker_->initializeWorker(iface_name, node_id);

    // Start the thread
    canWorkerThread_->start();

    // Disable start button after generating a node
    ui_->startLocalNodeButton->setEnabled(false);
}

void MorusMainWindow::workerFinished()
{
	qDebug() << "MorusMainWindow::workerFinished()";
	// Re-enable the start button when current current CAN thread is finished
	ui_->startLocalNodeButton->setEnabled(true);
}

void MorusMainWindow::handleErrorMessage(QString error)
{
	// Generate message box about the error
	this->generateMessageBox(error.toStdString());
}

void MorusMainWindow::updateCanMonitor(
		std::vector<NodeInfo_t> *activeNodesInfo)
{
	qInfo() << "MorusMainWindow::updateCanMonitor() "
			"- Number of active nodes: " << activeNodesInfo->size();

	//TODO(lmark): update the table with found nodes
}

void MorusMainWindow::generateMessageBox(std::string message)
{
    // Display message box
    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString(message));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MorusMainWindow::setupCanThreadConnections()
{
	// **** CanWorker / Thread connections ****
	connect(canNodeWorker_, // Connect worker error()...
			SIGNAL( error(QString) ),
			this, 			// To morus_main_window errorString()
			SLOT( handleErrorMessage(QString) ));

	connect(canWorkerThread_, 	// Connect thread started()...
			SIGNAL( started() ),
			canNodeWorker_, // to worker process()
			SLOT( process() ));

	connect(canNodeWorker_, // Connect worker finished()...
			SIGNAL( finished() ),
			canWorkerThread_,		// to thread quit() - exits the thread
			SLOT( quit() ));

	connect(canNodeWorker_, // Connect worker finished()...
			SIGNAL( finished() ),
			this,			// to morus_window workerFinished()
			SLOT( workerFinished() ));

	// Connect worker finished() to deleteLater() - QObject SLOT
	// ... marks objects for deletion
//	connect(canNodeWorker_,
//			SIGNAL( finished() ),
//			canNodeWorker_,
//			SLOT( deleteLater() ));
//	connect(canWorkerThread_,
//			SIGNAL( finished() ),
//			canWorkerThread_,
//			SLOT( deleteLater() ));

	connect(canNodeWorker_, // Connect information found signal...
			SIGNAL( nodeInformationFound(std::vector<NodeInfo_t>*) ),
			this, // ... to the update monitor slot.
			SLOT( updateCanMonitor(std::vector<NodeInfo_t>*) ));

}

void MorusMainWindow::setupMonitorThreadConnections()
{
	// **** CanMonitor / Thread connections ****
	// Connect errors
	connect(monitorWorker_,
			SIGNAL( error(QString) ),
			this,
			SLOT( handleErrorMessage(QString) ));
	// Connect started signal
	connect(monitorWorkerThread_,
			SIGNAL( started() ),
			monitorWorker_,
			SLOT( process() ));
	// Connect finished signal
	connect(monitorWorker_,
			SIGNAL( finished() ),
			monitorWorkerThread_,
			SLOT( quit() ));
	// Connect finished to delete later for both
//	connect(monitorWorker_,
//			SIGNAL( finished() ),
//			monitorWorker_,
//			SLOT( deleteLater() ));
//	connect(monitorWorkerThread_,
//			SIGNAL( finished() ),
//			monitorWorkerThread_,
//			SLOT( deleteLater() ));
}

