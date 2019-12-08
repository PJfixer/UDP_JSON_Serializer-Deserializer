#ifndef UPDApi_h
#define UDPApi_h

#include <string> 
#include <arpa/inet.h>

#define UDPBUFLEN 1024  //Max length of buffer

class UDPApi
{
	
	private:
	int 		SocketId;
	unsigned short 	Port;
	sockaddr_in 	Socket;
	std::string 	ErrorMsg;
	char 		MsgBuffer[UDPBUFLEN];
	
	public:
	//constructor/destructor
	UDPApi();
	~UDPApi();

	//socket opening/binding	
	bool open(unsigned port);
	
	//datagram sending
	bool send(const std::string& message, const std::string& dest_ip, const unsigned short& dest_port);
	
	//datagram receiving (blocking). Reply 1 if ok, -1 if error
	int receive(std::string& message, std::string& source_ip, unsigned short& source_port);
	
	//datagram receiving, with time-out. Reply 1 if ok, -1 if error, 0 if nothing (time-out)
	//receiveTimeOut is in milliseconds. 0 for infinite.
	int receive(std::string& message, std::string& source_ip, unsigned short& source_port, unsigned& duration, const unsigned& receiveTimeout = 0);
	
	//socket close
	bool close();

	//retrieve last error message
	const std::string& getError();

	private:
	socklen_t receive( struct sockaddr_in& remoteSocket);
};


#endif
