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
	qDebug() << "CanFirmwareVersionChecker::shouldRequestFirmwareUpdate() - "
			"called";

	// Check if update is requested
	if (!this->firmwareUpdateEnabled_)
	{
		qDebug() << "CanFirmwareVersionChecker::"
				"shouldRequestFirmwareUpdate() - "
				"Firmware update not requested by user - "
				"cancelling request...";
		return false;
	}

	// Check if node ID matches requested node ID
	if (node_id.get() == this->current_nodeId_)
	{
		qDebug() << "CanFirmwareVersionChecker::"
				"shouldRequestFirmwareUpdate() - "
				"Node ID does not match user - selected ID - "
				"canceling request...";
	}

	// TODO(lmark): Check if this file path was added correctly
	// Update is valid, make new firmware file path
	FirmwareFilePath newFilePath;
	newFilePath.operator += (this->current_firmwareFilePath_.c_str());

	qDebug() << "CanFirmwareVersionChecker::"
			"shouldRequestFirmwareUpdate() - "
			"Firmware update accepted.";
	return true;
}

bool CanFirmwareVersionChecker::shouldRetryFirmwareUpdate(
				NodeID node_id,
				const protocol::file::BeginFirmwareUpdate::Response&
					error_response,
				FirmwareFilePath& out_firmware_file_path)
{

	qDebug() << "CanFirmwareVersionChecker::shouldRetryFirmwareUpdate "
			"called";

	// Always retry
	return true;
}


void CanFirmwareVersionChecker::handleFirmwareUpdateConfirmation(
				NodeID node_id,
				const protocol::file::BeginFirmwareUpdate::Response& response)
{
	qDebug() << "CanFirmwareVersionChecker::"
			"handleFirmwareUpdateConfirmation() "
			"called";
}

void CanFirmwareVersionChecker::enableFirmwareUpdate(
		std::string firmwareFilePath,
		int nodeId)
{
	qDebug() << "CanFirmwareVersionChecker::enableFirmwareUpdate";

	// Save current update information
	this->current_nodeId_ = nodeId;
	this->current_firmwareFilePath_ = firmwareFilePath;
	firmwareUpdateEnabled_ = true;
}

