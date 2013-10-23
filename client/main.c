#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/usuarios.h"
#include "../libs/mysocket.h"
  
int main(int argc, char *argv[])
{
    //---------  Variaveis --------//
    char resposta[MAXRCVLEN + 1]; 
    char mensagem[MAXRCVLEN + 1];
    char login[50];
    unsigned short port_recv;
    int indice_porta;
    in_addr_t addressIp; 
    int numbytes, mysocket;
    struct sockaddr_in dest;
    port_recv = PORTNUM; 

    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) == -1){ perror("socket()"); exit(1); }

    memset(&dest, 0, sizeof(dest));           
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(IP);     
    dest.sin_port = htons(port_recv);           

    if (connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr)) == -1) {
        perror("connect()");
        exit(1);
    } 
   
    //Recebe boas vindas
    {
        if ((numbytes = recv(mysocket, resposta, MAXRCVLEN, 0)) == -1){
            perror("recv()");
            exit(1);
        } 
        resposta[numbytes] = '\0';
        printf("%s\n",resposta );  
    } 

    fgets(login, sizeof(login) , stdin);
    login[strlen(login)-1] = '\0';

    if (send(mysocket, login, strlen(login), 0) == -1){
        perror("send");
        exit(1);
    } 

    //Loop para enviar comandos para servidor
    while(1){ 
        printf("Deseja enviar uma mensagem?\n"); 
        fgets(mensagem, sizeof(mensagem) , stdin);
        mensagem[strlen(mensagem)-1] = '\0';

        if (send(mysocket, mensagem, strlen(mensagem), 0) == -1){
            perror("send");
            exit(1);
        } 
        if( strcmp(mensagem,"q") == 0 ) break; 
    }
   
    close(mysocket);
    
 
    return EXIT_SUCCESS;
}
