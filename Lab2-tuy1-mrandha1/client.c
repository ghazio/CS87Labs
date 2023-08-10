/*
 * Swarthmore College, CS 87
 * Copyright (c) 2020 Swarthmore College Computer Science Department,
 * Swarthmore PA, Professor Tia Newhall
 */
/*
 * cs87talk client
 *
 * TODO: remove this comment to you:
 *   Use man to find out about the system calls you need to use and
 *   what parameters they take (choose the POSIX version of system calls)
 *
 * (your names, the date)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <pthread.h>
#include "cs87talk.h"

int main(int argc, char **argv) {

  int sockfd, ret;
  struct sockaddr_in   saddr;  // server's IP & port number
  char* name = argv[2];

  if(argc != 3) {
    printf("Usage: ./cs87_client server_IP your_screen_name\n");
    exit(1);
  }
  printf("Hello %s, you are trying to connect to talk server %s\n",
      argv[2], argv[1]);
  printf("enter next message at the prompt or goodbye to quit\n\n");

  // step (1) create a TCP socket
  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if(sockfd == -1) {
    perror("Create Socket Failed");
    exit(1);
  }

  // initialize the saddr struct that you will pass to connect in step (2):
  // (***this part is complete, you don't need to change it)
  // server's IP is in argv[1] (a string like "130.58.68.129")
  //  inet_pton converts IP from dotted decimal formation
  //  to 32-bit binary address (inet_pton coverts the other way)
  saddr.sin_port =  htons(CS87_TALKSERVER_PORTNUM);
  saddr.sin_family =  AF_INET;
  if(!inet_aton(argv[1], &(saddr.sin_addr)) ) {
    perror("inet_aton");
    exit(1);
  }

  // step (2) connect to talk server
  // here is an example call:
  ret = connect(sockfd, (struct sockaddr*)&saddr, sizeof(saddr));
  if(ret <0) {
      perror("Create Socket Failed");
      exit(1);
    }
  printf("Going into the Sender hello\n");
  //send its hello
  ret = sender_Hello(sockfd,strlen(name), name);

  //if server is busy, display the message
  if(ret==(int)HELLO_ERROR){
    printf("Server is busy, try again later\n");
    perror("The number of clients is high\n");
    exit(1);
  }
  if(ret <0) {
    perror("There was error in client's send Hello function\n");
    exit(1);
  }

  printf("Starting Chat\n");
  printf("if you want to quit, type goodbye\n");
  //if the connection was accepted, then we should call the chat_client function
  ret = chat_client(sockfd,name);

  // step (3) enter a talking loop and start talking until the end
  // YOU CAN USE GOOD MODULAR DESIGN AND CALL A FUNCTION THAT DOES THIS STEP
  // Example of calling send and recv on a socket:
  //   if you have a variable buf declared as unsigned char buf[10]
  //   then, for example, you could send and receive 10 bytes into and from
  //   this buf with the following system calls:
  //      ret = recv(sockfd, buf, 10, 0);
  //      ret = send(sockfd, buf, 10, MSG_NOSIGNAL);
  // (your code should obviously use constants and include good error handling)


  // send "hello to server"
  // recieve the answer from server
  // if recieve = HEllo error,
    //terminate
  // else:
    //update the state and create a message buffer for the server

  //if return value indicates that the client quit, close your side of socket

  close(sockfd);
  printf("bye, bye\n");
}
