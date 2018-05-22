#ifndef NODE_HANDLER_H
#define NODE_HANDLER_H

#include <QMessageBox>
#include <QString>
#include <QTreeWidgetItem>

#include <uavcan/uavcan.hpp>
#include <uavcan/protocol/firmware_update_trigger.hpp>
#include <uavcan/protocol/node_info_retriever.hpp>

/*
 * POSIX-dependent classes and POSIX API.
 * (Portable Operating System Interface UNIX)
 * This means that the example will only work as-is on a POSIX-compliant system
 * (e.g. Linux, NuttX),
 * otherwise the said classes will have to be re-implemented.
 */
#include <uavcan_posix/basic_file_server_backend.hpp>

/*
 * Function pathname generator. Implements the rules defined in Shell and
 * Utilities volume of IEEE std.
 */
#include <glob.h>

#include "CanFirmwareVersionChecker.h"

using namespace std;

// Forward declaration of CanWorker
class CanWorker;

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

// Some parameter constants
extern const QString QSTRING_INT;
extern const QString QSTRING_UINT8;
extern const QString QSTRING_STRING;
extern const QString QSTRING_FLOAT;

extern const uint8_t NAME_INDEX;
extern const uint8_t TYPE_INDEX;
extern const uint8_t VALUE_INDEX;
extern const uint8_t DEFAULT_VALUE_INDEX;
extern const uint8_t MIN_VALUE_INDEX;
extern const uint8_t MAX_VALUE_INDEX;

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
         * Stops current node from doing further work. Unable to spin a node
         * in this state (Negative value will be returned).
         */
        void stopCurrentNode();

        /**
		 * This class monitors the output of uavcan::NodeInfoRetriever,
		 * and using this output decides which nodes need to update their
		 * firmware.
		 * When a node that requires an update is detected, the class sends
		 * a service request uavcan.protocol.file.BeginFirmwareUpdate to it.
		 *
		 * The application-specific logic that performs the checks is
		 * implemented in the class ExampleFirmwareVersionChecker,
		 * defined above in this file.
		 */
		CanFirmwareVersionChecker *versionChecker_ = NULL;

		/**
		 * Pointer to CAN node.
		 */
		CustomNode_t *canNode_ = NULL;

		/**
		 * ID of the node whose parameters are being read.
		 */
		int paramNodeID_ = -1;

		/**
		 * True if parameters will be read, false otherwise.
		 */
		bool readParametersFlag_ = false;

		/**
		 * True if parameters will be read, false otherwise.
		 */
		bool storeParametersFlag_ = false;

		/**
		 * List of changed parameters.
		 */
		std::vector<QTreeWidgetItem> changedParams_;

    private:

		/**
		 * This method does all the necessary work to setup and start the
		 * file server.
		 * If anything goes wrong runtime exception will be
		 * thrown.
		 */
		void setupNodeFileServer();

		/**
		 * Attempt to read all parameters from the node with the given ID.
		 */
		void readAllParameters();

		/**
		 * Attempt to store all changed parameters.
		 */
		void storeParameters();

        /**
         * Flag indicating if node is created.
         */
        bool nodeWorking_ = false;

		/**
		 * Can worker object reference used for emitting signals to the
		 * UI thread.
		 */
		CanWorker *canWorker_ = NULL;

		/**
		 * Used by the firmware version checker.
		 */
		NodeInfoRetriever *updateNodeInfoRetriever_ = NULL;

		/**
		 * Attached to the version checker. Triggers the update when needed.
		 */
		FirmwareUpdateTrigger *updateTrigger_ = NULL;

		/*
		 * TODO(lmark): add description to server / backend
		 */
		uavcan_posix::BasicFileServerBackend *fileServerBackend_ = NULL;
		uavcan::FileServer *fileServer_ = NULL;
};


/**
 * Returns a Node object.
 *
 * iface_name - Name of the currently available can interface.
 *            - e.g. "can0"
 */
CustomNode_t& getCanNode(std::string iface_name);

#endif //NODE_HANDLER_H
