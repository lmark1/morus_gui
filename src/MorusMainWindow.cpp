#include <QDebug>
#include <string>
#include <unordered_map>
#include <QtGui>
#include <QInputDialog>
#include <QLineEdit>

#include "MorusMainWindow.h"
#include "CanWorker.h"
#include "NodeHandler.h"
#include "MonitorWorker.h"
#include "CanUpdaterWindow.h"

const QString DEFAULT_IFACE_NAME = "can0";
const int DEFAULT_NODE_ID = 127;

// Define parameter type string constants
const QString QSTRING_INT("int");
const QString QSTRING_UINT8("uint8");
const QString QSTRING_STRING("string");
const QString QSTRING_FLOAT("float");

// Define parameter list column index constants
const uint8_t NAME_INDEX = 0;
const uint8_t TYPE_INDEX = 1;
const uint8_t VALUE_INDEX = 2;
const uint8_t DEFAULT_VALUE_INDEX = 4;
const uint8_t MIN_VALUE_INDEX = 5;
const uint8_t MAX_VALUE_INDEX = 6;

// Total number of columns
const uint8_t PARAM_COLUMN_COUNT = 7;

// Comparison delta for detecting parameter change
const double COMP_EPS = 1e-6;

// Initialize color constants
QBrush GRAY_COLOR(QColor(230, 230, 230, 255));
QBrush WHITE_COLOR(QColor(255, 255, 255, 255));
QBrush RED_COLOR(QColor(255, 0, 0, 200));

