#include <stdio.h>

/* Define our message types */
#define MF_SEND 1 /* Sending a message A -> B */
#define MF_RECV 2 /* A / B Receiving a message from B / A */
#define MF_COMD 3 /* Server direct commands */

const char *_FILTER_SEND_SYNTAX = "SEND";
const char *_FILTER_RECV_SYNTAX = "RECV";
const char *_FILTER_COMD_SYNTAX = "COMD";

#define _HOSTING_IP "10.0.0.32"
#define _HOSTING_PORT 6000

/* We need a struct to define a connection */
struct Broadcast {
    char *Message;
};

int IsValidMessage(char *Message);
int GetBroadcastType(struct Broadcast *BR);

void ServerLoop();