void iniciaListaUsuarios(){
	int i;  
	for(i=0;i<64;i++){
		listUsers[i] = NULL;
	}	
}

void imprimeLista(){
	int i; 
	printf("LISTA USUARIOS\n");
	for(i=0;i<64;i++){
		if( listUsers[i] != NULL)
		printf("%d %s\n" , listUsers[i]->id , listUsers[i]->nome );
	}
}

void qtsUsuariosAtivos(){
	printf("USUARIOS ATIVOS : %d\n", numUsersAtivos );
}

usuario* registra_user(int socketId , char nome[50] ){
	usuario* novo;
	novo = (usuario*) malloc( sizeof(usuario) );
	novo->socketId = socketId; 
	strncpy(novo->nome, nome, 50); 
	int i;

	for(i=0;i<64;i++){
		if( listUsers[i] == NULL){
			listUsers[i] = novo;
			novo->id = i;
			break;
		}
	} 

	return novo;
}

int usuario_encontrado( char nome[50] ){ 
	int i; 
	for(i=0;i<64;i++){
		if( listUsers[i] != NULL)
			if( strcmp( listUsers[i]->nome , nome ) == 0 )
				return i; 
	}

	return -1;
}
 