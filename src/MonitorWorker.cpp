/*
 * MonitorWorker.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: lmark
 */

#include "MonitorWorker.h"
#include <unordered_map>
#include <QDebug>

#include <uavcan_linux/uavcan_linux.hpp>
#include <uavcan/protocol/node_status_monitor.hpp>

/**
 * Node status monitor class. Used for tracking and monitoring
 * node activity.
 */
class CanNodeMonitor : public uavcan::NodeStatusMonitor
{

	/**
	 * Map registry where node information is saved.
	 */
	std::unordered_map<int, uavcan::protocol::NodeStatus> status_registry_;

	/**
	 * Called when a node becomes online, changes status or goes offline.
	 * Refer to uavcan.protocol.NodeStatus for the offline timeout value.
	 */
	void handleNodeStatusChange(const NodeStatusChangeEvent& event) override
	{
		qDebug() << "CanNodeMonitor::NodeStatusChangeEvent() "
						"- Found a node!";
	}

	/**
	 * Called for every received message uavcan.protocol.NodeStatus
	 * after handleNodeStatusChange(), even
	 * if the status code did not change.
	 */
	void handleNodeStatusMessage(
			const uavcan::ReceivedDataStructure<uavcan::protocol::NodeStatus>&
				msg) override
	{
		qDebug() << "CanNodeMonitor::handleNodeStatusMessage() "
				"- Found a node!";
		status_registry_[msg.getSrcNodeID().get()] = msg;
	}

public:

    explicit CanNodeMonitor(uavcan_linux::NodePtr node)
        : uavcan::NodeStatusMonitor(*node) { };
};

/**
 * Initializes node in passive mode.
 *
 * @param ifaces - String vector of all available interfaces.
 * @param node_name - Node name
 */
static uavcan_linux::NodePtr initNodeInPassiveMode(
    		const std::vector<std::string>& ifaces,
			const std::string& node_name)
{
	auto node = uavcan_linux::makeNode(
			ifaces, node_name.c_str(),
			uavcan::protocol::SoftwareVersion(),
			uavcan::protocol::HardwareVersion());
	node->setModeOperational();
	return node;
}

MonitorWorker::MonitorWorker(QObject *parent) : QObject(parent)
{
}

MonitorWorker::~MonitorWorker()
{
	qDebug() << "MonitorWorker::~MonitorWorker() "
			"- Deleting CanNodeMonitor.";
}

void MonitorWorker::initializeWorker(
		std::string& ifaceName,
		int nodeId)
{
	qDebug() << "MonitorWorker::initializeWorker";

	this->ifaceName_ = ifaceName;
	this->nodeID_ = nodeId;

	working_ = true;
}

void MonitorWorker::process()
{
	qDebug() << "MonitorWorker::process() "
			"- creating passive node.";

	uavcan_linux::NodePtr node;

	try
	{
		// Try initializing the node...
		std::vector<std::string> ifaces;
		ifaces.emplace_back(ifaceName_);

		node = initNodeInPassiveMode(
				ifaces,
				"org.uavcan.linux_app.node_status_monitor");
	}
	catch (const std::exception &ex)
	{

		// Node initialization failed.
		qCritical() << "MonitorWorker::process()"
						"- Failed to initialized node.";

		// Emit error message
		std::string error_message(ex.what());
		emit error(
			QString::fromStdString(
				 "MonitorWorker::process()\n "
				 "Error occurred while creating a new node.\n" +
				 error_message
				 )
			);

		stopWorker();
		return;
	}

	// Start the monitor
	CanNodeMonitor canNodeMonitor(node);
	int resStart = canNodeMonitor.start();
	if (resStart < 0)
	{
		qDebug() << "MonitorWorker::process() "
				"- Monitor did not start successfully";

		stopWorker();
		return;
	}

	// Start working
	qDebug() << "MonitorWorker::process() "
			"- Starting processing";

	while (working_)
	{
		const int res = node->spin(
				uavcan::MonotonicDuration::fromMSec(1000));

		if ( res < 0)
		{
			emit error(
					QString::fromStdString(
							"Error occurred while spinning monitor.")
			);
			stopWorker();
			return;
		}
	}

	qDebug() << "MonitorWorker::process() "
			"- Finished processing, deleting monitor";

	stopWorker();
}

void MonitorWorker::stopWorker()
{
	qDebug() << "MonitorWorker::stopWorker() "
			"- Stopping monitor worker.";
	working_ = false;
	emit finished();
}

