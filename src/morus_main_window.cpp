#include "morus_main_window.h"
#include "ui_morus_main_window.h"
#include "node_handler.h"

const QString DEFAULT_IFACE_NAME = "can0";
const int DEFAULT_NODE_ID = 127;

morus_main_window::morus_main_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::morus_main_window)
{
    ui->setupUi(this);
    
    // Set initial node-id and node interface values
    ui->local_node_id_spinBox->setValue(DEFAULT_NODE_ID);
    ui->local_node_id_spinBox->setMaximum(DEFAULT_NODE_ID);
    ui->can_iface_name_plainTextEdit->setPlainText(DEFAULT_IFACE_NAME);
}

morus_main_window::~morus_main_window()
{
    delete ui;
}

void morus_main_window::on_start_local_node_button_clicked()
{   

    // Get node id and interface name from GUI
    int node_id = ui->local_node_id_spinBox->value();
    std::string iface_name = ui->
    		can_iface_name_plainTextEdit->
			toPlainText().toStdString();

    // Initialize thread
    pCan_thread = new QThread();

    // Initialize CAN worker
    pCan_node_worker = new node_worker(iface_name, node_id);

    // Setup QThread object
    pCan_node_worker->moveToThread(pCan_thread);

    // Do connections...
    connect(pCan_node_worker, SIGNAL( error(QString) ), // Connect worker error()...
    		this, SLOT( errorString(QString) ));		// To morus_main_window errorString()

    connect(pCan_thread, SIGNAL( started() ),			// Connect thread started()...
    		pCan_node_worker, SLOT( process() ));		// to worker process()

    connect(pCan_node_worker, SIGNAL( finished() ),		// Connect worker finished()...
    		pCan_thread, SLOT( quit() ));				// to thread quit() - exits the thread

    connect(pCan_node_worker, SIGNAL( finished() ),		// Connect worker finished()...
    		this, SLOT( workerFinished() ));			// to morus_window workerFinished()

    connect(pCan_node_worker, SIGNAL( finished() ),
    		pCan_node_worker, SLOT( deleteLater() )); 	// Connect worker finished()...
    connect(pCan_thread, SIGNAL( finished() ),			// to deleteLater() - QObject SLOT that
    		pCan_thread, SLOT( deleteLater() ));		// marks objects for deletion

    pCan_thread->start();

    // Disable start button after generating a node
    ui->start_local_node_button->setEnabled(false);
}

void morus_main_window::workerFinished() {

	// Re-enable the start button when current current CAN thread is finished
	ui->start_local_node_button->setEnabled(true);
}

void morus_main_window::errorString(QString error) {
	this->generateDialog(error.toStdString());
}

void morus_main_window::generateDialog(std::string message) {

    // Display message box
    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString(message));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

