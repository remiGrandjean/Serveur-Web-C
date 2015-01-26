#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>

int creer_serveur(int port){
  int socket_server;
  int socket_client;

  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(8000);
  saddr.sin_addr.s_addr = INADDR_ANY;
 
  socket_server=socket(AF_INET,SOCK_STREAM,0);
  socket_client=accept(socket_server,NULL,NULL);

 

  if(socket_server==-1){
    perror("socket serveur");
    /*Traitement de l'erreur*/
  }
  /*Utilisation de la socket */

  if(socket_client==-1){
    perror("accept");
    /* Traitement de l'erreur*/
  }
  const char * message_bienvenue = "Bonjour , bienvenue sur mon serveur \n";
  write (socket_client , message_bienvenue , strlen(message_bienvenue));


  if(bind(socket_server,(struct sockaddr *)&saddr, sizeof(saddr)) == -1){
    perror("bind socket_server");
    /*Traitement de l'erreur*/
  }

  if(listen(socket_server,10)==-1){
    perror("listen socket_server");
    /*Traitement de l'erreur*/
  }
  
  
}
