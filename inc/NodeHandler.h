#ifndef NODE_HANDLER_H
#define NODE_HANDLER_H

#include <QMessageBox>
#include <QString>

#include <iostream>

#include "PlatformLinux.h"

using namespace std;

/**
 * Memory pool size largely depends on the number of CAN ifaces and on
 * application's logic. Please read the documentation for the class
 * uavcan::Node to learn more.
 */
constexpr unsigned NodeMemoryPoolSize = 16384;
typedef uavcan::Node<NodeMemoryPoolSize> Node;

/**
 * Default node name used by node handler.
 */
extern const uavcan::NodeStatusProvider::NodeName DEFAULT_NODE_NAME; 

/*
 * This class is used for handling CAN nodes.
 */
class NodeHandler {
    
    public:

        /**
         *  Initialize Node handler.
         */
        NodeHandler();

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
        int create_new_node(std::string iface_name, int node_id);

        /**
         *  Starts currently initialized node. Blocks current thread
         *  while node is spinning. If timeout or an error occurs while
         *  node is method will return.
         *
         *  timout_ms - Node timeout in milliseconds
         *
         *  returns - less than 0 if error occurs while node is running
         */
        int spin_current_node(int timeout_ms);

        /**
         * Destroy current Node object.
         */
        void destroy_current_node();

    private:

        /**
         * Flag indicating if node is created.
         */
        bool node_created = false;

        /**
         * Pointer to CAN node.
         */
        Node *pCan_node = NULL;
};


/**
 * Returns a Node object.
 *
 * iface_name - Name of the currently available can interface.
 *            - e.g. "can0"
 */
Node& getCanNode(std::string iface_name);

#endif //NODE_HANDLER_H
