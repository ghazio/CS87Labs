/*
 * Swarthmore College, CS 87
 * Copyright (c) 2020 Swarthmore College Computer Science Department,
 * Swarthmore PA, Professor Tia Newhall
 */

/* cs87talk.h
 *
 * Definitions and prototypes shared by client and server side
 * of the cs87talk interface (similar to unix talk)
 *
 * (your names, date)
 */

#ifndef _CS87TALK_H_
#define _CS87TALK_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <errno.h>
#include <pthread.h>


#include<readline/readline.h>
#include<readline/history.h>

static pthread_mutex_t mutex1;
static int counter;

// ***************** START: DO NOT CHANGE THESE DEFINITOINS
// The cs87talk server listen port number:
#define CS87_TALKSERVER_PORTNUM	1888

// Message Tag definitions:
#define HELLO_MSG               1
#define MSG_DATA                2
#define QUIT                    3
#define HELLO_OK                4
#define HELLO_ERROR             5

#define SERVER_COMP             1
#define CLIENT_COMP             2



// max size of a message associated with MSG_DATA
#define BUFMAX    256
// max name prefix for server-side message.  displayed as:
//   name:message
#define NAMEMAX   16


struct client {
    char* buf;
    int sockfd;
    int name_len;
    int active;

} client;

struct threadClientArg {
  int sockfd;
  char* name;
  int name_len;
  int* terminate_reciever;

} threadClientArg;


struct threadserverArg {
  int sockfd;
  int* open;
  struct client* client;
  struct client** other_clients;
  char* name_client;
  pthread_t threadid;

} threadserverArg;


// type to store/send/recve message tags and message sizes
// use tsize_t to declare variables storing these values:
//   tsize_t  len, tag;
// NOTE: can store up to 255 (related to BUFMAX size)
typedef unsigned char tsize_t;
// ***************** END: DO NOT CHANGE THESE DEFINITOINS

// Add any other definitions, function prototypes, etc. here:

/*
Inputs: takes in a socket id, a buffer, message buffer, length of the buffer,
Outputs:
Function:
*/




int sender_Hello(int sockfd, size_t len, char* name);
int receive_Hello(int sockfd, int* open, char* name);



/*input: Takes in the socket fd and name of the client
/ Output: returns either 0 on successfull error or -1 if there is some error
  Purpose: It creates two threads for the client. One of them recieves messages
          and the second one sends any input from the client to other clients on
          server. It quits when the client closes the sockfd by either SIGSEV signal
          or when the client types "goodbye"
*/

int chat_client(int sockfd, char* name);

void* client_sender_thread(void* threadarg);
void* client_receiver_thread(void* threadarg);

void* chat_server(void* server_args);




int sender(int sockfd, char* buf, size_t len, int flags);
int receiver(int sockfd, char* buf, int flags, int name_len);



#endif