// Forward declaration of QLineEdit - needed for IDE syntax errors
class QLineEdit;

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
    
    // Register this types for signaling parameters
    qRegisterMetaType<std::vector<uavcan::protocol::param::GetSet::Response>>(
    		"std::vector<uavcan::protocol::param::GetSet::Response>");
    qRegisterMetaType<std::vector<QTreeWidgetItem>>(
    		"std::vector<QTreeWidgetItem>");

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

	// Connect parameter list double - click signal
	connect(ui_->parameterTreeWidget,
			SIGNAL( itemDoubleClicked(QTreeWidgetItem*, int) ),
			this,
			SLOT( onParamListItemDoubleClicked(QTreeWidgetItem*, int) ));
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

	// Check if user selected the local node (the one performing the update)
	if (currentNodeID_ == ui_->localNodeIDSpinBox->value())
	{
		generateMessageBox("Node with selected ID performs "
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

	// Check if current ID selected is the same as the local node
	if (currentNodeID_ == ui_->localNodeIDSpinBox->value())
	{
		generateMessageBox("Node with selected ID performs parameter "
				"upload / download. Please select another node.");
		return;
	}

	// Clear local changed item list
	changedItems_.clear();
	canNodeWorker_->readParameterSignal(currentNodeID_);
}

void MorusMainWindow::on_storeParamButton_clicked()
{
	qDebug() << "MorusMainWindow::on_storeParamButton_clicked() - "
			<< changedItems_.size();

	// Check if a node is selected for firmware update
	if (currentNodeID_ == -1)
	{
		generateMessageBox("Please select a node first.");
		return;
	}

	// Check if current ID selected is the same as the local node
	if (currentNodeID_ == ui_->localNodeIDSpinBox->value())
	{
		generateMessageBox("Node with selected ID performs parameter "
				"upload / download. Please select another node.");
		return;
	}

	// Check if changed param list is empty
	if (changedItems_.size() == 0)
	{
		generateMessageBox("No parameters changed.");
		return;
	}

	canNodeWorker_->storeParametersRequested(changedItems_, currentNodeID_);
	changedItems_.clear();
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
	//qDebug() << "MorusMainWindow::updateCanMonitor() "
	//		"- found nodes " << activeNodesInfo->size();
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

				//qDebug() << "MorusMainWindow::updateCanMonitor() "
				//		"- updating existing node";
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
			//qDebug() << "MorusMainWindow::updateCanMonitor() "
			//		"- Adding new node";
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
	connect(canNodeWorker_, 		// Connect worker error()...
			SIGNAL( error(QString) ),
			this, 					// To morus_main_window errorString()
			SLOT( handleErrorMessage(QString) ));

	connect(canWorkerThread_, 		// Connect thread started()...
			SIGNAL( started() ),
			canNodeWorker_, 		// to worker process()
			SLOT( process() ));

	connect(canNodeWorker_, 		// Connect worker finished()...
			SIGNAL( finished() ),
			canWorkerThread_,		// to thread quit() - exits the thread
			SLOT( quit() ));

	connect(canNodeWorker_, 		// Connect worker finished()...
			SIGNAL( finished() ),
			this,					// to morus_window workerFinished()
			SLOT( workerFinished() ));

	// Connect firmware update request
	connect(this,
			SIGNAL( requestFirmwareUpdate(std::string, int) ),
			canNodeWorker_,
			SLOT( firmwareUpdateRequested(std::string, int) ));

	// Connect signal for fetching parameters
	connect(canNodeWorker_,
			SIGNAL( nodeParametersFound(
					std::vector
					<uavcan::protocol::param::GetSet::Response>) ),
			this,
			SLOT( updateNodeParameters(
					std::vector
					<uavcan::protocol::param::GetSet::Response>) ));

	// Connect signal for storing parameters
	connect(this,
			SIGNAL( storeParametersRequest(std::vector<QTreeWidgetItem>,int) ),
			canNodeWorker_,
			SLOT( storeParametersRequested(std::vector<QTreeWidgetItem>,int) ));
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

void MorusMainWindow::onParamListItemDoubleClicked(
		QTreeWidgetItem *item, int column)
{
	qDebug() << "MorusMainWindow::onParamListItemDoubleClicked()";
	bool ok_pressed = false;

	// TODO(lmark): Add index or name list for parameters that were changed
	// TODO(lmark): Make a row - coloring function

	// Check which type user pressed
	// INTEGER
	if (QString::compare(
			item->text(TYPE_INDEX),
			QSTRING_INT,
			Qt::CaseSensitive) == 0)
	{
		// Find out what minimum value to use
		int min_value = item->text(MIN_VALUE_INDEX).toInt();
		if (QString::compare(item->text(MIN_VALUE_INDEX), "-") == 0)
			min_value = -INT32_MAX;

		// Find out what maximum value to use
		int max_value = item->text(MAX_VALUE_INDEX).toInt();
		if (QString::compare(item->text(MAX_VALUE_INDEX), "-") == 0)
			max_value = INT32_MAX;

		// Display dialog
		int desired_value = QInputDialog::getInt(
				this,
				tr(item->text(NAME_INDEX).toStdString().c_str()),
				tr("Value:"),
				item->text(VALUE_INDEX).toInt(),
				min_value,
				max_value,
				1, &ok_pressed);

		// Check if value is different
		if (ok_pressed &&
			desired_value != item->text(VALUE_INDEX).toInt())
		{
			item->setText(
					VALUE_INDEX,
					QString::fromStdString(std::to_string(desired_value)));
			for (int k = 0; k < PARAM_COLUMN_COUNT; k++)
				item->setBackground(k, RED_COLOR);
		}
	}
	// FLOAT
	else if (QString::compare(
				item->text(TYPE_INDEX),
				QSTRING_FLOAT,
				Qt::CaseSensitive) == 0)
	{
		// Find out what minimum value to use
		double min_value = item->text(MIN_VALUE_INDEX)
				.replace(",", ".")
				.toDouble();
		if (QString::compare(item->text(MIN_VALUE_INDEX), "-") == 0)
			min_value = -DBL_MAX;

		// Find out what maximum value to use
		double max_value = item->text(MAX_VALUE_INDEX)
				.replace(",", ".")
				.toDouble();
		if (QString::compare(item->text(MAX_VALUE_INDEX), "-") == 0)
			max_value = DBL_MAX;

		// Display dialog
		double desired_value = QInputDialog::getDouble(
				this,
				tr(item->text(NAME_INDEX).toStdString().c_str()),
				tr("Value:"),
				item->text(VALUE_INDEX).replace(",", ".").toDouble(),
				min_value,
				max_value,
				5, &ok_pressed);

		// Check if value is different
		if (ok_pressed &&
			fabs(desired_value - item->text(VALUE_INDEX).toDouble()) > COMP_EPS)
		{
			item->setText(
					VALUE_INDEX,
					QString::fromStdString(std::to_string(desired_value)));
			for (int k = 0; k < PARAM_COLUMN_COUNT; k++)
				item->setBackground(k, RED_COLOR);
		}
	}
	// UINT8
	else if (QString::compare(
				item->text(TYPE_INDEX),
				QSTRING_UINT8,
				Qt::CaseSensitive) == 0)
	{
		// Find out what minimum value to use
		int min_value = item->text(MIN_VALUE_INDEX).toInt();
		if (QString::compare(item->text(MIN_VALUE_INDEX), "-") == 0)
			min_value = -INT8_MAX;

		// Find out what maximum value to use
		int max_value = item->text(MAX_VALUE_INDEX).toInt();
		if (QString::compare(item->text(MAX_VALUE_INDEX), "-") == 0)
			max_value = INT8_MAX;

		// Display dialog
		int desired_value = QInputDialog::getInt(
				this,
				tr(item->text(NAME_INDEX).toStdString().c_str()),
				tr("Value:"),
				item->text(VALUE_INDEX).toInt(),
				min_value,
				max_value,
				1, &ok_pressed);

		// Check if value is different
		if (ok_pressed &&
			desired_value != item->text(VALUE_INDEX).toInt())
		{
			item->setText(
					VALUE_INDEX,
					QString::fromStdString(std::to_string(desired_value)));
			for (int k = 0; k < PARAM_COLUMN_COUNT; k++)
				item->setBackground(k, RED_COLOR);
		}
	}
	// STRING
	else if (QString::compare(
				item->text(TYPE_INDEX),
				QSTRING_STRING,
				Qt::CaseSensitive) == 0)
	{
		// TODO(lmark): Unable to check this functionality due to the lack of STRING parameters
		QString desired_text = QInputDialog::getText(
				this,
				tr(item->text(NAME_INDEX).toStdString().c_str()),
                tr("Value:"),
				QLineEdit::Normal,
                item->text(VALUE_INDEX),
				&ok_pressed);

		// Check if value is different
		if (ok_pressed &&
			QString::compare(
					desired_text,
					item->text(VALUE_INDEX),
					Qt::CaseSensitive) != 0)
		{
			item->setText(
					VALUE_INDEX,
					desired_text);
			for (int k = 0; k < PARAM_COLUMN_COUNT; k++)
				item->setBackground(k, RED_COLOR);
		}
	}
	// ... unknown type - throw exception
	else
	{
		throw runtime_error("Trying to change unknown parameter type...");
	}

	// Push items to changed list
	changedItems_.push_back(*item);
}

void MorusMainWindow::updateNodeParameters(
				std::vector
				<uavcan::protocol::param::GetSet::Response> params)
{
	qDebug() << "MorusMainWindow::updateNodeParameters() - "
			<< params.size();

	QTreeWidget *paramTree = ui_->parameterTreeWidget;

	// Clear current parameter configuration
	paramTree->clear();

	for (int index = params.size()-1; index >= 0; index--)
	{
		QTreeWidgetItem *paramItem = new QTreeWidgetItem();

		std::string paramName(params[index].name.c_str());
		paramItem->setText(NAME_INDEX, QString::fromStdString(paramName));

		// Color columns differently
		if (index % 2 == 0)
		{ for (int k=0; k<7; k++) paramItem->setBackground(k, GRAY_COLOR); }
		else
		{ for (int k=0; k<7; k++) paramItem->setBackground(k, WHITE_COLOR); }

		// Check parameter type and set fields accordingly
		// INTEGER
		if (params[index].value.is(param_ns::Value::Tag::integer_value))
		{
			// Set int type and value
			paramItem->setText(TYPE_INDEX, QSTRING_INT);
			paramItem->setText(VALUE_INDEX,
					QString::fromStdString(std::to_string
					(
						params[index].value.to
						<param_ns::Value::Tag::integer_value>()
					).c_str()));
			paramItem->setText(DEFAULT_VALUE_INDEX,
					QString::fromStdString(std::to_string
					(
						params[index].default_value.to
						<param_ns::Value::Tag::integer_value>()
					).c_str()));

			// Initialize min values
			if (params[index].min_value.is(param_ns::NumericValue::Tag::empty))
				paramItem->setText(MIN_VALUE_INDEX, QString("-"));
			else
				paramItem->setText(MIN_VALUE_INDEX,
						QString::fromStdString(std::to_string
						(
							params[index].min_value.to
							<param_ns::NumericValue::Tag::integer_value>()
						).c_str()));

			// Initialize max values
			if (params[index].max_value.is(param_ns::NumericValue::Tag::empty))
				paramItem->setText(MAX_VALUE_INDEX, QString("-"));
			else
				paramItem->setText(MAX_VALUE_INDEX,
						QString::fromStdString(std::to_string
						(
							params[index].max_value.to
							<param_ns::NumericValue::Tag::integer_value>()
						).c_str()));
		}
		// REAL
		else if (params[index].value.is(param_ns::Value::Tag::real_value))
		{
			paramItem->setText(TYPE_INDEX, QSTRING_FLOAT);
			paramItem->setText(VALUE_INDEX,
					QString::fromStdString(std::to_string
					(
						params[index].value.to
						<param_ns::Value::Tag::real_value>()
					).c_str()));
			paramItem->setText(DEFAULT_VALUE_INDEX,
					QString::fromStdString(std::to_string
					(
						params[index].default_value.to
						<param_ns::Value::Tag::real_value>()
					).c_str()));

			// Set minimum value
			if (params[index].min_value.is(param_ns::NumericValue::Tag::empty))
				paramItem->setText(MIN_VALUE_INDEX, "-");
			else
				paramItem->setText(MIN_VALUE_INDEX,
						std::to_string
						(
							params[index].min_value.to
							<param_ns::NumericValue::Tag::real_value>()
						).c_str());

			// Set maximum value
			if (params[index].max_value.is(param_ns::NumericValue::Tag::empty))
				paramItem->setText(MAX_VALUE_INDEX, "-");
			else
				paramItem->setText(MAX_VALUE_INDEX,
						std::to_string
						(
							params[index].max_value.to
							<param_ns::NumericValue::Tag::real_value>()
						).c_str());
		}
		// STRING
		else if (params[index].value.is(param_ns::Value::Tag::string_value))
		{
			paramItem->setText(TYPE_INDEX, QSTRING_STRING);
			paramItem->setText(VALUE_INDEX,
					QString::fromStdString(std::string
					(
						params[index].value.to
						<param_ns::Value::Tag::string_value>().c_str()
					)));
			paramItem->setText(DEFAULT_VALUE_INDEX,
					QString::fromStdString
					(
						std::string(params[index].default_value.to
						<param_ns::Value::Tag::string_value>().c_str()
					)));
			paramItem->setText(MIN_VALUE_INDEX, "-");
			paramItem->setText(MAX_VALUE_INDEX, "-");
		}
		// BOOLEAN
		else if (params[index].value.is(param_ns::Value::Tag::boolean_value))
		{
			paramItem->setText(TYPE_INDEX, QSTRING_UINT8);
			paramItem->setText(VALUE_INDEX,
					QString::fromStdString(std::to_string
					(
						params[index].value.to
						<param_ns::Value::Tag::boolean_value>()
					).c_str()));
			paramItem->setText(DEFAULT_VALUE_INDEX,
					QString::fromStdString(std::to_string(
						params[index].default_value.to
						<param_ns::Value::Tag::boolean_value>()
					).c_str()));

			if (params[index].min_value.is(param_ns::NumericValue::Tag::empty))
				paramItem->setText(MIN_VALUE_INDEX, "-");
			else
				paramItem->setText(MIN_VALUE_INDEX,
						std::to_string
						(
							params[index].min_value.to
							<param_ns::NumericValue::Tag::integer_value>()
						).c_str());

			if (params[index].max_value.is(param_ns::NumericValue::Tag::empty))
				paramItem->setText(MAX_VALUE_INDEX, "-");
			else
				paramItem->setText(MAX_VALUE_INDEX,
						std::to_string
						(
							params[index].max_value.to
							<param_ns::NumericValue::Tag::integer_value>()
						).c_str());
		}

		paramTree->insertTopLevelItem(0, paramItem);
	} // Param index FOR loop

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

