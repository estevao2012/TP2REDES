void inicitLista(){
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
usuario* registra_user(int socketId , char nome[MAXLOGIN]){
	usuario* novo;
	novo = (usuario*) malloc( sizeof(usuario) );
	novo->socketId = socketId; 
	strncpy(novo->nome, nome, MAXLOGIN); 
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

void *connection_handler(void *socket_desc){
   int consocket = *(int*)socket_desc;
   int len;
   char login[MAXLOGIN+1];
   char buffer[MAXRCVLEN + 1];
   char boasvindas[] = "Bem vindo , por favor digite seu usuário!";

   usuario *User;
   numUsersAtivos++;
   printf("USUARIOS ATIVOS : %d\n", numUsersAtivos );
    
   send(consocket, boasvindas, MAXRCVLEN, 0); 

   len = recv(consocket,login,MAXLOGIN,0); 
   printf("%s conectou.\n", login);
   User = registra_user( consocket , login ); 
   imprimeLista();
   while(1){
        len = recv(consocket,buffer,MAXRCVLEN,0);  
        buffer[len] = '\0';
        if(len == 0) break; 
        if(strcmp(buffer,"q") == 0) break;
        // printf("%d\n", numUsersAtivos );
        printf("%d : %s disse ", User->id , User->nome ); 
        printf("%s\n", buffer ); 
    } 
    printf("%s saiu\n",User->nome);
    listUsers[User->id] = NULL;
    numUsersAtivos--;
   	free(socket_desc);
     
   return 0;
}