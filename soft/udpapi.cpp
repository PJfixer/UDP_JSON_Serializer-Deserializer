#include <udpapi.h> 
#include <string.h> 
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

UDPApi::UDPApi()
{
	SocketId = 0;
	Port = 0;
}

UDPApi::~UDPApi()
{
	close();
}


bool UDPApi::open(unsigned port)
{
    
	ErrorMsg.clear();   

	// check status
	if (SocketId > 0)
	{ 
		ErrorMsg = "socket already active" ;  
		return false;
	}

	// create a UDP socket
	if ((SocketId=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		ErrorMsg = "cannot create UDP socket";
		ErrorMsg += errno;  
		return false;
	}

	// zero out the structure
	memset((char *) &Socket, 0, sizeof(Socket));

	// fill socket structure
	Socket.sin_family = AF_INET;
	Socket.sin_port = htons(port);
	Socket.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind socket to port
	if( bind(SocketId , (struct sockaddr*)&Socket, sizeof(Socket) ) == -1)
	{
		ErrorMsg = "cannot open port" ;
		ErrorMsg += errno;  
		return false;
	}

	// ok
	return true;
}

//datagram sending
bool UDPApi::send(const std::string& message, const std::string& dest_ip, const unsigned short& dest_port)
{
	ErrorMsg.clear();   
	
	// check status
    	if (SocketId == 0)
	{ 
		ErrorMsg = "socket closed" ;  
		return false;
	}

	// set remote socket structure
	struct sockaddr_in wRemoteSocket;
	memset((char *) &wRemoteSocket, 0, sizeof(wRemoteSocket));
	wRemoteSocket.sin_family = AF_INET;
	wRemoteSocket.sin_port = htons(dest_port);
    	
 
     	// set remote IP
	if (inet_aton(dest_ip.c_str(), &wRemoteSocket.sin_addr) == 0) 
	{
		ErrorMsg = "bad format dest IP address" ;  
		return false;
	}
 
	// set buffer
	if (message.size() > UDPBUFLEN) 
	{
		ErrorMsg = "message too long" ;  
		return false;
	}
	strncpy(MsgBuffer, message.data(), message.size());
         
	// send the message
	if (sendto(SocketId, MsgBuffer, message.size(), 0 , (struct sockaddr *) &wRemoteSocket, sizeof(wRemoteSocket))==-1)
	{
	    	ErrorMsg = "udp sending error" ; 
		ErrorMsg += errno;  
		return false;
	}

	// ok
	return true;
}

// datagram receiving system call (private). reply >0 if ok, -1 if ko, 0 if waiting
socklen_t UDPApi::receive( struct sockaddr_in& remoteSocket)
{
	
	// init parameters
	socklen_t wSlen = sizeof(remoteSocket);
	socklen_t wReadLen = 0;

	// try to receive some data
        if ((wReadLen = recvfrom(SocketId, MsgBuffer, UDPBUFLEN, 0, (struct sockaddr *) &remoteSocket, &wSlen)) == -1)
        {
		// case non blocking call            	
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return 0;
		
		// other errors
		ErrorMsg = "udp receive error" ;
		ErrorMsg += errno;   
        }

    	return wReadLen;
}



//datagram receiving, with time-out. 
int UDPApi::receive(std::string& message, std::string& source_ip, unsigned short& source_port, unsigned& duration, const unsigned& receiveTimeout )
{
	
	//init parameters
	message = "";
	source_ip = "0.0.0.0";
	source_port = 0;
	duration = 0;
	struct sockaddr_in wRemoteSocket;
	socklen_t wReadLen = 0;
	ErrorMsg.clear();
	
	// check status
    	if (SocketId == 0)
	{ 
		ErrorMsg = "socket closed" ;
		return -1;
	}

	// set start time
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long long wtpstart = (long long) tp.tv_sec * 1000L + tp.tv_usec / 1000; //get current timestamp in milliseconds
	long long wtpcurrent = wtpstart;

	// case time-out waiting
	if (receiveTimeout > 0)
	{
		// set non blocking flags	
		int wSaveFlags = fcntl(SocketId, F_GETFL);
		int wFlags = wSaveFlags;
		wFlags |= O_NONBLOCK;
		fcntl(SocketId, F_SETFL, wFlags);
		
		// loop to receive some data, this is a non-blocking call
		while ((wReadLen = receive(wRemoteSocket)) == 0)
		{
			// check for time-out
			gettimeofday(&tp, NULL);
			wtpcurrent = (long long) tp.tv_sec * 1000L + tp.tv_usec / 1000;
			duration = (unsigned) (wtpcurrent - wtpstart);

			if ( duration > receiveTimeout)
				break;
			
			// sleep 10msec
			usleep(10000);
			duration += 10;
		}
				
		// reset blocking flags
		fcntl(SocketId, F_SETFL, wSaveFlags);
	}
	else
	{
		// try to receive some data, this is a non-blocking call
		wReadLen = receive(wRemoteSocket);
		gettimeofday(&tp, NULL);
		wtpcurrent = (long long) tp.tv_sec * 1000L + tp.tv_usec / 1000;
		duration = (unsigned) (wtpcurrent - wtpstart);

	}
		
	// message read	
	if (wReadLen > 0)
	{		
		// get information
		message = std::string(MsgBuffer, wReadLen);
		source_ip = inet_ntoa(wRemoteSocket.sin_addr);
		source_port = ntohs(wRemoteSocket.sin_port);
		return 1;
	}

    	return wReadLen;
}


//datagram receiving (blocking).
int UDPApi::receive(std::string& message, std::string& source_ip, unsigned short& source_port)
{
	unsigned duration = 0;
	receive(message, source_ip, source_port, duration);
}
	
//socket close
bool UDPApi::close()
{
	ErrorMsg.clear();   
	
	// check status
    	if (SocketId == 0)
	{ 
		ErrorMsg = "socket already closed" ;  
		return false;
	}

	// close socket
	::close(SocketId);
	SocketId = 0;
	return true;
}

//retrieve last error message
const std::string& UDPApi::getError()
{
	return ErrorMsg;
}

//end of file

