/** 
 * Created by: Oliver Jeremiah E Fernandez and Milton Melson
*/

#include "Client.h"
#include "Server.h"

int main(int argc, char *argv[]) {
   // server
   if (argc == 1) {
      Socket server;
      server.createServer();
   }
   // client
   else if (argc == 2) {
      Socket socket;
      socket.createClient(argv[1]);
      Client player(socket.getClientSD());
   }
   return 0;
}


