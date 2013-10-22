usuario* registra_user(int socketId , char nome[MAXLOGIN]){
	usuario* novo;
	novo = (usuario*) malloc( sizeof(usuario) );
	novo->socketId = socketId; 
	strncpy(novo->nome, nome, MAXLOGIN); 
	return novo;
}