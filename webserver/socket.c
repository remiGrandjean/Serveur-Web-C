#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

int creer_serveur(int port){
  int socket_server;
  int option =1;
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(port);
  saddr.sin_addr.s_addr = INADDR_ANY;
 
  socket_server=socket(AF_INET,SOCK_STREAM,0);

  initialiser_signaux();

  if(socket_server==-1){
    perror("socket serveur");
    /*Traitement de l'erreur*/
  }
  if(setsockopt( socket_server , SOL_SOCKET , SO_REUSEADDR ,&option , sizeof(int))==-1){
    perror("Can not set SO_REUSEADDR option");
  }

  if(bind(socket_server,(struct sockaddr *)&saddr, sizeof(saddr)) == -1){
    perror("bind socket_server");
    /*Traitement de l'erreur*/
  }

  if(listen(socket_server,10)==-1){
    perror("listen socket_server");
    /*Traitement de l'erreur*/
  }
  
  int socket_client; 
  struct sockaddr_in caddr; 
  socklen_t csize = sizeof(caddr);
    
  if((socket_client = accept(socket_server, (struct sockaddr *)&caddr, &csize)) < 0){    
    perror("accept");
    return errno;
  }

   const char *message = "Bienvenue sur le serveur web TomCinq\n";
   sleep(1);
   write(socket_client, message,strlen(message)); 
   
  
  char nom[20]; 
  if(read(socket_client, &nom, sizeof(nom)) == -1)
    {
      perror("read nom");
      return errno;
    }
    
  printf("%s s'est connecte.\n", nom);
 
    
  char buffer[1024];    
  int continuer = 0;
  ssize_t size;
  do
    {
      size=read(socket_client, buffer, sizeof(buffer));
      if(size == -1)
        {
	  perror("read");
	  return errno;
        }
      
      if(strcmp(buffer, "exit") == 0){
	continuer = 1;
      }else{
	buffer[size+1]='\0';
	 
	printf("\033[40;31m");
	printf("%s   %s\n", nom, buffer);
	    
      } 	    
    }while(continuer == 0);    
       
  printf("%s s'est deconnecte.\n", nom);   
  close(socket_server);

  return 0; 

  
  
}

void initialiser_signaux(void){


  if(signal(SIGPIPE,SIG_IGN)==SIG_ERR)
    {
      perror("signal");
    }

}

