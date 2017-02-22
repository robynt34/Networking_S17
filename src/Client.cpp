#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

int main(int argc, char* argv[])
{
    int socketFileDescriptor;
    struct addrinfo hints, *serverInfo, *p;
    int status;
    int numBytes;

    // args
    char *serverIP, *portNum, *logfileName, *clientName;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if(argc != 9)
    {
        cout << "Usage: client -s ServerIP -p portno -l logfile -n myname" << endl;
        exit(1);
    }
    else
    {
        int sCounter, pCounter, lCounter, nCounter;
        for(int i = 0; i < argc; i++)
        {
            if(strcmp(argv[i], "-s") == 0)
            {
                sCounter = i+1;
            }
            else if(strcmp(argv[i], "-p") == 0)
            {
                pCounter = i+1;
            }
            else if(strcmp(argv[i], "-l") == 0)
            {
                lCounter = i+1;
            }
            else if(strcmp(argv[i], "-n") == 0)
            {
                nCounter = i+1;
            }
        }

        serverIP = argv[sCounter];
        portNum = argv[pCounter];
        logfileName = argv[lCounter];
        clientName = argv[nCounter];

        cout << "DEBUG: serverIP located at " << serverIP << " on port " << portNum << " from client " << clientName << " recorded in logfile " << logfileName << endl;
    }

    if((status = getaddrinfo(serverIP, portNum, &hints, &serverInfo)) != 0)
    {
        cout << "DEBUG: setup error with hints and serverInfo" << endl;
        return 1;
    }


    for(p = serverInfo; p != NULL; p = p->ai_next)
    {
        if((socketFileDescriptor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            cout << "DEBUG: error with socketFileDescriptor" << endl;
            continue;
        }

        break;
    }

    if(p == NULL)
    {
        cout << "DEBUG: failed to bind socket" << endl;
        return 2;
    }

    // SENDING FIRST MESSAGE
    char message[90];
    strcat(message, "register ");
    strcat(message, clientName);
    cout << "ehck" << endl;
    cout << "DEBUG: message is: " << message << endl;
    if((numBytes = sendto(socketFileDescriptor, message, strlen(message), 0, p->ai_addr, p->ai_addrlen)) == -1)
    {
        cout << "DEBUG: error on sending message" << endl;
        exit(1);
    }

    freeaddrinfo(serverInfo);

    cout << "Sent " << numBytes << " bytes to server" << endl;
    close(socketFileDescriptor);

    return 0;
}