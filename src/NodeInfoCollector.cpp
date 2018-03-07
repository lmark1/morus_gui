/*
 *	NodeInfoCollector.cpp
 *
 *  Created on: Mar 5, 2018
 *      Author: lmark
 */

#include <iostream>
#include <unistd.h>
#include <uavcan/uavcan.hpp>

#include "NodeInfoCollector.h"

NodeInfoCollector::NodeInfoCollector() { }

NodeInfoCollector::~NodeInfoCollector() { }

void NodeInfoCollector::handleNodeInfoRetrieved(
				NodeID node_id,
				const protocol::GetNodeInfo::Response& node_info) {

	registry_[node_id] = node_info;
}

void NodeInfoCollector::handleNodeInfoUnavailable(
				NodeID node_id) {

	// In this implementation we're using empty struct to indicate that the node info is missing.
	registry_[node_id] = uavcan::protocol::GetNodeInfo::Response();
}

void NodeInfoCollector::handleNodeStatusChange(
				const NodeStatusMonitor::NodeStatusChangeEvent& event) {

	if (event.status.mode == uavcan::protocol::NodeStatus::MODE_OFFLINE) {
		registry_.erase(event.node_id);
	}
}
void NodeInfoCollector::handleNodeStatusMessage(
				const ReceivedDataStructure<protocol::NodeStatus>& msg) {

	auto x = registry_.find(msg.getSrcNodeID());
	if (x != registry_.end()) {
		x->second.status = msg;
	}
}

std::uint8_t NodeInfoCollector::getNumberOfNodes() const
{
	return static_cast<std::uint8_t>(registry_.size());
}

const uavcan::protocol::GetNodeInfo::Response* NodeInfoCollector::getNodeInfo(
		uavcan::NodeID node_id) const {

	auto x = registry_.find(node_id);
	if ( x != registry_.end() ) {

		return &x->second;
	}
	else {

		return nullptr;
	}
}


