// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#include "Client.h"
#include "Server.h"


/**
 * @brief This is the driver function for the game.
 * The server and client will be determined based on arguments provided
 * @param argc If is it 1 then the server will be created. If it is 2 then 
 * the client will be created with provided destination IP/hostname.
*/
int main(int argc, char *argv[]) {
   // server
   if (argc == 1) {
      Socket server;    // initialize socket
      server.createServer();  // create the server 
   }
   // client
   else if (argc == 2) {
      Socket socket;    // initialize socket

      // trys to create the client socket and connects to server
      if (socket.createClient(argv[1])) { 
         // if connected create the client and pass in the socket descriptor
         Client player(socket.getClientSD());   
      }
   }
   return 0;
}


