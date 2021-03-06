#ifndef MORUS_MAIN_WINDOW_H
#define MORUS_MAIN_WINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QCloseEvent>

#include <uavcan/protocol/param/GetSet.hpp>
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

/**
 * Define parameter namespace.
 */
namespace param_ns = uavcan::protocol::param;

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

		/**
		 * Method will execute when fetchParamButton is clicked. It will
		 * fetch all parameters from selected node and add them to the
		 * parameter list.
		 */
		void on_fetchParamButton_clicked();

		/**
		 * Method will execute when Store Parameters button is clicked. It will
		 * store all parameters in the Parameter tree widget in the flash
		 * memory.
		 */
		void on_storeParamButton_clicked();

		/**
		 * Method will execute when Update All button is clicked. It will
		 * store changed parameters on the selected node.
		 */
		void on_updateButton_clicked();

		/**
		 * Method will execute when loadParametersButton is clicked. It will
		 * prompt user to select an appropriate YAML parameter configuration
		 * file.
		 * Parameters found in the configuration file will be updated
		 * to the local parameters or appended to the list as new
		 * parameters.
		 */
		void on_loadParametersButton_clicked();

		/**
		 * Method will execute when exportParametersButton is clicked. If there
		 * are any parameters in the parameter list it will attempt to export
		 * them.
		 * User will be prompted to select where the parameters will be
		 * stored.
		 */
		void on_exportParametersButton_clicked();

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

		/**
		 * This slot will be activated when user double clicks on a parameter
		 * in the list. User will be prompted to edit that parameter.
		 * If user decides to change that parameter, the value in the list
		 * will change and parameter will be marked for upload.
		 */
		void onParamListItemDoubleClicked(QTreeWidgetItem *item, int column);

		/**
		 * This slot will be activated when item contents changed. When
		 * value changes color the whole item red.
		 */
		void onParamListItemChanged(QTreeWidgetItem *item, int column);

		/**
		 *	Update node parameter list with the given set of parameters.
		 *
		 *	@param params
		 */
		void updateNodeParameters(
				std::vector
				<uavcan::protocol::param::GetSet::Response> params);

	signals:

		/**
		 * Signal emitted when user has requested a firmware update.
		 *
		 * @param firmwareFilePath - Path to the binary file
		 * @param nodeId
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
		 * Check if parameter is valid. Checks if type matches the found
		 * value.
		 */
		bool isParamValid(QTreeWidgetItem param);

		/**
		 * Add parameter to parameter list.
		 */
		void addItemToTreeWidget(QTreeWidgetItem *param);

		/**
		 * Add parameter to changed items. It will be marked for storage
		 * Also check if it is already added, and just change value.
		 */
		void addToChangedParams(QTreeWidgetItem param);

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

		/**
		 * This list is used to store changed items on the parameter list.
		 * It will be propagated towards NodeHandler when store parameter
		 * command is requested.
		 */
		std::vector<QTreeWidgetItem> changedItems_;
};

#endif // MORUS_MAIN_WINDOW_H
