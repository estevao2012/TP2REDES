#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>   
#include <errno.h> 
#include <netdb.h> 
#include "../libs/usuarios.h"

#define PORTNUM 2343
#define MAXRCVLEN 500
#define MAXRBUFFER 570
#define MAXLOGIN 50 

pthread_mutex_t mLogin; 

void broadcast(char *msg);
void *conexao_servidor(void *socket_desc);
void *conexao_usuario_escuta(void *socket_desc);
void *conexao_usuario_fala(void *socket_desc);

#include "mysocket.c"