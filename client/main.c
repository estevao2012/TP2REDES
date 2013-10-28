#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../libs/mysocket.h"
#include "../libs/utils.h"

#define IP "127.0.0.1"
// #define IP "187.20.32.47"

  
int main(int argc, char *argv[])
{
    //---------  Variaveis --------//
    char resposta[MAXRBUFFER + 1]; 
    char mensagem[MAXRBUFFER + 1];
    char login[50]; 
    int numbytes, mysocket; 
    int *new_sock;
    struct sockaddr_in dest; 

    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) == -1){ perror("socket()"); exit(1); }

    memset(&dest, 0, sizeof(dest));           
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(IP);     
    dest.sin_port = htons(PORTNUM);           

    if (connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr)) == -1) {
        perror("connect()");
        exit(1);
    } 
    limpa_tela();
    gotoxy(0,0);
   
    //Recebe boas vindas e solicita login
    
    if ((numbytes = recv(mysocket, resposta, MAXRBUFFER, 0)) == -1){ perror("recv()");exit(1);}  
    // resposta[numbytes] = '\0';
    printf("%s\n",resposta );   

    fgets(login, sizeof(login) , stdin);
    login[strlen(login)-1] = '\0';

    if (send(mysocket, login, strlen(login), 0) == -1){ perror("send");exit(1);} 

    //Mensagem confirmação
    while(1){  

        if ((numbytes = recv(mysocket, resposta, MAXRBUFFER, 0)) == -1){ perror("recv()");exit(1);} 
        if( strcmp( resposta, "ok") == 0 ) break; 
        printf("%s\n",resposta );

        fgets(login, sizeof(login) , stdin);
        login[strlen(login)-1] = '\0';  

        if (send(mysocket, login, MAXLOGIN, 0) == -1){ perror("send");exit(1);} 

    }  

    printf("Usuário confirmado !\n");
    sleep(1);
    limpa_tela();
    gotoxy(0,0); 

    pthread_t fala,escuta;
    new_sock = malloc(1);
    *new_sock = mysocket; 

    if( pthread_create( &fala , NULL ,  conexao_usuario_escuta , (void*) new_sock) < 0){
        perror("could not create thread");
        return 1;
    }

    if( pthread_create( &escuta , NULL ,  conexao_usuario_fala , (void*) new_sock) < 0){
        perror("could not create thread");
        return 1;
    }

    pthread_join( fala , NULL );
    pthread_join( escuta , NULL );
   
    pthread_exit(NULL);
    close(mysocket);
    
 
    return EXIT_SUCCESS;
}
