#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/mysocket.h"
#include "../libs/usuarios.h"
  

 
int main(int argc, char *argv[])
{
    char boasvindas[] = "Bem vindo , por favor digite seu usuário!\0";
    char* login;
    login = (char*)malloc( sizeof(char*)* MAXLOGIN);

    char buffer[MAXRCVLEN + 1]; /* +1 so we can add null terminator */
    struct sockaddr_in dest; /* socket info about the machine connecting to us */
    struct sockaddr_in serv; /* socket info about our server */
    int mysocket;            /* socket used to listen for incoming connections */
    int len;
    int consocket;
    socklen_t socksize = sizeof(struct sockaddr_in);
 
    memset(&serv, 0, sizeof(serv));           /* zero the struct before filling the fields */
    serv.sin_family = AF_INET;                /* set the type of connection to TCP/IP */
    serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
    serv.sin_port = htons(PORTNUM);           /* set the server port number */    
 
    mysocket = socket(AF_INET, SOCK_STREAM, 0);

    bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));
 
    listen(mysocket, 5);
 
    while(1) 
    {   
        
        consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
       
        if( fork() == 0){
            //Inicializa novo usuario
            usuario* User = init_user();

            close(mysocket);
            send(consocket, boasvindas, strlen(boasvindas), 0); 

            len = recv(consocket,login,MAXLOGIN,0); 
            printf("%s conectou.\n", login);

            User = registra_user( consocket , login );
            while( 1 ){
                len = recv(consocket,buffer,MAXRCVLEN,0);  
                buffer[len] = '\0';
                if(len == 0) break; 
                if(strcmp(buffer,"q") == 0) break;
                printf("%s disse ", User->nome ); 
                printf("%s\n", buffer ); 
            }

            printf("saiu\n");
            exit(0);
        }else{ 
            close(consocket);
        }
        
        // consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
    }
 
    close(mysocket);
    return EXIT_SUCCESS;
}
