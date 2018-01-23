#include "morus_main_window.h"
#include "ui_morus_main_window.h"
#include "node_handler.h"

morus_main_window::morus_main_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::morus_main_window)
{
    ui->setupUi(this);

    // Initialize the node handler.
    new_node_handler = new node_handler();
}

morus_main_window::~morus_main_window()
{
    delete ui;
    delete new_node_handler;
}

void morus_main_window::on_start_local_node_button_clicked()
{
	new_node_handler->create_new_node("can0", 1);
}