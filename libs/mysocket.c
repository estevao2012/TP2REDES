void broadcast(char *msg){
  int i;
  printf("Enviando broadcast\n");
  for(i=0;i<64;i++){
    if( listUsers[i] != NULL ){ 
      send(listUsers[i]->socketId, msg, MAXRCVLEN, 0); 
    }
  }
}

void *conexao_servidor(void *socket_desc){
   int consocket = *(int*)socket_desc;
   int len;
   char login[MAXLOGIN+1];
   char buffer[MAXRCVLEN + 1];
   char boasvindas[] = "Bem vindo , por favor digite seu usuário!"; 
   char novologin[] = "Usuário já logado , digite outro por favor."; 
   usuario *User;

   //Contando o novo usuario
   numUsersAtivos++; 
   qtsUsuariosAtivos();
    
   //Enviando as boas vindas
   send(consocket, boasvindas, MAXRCVLEN, 0); 

   len = recv(consocket,login,MAXLOGIN,0);  
   printf("%s\n", login );

   while( 1 ){       
     if( usuario_valido(login) == 1 ) break;
     else{ 
        printf("%s\n",novologin );
        send(consocket, novologin, MAXRCVLEN, 0);  
        if(len = recv(consocket, login, MAXRCVLEN, 0) == -1){ perror("recv()");exit(1);}    
     }

   }

   printf("User ok\n");

   send(consocket, "ok", MAXRCVLEN, 0); 

   // exit(0); 

   User = registra_user( consocket , login ); 
   sprintf(buffer, "%s conectou.\n", login); 
   broadcast( buffer );

   while(1){

        len = recv(consocket,buffer,MAXRCVLEN,0);  
        buffer[len] = '\0';
        if(len == 0) break; 
        if(strcmp(buffer,":q") == 0) break; 

        printf("%d : %s disse ", User->id , User->nome ); 
        printf("%s\n", buffer );

        broadcast( buffer );
    } 
    printf("%s saiu\n",User->nome);

    send(consocket, buffer, MAXRCVLEN, 0); 
    listUsers[User->id] = NULL;
    free( User );
    numUsersAtivos--;
    close(consocket);
   	free(socket_desc);
     
   return 0;
} 

void *conexao_usuario_escuta(void *socket_desc){
  int mysocket = *(int*)socket_desc;
  int numbytes;
  char resposta[MAXRCVLEN + 1]; 
  int posY = 0;
  while(1){
 
      if ((numbytes = recv(mysocket, resposta, MAXRCVLEN, 0)) == -1){perror("recv()");exit(1); } 
      resposta[numbytes] = '\0';
      if(strcmp(resposta,":q") == 0) break;  
      printf("\033[%d;0f%s\033[K\n",posY,resposta );  
      printf("\033[1L");
      printf("Deseja enviar uma mensagem? %i\n\33[1K\33[K\33[J",posY);   
      posY++;   
  }
 
  // free(socket_desc); 
  return 0;
}

void *conexao_usuario_fala(void *socket_desc){
  int mysocket = *(int*)socket_desc;
  int numbytes;
  char mensagem[MAXRCVLEN + 1];
  int i=0;
  while(1){ 
      // gotoxy(10,0); 

      fgets(mensagem, sizeof(mensagem) , stdin);
      mensagem[strlen(mensagem)-1] = '\0'; 

      if (send(mysocket, mensagem, strlen(mensagem), 0) == -1){perror("send");exit(1); } 

      if( strcmp(mensagem,":q") == 0 ) break;
      i++;
  }

  free(socket_desc); 
  return 0;
}