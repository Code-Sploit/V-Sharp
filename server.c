#include "server.h"
#include "error.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

/* Is the message we got valid? */
int IsValidMessage(char *Message)
{
    char *TOKEN = strtok(Message, "~");

    int validRet = 0;
    int argCount = 0;

    /* Prevent dupes */
    int foundSend = 0;
    int foundRecv = 0;
    int foundComd = 0;

    int foundVal = 0;

    while (TOKEN != NULL)
    {
        if (argCount == 0)
        {
            /* We expect either: SEND, RECV or COMD */
            if (strcmp(TOKEN, "SEND") == 0)
            {
                foundSend = 1;
            }

            if (strcmp(TOKEN, "RECV") == 0)
            {
                foundRecv = 1;
            }

            if (strcmp(TOKEN, "COMD") == 0)
            {
                foundComd = 1;
            }
        }

        if (argCount == 1)
        {
            /* We know there is something here so we set foundVal to 1*/
            foundVal = 1;
        }

        TOKEN = strtok(NULL, "~");

        argCount++;
    }

    /* Return value check */
    validRet = (foundSend + foundRecv + foundComd + foundVal);

    if (validRet == 0)
    {
        /* Valid message */
        return 0;
    } else {
        /* Invalid message */
        return 1;
    }

    /* Something went wrong let's return (-1) */
    return -1;
}

/* Get the type of the message sent to the server */
int GetBroadcastType(struct Broadcast *BR)
{
    /* Let's check if the message is even there and or valid */
    if (BR->Message == "" || IsValidMessage(BR->Message))
    {
        printf("%s", "Error message to server is not valid returning _ERROR_BAD_MESSAGE (-1)");

        return _ERROR_BAD_MESSAGE;
    }

    /* We are getting a plain string so let's split it into tokens */
    char *TYPE;
    char *VALUE;

    char *TOKEN = strtok(BR->Message, "~");

    while (TOKEN != NULL)
    {
        if (strcmp(TOKEN, _FILTER_SEND_SYNTAX) == 0)
        {
            return MF_SEND;
        }

        if (strcmp(TOKEN, _FILTER_RECV_SYNTAX) == 0)
        {
            return MF_RECV;
        }

        if (strcmp(TOKEN, _FILTER_COMD_SYNTAX) == 0)
        {
            return MF_COMD;
        }

        TOKEN = strtok(NULL, "~");
    }

    /* Something went wrong, let's return (-1) */

    return -1;
}

void ServerLoop()
{
    /* Initlialize a socket */
    struct sockaddr_in Address;

    int ServerFD, NewSocket, ValueRead;

    int addrlen = sizeof(Address);
    int opt = 1;

    /* Let's check if this socket is legit! */
    if ((ServerFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Failed to initialize server socket!");

        exit(1);
    }

    /* Set socket options */
    if (setsockopt(ServerFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("Failed to initialize server socket options!");

        exit(1);
    }

    /* Define address input */
    Address.sin_family      = AF_INET;
    Address.sin_addr.s_addr = INADDR_ANY;
    Address.sin_port        = htons(_HOSTING_PORT);

    /* Attach socket */
    if (bind(ServerFD, (struct sockaddr*)&Address, sizeof(Address)) < 0)
    {
        perror("Failed to bind server to hardcoded port!");

        exit(1);
    }

    if (listen(ServerFD, 3) < 0)
    {
        perror("Failed to initialize listening for connections!");

        exit(1);
    }

    if ((NewSocket = accept(ServerFD, (struct sockaddr*)&Address, (socklen_t*)&addrlen)) < 0) {
        perror("Failed to accept connection!");
        
        exit(1);
    }

    /* While loop for Mail server */
    char buffer[1024];

    /*
    While true:
        Read for commands
        Get command arguments
        Is valid command?
        Parse command
        Get command output and send to client
    */

    /* Close the socket */
    close(NewSocket);
    shutdown(ServerFD, SHUT_RDWR);
}

int main(int argc, char** argv)
{
    ServerLoop();

    return 0;
}