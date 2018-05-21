#include <QDebug>

#include <string>
#include <unordered_map>

#include "MorusMainWindow.h"
#include "CanWorker.h"
#include "NodeHandler.h"
#include "MonitorWorker.h"
#include "CanUpdaterWindow.h"

const QString DEFAULT_IFACE_NAME = "can0";
const int DEFAULT_NODE_ID = 127;

/**
 * Returns string representation of node health.
 *
 * @param health - integer representation of node health
 */
static std::string healthToString(const std::uint8_t health);

/**
 * Returns string representation of node mode.
 *
 * @param mode - integer representation of node mode
 */
static std::string modeToString(const std::uint8_t mode);

MorusMainWindow::MorusMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MorusMainWindow)
{
    ui_->setupUi(this);
    
    qRegisterMetaType<std::vector<uavcan::protocol::param::GetSet::Response>>(
    		"std::vector<uavcan::protocol::param::GetSet::Response>");
    // Set initial node-id and node interface values
    ui_->localNodeIDSpinBox->setValue(DEFAULT_NODE_ID);
    ui_->localNodeIDSpinBox->setMaximum(DEFAULT_NODE_ID);
    ui_->canIfaceNamePlainTextEdit->setPlainText(DEFAULT_IFACE_NAME);

    // Initialize monitor thread
	monitorWorkerThread_ = new QThread();

	// Initialize monitorWorker
	monitorWorker_ = new MonitorWorker();

	// Setup QThread object
	monitorWorker_->moveToThread(monitorWorkerThread_);

	// Setup signal / slot connections
	setupMonitorThreadConnections();

	// Start monitor
	std::string iface(DEFAULT_IFACE_NAME.toStdString());
	monitorWorker_->initializeWorker(iface, 1);
	monitorWorkerThread_->start();

	// TODO(lmark): disable update / restart buttons until user selects a node

	// Connect canMonitor click signal
	connect(ui_->canNodeMonitor,
			SIGNAL( itemClicked(QTreeWidgetItem*, int) ),
			this,
			SLOT ( onCanMonitorItemClicked(QTreeWidgetItem*, int) ));
}

MorusMainWindow::~MorusMainWindow()
{
	qDebug() << "MorusMainWindow::~MorusMainWindow "
				"- Stopping CanWorker.";
	if (canNodeWorker_ != NULL && canNodeWorker_->isRunning()) {
		canNodeWorker_->stopWorker();
	}

	qDebug() << "MorusMainWindow::~MorusMainWindow "
			"- Stopping monitorWorker";
	if (monitorWorker_ != NULL && monitorWorker_->isRunning()) {
		monitorWorker_->stopWorker();
	}

	monitorWorkerThread_->terminate();
	monitorWorkerThread_->wait();

	if (canWorkerThread_)
	{
		canWorkerThread_->terminate();
		canWorkerThread_->wait();
	}

	qDebug() << "MorusMainWindow::~MorusMainWindow "
				"- deleting UI.";
	delete canNodeWorker_;
	delete monitorWorker_;
	delete ui_;
}

void MorusMainWindow::on_startLocalNodeButton_clicked()
{
	// Get node id and interface name from GUI
    int node_id = ui_->localNodeIDSpinBox->value();
    std::string iface_name = ui_->
    		canIfaceNamePlainTextEdit->
			toPlainText().toStdString();

    /*
     * Can worker needs to be initialized inside the start button
     * callback. canWorker gets deleted each time
     * they finish working.
     */
    canWorkerThread_ = new QThread();
    canNodeWorker_ = new CanWorker();
    canNodeWorker_->moveToThread(canWorkerThread_);
    setupCanThreadConnections();

    // Initialize node worker
    canNodeWorker_->initializeWorker(iface_name, node_id);

    // Start the thread
    canWorkerThread_->start();

    // Disable start button after generating a node
    ui_->startLocalNodeButton->setEnabled(false);
}

void MorusMainWindow::on_updateFirmwareButton_clicked()
{
	qDebug() << "MorusMainWindow::on_updateFirmwareButton_clicked()";

	// Check if a node is selected for firmware update
	if (currentNodeID_ == -1)
	{
		generateMessageBox("Please select a node first.");
		return;
	}

	// Check if user selected the default node (the one performing the update)
	if (currentNodeID_ == DEFAULT_NODE_ID)
	{
		generateMessageBox("Node with ID performs "
				"firmware update. Please select another node");
		return;
	}

	// IMPORTANT: Pause all local nodes before
	pauseLocalNodes();

	// Prompt the user with updater dialog
	CanUpdaterWindow updaterDialog;
	int dialogResult = updaterDialog.exec();

	// Return if dialog is rejected
	if (dialogResult == QDialog::Rejected)
	{
		resumeLocalNodes();
		return;
	}

	qDebug() << updaterDialog.getFirmwarePath().c_str();
	// IMPORTANT: Resume all local nodes
	resumeLocalNodes();

	emit requestFirmwareUpdate(
			updaterDialog.getFirmwarePath(),
			currentNodeID_);
}

