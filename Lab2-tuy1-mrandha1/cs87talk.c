/*
 * Swarthmore College, CS 87
 * Copyright (c) 2020 Swarthmore College Computer Science Department,
 * Swarthmore PA, Professor Tia Newhall
 */
/*
 * Messaging (and other) functions shared by both the client
 * and the server
 *
 * (your names, date)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "cs87talk.h"

static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int running=1;


int sender_Hello(int sockfd, size_t len, char* name){


  //send the HELLO_MSG TAG
  unsigned char buf;
  buf = HELLO_MSG;

  int ret = send(sockfd,&buf, 1, 0);
  if(ret <0) {
    printf("sending failed");
    return -1;
  }

  //send len
  buf = len;
  ret= send(sockfd,&buf, 1, 0);
  if(ret <0) {
    printf("len failed");
    return -1;
  }
  printf("name of the client: %s len recieved: %ld\n",name, len);
  //send name
  char* buf2 = malloc(sizeof(char)*(int)(len+1));
  buf2[len]='\0';
  strcpy(buf2,name);

  ret= send(sockfd,buf2, len, 0);
  if(ret <0) {
    printf("send name not working");
    return -1;
  }
  free(buf2);

  ret= recv(sockfd,&buf,1,0);
  if(ret < 0) {
    printf("receiving response broke\n");
    perror("current error value");
    return -1;
  }
  if((int)buf==(int)HELLO_OK){
    printf("%s\n", "This means hello has succeeded." );
    return 0;

  }else if((int)buf==(int)HELLO_ERROR){
    return (int)HELLO_ERROR;
  }
  return -1;//succesfully sent
}


int receive_Hello(int sockfd, int* open,char* name){
  unsigned char buf[1];
  int status = HELLO_OK;

  //receive the 1 bit response
  int ret= recv(sockfd,buf,1,0);
  if(ret<0){
    perror("problem receiving hello\n");
    return -1;
  }
  //if hello message received and we can take on a client
  if((int)(buf[0]) == (int)HELLO_MSG){

    ret= recv(sockfd,buf,1,0);
    if(ret<0){
      perror("problem receiving length\n");
      return -1;
    }

    int len = (int)buf[0];

    int ret= recv(sockfd,name,len,0);
    if(ret<0){
          perror("problem receiving name\n");
          return -1;
        }
    name[len]='\0';
    if(*open<=0){
      status= HELLO_ERROR;
    }
    else{
      *open=*open - 1;
      }
    ret= send(sockfd,&status, 1, 0);
    if(ret <0) {
          perror("problem sending the final message\n");
          return -1;
        }
    if(status==HELLO_ERROR){
      return 2;
    }
    return 1;

  }

  return -1;
}


int sender(int sockfd, char* buf, size_t len, int flags){
    int done = 0;
    int start = 0;
    unsigned char tag[1];
    tag[0] = MSG_DATA;

    int ret = send(sockfd, tag, 1, MSG_NOSIGNAL);

    if(ret<0){
      if(errno==EPIPE){
        perror("Connection was unexpectedly killed and send is not working\n");
        return 3;
      }
      perror("Not sent properly\n");
      return -1;
    }

    unsigned char length[1];

    length[0] = (int)len;
    ret = send(sockfd, length, 1, MSG_NOSIGNAL);
    printf("%d\n",(int)len);
    if(ret<0){
      if(errno==EPIPE){
        perror("Connection was unexpectedly killed and send is not working\n");
        return 3;
      }
      perror("Not sent properly\n");
      return -1;
    }

    while(done !=1){
      ret = send(sockfd,buf+start, len, MSG_NOSIGNAL);
      start += ret;
      if(ret==-1){
        if(errno==EPIPE){
          perror("Connection was unexpectedly killed and send is not working\n");
          return 3;
        }
        perror("problem sending\n");
        return -1;
      }
      else if(start == len){
        buf[len] = '\0';
        printf("sent properly: %s\n",buf);
        return 1;
      }

    }
  return 1;
}

// unsigned char

int receiver(int sockfd, char* buf, int flags, int name_len){
    int done = 0;
    int start = 0;
    unsigned char tag[1];
    unsigned char len[1];

    int ret1= recv(sockfd,tag,1,0);
    if (ret1 ==0){
      perror("Socket was closed receiver\n");
      return 3;
    }
    if(ret1 < 0){
      perror("something is wrong receiving the message tag\n");
      //dont exit while running;
      return -1;
    }
    int ret2 = recv(sockfd,len,1,0);
    int length = (int)(len[0]);
    printf("length received%d\n", length);
    if (ret2 ==0){
      perror("Socket was closed receiver 2\n");
      return 3;
    }
    if(ret2 < 0){
      perror("something is wrong\n");
      return -1;
    }
    if(length>256){
      length=256-name_len;
    }
    if((int)tag[0] == (int)QUIT){
      buf[0] = QUIT;
      buf[1] = '\0';
      return 1;
    }
    else if((int)(tag[0]) == (int)MSG_DATA){
      //dont truncate the message recieved
      while(done !=1){
        printf("length of messaged recieved: %d\n",length);
        int ret = recv(sockfd,buf+start, length, 0);
        if (ret2 ==0){
          perror("Socket was closed receiver 3\n");
          return 3;
        }
        if(ret==-1){
          perror("problem receiving\n");
          return 1;
        }else if(ret == length){
          buf[length] = '\0';
          return 1;
        }else{
          done += ret;
          start+=ret;
        }
      }
    }else{
      printf("Else of reciever\n");
      return 1;
    }
return 1;
}



//this function is run by the sending thread of the
void* client_sender_thread(void* threadarg){
  printf("Starting the client sender thread\n");
  struct threadClientArg* arg = threadarg;
  int successfully = 1;
  while(running == 1){
    char* message = readline("$:  ");
    if(message==NULL){
      perror("Error in readline\n");
      successfully=-1;
      int* retval = &successfully;
      return retval;
    }
    char* quit = "goodbye";

    if(strcmp(message,quit)==0){
        char q = QUIT;
        int ret = sender(arg->sockfd, &q, 1, 0);
        // pthread_mutex_lock( &mutex1 );
        printf("Sending Quit\n");
        running=0;
        // pthread_mutex_unlock( &mutex1 );
        int* retval = &successfully;
        return retval;
    }


    int ret = sender(arg->sockfd, message, strlen(message), 0);
    if(ret < 0) {
      perror("Not sent properly\n");
      int* retval = &successfully;
      return retval;
      }

  }
  printf("Exiting the client sender thread\n");
  int* retval = &successfully;
  return retval;

}

//this function is run by the sending thread of the
void* client_receiver_thread(void* threadarg){
  struct threadClientArg* arg = threadarg;

  printf("Starting the client receiver thread\n");
  int successfully = 1;

  while(running == 1){
    char* message = malloc(sizeof(char)*BUFMAX);
    int ret = receiver(arg->sockfd, message, 0, arg->name_len);
    printf("i am here after the receiver\n");
    if(ret < 0) {
      perror("Not recieved properly\n");
      printf("in the ret < 0 condotion\n");
      int* retval = &successfully;
      return retval;
      }
    else if (ret ==3){
      perror("socket closed\n");
      printf("in the ret 3 condotion\n");
      running = 0;
      successfully = -1;
    }
      printf("%s\n",message);

    free(message);
  }
  printf("Exiting the client receiver thread\n");
  int* retval = &successfully;
  return retval;

}



int chat_client(int sockfd, char* name){
    //for each client that is able to join the network,
      //create two threads by pthread library
    pthread_t thread_receiver, thread_sender;

    char *message1 = "Receiver Thread";
    char *message2 = "Sender Thread";
    struct threadClientArg thread1;
    struct threadClientArg thread2;
    thread1.sockfd = sockfd;
    thread1.name_len = strlen(name);
    thread2.sockfd = sockfd;

    thread2.name_len = strlen(name);


    int  iret1, iret2;
    printf("Sender thread would be launched now been launched\n");
    iret1 = pthread_create( &thread_sender, NULL, client_sender_thread, (void*) &thread1);
    if(iret1<0){
      perror("There was an error in sender thread");
    }
    printf("receiver thread would be launched now been launched\n");

    iret2 = pthread_create( &thread_receiver, NULL, client_receiver_thread, (void*) &thread2);
    if(iret2<0){
      perror("There was an error in receiver thread");
    }


    pthread_join(thread_sender, NULL);
    pthread_join(thread_receiver, NULL);

    printf("Threads have joined\n");


return 1;
}
/*TODO: Need to finish writing this function
Ideally, each time it gets a message from its own? client,
it should store that message and update it in that client's messenger
It sho
*/
void* chat_server(void* server_args){
    struct threadserverArg* serverargs = server_args;
    for (int i = 0; i < 10; i++ ){

      printf("Thread number %d\n",i);
      if(serverargs->other_clients[i]!=NULL){
        printf("Thread's buffer %p\n", serverargs->other_clients[i]->buf);
      }
    }
    //For this server, each time one of the clients sends
    int running =1;
    int ret= 1;
    char* message=serverargs->client->buf+serverargs->client->name_len+1;

    while (running !=0){
      printf("here we go again\n");
      int ret = receiver(serverargs->client->sockfd, message, 0,serverargs->client->name_len);
      if(ret < 0) {
          perror("Server not recieving properly in chat\n");
          pthread_exit(&ret);
      }//if return value indicates the socket being shut off by client
      else if(ret==3){
        //increase the open value to 10.
        *serverargs->open = *serverargs->open+1;
        running = 0;
        int* retval = &ret;
        return retval;
      }

      printf("%s\n",serverargs->client->buf);
      printf("%d\n",serverargs->client->sockfd);

      //if the client says to quit, then quit
      if(*message==QUIT){
        //close the loop and return 1
        *serverargs->open = *serverargs->open+1;
        printf("Client %s has left the chat\n",serverargs->name_client);
        printf("Server ending connection\n");
        int* retval = &ret;
        return retval;
        }
     // if the recieved message is not the server quiting,
     // then it sends out to the rest of people.
     else{

     for(int i = 0; i<10;i++){

       printf("other client fd %d fd: %d i: %d\n",serverargs->other_clients[i]->sockfd,serverargs->client->sockfd,i);
       if(serverargs->other_clients[i]->active != 0){
         printf("Sending '%s' into: Sockfd %d from Sockfd %d\n",
         serverargs->client->buf,serverargs->other_clients[i]->sockfd,
         serverargs->sockfd);
          //if it quting, should it send it to each of the other clients?
          ret = sender(serverargs->other_clients[i]->sockfd, serverargs->client->buf,strlen(serverargs->client->buf),0);
          if(ret < 0) {
              perror("Not recieved properly, something is wrong with the sender function\n");
              // pthread_exit(&ret);
              ret=-1;
              break;
              }
          //if we dectected an error while sending
          else if(ret==3){
              //set open = 1
                *serverargs->open = *serverargs->open+1;
                running = 0;
                printf("I go into the 3rd return and breakout: chat server\n");
                break;

              }
            }
          }

        //if we need to quit out of the while loop
        if(ret<0 || ret==3){
          printf("in the extra condition for breaking\n");
          break;
        }

        printf(" i have checked each of the 10 addresses \n");
      }

    }

    printf("Client %s is here\n",serverargs->name_client);
    serverargs->client->active=0;
    *serverargs->open=*(serverargs->open)-1;
    pthread_exit(&ret);

    int* retval = &ret;
    return retval;
}




/**************************************************************/
