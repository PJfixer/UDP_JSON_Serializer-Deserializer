#include <iostream>
#include <string>
#include <udpapi.h>
#include <stdlib.h>
#include <message.h>
/*********************************************

	TP M2M
	Sample C++ server using class UDPApi

**********************************************/

int main(int argc, char**argv)
{

	//init parameters
	if (argc < 2 )
	{
		std::cout << "usage is : " << argv[0] << " local_port\n";
		return 1;
	}
// ----------- Variables --------------------------
  bool erreur;
	unsigned short local_port = strtol(argv[1],0,10);
  std::string message;
  std::string ack = "ACK";
  std::string source_ip;
  unsigned short source_port;
  std::map<string,string> theMap ; 
// ----------- Instances --------------------------
	UDPApi myApi;
	Message myMessage;

	myApi.open(local_port);
  erreur = myApi.receive( message, source_ip, source_port);
  if(erreur == -1){
    std::cout << "PAQUET NON RECU" << std::endl;
  }
  else{
    std::cout << "RECEPTION :" << message << " de la part de :"<< source_ip << ":" << source_port << "\n";
     theMap =  myMessage.deserialize(message) ;
     	cout << theMap["lastName"] << endl ; 
	cout << theMap["firstName"] << endl ; 
	cout << theMap["age"] << endl ; 
	cout << theMap["gender"] << endl ; 
	cout << theMap["ssn"] << endl ; 
	cout << theMap["phone"] << endl ; 

  
    myApi.send(ack, source_ip, source_port);
    std::cout << "ENVOI :" << ack << " à :"<< source_ip << ":" << source_port << "\n";

  }
  myApi.close();

	/*TODO*/

	return 0;
}