void MorusMainWindow::on_fetchParamButton_clicked()
{
	qDebug() << "MorusMainWindow::on_fetchParamButton_clicked()";
	if (canNodeWorker_ == NULL)
	{
		generateMessageBox("Please initialize the local node worker first");
		return;
	}

	// Check if a node is selected for firmware update
	if (currentNodeID_ == -1)
	{
		generateMessageBox("Please select a node first.");
		return;
	}

	canNodeWorker_->readParameterSignal(currentNodeID_);
}

void MorusMainWindow::workerFinished()
{
	qDebug() << "MorusMainWindow::workerFinished()";
	// Re-enable the start button when current current CAN thread is finished
	ui_->startLocalNodeButton->setEnabled(true);
}

void MorusMainWindow::handleErrorMessage(QString error)
{
	// Generate message box about the error
	this->generateMessageBox(error.toStdString());
}

void MorusMainWindow::updateCanMonitor(
		std::vector<NodeInfo_t> *activeNodesInfo)
{
	qDebug() << "MorusMainWindow::updateCanMonitor() "
			"- found nodes " << activeNodesInfo->size();
	// Check all the recieved nodes
	for (uint8_t i = 0; i < activeNodesInfo->size(); i++)
	{
		bool alreadyExists = false;

		// Go through all existing canMonitor items and
		// check if this one exists
		for (int j = 0; j < ui_->canNodeMonitor->topLevelItemCount(); j++)
		{
			// Check if the node already exits in the node monitor
			if (ui_->canNodeMonitor->topLevelItem(j)->text(0).toInt()
					== activeNodesInfo->at(i).id)
			{
				alreadyExists = true;

				qDebug() << "MorusMainWindow::updateCanMonitor() "
						"- updating existing node";
				// Update existing canMonitor text
				ui_->canNodeMonitor->topLevelItem(j)->setText(
						0,
						std::to_string(
								int(activeNodesInfo->at(i).id)).c_str());
				ui_->canNodeMonitor->topLevelItem(j)->setText(
						1,
						"N/A");
				ui_->canNodeMonitor->topLevelItem(j)->setText(
						2,
						modeToString(activeNodesInfo->at(i).mode).c_str());
				ui_->canNodeMonitor->topLevelItem(j)->setText(
						3,
						healthToString(activeNodesInfo->at(i).health).c_str());
				ui_->canNodeMonitor->topLevelItem(j)->setText(
						4,
						std::to_string(activeNodesInfo->at(i).uptime).c_str());
				ui_->canNodeMonitor->topLevelItem(j)->setText(
						5,
						std::to_string(activeNodesInfo->at(i).
								vendorSpecificStatusCode).c_str());

				// Node is found break out of the loop
				break;
			}
		} // canMonitor->size() loop

		// If there is no such nodes add a new node
		if (!alreadyExists)
		{
			qDebug() << "MorusMainWindow::updateCanMonitor() "
					"- Adding new node";
			QTreeWidgetItem *newItem = new QTreeWidgetItem();
			newItem->setText(
					0,
					std::to_string(
							int(activeNodesInfo->at(i).id)).c_str());
			newItem->setText(
					1,
					"N/A");
			newItem->setText(
					2,
					modeToString(activeNodesInfo->at(i).mode).c_str());
			newItem->setText(
					3,
					healthToString(activeNodesInfo->at(i).health).c_str());
			newItem->setText(
					4,
					std::to_string(activeNodesInfo->at(i).uptime).c_str());
			newItem->setText(
					5,
					std::to_string(activeNodesInfo->at(i).
							vendorSpecificStatusCode).c_str());

			// Add new item to canMonitor
			ui_->canNodeMonitor->addTopLevelItem(newItem);
		}

	} // activeNodes.size() loop

	// Delete any existing items if necessary
	if (ui_->canNodeMonitor->topLevelItemCount() >
		int(activeNodesInfo->size()))
	{
		// Go through all existing items on the list
		for (int i = 0; i < ui_->canNodeMonitor->topLevelItemCount(); i++)
		{
			bool alreadyExists = false;

			for (uint8_t j = 0; j < activeNodesInfo->size(); j++)
			{
				// Check if the node already exits in the node monitor
				if (ui_->canNodeMonitor->topLevelItem(i)->text(0).toInt()
						== activeNodesInfo->at(j).id)
				{
					alreadyExists = true;
					break;
				}

				// Delete it if it does not exist
				if (!alreadyExists)
				{
					qDebug() << "MorusMainWindow::updateCanMonitor() "
							"- deleting existing node.";
					delete ui_->canNodeMonitor->takeTopLevelItem(i);
				}

			} // activeNodes list loop
		} // canMonitor list loop
	} // If clause
}

