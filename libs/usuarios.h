#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysocket.h"

typedef struct  {
      int socketId;
      char nome[MAXLOGIN]; 
}usuario; 

usuario allUsers[64];
int numUsersAtivos = 0;

usuario* registra_user(int socketId , char nome[MAXLOGIN]);

#include "usuarios.c"