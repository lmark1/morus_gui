#ifndef MORUS_MAIN_WINDOW_H
#define MORUS_MAIN_WINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QCloseEvent>

#include "UiMorusMainWindow.h"
#include "NodeInfo.h"

// Forward declaration of CanWorker
class CanWorker;

// Forward declaration of NodeHandler
class NodeHandler;

// Forward declaration of MonitorWorker
class MonitorWorker;

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
		 * This method executes when start button is clicked.
		 * Current implementations starts the CAN node.
		 */
		void on_startLocalNodeButton_clicked();

		/**
		 * This method executes when update firmware button is clicked.
		 * It will prompt the user to find a path to the firmware image file.
		 */
		void on_updateFirmwareButton_clicked();

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

		/**
		 * Slot will be activated when user clicks on an item from the
		 * canMonitor widget tree.
		 */
		void onCanMonitorItemClicked(QTreeWidgetItem *item, int column);

	signals:

		/**
		 * Signal that the user has requested a firmware update.
		 */
		void requestFirmwareUpdate(std::string firmawareFilePath, int nodeId);

	private:

		/**
		 * Perform all necessary signal / slot connections
		 * between canWorker and canThread.
		 */
		void setupCanThreadConnections();

		/**
		 * Perform all necessary signal / slot connections
		 * between monitorWorker and monitorThread.
		 */
		void setupMonitorThreadConnections();

		/**
		 * Pauses all local nodes. Prevents them from spinning and performing
		 * actions.
		 */
		void pauseLocalNodes();

		/**
		 * Resumes all local nodes. They continue performing their respective
		 * actions.
		 */
		void resumeLocalNodes();

		/**
		 * Pointer to morus_main_window UI.
		 */
		Ui::MorusMainWindow *ui_;

		/**
		 * Pointer to the CAN node worker. It acts as a wrapper
		 * object for node_handler. Used for safe QThread work.
		 * Runs on canWorkerThread.
		 */
		CanWorker *canNodeWorker_ = NULL;

		/**
		 * Monitor worker. Used for distributing information
		 * about nodes. Run on the monitorWorkerThread.
		 */
		MonitorWorker *monitorWorker_ = NULL;

		/**
		 * Thread used by pCan_node_worker. Used for spinning CAN
		 * node.
		 */
		QThread *canWorkerThread_ = NULL;

		/**
		 * Thread used by CanMonitorWorker. Used for collecting information
		 * about other nodes.
		 */
		QThread *monitorWorkerThread_ = NULL;

		/**
		 * Currently selected node ID in the canNodeMonitor list.
		 * Value -1 signals that no node is currently selected.
		 */
		int currentNodeID_ = -1;
};

#endif // MORUS_MAIN_WINDOW_H
