#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../libs/mysocket.h"
#include "../libs/usuarios.h"


int main(int argc, char *argv[])
{
    char buffer[MAXRCVLEN + 1];
    char login[MAXLOGIN + 1];

    // pthread_t Users[64];
    // int tUser;

     
    struct sockaddr_in dest; /* socket info about the machine connecting to us */
    struct sockaddr_in serv; /* socket info about our server */
    int mysocket;            /* socket used to listen for incoming connections */
    int len;
    int consocket;
    int *new_sock;
    socklen_t socksize = sizeof(struct sockaddr_in);
 
    memset(&serv, 0, sizeof(serv));           /* zero the struct before filling the fields */
    serv.sin_family = AF_INET;                /* set the type of connection to TCP/IP */
    serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
    serv.sin_port = htons(PORTNUM);           /* set the server port number */    
 
    mysocket = socket(AF_INET, SOCK_STREAM, 0);

    bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));
 
    listen(mysocket, 64);
 
    // while(numUsersAtivos < 64) 
    // {   
        
    //     consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
       
    //     //Inicializa novo usuario
    //     // usuario* User;        
    //     tUser = pthread_create(&Users[numUsersAtivos],NULL,trabalhaUsuario,(void *)consocket);
    //     pthread_detach(Users[numUsersAtivos]);
    //     // while(1){
    //     //     len = recv(consocket,buffer,MAXRCVLEN,0);  
    //     //     buffer[len] = '\0';
    //     //     if(len == 0) break; 
    //     //     if(strcmp(buffer,"q") == 0) break;
    //     //     printf("%d\n", numUsersAtivos );
    //     //     printf("%s disse ", User->nome ); 
    //     //     printf("%s\n", buffer ); 
    //     // }
    //     // numUsersAtivos--;
    //     // printf("saiu\n");
    //         // exit(0); 
    //     // }else{
    //     // close(consocket);
    //     // }  

    //     // consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);

    // }
    while( (consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize ) ) )
    {
        // printf("Connection accepted\n");
         
        pthread_t sniffer_thread[64];
        new_sock = malloc(1);
        *new_sock = consocket;

        if( pthread_create( &sniffer_thread[numUsersAtivos] , NULL ,  connection_handler , (void*) new_sock) < 0){
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        // pthread_join( sniffer_thread[numUsersAtivos] , NULL);
        // numUsersAtivos--;
        // printf("Fechando a casa para %d", numUsersAtivos);
    }
     
    if (consocket < 0)
    {
        perror("accept failed");
        return 1;
    }

    close(mysocket);
    pthread_exit(NULL);
    return EXIT_SUCCESS;
}
