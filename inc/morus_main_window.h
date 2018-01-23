#ifndef MORUS_MAIN_WINDOW_H
#define MORUS_MAIN_WINDOW_H

#include <QMainWindow>
#include "node_handler.h"

namespace Ui {
class morus_main_window;
}

class morus_main_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit morus_main_window(QWidget *parent = 0);
    ~morus_main_window();

private slots:
	
	void on_start_local_node_button_clicked();

private:
    Ui::morus_main_window *ui;

    node_handler *new_node_handler;
};

#endif // MORUS_MAIN_WINDOW_H
