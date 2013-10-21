#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/mysocket.h"
 
#define MAXRCVLEN 500
#define PORTNUM 80
#define IP "127.0.0.1"

int main(int argc, char *argv[])
{
    //---------  Variaveis --------//
    char resposta[MAXRCVLEN + 1]; 
            
    char ip[16]; 
    unsigned short port_recv;
    int indice_porta;
    in_addr_t addressIp;
    //---------  Variaveis --------// 
    int numbytes, mysocket;
    struct sockaddr_in dest;
    port_recv = PORTNUM;

    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket()");
        exit(1);
    }

    memset(&dest, 0, sizeof(dest));           
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(IP);     
    dest.sin_port = htons(port_recv);           

    // if (connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr)) == -1) {
    //     printf("ERRO ip: %s",ip);
    //     printf(" port: %i\n",port_recv);
    //     perror("connect()");
    //     exit(1);
    // }
    
    // if (send(mysocket, msg, strlen(msg), 0) == -1){
    //     perror("send");
    //     exit(1);
    // }

    // if ((numbytes = recv(mysocket, resposta, MAXRCVLEN, 0)) == -1){
    //     perror("recv()");
    //     exit(1);
    // }

    resposta[numbytes] = '\0';
   
     
   
    close(mysocket);
    
 
    return EXIT_SUCCESS;
}
