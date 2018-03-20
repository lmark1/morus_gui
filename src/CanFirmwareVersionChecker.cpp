/*
 * CanFirmwareVersionChecker.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: lmark
 */


#include "CanFirmwareVersionChecker.h"

#include <QDebug>

CanFirmwareVersionChecker::CanFirmwareVersionChecker() { }

CanFirmwareVersionChecker::~CanFirmwareVersionChecker() { }

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

	qDebug() << "CanFirmwareVersionChecker::shouldRequestFirmwareUpdate "
			"called";
	return false;
}

bool CanFirmwareVersionChecker::shouldRetryFirmwareUpdate(
				NodeID node_id,
				const protocol::file::BeginFirmwareUpdate::Response&
					error_response,
				FirmwareFilePath& out_firmware_file_path)
{

	qDebug() << "CanFirmwareVersionChecker::shouldRetryFirmwareUpdate "
			"called";
	return false;
}


void CanFirmwareVersionChecker::handleFirmwareUpdateConfirmation(
				NodeID node_id,
				const protocol::file::BeginFirmwareUpdate::Response& response)
{
	qDebug() << "CanFirmwareVersionChecker::handleFirmwareUpdateConfirmation "
				"called";
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

std::vector<std::string> findAvailableFirmwareFiles(
		const uavcan::protocol::GetNodeInfo::Response& info)
{

}

