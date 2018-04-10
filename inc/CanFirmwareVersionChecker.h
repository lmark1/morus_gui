/*
 * CanFirmwareVersionChecker.h
 *
 *	This class implements the application-specific part of
 *	uavcan::FirmwareUpdateTrigger via the interface
 *	uavcan::IFirmwareVersionChecker.
 *
 *  Created on: Mar 12, 2018
 *      Author: lmark
 */

#ifndef CAN_FIRMWARE_VERSION_CHECKER_H
#define CAN_FIRMWARE_VERSION_CHECKER_H

#include<vector>

#include <uavcan/protocol/firmware_update_trigger.hpp>

using namespace uavcan;

class CanFirmwareVersionChecker final :
	public uavcan::IFirmwareVersionChecker
{

	public:

		CanFirmwareVersionChecker();
		~CanFirmwareVersionChecker();

		/**
		 * This method will be invoked when the class obtains a
		 * response to GetNodeInfo request.
		 *
		 * @param node_id
		 * 		Node ID that this GetNodeInfo response was received from.
		 *
		 * @param node_info
		 * 		Actual node info structure;
		 * 		refer to uavcan.protocol.GetNodeInfo for details.
		 *
		 * @param out_firmware_file_path
		 * 		The implementation should return the firmware
		 * 		image path via this argument. Note that this path
		 * 		must be reachable via uavcan.protocol.file.Read service.
		 * 	 	Refer to @ref FileServer and @ref BasicFileServer for details.
		 *
		 * @return
		 * 		True - the class will begin sending update requests.
		 *		False - the node will be ignored, no request will be sent.
		 */
		bool shouldRequestFirmwareUpdate(
				NodeID node_id,
				const protocol::GetNodeInfo::Response& node_info,
				FirmwareFilePath& out_firmware_file_path) override;

		/**
		 * This method will be invoked when a node responds to the update
		 * request with an error. If the request simply times out,
		 * this method will not be invoked. Note that if by the time of
		 * arrival of the response the node is already removed,
		 * this method will not be called.
		 *
		 * SPECIAL CASE: If the node responds with ERROR_IN_PROGRESS,
		 * 				 the class will assume that further requesting
		 *               is not needed anymore. This method will not
		 *               be invoked.
		 *
		 * @param node_id
		 * 		Node ID that returned this error.
		 *
		 * @param error_response
		 * 		Contents of the error response.
		 * 		It contains error code and text.
		 *
		 * @param out_firmware_file_path
		 * 		New firmware path if a retry is needed. Note that
		 * 		this argument will be initialized with old path,
		 * 		so if the same path needs to be reused, this
		 * 		argument should be left unchanged.
		 *
		 * @return
		 * 		True - the class will continue sending
		 * 			update requests with new firmware path.
		 *		False - the node will be forgotten,
		 *			new requests will not be sent.
		 */
		bool shouldRetryFirmwareUpdate(
				NodeID node_id,
				const protocol::file::BeginFirmwareUpdate::Response&
					error_response,
				FirmwareFilePath& out_firmware_file_path) override;

		/**
		 * This node is invoked when the node responds to the update
		 * request with confirmation. Note that if by the time of arrival
		 * of the response the node is already removed,
		 * this method will not be called.
		 *
		 * @param node_id
		 * 		Node ID that confirmed the request.
		 *
		 * @param response
		 * 		Actual response.
		 */
		void handleFirmwareUpdateConfirmation(
				NodeID node_id,
				const protocol::file::BeginFirmwareUpdate::Response& response)
				override;

		/**
		 * Enable firmware update for the node with the given ID.
		 *
		 * @param nodeId
		 * 		Node ID which requested update
		 *
		 * @param firmwareFilePath
		 * 		File path containing firmware image.
		 */
		void enableFirmwareUpdate(std::string firmwareFilePath, int nodeId);

	private:

		/*
		 * This function is specific for this example implementation.
		 * It computes the name of a symlink to the firmware file.
		 * Please see explanation above, where the function is called from.
		 * The implementation is made so that it can work even on a
		 * deeply embedded system.
		 */
		FirmwareFilePath makeFirmwareFileSymlinkName(
			const char* file_name,
			unsigned file_name_length);

		/*
		 * This function is specific for this example implementation.
		 * It extracts the version information from firmware file name.
		 * The implementation is made so that it can work even on a
		 * deeply embedded system.
		 *
		 * Assumed format is:
		 *      <node-name>-<hw-major>.
		 *      <hw-minor>-<sw-major>.
		 *      <sw-minor>.<vcs-hash-hex>.uavcan.bin
		 */
		uavcan::protocol::GetNodeInfo::Response parseFirmwareFileName(
				const char* name);

		/*
		 * This function is specific for this example implementation.
		 * It returns the firmware files available for given node info struct.
		 */
		std::vector<std::string> findAvailableFirmwareFiles(
				const uavcan::protocol::GetNodeInfo::Response& info);

		/**
		 * Flag indicating if firmware update is enabled.
		 */
		bool firmwareUpdateEnabled_ = false;

		/**
		 * Current firmware file path.
		 */
		std::string current_firmwareFilePath_;

		/**
		 * Current node id.
		 */
		int current_nodeId_ = -1;

};

#endif /* CAN_FIRMWARE_VERSION_CHECKER_H */

