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

/**
 * Node status monitor class. Used for tracking and monitoring
 * node activity.
 */
class CanNodeMonitor : public uavcan::NodeStatusMonitor
{

	/**
	 * Timer object used for periodic function calls.
	 */
	uavcan_linux::TimerPtr timer_;

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

	std::vector<NodeInfo_t> activeNodesInfo;

    explicit CanNodeMonitor(uavcan_linux::NodePtr node)
        : uavcan::NodeStatusMonitor(*node) { };

    void checkForNodes()
	{

    	qDebug() << "checkForNodes() "
    			"- Checking for nodes.";
		activeNodesInfo.clear();
		NodeInfo_t tempNodeInfo;

		for (unsigned i = 1; i <= uavcan::NodeID::Max; i++)
		{
			if (isNodeKnown(i))
			{
				NodeStatus status = getNodeStatus(i);
				// Collect node information
				tempNodeInfo.id = int(i);
				tempNodeInfo.uptime = status_registry_[i].uptime_sec;
				tempNodeInfo.health = status.health;
				tempNodeInfo.mode = status.mode;
				tempNodeInfo.vendorSpecificStatusCode =
						status_registry_[i].vendor_specific_status_code;

				activeNodesInfo.emplace_back(tempNodeInfo);
			}
		}
	};
};

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
		mutex_.lock();
		// TODO(lmark): protect this spin call with try - catch.
		const int res = node->spin(
				uavcan::MonotonicDuration::fromMSec(1000));

		canNodeMonitor.checkForNodes();
		emit foundNodes(&canNodeMonitor.activeNodesInfo);
		if ( res < 0)
		{
			emit error(
					QString::fromStdString(
							"Error occurred while spinning monitor.")
			);
			working_ = false;
		}
		mutex_.unlock();
	}

	qDebug() << "MonitorWorker::process() "
			"- Finished processing, deleting monitor";

	stopWorker();
	emit finished();
}

void MonitorWorker::stopWorker()
{
	while (mutex_.tryLock()) {
		qDebug() << "MonitorWorker::stopWorker() "
				"- Trying to lock mutex.";
	}

	qDebug() << "MonitorWorker::stopWorker() "
			"- Stopping monitor worker.";
	working_ = false;

	mutex_.unlock();
}

bool MonitorWorker::isRunning()
{
//	while (mutex_.tryLock())
//	{
//		qDebug() << "MonitorWorker::isRunning() "
//				"- Trying to lock mutex.";
//	}
	bool value = working_;
//	mutex_.unlock();

	return value;
}

