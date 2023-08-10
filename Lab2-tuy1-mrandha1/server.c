/*
 * Swarthmore College, CS 87
 * Copyright (c) 2020 Swarthmore College Computer Science Department,
 * Swarthmore PA, Professor Tia Newhall
 */

/*
 * cs87talk server
 *
 * TODO: (your names, date)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <pthread.h>
#include <stdbool.h>

#include "cs87talk.h"


#define BACKLOG 3

// define any types that are local to the server here
// define any global variables you want here

//this is the clients array
struct client* clients[10];
pthread_t thread_worker[10];
int open = 10;


/*********************************************/
int main(int argc, char **argv) {

  int listenfd, confd, ret;
  struct sockaddr_in saddr, caddr;
  struct linger linger_val;
  int optval;
  unsigned int socklen;

  // step (0) init any server-side data structs

  // step (1) create a TCP socket
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if(listenfd == -1) {
    perror("Create Socket Failed\n");
    exit(1);
  }

  // set socket opts on the listen socket SO_LINGER off and SO_REUSEADDR
  // here is the code to do it (it is complete)
  linger_val.l_onoff = 0;
  linger_val.l_linger = 0;
  ret = setsockopt(listenfd, SOL_SOCKET, SO_LINGER, (void *)&linger_val,
      (socklen_t) (sizeof(struct linger)));
  if(ret < 0) {
    perror("Setting socket option failed");
    exit(1);
  }
  // set SO_REUSEADDR on a socket to true (1):
  optval = 1;
  ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  if(ret < 0) {
    perror("Setting socket option failed");
    exit(1);
  }

  // setp (2) bind to port CS87_TALKSERVER_PORTNUM
  // (a) create and init an address struct (this is complete)
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(CS87_TALKSERVER_PORTNUM);
  saddr.sin_addr.s_addr = INADDR_ANY;
  // TODO: uncomment bind call
   ret = bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr));
   if(ret < 0) {
     perror("Binding socket failed");
     exit(1);
   }
  // (b) call bind
  // (c) check and handle error


  // step (3) tell OS we are going to listen on this socket
  // BACKLOG specifies how much space the OS should reserve for incomming
  // connections that have not yet been accepted by this server.
  // TODO: uncomment listen call and add error handling
   ret = listen(listenfd, BACKLOG);
   if(ret < 0) {
     perror("Binding socket failed");
     exit(1);
   }


  socklen = sizeof(caddr);
  // now the server is ready to accept connections from clients

  // TODO: remove this debugging output (it is just for compilation)
  printf("socklen = %u sockaddr addr %u\n", socklen, saddr.sin_addr.s_addr);
  //we make a variable to determine the number of clients and a struct of client
  //information.
  // struct client** clients=malloc(sizeof(struct client*)*10);
  for(int i = 0; i < 10; i++){
    clients[i] = malloc(sizeof(struct client));
    clients[i]->active=0;
  }
  pthread_t* alive_functions[10];
  int numberofClients = 0;
  // pthread_t thread_worker[10];

  while(1) {

    // step (4) accept the next connectino from a client
      confd = accept(listenfd, (struct sockaddr *) &caddr, &socklen);
      if(confd<0){
        perror("accepting socket failed");
        exit(1);
      }
       char* name_client = malloc(sizeof(char)*(NAMEMAX));
       ret = receive_Hello(confd,&open,name_client);
       if(ret<0){
          close(confd);
          perror("Connection with client not established");
          exit(1);
       }
       //if connection was not accepted(by return value 2)
       else if(ret== 2){
         printf("Connection from client %s was not accepted because we already got too many connections",name_client);
       }
       //if ret was 1, connection was accepted
       else{
          printf("Accepted new client socket. Client name: %s\n", name_client);
          //put race conditions here for accessing the struct of clients
          int i = 0;
          int free_client_index=0;
          int found = 0;
          while(i<10 && found == 0){
            if(clients[i]->active==0){
              free_client_index=i;
              found = 1;
            }
            else{
              i++;
            }

          }
          clients[free_client_index]->buf = malloc(sizeof(char)*BUFMAX);
          // printf("%d\n",&clients[free_client_index]->buf );
          strcpy(clients[free_client_index]->buf,name_client);
          clients[free_client_index]->name_len=strlen(name_client);
          clients[free_client_index]->buf[strlen(name_client)] = ':';
          clients[free_client_index]->buf[255]='\0';
          clients[free_client_index]->sockfd=confd;
          clients[free_client_index]->active=1;

          //global variable of number of clients should be updated
          //race conditions end here.

          //now, the server should ideally call a pthread on this and continue
          //doing its thing in the while loop of wait.
          // pthread_t thread_worker;
          char *message_worker="I will handle it :)\n";
          struct threadserverArg serverargs;
          serverargs.name_client=malloc(sizeof(char)*NAMEMAX);
          serverargs.sockfd = confd;
          serverargs.open=&open;
          serverargs.threadid=thread_worker[free_client_index];
          serverargs.other_clients=clients;
          serverargs.client=clients[free_client_index];
          strcpy(serverargs.name_client,name_client);


          //This calls the thread worker function and that thread handles the communication
          //between this server and the rest
          //I think we should only pass the args of one of the children
          int ret_pthread = pthread_create(&thread_worker[free_client_index],NULL,chat_server,(void*) &serverargs);
          if (ret_pthread<0){
            perror("Server Thread was not created");
            //should i return or check for errors? i think I would have to check for
            //errors
            return -1;
          }

          // clients[free_client_index] = NULL;
          //once the process returns, kill that thread
          //the main thread carries on
          //the worker server finishes the loop and then joins it at the
      }
       //server recieves first and then sends
       // for(int i = 0)
       //  pthread_join(thread_worker[free_client_index], NULL);
       // pthread_kill(thread_worker[free_client_index], 1);


    // step (5) connection estabilished!  USE confd to have
    //          a dedicatedchat session with this client
    // I suggest passing confd to a function that handles this chat session
    // (use good modular design)
    // Also add common messaging functions to cs87talk.[ch]
    // (functionality shared by both the client and server)

    // note: make sure to close the socket returned by accept
    //       after the chat session ends (or on some error in the session)
    free(name_client);
  }
  free(clients);
  close(listenfd);
}
/*********************************************/
