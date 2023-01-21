/** 
 * Created by: Oliver Jeremiah E Fernandez and Milton Melson
 * 
 * Server.cpp
 * 
 * The server side will wait for two clients to connect before 
 * proceeding with the game logic of rock, paper, scissors.
 * Once the two clients have connected, the server waits for two 
 * responses from the clients which will be error checked on client side 
 * prior to sending their answer. The rules will follow basic rock, paper, scissors. 
 * Which is (Rock beats scissors, Scissors beats paper, Paper beats rock).
*/

#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

using namespace std;

// Server side
int main(int argc, char *argv[]) {

   // for the server, we only need to specify a port number
   if(argc != 2)
   {
      cerr << "Usage: port" << endl;
      exit(0);
   }

   int port = atoi(argv[1]);

   // buffer to send and receive messages with
   char msg[1500];
   
   // setup a socket and connection tools
   sockaddr_in servAddr;
   memset(&servAddr, 0, sizeof(servAddr));
   servAddr.sin_family = AF_INET;
   servAddr.sin_addr.s_addr = htonl(INADDR_ANY);  
   servAddr.sin_port = htons(port);

   // open stream oriented socket with internet address
   // also keep track of the socket descriptor
   int serverSd = socket(AF_INET, SOCK_STREAM, 0);
   if(serverSd < 0) {
      cerr << "Error establishing the server socket" << endl;
      exit(0);
   }

   // lose the pesky "Address already in use" error message
   int yes=1;
   if (setsockopt(serverSd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) == -1) {
      perror("setsockopt");
      exit(1);
   } 

   // bind the socket to its local address
   int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, sizeof(servAddr));
   if(bindStatus < 0) {
      cerr << "Error binding socket to local address" << endl;
      exit(0);
   }

   cout << "Waiting for a client to connect..." << endl;

   // listen for up to 5 requests at a time
   listen(serverSd, 5);

   // receive a request from client using accept
   // we need a new address to connect with the client
   sockaddr_in newSockAddr;    // address container
   socklen_t newSockAddrSize = sizeof(newSockAddr);

   //  accept, create a new socket descriptor to 
   //  handle the new connection with client
   int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
   if(newSd < 0) {
      cerr << "Error accepting request from client!" << endl;
      exit(1);
   }

   cout << "Connected with client!" << endl;

   // Game logic


   // close socket descriptors
   close(newSd);
   close(serverSd);
   return 0;
}