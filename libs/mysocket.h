#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>   
#include <errno.h> 
#include <netdb.h> 

#define PORTNUM 2343
#define IP "127.0.0.1"
#define MAXRCVLEN 500
#define MAXLOGIN 50 

void broadcast(char *msg);
void *conexao_servidor(void *socket_desc);

#include "mysocket.c"