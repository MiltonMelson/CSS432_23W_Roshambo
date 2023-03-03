// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

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
      if (socket.createClient(argv[1])) {
         Client player(socket.getClientSD());
      }
   }
   return 0;
}


