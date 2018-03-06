#include "MorusMainWindow.h"
#include "NodeHandler.h"
#include "UiMorusMainWindow.h"

const QString DEFAULT_IFACE_NAME = "can0";
const int DEFAULT_NODE_ID = 127;

MorusMainWindow::MorusMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MorusMainWindow)
{
    ui->setupUi(this);
    
    // Set initial node-id and node interface values
    ui->localNodeIDSpinBox->setValue(DEFAULT_NODE_ID);
    ui->localNodeIDSpinBox->setMaximum(DEFAULT_NODE_ID);
    ui->canIfaceNamePlainTextEdit->setPlainText(DEFAULT_IFACE_NAME);
}

MorusMainWindow::~MorusMainWindow()
{
	//TODO(lmark): Stop worker and thread
    delete ui;
    delete canNodeWorker;
    delete canThread;
}

void MorusMainWindow::on_startLocalNodeButton_clicked()
{   
	cout << "Hello from start button";
    // Get node id and interface name from GUI
    int node_id = ui->localNodeIDSpinBox->value();
    std::string iface_name = ui->
    		canIfaceNamePlainTextEdit->
			toPlainText().toStdString();

    // Initialize thread
    canThread = new QThread();

    // Initialize CAN worker
    canNodeWorker = new CanWorker(iface_name, node_id);

    // Setup QThread object
    canNodeWorker->moveToThread(canThread);

    // Do connections...
    connect(canNodeWorker, SIGNAL( error(QString) ),  		// Connect worker error()...
    		this, SLOT( handleErrorMessage(QString) ));		// To morus_main_window errorString()

    connect(canThread, SIGNAL( started() ),			// Connect thread started()...
    		canNodeWorker, SLOT( process() ));		// to worker process()

    connect(canNodeWorker, SIGNAL( finished() ),	// Connect worker finished()...
    		canThread, SLOT( quit() ));				// to thread quit() - exits the thread

    connect(canNodeWorker, SIGNAL( finished() ),	// Connect worker finished()...
    		this, SLOT( workerFinished() ));		// to morus_window workerFinished()

    connect(canNodeWorker, SIGNAL( finished() ),
    		canNodeWorker, SLOT( deleteLater() )); 	// Connect worker finished()...
    connect(canThread, SIGNAL( finished() ),		// to deleteLater() - QObject SLOT that
    		canThread, SLOT( deleteLater() ));		// marks objects for deletion

    connect(canNodeWorker, SIGNAL( nodeInformationFound(NodeInfo_t) ),
    		this, SLOT( updateCanMonitor(NodeInfo_t) ));

    canThread->start();

    // Disable start button after generating a node
    ui->startLocalNodeButton->setEnabled(false);
}

void MorusMainWindow::workerFinished() {

	// Re-enable the start button when current current CAN thread is finished
	ui->startLocalNodeButton->setEnabled(true);
}

void MorusMainWindow::handleErrorMessage(QString error) {

	// Generate message box about the error
	this->generateMessageBox(error.toStdString());
}

void MorusMainWindow::updateCanMonitor(NodeInfo_t *nodeInfo) {
	// TODO(lmark): update canMonitor
}

void MorusMainWindow::generateMessageBox(std::string message) {

    // Display message box
    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString(message));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

