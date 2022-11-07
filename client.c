#include "client.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void ClientLoop()
{
    struct sockaddr_in ServerAddress;

    int sock, valread, ClientFD;

    /* Can we connect */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Failed to connect to server!");

        exit(1);
    }

    /* Define input variables */
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port   = htons(_HOSTING_PORT);

    /* Convert IP to binary form */
    if (inet_pton(AF_INET, _HOSTING_IP, &ServerAddress.sin_addr) <= 0)
    {
        perror("Hosting address is either not supported or does not exist.");

        exit(1);
    }
 
    if ((ClientFD = connect(sock, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress))) < 0) {
        perror("Server connection failed!");
        
        exit(1);
    }

    /*
    While true:
        Read command
        Get all command arguments
        Check valid command?
        Parse arguments
        Send command to server
        Recieve output
    */

    /* Close connection */
    close(ClientFD);
}

int main(int argc, char** argv)
{
    ClientLoop();

    return 0;
}