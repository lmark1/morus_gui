#ifndef NODE_HANDLER_H
#define NODE_HANDLER_H

#include "platform_linux.h"

#include <QMessageBox>
#include <QString>

#include <iostream>

using namespace std;

/**
 * Memory pool size largely depends on the number of CAN ifaces and on application's logic.
 * Please read the documentation for the class uavcan::Node to learn more.
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
class node_handler {
    
    public:

        /**
         *  Initialize Node handler.
         */
        node_handler();

        /**
         *  Shuts down the node handler, shuts down all active nodes.
         */
        ~node_handler();

        /** 
         *  Creates and initializes a new node with given node name and ID.
         * 
         *  iface_name - CAN interface name
         *  node_id    - new node ID
         *
         *  returns - 0 if node is successfully created 
         *            1 otherwise.
         */
        int create_new_node(std::string iface_name, int node_id);

        /**
         *  Starts currently initialized node. Blocks current thread
         *  while node is running. If timout or an error occurs while
         *  node is method will return.
         *
         *  timout_ms - Node timout in milliseconds
         *
         *  returns - 1 if error occurs while node is running.
         */
        int start_current_node(int timeout_ms);

    private:

        bool node_created = false;
        Node *can_node;
};


/**
 * Returns a Node object.
 *
 * iface_name - Name of the currently available can interface.
 *            - e.g. "can0"
 */
Node& getCanNode(std::string iface_name);

/**
 * Generate an error message dialog.
 */
void generateDialog(std::string message);

#endif //NODE_HANDLER_H
