/*
 * CanFirmwareVersionChecker.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: lmark
 */


#include "CanFirmwareVersionChecker.h"

/*
 * This function is specific for this example implementation.
 * It returns the firmware files available for given node info struct.
 */
static std::vector<std::string> findAvailableFirmwareFiles(
		const uavcan::protocol::GetNodeInfo::Response& info);

CanFirmwareVersionChecker::CanFirmwareVersionChecker()
{
	// TODO Auto-generated constructor stub
}

CanFirmwareVersionChecker::~CanFirmwareVersionChecker()
{
	// TODO Auto-generated destructor stub
}

bool CanFirmwareVersionChecker::shouldRequestFirmwareUpdate(
		NodeID node_id,
		const protocol::GetNodeInfo::Response& node_info,
		FirmwareFilePath& out_firmware_file_path)
{
	/*
	 * We need to make the decision, given the inputs,
	 * whether the node requires an update.
	 * This part of the logic is deeply application-specific,
	 * so the solution provided here may not work
	 * for some real-world applications.
	 */
	return false;
}

bool CanFirmwareVersionChecker::shouldRetryFirmwareUpdate(
				NodeID node_id,
				const protocol::file::BeginFirmwareUpdate::Response&
					error_response,
				FirmwareFilePath& out_firmware_file_path)
{
	return false;
}


void CanFirmwareVersionChecker::handleFirmwareUpdateConfirmation(
				NodeID node_id,
				const protocol::file::BeginFirmwareUpdate::Response& response)
{

}


uavcan::IFirmwareVersionChecker::FirmwareFilePath
	CanFirmwareVersionChecker::makeFirmwareFileSymlinkName(
		const char* file_name,
		unsigned file_name_length)
{

}

uavcan::protocol::GetNodeInfo::Response
	CanFirmwareVersionChecker::parseFirmwareFileName(
		const char* name)
{

}

