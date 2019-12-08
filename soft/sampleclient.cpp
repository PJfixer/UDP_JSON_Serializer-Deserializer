#include <iostream>
#include <string>
#include <udpapi.h>
#include <stdlib.h>
#include <message.h>
#include <map>
#include <sstream>
using namespace std;
/*********************************************

	TP M2M
	Sample C++ client using class UDPApi

**********************************************/

int main(int argc, char**argv)
{
    
	//init parameters	
	if (argc < 5 ) 
	{
		std::cout << "usage is : " << argv[0] << " local_port  dest_ip  dest_port timeout(msec)\n";
		return 1;
	}

	unsigned short local_port = strtol(argv[1],0,10);
	std::string dest_ip = argv[2];
	unsigned short dest_port = strtol(argv[3],0,10);
	std::string message;
	unsigned receiveTimeout = strtol(argv[4],0,10);
	
	UDPApi myApi;
	Message myMessage;
	
	std::string msg;
	std::string recv_ip;
	unsigned short source_port;
	

	// JSON serialisation--------------------
	map<string,string> monDictJson;
	monDictJson["lastName"]="Jeanne";
	monDictJson["firstName"]="Pierre";
	monDictJson["age"]="23";
	monDictJson["gender"]="h";
	monDictJson["ssn"]="cpam";
	monDictJson["phone"]="065324562";


	message = myMessage.serialize(monDictJson);

	// --------------------------------------
	
	myApi.open(local_port);
	
	if(myApi.send(message,dest_ip,dest_port) != 0)
	{
		std::cout << "send succes" << std::endl;
	}
	else
	{
		std::cout << "send fail" << std::endl;
	}
	
	if(myApi.receive(msg,recv_ip,source_port))
	{
		if(msg.compare("ACK") == 0)
		{
			std::cout << "got confirmation !!" << std::endl;
		}
		else
		{
			std::cout << "bad ack" << std::endl;
		}
	}
	else
	{
		std::cout << "fail receive" << std::endl;
	}
	
	myApi.close();

	return 0;
}
