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

#include <unordered_map>
#include <uavcan/protocol/node_info_retriever.hpp>

using namespace uavcan;

/**
 *	NodeID hash function. For use with unordered_map(...) -
 *	Custom keys need their own hash function implementation.
 */
struct NodeIDHash {

	std::size_t operator()(NodeID nid) const {
			return nid.get();
	}
};

class NodeInfoCollector final : public uavcan::INodeInfoListener{

	public:

		NodeInfoCollector();
		~NodeInfoCollector();

		/**
		 * Called when a response to GetNodeInfo request is received.
		 * This happens shortly after the node restarts or
		 * becomes online for the first time.
		 *
		 * @param node_id   Node ID of the node
		 * @param response  Node info struct
		 */
		void handleNodeInfoRetrieved(
				NodeID node_id,
				const protocol::GetNodeInfo::Response& node_info
				) override;

		/**
		 * Called when the retriever decides that the node does not
		 * support the GetNodeInfo service. This method will never be
		 * called if the number of attempts is unlimited.
		 */
		void handleNodeInfoUnavailable(NodeID node_id) override;

		/**
		 * This call is routed directly from @ref NodeStatusMonitor.
		 *
		 * @param event     Node status change event
		 */
		void handleNodeStatusChange(
				const NodeStatusMonitor::NodeStatusChangeEvent& event
				) override;

		/**
		 * This call is routed directly from @ref NodeStatusMonitor.
		 *
		 * @param msg       Node status message
		 */
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
		 *	Map object used for storing Node responses.
		 */
		std::unordered_map<
			uavcan::NodeID,
			uavcan::protocol::GetNodeInfo::Response,
			NodeIDHash> registry_;

};

#endif /* SRC_NODEINFOCOLLECTOR_H_ */
