#pragma once

/********************************************************************************
**
**	Function1	:protocol
**	Detail		:server cluster protocol
**
**	Author		:nemo
**	Date		:2016/8/29
**
********************************************************************************/


/********************************************************************************
request:
header:
Operation:			String		0-32bytes
ContentLength:		int			0-8388608(1-7bytes,8Mb)
TimeStamp:		String		23bytes
[InstanceName:		String]		0-32bytes
[SessionID:			String]		16bytes
'\n'
Content data:
[binary data] |
[XML]		|
[JSON data] |
[String]


response:
header:
Operation:			String
OperationStatus:	String		0-32 bytes
ContentLength:		int
TimeStamp:		String
InstanceName:		String
[SessionID:			String]
'\n'
Content data:
[binary data] |
[XML]		|
[JSON data] |
[String]

note:
1. HeaderLength + ContentLength = packet length , and head length < 256 bytes
2.time stamp format : YYYY/MM/DD HH:MM:SS:MMM
3.JSON data example
{
"InstanceName":"Server",
"Key":"value"
....
}

example:
Operation:NULL
ContentLength:0
TimeStamp:YYYY/MM/DD HH:MM:SS:MMM
InstanceName:Server
SessionID:AAAABBBBCCCCDDDD
'\n'

********************************************************************************/

#include "protocol.h"
#include <cstdio>

//Operation:NULL\nContentLength:0\nTimeStamp:YYYY/MM/DD HH:MM:SS:MMM\nInstanceName:Server\nSessionID:AAAABBBBCCCCDDDD\n\n
//(Operation:(\w{1,32})\s(OperationStatus:(\w{1,32})\s)?ContentLength:(\d{1,7})\sTimeStamp:(.{23})\s(InstanceName:(\w{1,32})\s)?(SessionID:(\w{16})\s)?\s).*
const std::regex protocol::Packet::pattern(R"((Operation:(\w{1,32})\s(OperationStatus:(\w{1,32})\s)?ContentLength:(\d{1,7})\sTimeStamp:(.{23})\s(InstanceName:(\w{1,32})\s)?(SessionID:(\w{16})\s)?\s).*)");

std::string protocol::Packet::createHeader(void)
{
	char data[32] = { 0 };
	sprintf_s<32>(data, "%d", ContentLength);
	std::string ContLen(data);
	std::string header("");

	header += std::string("Operation:") + Operation;
	header += '\n';

	if (OperationStatus.size() > 0) {
		header += std::string("OperationStatus:") + OperationStatus;
		header += '\n';
	}

	header += std::string("ContentLength:") + ContLen;
	header += '\n';
	header += std::string("TimeStamp:") + TimeStamp;
	header += '\n';

	if (InstanceName.size() > 0) {
		header += std::string("InstanceName:") + InstanceName;
		header += '\n';
	}

	if (SessionID.size() > 0) {
		header += std::string("SessionID:") + SessionID;
		header += '\n';
	}

	header += '\n';
	headLength = header.size();

	return header;
}

bool protocol::Packet::matchHeader(const char * data, int length) {
	if (data == NULL || length <= 0) {
		return false;
	}
	length = length > 256 ? 256 : length;

	bool ret = false;
	std::string buf(data, length);
	std::match_results<std::string::const_iterator> result;

	bool flag = std::regex_match(buf, result, pattern);

	if (flag) {
		ret = true;
		Operation = result[2].str();
		OperationStatus = result[4].str();

		if (sscanf_s(result[5].str().c_str(), "%d", &ContentLength) != 1) {
			ret = false;
		}

		TimeStamp = result[6].str();
		InstanceName = result[8].str();
		SessionID = result[10].str();
		headLength = result[1].str().size();
	}

	return ret;
}
