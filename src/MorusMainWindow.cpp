#include <QDebug>

#include <string>

#include "MorusMainWindow.h"
#include "UiMorusMainWindow.h"
#include "CanWorker.h"
#include "NodeHandler.h"

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

    // Initialize thread
	canThread_ = new QThread();

	// Initialize CAN worker
	canNodeWorker_ = new CanWorker();

	// Setup QThread object
	canNodeWorker_->moveToThread(canThread_);

	// Setup signal / slot connections
	setupThreadConnections();
}

MorusMainWindow::~MorusMainWindow()
{
	qDebug() << "MorusMainWindow::~MorusMainWindow "
				"- Stopping CanWorker.";
	// Stop can worker if it's initialized
	if (canNodeWorker_ != NULL) {
		canNodeWorker_->stopWorker();
	}

	// TODO(lmark): segmentation fault when stopping canThread...
	//				(Check the issue on the repository)
	qDebug() << "MorusMainWindow::~MorusMainWindow "
				"- stopping CanThread";
	// Stopping canNodeWorker should stop and delete QThread...
	// Do it anyway
	if (canThread_ != NULL && canThread_->isRunning()) {
		canThread_->terminate();
		canThread_->wait();
	}

	qDebug() << "MorusMainWindow::~MorusMainWindow "
				"- deleting UI.";
	// canWorker and canThread are connected to
	// the deleteLater() funcion when they are finished
    delete ui_;
}

void MorusMainWindow::on_startLocalNodeButton_clicked()
{
	// Get node id and interface name from GUI
    int node_id = ui_->localNodeIDSpinBox->value();
    std::string iface_name = ui_->
    		canIfaceNamePlainTextEdit->
			toPlainText().toStdString();

    // Initialize node worker
    canNodeWorker_->initializeWorker(iface_name, node_id);

    // Start the thread
    canThread_->start();

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

void MorusMainWindow::setupThreadConnections()
{
	// Do connections...
	connect(canNodeWorker_, // Connect worker error()...
			SIGNAL( error(QString) ),
			this, 			// To morus_main_window errorString()
			SLOT( handleErrorMessage(QString) ));

	connect(canThread_, 	// Connect thread started()...
			SIGNAL( started() ),
			canNodeWorker_, // to worker process()
			SLOT( process() ));

	connect(canNodeWorker_, // Connect worker finished()...
			SIGNAL( finished() ),
			canThread_,		// to thread quit() - exits the thread
			SLOT( quit() ));

	connect(canNodeWorker_, // Connect worker finished()...
			SIGNAL( finished() ),
			this,			// to morus_window workerFinished()
			SLOT( workerFinished() ));

	// Connect worker finished() to deleteLater() - QObject SLOT
	// ... marks objects for deletion
	connect(canNodeWorker_,
			SIGNAL( finished() ),
			canNodeWorker_,
			SLOT( deleteLater() ));
	connect(canThread_,
			SIGNAL( finished() ),
			canThread_,
			SLOT( deleteLater() ));

	connect(canNodeWorker_, // Connect information found signal...
			SIGNAL( nodeInformationFound(std::vector<NodeInfo_t>*) ),
			this, // ... to the update monitor slot.
			SLOT( updateCanMonitor(std::vector<NodeInfo_t>*) ));

}

