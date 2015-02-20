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
#include <sys/wait.h>

int creer_serveur(int port){
  int socket_server;
  int option =1;
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(port);
  saddr.sin_addr.s_addr = INADDR_ANY;
 
  socket_server=socket(AF_INET,SOCK_STREAM,0);

 

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
  char buffer[1024];    
  int pid;
  const char *message = "Bienvenue sur le serveur web TomCinq\n";
  int status;
  FILE *f;

  initialiser_signaux();
 
  while(1){
    if((socket_client = accept(socket_server, (struct sockaddr *)&caddr, &csize)) < 0){    
      perror("accept");
      return errno;
    }
    if((pid=fork())==-1){
      perror("fork");
    }
    f=fdopen(socket_client,"w+");
    if(f==NULL){
      perror("fdopen");
      return errno;
    }
   
    if(pid==0){
      fwrite(message,1,strlen(message),f);
      while(1)
	{  
  
	  if(fgets(buffer,sizeof(buffer),f)==NULL){
	    perror("fgets");
	    return errno;
	   }
	  if(strncmp(buffer, "exit\r\n",strlen(buffer)) == 0){
	    printf("deconnection.\n");
	    close(socket_server);
	    exit(0);
	  }else{
	    fprintf(f,"<TomCinq> %s ",buffer);
	    // fwrite(buffer,1,strlen(buffer),f);	    
	  }	 
	}   
    }  
    waitpid(pid,&status,0);
    fclose(f);
    close(socket_client);    
    return 0; 
  }
}
   void traitement_signal(int sig){

     printf("Signal %d recu\n",sig);
   }
   void initialiser_signaux(void){
     struct sigaction sa;
     sa.sa_handler = traitement_signal;
     sigemptyset(&sa.sa_mask);
     sa.sa_flags=SA_RESTART;
     if(sigaction(SIGCHLD, &sa, NULL)==-1)
       {
	 perror("sigaction(SIGCHLD)");
       }
     if(signal(SIGPIPE,SIG_IGN)==SIG_ERR)
       {
	 perror("signal");
       }

   }
