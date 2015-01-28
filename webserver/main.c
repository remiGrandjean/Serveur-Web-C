#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include <unistd.h>

int main(void){
  int socket_serveur = creer_serveur(8000);
  
  while(socket_serveur!=-1){
    int socket_client ;
    socket_client = accept(socket_serveur , NULL , NULL);
    if (socket_client == -1)
      {
	perror("accept");
      }
    
  }
  return 0;
}
