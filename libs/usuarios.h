#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct {
      int socketId;
      char nome[50]; 
      int id;
}usuario; 
 

int numUsersAtivos = 0;
usuario* listUsers[64];

void iniciaListaUsuarios();
void imprimeLista();
void qtsUsuariosAtivos();
usuario* registra_user(int socketId , char nome[50]);
int usuario_valido( char nome[50] );

#include "usuarios.c"