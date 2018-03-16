/*
 * CanFirmwareVersionChecker.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: lmark
 */

#include "CanFirmwareVersionChecker.h"

CanFirmwareVersionChecker::CanFirmwareVersionChecker()
{
	// TODO Auto-generated constructor stub
}

CanFirmwareVersionChecker::~CanFirmwareVersionChecker()
{
	// TODO Auto-generated destructor stub
}

bool CanFirmwareVersionChecker::shouldRequestFirmwareUpdate(NodeID node_id,
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

}

