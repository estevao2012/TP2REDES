usuario* init_user(){
	usuario* novo = (usuario*) malloc( sizeof(usuario));
	return novo;
}

usuario* registra_user(int socketId , char *nome){
	usuario* novo;
	novo = (usuario*) malloc( sizeof(usuario) );
	novo->socketId = socketId;
	novo->nome = nome;
	// strcpy(nome , novo->nome);
	return novo;
}