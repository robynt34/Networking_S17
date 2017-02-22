
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;
void *get_in_addr(struct sockaddr* sa)
{
	if(sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char* argv[])
{
    struct addrinfo hints, *serverInfo, *p;
    int buffSize = 1024;
    int numbytes;
    char* portNum, *logFileName;
    struct sockaddr_storage client_addr;
    socklen_t addr_len;
    int socketFileDescriptor;
    char buffer[buffSize];
    char s[INET6_ADDRSTRLEN];
    int status; // tells us if our setup is valid

    struct sockaddr_in server_addr;
    socklen_t size;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use own IP

    cout << "Num args: " << argc << endl;
    if(argc != 5)
    {
    	cout << "Usage: server -p portno -l logfile" << endl;
    	exit(1);
    }else{
    	// Correct num args
    	int portCounter = 0, logCounter = 0;;
    	for(int i = 0; i < argc; i++)
    	{
    		if(strcmp(argv[i], "-p") == 0)
    		{
    			portCounter = i+1;
    		}
    		else if(strcmp(argv[i], "-l") == 0)
			{
				logCounter = i+1;
			}
    	}
    	portNum = argv[portCounter];
    	cout << "DEBUG: portNum is " << portNum << endl;
    	logFileName = argv[logCounter];
    	cout << "DEBUG: logFileName is " << logFileName << endl;
    }

   if((status = getaddrinfo(NULL, portNum, &hints, &serverInfo)))
   {
   	cout << "DEBUG: Break on line 32...getaddrinfo w/ hints and serverInfo" << endl;
   	return 1;
   }

   // loop through addrinfo starting at serverinfo and bind to first one
   for(p = serverInfo; p != NULL; p=p->ai_next)
   {
   	if((socketFileDescriptor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
   	{
   		cout << "DEBUG: error on socket listener" << endl;
   		continue;
   	}

   	if(bind(socketFileDescriptor, p->ai_addr, p->ai_addrlen) == -1)
   	{
   		close (socketFileDescriptor);
   		cout << "DEBUG: error in socket binding" << endl;
   		continue;
   	}

   	cout << "DEBUG: successful bind to socket" << endl;
   	break; // break if success;
   }

   if(p == NULL)
   {
   	cout << "DEBUG: could not bind to socket" << endl;
   	return 2;
   }

   freeaddrinfo(serverInfo);

   cout << "DEBUG: Waiting for recvfrom..." << endl;
   addr_len = sizeof client_addr;
   if((numbytes = recvfrom(socketFileDescriptor, buffer, buffSize-1, 0, (struct sockaddr*) &client_addr, &addr_len)) == -1)
   {
   	cout << "DEBUG: error on recvfrom" << endl;
   	exit(1);
   }
   
   cout << "recvfrom: " << inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *) &client_addr), s, sizeof s) << " port " << portNum << endl;
   cout << "-- packet is " << numbytes << " long" << endl;
   cout << "-- packet contains '" << buffer << "'" << endl;

   close(socketFileDescriptor);

    return 0;
}