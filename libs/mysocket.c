void *conexao_servidor(void *socket_desc){
   int consocket = *(int*)socket_desc;
   int len;
   char login[MAXLOGIN+1];
   char buffer[MAXRCVLEN + 1];
   char boasvindas[] = "Bem vindo , por favor digite seu usuário!"; 
   usuario *User;

   //Contando o novo usuario
   numUsersAtivos++; 
   qtsUsuariosAtivos();
    
   //Enviando as boas vindas
   send(consocket, boasvindas, MAXRCVLEN, 0); 

   len = recv(consocket,login,MAXLOGIN,0);  
   User = registra_user( consocket , login ); 
   printf("%s conectou.\n", login); 

   while(1){

        len = recv(consocket,buffer,MAXRCVLEN,0);  
        buffer[len] = '\0';
        if(len == 0) break; 
        if(strcmp(buffer,"q") == 0) break; 

        printf("%d : %s disse ", User->id , User->nome ); 
        printf("%s\n", buffer ); 
    } 
    printf("%s saiu\n",User->nome);

    listUsers[User->id] = NULL;
    numUsersAtivos--;
   	free(socket_desc);
     
   return 0;
}