/*
 * 	NodeInfoCollector.h
 *
 *	This class will be collecting information from uavcan::NodeInfoRetriever
 *	via the interface uavcan::INodeInfoListener.
 *
 *  Created on: Mar 5, 2018
 *      Author: lmark
 */

#ifndef SRC_NODEINFOCOLLECTOR_H_
#define SRC_NODEINFOCOLLECTOR_H_

#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <uavcan/uavcan.hpp>
#include <uavcan/protocol/node_info_retriever.hpp>

using namespace uavcan;

class NodeInfoCollector final : public uavcan::INodeInfoListener{

	public:

		NodeInfoCollector();
		~NodeInfoCollector();

		// Declaring virtual methods concrete implementation
		// Explanation found in uavcan/protocol/node_infor_retriever.hpp
		void handleNodeInfoRetrieved(
				NodeID node_id,
				const protocol::GetNodeInfo::Response& node_info
				) override;
		void handleNodeInfoUnavailable(NodeID node_id) override;
		void handleNodeStatusChange(
				const NodeStatusMonitor::NodeStatusChangeEvent& event
				) override;
		void handleNodeStatusMessage(
				const ReceivedDataStructure<protocol::NodeStatus>& msg
				) override;

		/**
		 * Return number of known nodes in the registry.
		 */
		std::uint8_t getNumberOfNodes() const;

		/**
		  * Returns a pointer to the node info structure for the given node,
		  * if such node is known. If the node is not known,
		  * a null pointer will be returned.
		  * Note that the pointer may be invalidated afterwards,
		  * so the object MUST be copied if further use is intended.
		  */
		const uavcan::protocol::GetNodeInfo::Response* getNodeInfo(
				uavcan::NodeID node_id) const;

	private:

		/**
		 *	NodeID hash function. For use with unordered_map(...) -
		 *	Custom keys need their own hash function implementation.
		 */
		struct NodeIDHash {

			std::size_t operator()(NodeID nid) const {
		        	return nid.get();
			}
		};

		/**
		 *	Map object used for storing Node responses.
		 */
		std::unordered_map<
			uavcan::NodeID,
			uavcan::protocol::GetNodeInfo::Response,
			NodeIDHash> registry_;

};

#endif /* SRC_NODEINFOCOLLECTOR_H_ */
