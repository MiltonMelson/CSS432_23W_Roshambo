/** 
 * Created by: Oliver Jeremiah E Fernandez and Milton Melson
*/

#include "Roshambo.h"

int main(int argc, char *argv[]) {
   // server
   if (argc == 1) {
      Socket server;
      server.createServer();
   }
   // client
   else if (argc == 2) {
      Socket client;
      client.createClient(argv[1]);
      Roshambo player;
      player.enterLobby();
   }
   return 0;
}


