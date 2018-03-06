/*
 * NodeInfo.h
 *
 *	Structure defining a node information object.
 *
 *  Created on: Mar 6, 2018
 *      Author: lmark
 */

#ifndef NODEINFO_H_
#define NODEINFO_H_

#include<iostream>
using namespace std;

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


#endif /* NODEINFO_H_ */
