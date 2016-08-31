#pragma once
#include "svrlib.h"
namespace protocol {
	class Request;
	class Response;
	class Packet;
}

class protocol::Packet : public Object {
private:
	int headLength;
public:
	std::string Operation;
	std::string OperationStatus;
	int ContentLength;
	std::string TimeStamp;
	std::string InstanceName;
	std::string SessionID;
	char * pData;

	const static std::regex pattern;
public:
	Packet() : Operation(""), ContentLength(0), TimeStamp(""), pData(NULL), headLength(0) {

	}

	Packet(const std::string & op, const std::string stamp) : Operation(op), TimeStamp(stamp), pData(NULL), headLength(0), ContentLength(0)
	{

	}

	Packet(const std::string & op, int contLen, const std::string stamp, void * data) : Operation(op), ContentLength(contLen), TimeStamp(stamp), pData((char*)data), headLength(0)
	{

	}

	void setContent(void * data, int length) {
		if (data == NULL || length < 0)
			return;
		pData = (char*)data;
		ContentLength = length;
	}

	std::string createHeader(void);

	bool matchHeader(const char * data, int length);

	int getHeaderLength(void) {
		return headLength;
	}

	int getPacketLength(void) {
		return headLength + ContentLength;
	}
};