void MorusMainWindow::generateMessageBox(std::string message)
{
    // Display message box
    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString(message));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MorusMainWindow::setupCanThreadConnections()
{
	// **** CanWorker / Thread connections ****
	connect(canNodeWorker_, // Connect worker error()...
			SIGNAL( error(QString) ),
			this, 			// To morus_main_window errorString()
			SLOT( handleErrorMessage(QString) ));

	connect(canWorkerThread_, 	// Connect thread started()...
			SIGNAL( started() ),
			canNodeWorker_, // to worker process()
			SLOT( process() ));

	connect(canNodeWorker_, // Connect worker finished()...
			SIGNAL( finished() ),
			canWorkerThread_,		// to thread quit() - exits the thread
			SLOT( quit() ));

	connect(canNodeWorker_, // Connect worker finished()...
			SIGNAL( finished() ),
			this,			// to morus_window workerFinished()
			SLOT( workerFinished() ));

	// Connect firmware update request
	connect(this,
			SIGNAL( requestFirmwareUpdate(std::string, int) ),
			canNodeWorker_,
			SLOT( firmwareUpdateRequested(std::string, int) ));

	// Connect signal for updating parameters
	connect(canNodeWorker_,
			SIGNAL( nodeParametersFound(
					std::vector
					<uavcan::protocol::param::GetSet::Response>) ),
			this,
			SLOT( updateNodeParameters(
					std::vector
					<uavcan::protocol::param::GetSet::Response>) ));

}

void MorusMainWindow::setupMonitorThreadConnections()
{
	// **** CanMonitor / Thread connections ****
	// Connect errors
	connect(monitorWorker_,
			SIGNAL( error(QString) ),
			this,
			SLOT( handleErrorMessage(QString) ));
	// Connect started signal
	connect(monitorWorkerThread_,
			SIGNAL( started() ),
			monitorWorker_,
			SLOT( process() ));
	// Connect finished signal
	connect(monitorWorker_,
			SIGNAL( finished() ),
			monitorWorkerThread_,
			SLOT( quit() ));

	connect(monitorWorker_, // Connect information found signal...
			SIGNAL( foundNodes(std::vector<NodeInfo_t>*) ),
			this, // ... to the update monitor slot.
			SLOT( updateCanMonitor(std::vector<NodeInfo_t>*) ));
}

void MorusMainWindow::pauseLocalNodes()
{
	// Pause all available local nodes
	if (monitorWorker_ != NULL) { monitorWorker_->pauseWorker(); }
	if (canNodeWorker_ != NULL) { canNodeWorker_->pauseWorker(); }
}

void MorusMainWindow::resumeLocalNodes()
{
	// Resume all available local nodes
	if (monitorWorker_ != NULL) { monitorWorker_->resumeWorker(); }
	if (canNodeWorker_ != NULL) { canNodeWorker_->resumeWorker(); }
}

void MorusMainWindow::onCanMonitorItemClicked(
		QTreeWidgetItem *item, int column)
{
	currentNodeID_ = item->text(0).toInt();
	qDebug() << "MorusMainWindow::onCanMonitorItemClicked() - "
			<< item->text(0).toStdString().c_str();
}

void MorusMainWindow::updateNodeParameters(
				std::vector
				<uavcan::protocol::param::GetSet::Response> params)
{
	qDebug() << "MorusMainWindow::updateNodeParameters() - "
			<< params.size();
}

static std::string healthToString(const std::uint8_t health)
{

	// TODO(lmark): Put these 2 conversion methods somewhere where it makes
	// More sense
	static const std::unordered_map<std::uint8_t, std::string> map
	{
	   { uavcan::protocol::NodeStatus::HEALTH_OK,       "OK" },
	   { uavcan::protocol::NodeStatus::HEALTH_WARNING,  "WARNING"},
	   { uavcan::protocol::NodeStatus::HEALTH_ERROR,    "ERROR"},
	   { uavcan::protocol::NodeStatus::HEALTH_CRITICAL, "CRITICAL"}
	};
	try
	{
	   return map.at(health);
	}
	catch (std::out_of_range&)
	{
	   return std::to_string(health);
	}
}

static std::string modeToString(const std::uint8_t mode)
{
	// TODO(lmark): Put these 2 conversion methods somewhere where it makes
	// More sense
	static const std::unordered_map<std::uint8_t, std::string> map
	{
		{ uavcan::protocol::NodeStatus::MODE_OPERATIONAL,     "OPERATIONAL"},
		{ uavcan::protocol::NodeStatus::MODE_INITIALIZATION,  "INITIALIZATION"},
		{ uavcan::protocol::NodeStatus::MODE_MAINTENANCE,     "MAINTENANCE"},
		{ uavcan::protocol::NodeStatus::MODE_SOFTWARE_UPDATE, "SOFTWARE_UPDATE"},
		{ uavcan::protocol::NodeStatus::MODE_OFFLINE,         "OFFLINE" }
	};
	try
	{
		return map.at(mode);
	}
	catch (std::out_of_range&)
	{
		return std::to_string(mode);
	}
}

