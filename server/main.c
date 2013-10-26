#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> 
#include "../libs/mysocket.h"


int main(int argc, char *argv[])
{
    char buffer[MAXRBUFFER + 1];
    char login[MAXLOGIN + 1]; 
     
    struct sockaddr_in dest; /* socket info about the machine connecting to us */
    struct sockaddr_in serv; /* socket info about our server */
    int mysocket;            /* socket used to listen for incoming connections */
    int len;
    int consocket;
    int *new_sock;
    int idThread;
    socklen_t socksize = sizeof(struct sockaddr_in);
    
    iniciaListaUsuarios();
    
    memset(&serv, 0, sizeof(serv));           /* zero the struct before filling the fields */
    serv.sin_family = AF_INET;                /* set the type of connection to TCP/IP */
    serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
    serv.sin_port = htons(PORTNUM);           /* set the server port number */    
 
    mysocket = socket(AF_INET, SOCK_STREAM, 0);

    bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));
 
    listen(mysocket, 64); 

    while( (consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize ) ) )
    {
        // printf("Connection accepted\n");
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = consocket;

        if( pthread_create( &sniffer_thread , NULL ,  conexao_servidor , (void*) new_sock) < 0){
            perror("could not create thread");
            return 1;
        }

         
        //Now join the thread , so that we dont terminate before the thread
        // pthread_join( sniffer_thread , NULL); 
    }
     
    if (consocket < 0){
        perror("accept failed");
        return 1;
    }

    close(mysocket);
    pthread_exit(NULL);
    return EXIT_SUCCESS;
}
