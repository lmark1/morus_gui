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
    
    // Initialize the node handler.
    new_node_handler = new node_handler();

    // Set initial node-id and node interface values
    ui->local_node_id_spinBox->setValue(DEFAULT_NODE_ID);
    ui->can_iface_name_plainTextEdit->setPlainText(DEFAULT_IFACE_NAME);
}

morus_main_window::~morus_main_window()
{
    delete ui;
    delete new_node_handler;
}

void morus_main_window::on_start_local_node_button_clicked()
{   

    // Get node id and interface name from GUI
    int node_id = ui->local_node_id_spinBox->value();
    std::string iface_name = ui->can_iface_name_plainTextEdit->toPlainText().toStdString();

    // Try generating a new can node.
    try {
        int res_node = new_node_handler->create_new_node(iface_name, node_id);
        if (res_node != 0) {
                generateDialog("Unable to create new CAN node.");
                return;
        }
    } catch (const runtime_error & error) {
        generateDialog("Error occured while creating a new node - check CAN interface name.");
        std::cerr << error.what() << std::endl;
        return;
    }
    
    // Disable start button after generating a node
    ui->start_local_node_button->setEnabled(false);
}   

