#include <string>

#include "MorusMainWindow.h"
#include "UiMorusMainWindow.h"
#include "CanWorker.h"
#include "NodeHandler.h"

const QString DEFAULT_IFACE_NAME = "can0";
const int DEFAULT_NODE_ID = 127;

MorusMainWindow::MorusMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MorusMainWindow) {

    ui_->setupUi(this);
    
    // Set initial node-id and node interface values
    ui_->localNodeIDSpinBox->setValue(DEFAULT_NODE_ID);
    ui_->localNodeIDSpinBox->setMaximum(DEFAULT_NODE_ID);
    ui_->canIfaceNamePlainTextEdit->setPlainText(DEFAULT_IFACE_NAME);
}

MorusMainWindow::~MorusMainWindow() {

	//TODO(lmark): Stop worker and thread
    delete ui_;
    delete canNodeWorker_;
    delete canThread_;
}

void MorusMainWindow::on_startLocalNodeButton_clicked() {

	cout << "Hello from start button";
    // Get node id and interface name from GUI
    int node_id = ui_->localNodeIDSpinBox->value();
    std::string iface_name = ui_->
    		canIfaceNamePlainTextEdit->
			toPlainText().toStdString();

    // Initialize thread
    canThread_ = new QThread();

    // Initialize CAN worker
    canNodeWorker_ = new CanWorker(iface_name, node_id);

    // Setup QThread object
    canNodeWorker_->moveToThread(canThread_);

    // Do connections...
    connect(canNodeWorker_, SIGNAL( error(QString) ),  		// Connect worker error()...
    		this, SLOT( handleErrorMessage(QString) ));		// To morus_main_window errorString()

    connect(canThread_, SIGNAL( started() ),			// Connect thread started()...
    		canNodeWorker_, SLOT( process() ));		// to worker process()

    connect(canNodeWorker_, SIGNAL( finished() ),	// Connect worker finished()...
    		canThread_, SLOT( quit() ));				// to thread quit() - exits the thread

    connect(canNodeWorker_, SIGNAL( finished() ),	// Connect worker finished()...
    		this, SLOT( workerFinished() ));		// to morus_window workerFinished()

    connect(canNodeWorker_, SIGNAL( finished() ),
    		canNodeWorker_, SLOT( deleteLater() )); 	// Connect worker finished()...
    connect(canThread_, SIGNAL( finished() ),		// to deleteLater() - QObject SLOT that
    		canThread_, SLOT( deleteLater() ));		// marks objects for deletion

    connect(canNodeWorker_, // Connect information found signal...
    		SIGNAL( nodeInformationFound(std::vector<NodeInfo_t>) ),
    		this, // ... to the update monitor slot.
			SLOT( updateCanMonitor(std::vector<NodeInfo_t>) ));

    canThread_->start();

    // Disable start button after generating a node
    ui_->startLocalNodeButton->setEnabled(false);
}

void MorusMainWindow::workerFinished() {

	// Re-enable the start button when current current CAN thread is finished
	ui_->startLocalNodeButton->setEnabled(true);
}

void MorusMainWindow::handleErrorMessage(QString error) {

	// Generate message box about the error
	this->generateMessageBox(error.toStdString());
}

void MorusMainWindow::updateCanMonitor(
		std::vector<NodeInfo_t> *activeNodesInfo) {

	cout << activeNodesInfo->size();
}

void MorusMainWindow::generateMessageBox(std::string message) {

    // Display message box
    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString(message));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

