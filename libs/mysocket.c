void broadcast(char *msg){
  int i;
  printf("Enviando mensagem para todos usuarios logados .\n");
  for(i=0;i<64;i++){
    if( listUsers[i] != NULL ){ 
      send(listUsers[i]->socketId, msg, MAXRBUFFER, 0); 
    }
  }
}

void listaUsuariosLogados(int socketId){
  int i;
  printf("Enviando para %d a lista de usuarios\n", socketId);
  send(socketId, "\33[1mPessoas online\33[0m", MAXRBUFFER, 0); 
  for(i=0;i<64;i++){
    if( listUsers[i] != NULL ){ 
      send(socketId, listUsers[i]->nome, MAXRBUFFER, 0); 
    }
  }
}

void iniciaMutexs(){
  if (pthread_mutex_init(&mLogin, NULL) != 0){printf("\n mutex init failed\n"); } 
}

int mensagem_direta( usuario *User ){

  int len;
  char login[MAXLOGIN+1];  
  int to_Id;
  char buffer[MAXRCVLEN+1];
  char padraoMensagem[MAXRBUFFER +1];

  send(User->socketId , "Digite o nick do destinatário da mensagem." , MAXRBUFFER, 0);

  len = recv(User->socketId,login,MAXLOGIN,0);   
  login[len] = '\0'; 
 
  to_Id = usuario_encontrado(login) ;
  if( to_Id == -1){
    send(User->socketId , "\33[31;49mUsuário não encontrado.\33[39;49m" , MAXRBUFFER, 0);      
  }else{
    send(User->socketId , "Digite sua mensagem." , MAXRBUFFER, 0);   

    len = recv(User->socketId,buffer,MAXRCVLEN,0);   
    buffer[len] = '\0';
    if( strcmp( buffer , "") == 0){
      len = recv(User->socketId,buffer,MAXRCVLEN,0);   
      buffer[len] = '\0';
    }
    printf("Mensagem privada\n");
    printf("MENSAGEM : %s\n",buffer ); 
    printf("Nome destino %s\n", listUsers[to_Id]->nome);
    sprintf( padraoMensagem , "\33[1m\33[33;49m(PRIVATE)\33[39;49m\33[34;49m%s \33[39;49mdisse para \33[35;49m%s\33[39;49m:\33[0m %s ", User->nome , listUsers[to_Id]->nome , buffer );
    
    send(listUsers[to_Id]->socketId , padraoMensagem , MAXRBUFFER, 0);    
    send(User->socketId , padraoMensagem , MAXRBUFFER, 0);    

    printf("%s enviou mensagem para %s\n",User->nome , listUsers[to_Id]->nome );
  }
  return 0;
}

usuario* troca_login(int consocket , int id){

  usuario *User ;
  int len; 
  char oldLogin[ MAXLOGIN + 1];
  char padraoMensagem[MAXRBUFFER + 1];
  char login[MAXLOGIN+1];

  User = listUsers[id];

  send(consocket, "Por favor digite o nick desejado." , MAXRBUFFER, 0); 

  len = recv(consocket,login,MAXLOGIN,0);   
  login[len] = '\0';  
         
  if( usuario_encontrado(login) != -1 ){
    printf("Usuário já possui esse nick.");
    send(consocket, "Usuário já possui esse nick.", MAXRBUFFER, 0);   
  }else{  
  
    send(consocket, "ok", MAXRBUFFER, 0);

    printf("Ação Troca login para %s\n", User->nome); 

    strncpy(oldLogin, User->nome, 50);
    strncpy(User->nome,login,50);

    sprintf( padraoMensagem , "\33[1m%s trocou login para %s\33[0m" , oldLogin , User->nome );
    broadcast( padraoMensagem );
  }
  return User;
}

