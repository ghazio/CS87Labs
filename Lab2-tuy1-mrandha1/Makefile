#
# Makefile for string mangler client/server 
# to build client and server programs:
#  % make
# to clean up all stuff that can be re-built:
#  % make clean
#

CC = gcc
CFLAGS = -g -Wall -Wvla -Werror -Wno-error=unused-variable

LIBDIR = -L./
INCDIR = -I./

INCLUDES = cs87talk.h

LIBS = $(LIBDIRS) -lm -lreadline  -lpthread

CLIENT = cs87_client
SERVER = cs87_server

.PHONY: clean

all: $(SERVER) $(CLIENT)

cs87talk.o: cs87talk.c $(INCLUDES) 
	$(CC) $(INCDIRS) $(CFLAGS) -c cs87talk.c 

	
client.o: client.c $(INCLUDES) 
	$(CC) $(INCDIRS) $(CFLAGS) -c client.c 

$(CLIENT): client.o cs87talk.o $(INCLUDES) 
	$(CC) $(INDIRS) $(CFLAGS) -o $(CLIENT) client.o cs87talk.o $(LIBS) 

server.o: server.c $(INCLUDES) 
	$(CC) $(INCDIRS) $(CFLAGS) -c server.c 

$(SERVER): server.o cs87talk.o $(INCLUDES) 
	$(CC) $(INDIRS) $(CFLAGS) -o $(SERVER) server.o cs87talk.o $(LIBS) 


clean:
	$(RM) *.o core $(CLIENT) $(SERVER)

