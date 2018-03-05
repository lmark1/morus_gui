/*
 *	NodeInfoCollector.cpp
 *
 *  Created on: Mar 5, 2018
 *      Author: lmark
 */

#include "NodeInfoCollector.h"

NodeInfoCollector::NodeInfoCollector() {
	// TODO Auto-generated constructor stub

}

NodeInfoCollector::~NodeInfoCollector() {
	// TODO Auto-generated destructor stub
}

void NodeInfoCollector::handleNodeInfoRetrieved(
				NodeID node_id,
				const protocol::GetNodeInfo::Response& node_info) {

}

void NodeInfoCollector::handleNodeInfoUnavailable(
				NodeID node_id) {

}

void NodeInfoCollector::handleNodeStatusMessage(
				const ReceivedDataStructure<protocol::NodeStatus>& msg) {

}


