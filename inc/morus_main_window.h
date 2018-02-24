#ifndef MORUS_MAIN_WINDOW_H
#define MORUS_MAIN_WINDOW_H

#include "node_handler.h"
#include "node_worker.h"

#include <QMainWindow>

#include <string>

/**
 * Default CAN interface name. Set as initial value in GUI.
 */
extern const QString DEFAULT_IFACE_NAME;

/**
 * Default CAN node ID. Set as initial value in GUI.
 */
extern const int DEFAULT_NODE_ID;

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

		/**
		 * Pointer to morus_main_window UI.
		 */
		Ui::morus_main_window *ui;

		/**
		 * Pointer to the CAN node worker. It acts as a wrapper
		 * object for node_handler. Used for safe QThread work.
		 */
		node_worker *pCan_node_worker = NULL;

		/**
		 * Thread used by pCan_node_worker. Used for spinning CAN
		 * node.
		 */
		QThread *pCan_thread = NULL;
};

#endif // MORUS_MAIN_WINDOW_H
