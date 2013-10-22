usuario* registra_user(int socketId , char nome[MAXLOGIN]){
	usuario* novo;
	novo = (usuario*) malloc( sizeof(usuario) );
	novo->socketId = socketId; 
	strncpy(novo->nome, nome, MAXLOGIN); 
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

   while(1){
        len = recv(consocket,buffer,MAXRCVLEN,0);  
        buffer[len] = '\0';
        if(len == 0) break; 
        if(strcmp(buffer,"q") == 0) break;
        // printf("%d\n", numUsersAtivos );
        printf("%s disse ", User->nome ); 
        printf("%s\n", buffer ); 
    } 
    printf("%s saiu\n",User->nome);

        numUsersAtivos--;
   free(socket_desc);
     
   return 0;
}