/**
 * Created by: Oliver Jeremiah E Fernandez and Milton Melson
 * 
 * Client.cpp
 * 
 * The client side will establish a connecton to the server. 
 * Once connected it will display the rules of the game and 
 * the logic required to verify the correct input before sending to the server.
 * Once both players (Clients) have submitted their answer the server will reply with
 * a message stating if the player won, loss, or was a draw. 
*/

#include <iostream>
#include <cstdlib>        // atoi
#include <sys/time.h>     // gettimeofday
#include <sys/types.h>    // socket, bind 
#include <sys/socket.h>   // socket, bind, listen, inet_ntoa 
#include <netinet/in.h>   // htonl, htons, inet_ntoa 
#include <arpa/inet.h>    // inet_ntoa 
#include <netdb.h>        // gethostbyname 
#include <unistd.h>       // read, write, close 
#include <strings.h>      // bzero 
#include <netinet/tcp.h>  // SO_REUSEADDR 
#include <sys/uio.h>      // writev 
#include <cstring>        // memset 