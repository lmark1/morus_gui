#ifndef NODE_HANDLER_H
#define NODE_HANDLER_H

#include <QMessageBox>
#include <QString>

#include <iostream>

#include "PlatformLinux.h"
#include "NodeInfoCollector.h"

using namespace std;

/**
 * Memory pool size largely depends on the number of CAN ifaces and on
 * application's logic. Please read the documentation for the class
 * uavcan::Node to learn more.
 */
constexpr unsigned NodeMemoryPoolSize = 16384;
typedef uavcan::Node<NodeMemoryPoolSize> CustomNode_t;

/**
 * Default node name used by node handler.
 */
extern const uavcan::NodeStatusProvider::NodeName DEFAULT_NODE_NAME; 

// Forward declaration for the CanWorker class.
class CanWorker;

/**
 * Structure defining node information.
 */
typedef struct{
	uint8_t id;
	std::string nodeName;
	uint8_t softwareVersionMajor;
	uint8_t hardwareVersionMajor;
	uint8_t softwareVersionMinor;
	uint8_t hardwareVersionMinor;
	uint32_t uptime;
	uint8_t mode;
	uint8_t health;
	uint32_t vendorSpecificStatusCode;
} NodeInfo_t;

/*
 * This class is used for handling CAN nodes.
 */
class NodeHandler {
    
    public:

        /**
         *  Initialize Node handler.
         */
        NodeHandler(CanWorker& worker);

        /**
         *  Shuts down the node handler, shuts down all active nodes.
         */
        ~NodeHandler();

        /** 
         *  Creates and initializes a new node with given node name and ID.
         * 
         *  iface_name - CAN interface name
         *  node_id    - new node ID
         *
         *  returns - less than 0 if error occurs while creating node
         */
        int createNewNode(std::string iface_name, int node_id);

        /**
         *  Starts currently initialized node. Blocks current thread
         *  while node is spinning. If timeout or an error occurs while
         *  node is method will return.
         *
         *  timout_ms - Node timeout in milliseconds
         *
         *  returns - less than 0 if error occurs while node is running
         */
        int spinCurrentNode(int timeout_ms);

        /**
         * Destroy current Node object.
         */
        void destroyCurrentNode();

        /**
		 * This method collects node information through the
		 * and emits it using CanWorker signals to the main UI thread.
		 */
		void collectNodeInformation();

    private:

        /**
         * Flag indicating if node is created.
         */
        bool nodeCreated = false;

        /**
         * Pointer to CAN node.
         */
        CustomNode_t *canNode = NULL;

        /**
		 * Node information collector attached to the canNode.
		 */
		NodeInfoCollector collector;

		/**
		 * Can worker object reference used for emiting signals to the
		 * UI thread.
		 */
		CanWorker *canWorker = NULL;
};


/**
 * Returns a Node object.
 *
 * iface_name - Name of the currently available can interface.
 *            - e.g. "can0"
 */
CustomNode_t& getCanNode(std::string iface_name);

#endif //NODE_HANDLER_H
