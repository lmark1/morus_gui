#ifndef MORUS_MAIN_WINDOW_H
#define MORUS_MAIN_WINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QCloseEvent>

#include "NodeInfo.h"

// Forward declaration of CanWorker
class CanWorker;

// Forward declaration of NodeHandler
class NodeHandler;

/**
 * Default CAN interface name. Set as initial value in GUI.
 */
extern const QString DEFAULT_IFACE_NAME;

/**
 * Default CAN node ID. Set as initial value in GUI.
 */
extern const int DEFAULT_NODE_ID;

namespace Ui {
class MorusMainWindow;
}

class MorusMainWindow : public QMainWindow
{
    Q_OBJECT

	public:
		explicit MorusMainWindow(QWidget *parent = 0);
		~MorusMainWindow();


		/**
		 * Generate a pop-up dialog message .
		 */
		void generateMessageBox(std::string message);

	private slots:

		/**
		 * Start button click method.
		 */
		void on_startLocalNodeButton_clicked();

	public slots:

		/**
		 * Slot method used for displaying errors in the main thread.
		 */
		void handleErrorMessage(QString error);

		/**
		 * Slot used for performing actions when CAN worker finishes.
		 */
		void workerFinished();

		/**
		 * Update canMonitor widget with new information.
		 */
		void updateCanMonitor(std::vector<NodeInfo_t> *activeNodesInfo);

	private:

		/**
		 * Perform all necessary signal / slot connections
		 * between canWorker and canThread.
		 */
		void setupThreadConnections();

		/**
		 * Pointer to morus_main_window UI.
		 */
		Ui::MorusMainWindow *ui_;

		/**
		 * Pointer to the CAN node worker. It acts as a wrapper
		 * object for node_handler. Used for safe QThread work.
		 */
		CanWorker *canNodeWorker_ = NULL;

		/**
		 * Thread used by pCan_node_worker. Used for spinning CAN
		 * node.
		 */
		QThread *canThread_ = NULL;
};

#endif // MORUS_MAIN_WINDOW_H