usuario* autentica_login(int consocket ){

  usuario *User;
  int len;
  char login[MAXLOGIN+1]; 
  char buffer[MAXRCVLEN+1];

  //Enviando as boas vindas
  send(consocket, "Por favor digite seu nick.", MAXRBUFFER, 0);  

  len = recv(consocket,login,MAXLOGIN,0);   
  login[len] = '\0'; 

  while( 1 ){  

    if( usuario_encontrado(login) == -1  ) 
      break; 
    else { 
      printf("Um usuário já utiliza esse nick , digite outro por favor." );
      send(consocket, "Um usuário já utiliza esse nick , digite outro por favor.", MAXRBUFFER, 0);  
      if(len = recv(consocket, login, MAXRBUFFER, 0) == -1){ perror("recv()");exit(1);}  
    } 

  }  
  send(consocket, "ok", MAXRBUFFER, 0);  

  //Contando o novo usuario 
  pthread_mutex_lock(&mLogin);
    numUsersAtivos++; 
    User = registra_user( consocket , login ); 
  pthread_mutex_unlock(&mLogin);

  sprintf(buffer, "%s conectou.", login); 
  broadcast( buffer );

  printf("User %s entrou\n", login );
  return User; 
}

void *conexao_servidor(void *socket_desc){

  int consocket = *(int*)socket_desc;
  int len;
  char buffer[MAXRCVLEN + 1];
  char padraoMensagem[MAXRBUFFER + 1]; 
  usuario *User;

  User = autentica_login( consocket );  

  while(1){ 
    len = recv(consocket,buffer,MAXRCVLEN,0);  
    buffer[len] = '\0'; 

    if(len == 0)break;  

    if(strcmp( buffer ,":q") == 0)
      break;
    else if( strcmp( buffer ,":l") == 0 )
      listaUsuariosLogados(User->socketId);   
    else if( strcmp( buffer , ":u") == 0) 
      User = troca_login( User->socketId , User->id);
    else if( strcmp( buffer , ":m") == 0 ){
      mensagem_direta( User ); 
    }
    else{
      if( strcmp( buffer , "") != 0 && strcmp( buffer , "\0") != 0){ 
        printf("\33[1m\33[34;49m%s \33[39;49mdisse:\33[0m %s\n", User->nome , buffer ); 
        sprintf( padraoMensagem , "\33[1m\33[34;49m%s \33[39;49mdisse:\33[0m %s", User->nome , buffer );  
        broadcast( padraoMensagem );   
      }       
    }
  }  

  pthread_mutex_lock(&mLogin);
  listUsers[User->id] = NULL;
  numUsersAtivos--; 
  pthread_mutex_unlock(&mLogin);

  send(User->socketId, ":q", MAXRBUFFER, 0);  
  sprintf(buffer , "%s saiu",User->nome);
  broadcast( buffer ) ;

  close(consocket);
  free( User ); 
  free(socket_desc);

  return 0;
} 

void *conexao_usuario_escuta(void *socket_desc){

  int mysocket = *(int*)socket_desc;
  int numbytes;
  char resposta[MAXRBUFFER + 1]; 
  int posY = 1;

  while(1){ 

    if ((numbytes = recv(mysocket, resposta, MAXRBUFFER, 0)) == -1){perror("recv()");exit(1); } 
    resposta[numbytes] = '\0'; 

    if(strcmp(resposta,":q") == 0)
      break;  
    else if( strcmp(resposta,"ok") == 0 ){}
    else if(strcmp(resposta,"") == 0 ){}
    else{
      printf("%s\n----\n\33[J\33[1A",resposta );  
      // printf("\033[1L");
      // printf("");    
      // posY++;   
    }

  } 

  free(socket_desc); 
  return 0;
}

void *conexao_usuario_fala(void *socket_desc){
  int mysocket = *(int*)socket_desc;
  int numbytes;
  char mensagem[MAXRCVLEN + 1]; 

  while(1){  

    fgets(mensagem, sizeof(mensagem) , stdin);
    mensagem[strlen(mensagem)-1] = '\0';   
    printf("\33[1A\33[J");
    if (send(mysocket, mensagem, MAXRCVLEN, 0) == -1){perror("send");exit(1); } 
    if( strcmp(mensagem,":q") == 0 ) break; 
 
  }

  return 0;
}