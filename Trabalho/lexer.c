//** Lexer.c **//

char *textoDoArquivo(char* arquivo_entrada){

  FILE *arq  = fopen(arquivo_entrada,"r");
  char *text = (char*) malloc(sizeof(char));
  char c;
  int size = 0;

  while( EOF != (c = fgetc(arq))) {

    text[size] = toupper(c);
    text = realloc(text, sizeof(char)*(size+1));
    size++;

  }

  text = realloc(text, sizeof(char)*(size+1));
  text[size] = '\0';

  fclose(arq);

  return text;

}

token construirToken (char *conteudo, tipoToken tiptok){

  token tok;

  tok.tipo = tiptok;
  tok.conteudo = conteudo;

  return tok;

}

void temErroLexico (token tok, int line) {

  switch (tok.tipo) {

  case ERROCHAR :

    printf("Linha %d : Caracter inválido.", line);
    exit(1);

  case ERRONUM :

    printf("Linha %d : Número mal formado.", line);
    exit(1);

  default : break;
  
  }

}

// O Scanner gera os tokens
token scanner (char *texto, int *posicaotexto){
  
  token tok;
  int i = *posicaotexto;
  int j = 1;

  char *conteudoToken = (char*) malloc(sizeof(char));
  conteudoToken[0] = texto[i];

  if ( isalpha(texto[i]) ){ 
    
    // Se o primeiro caracter do conteudo e uma letra, provavelmente e WORD
    
    while( isalpha(texto[i+j]) 
	   || texto[i+j]=='_' 
	   || isdigit(texto[i+j])){

      conteudoToken = realloc(conteudoToken, (j + 1) * sizeof(char));
      conteudoToken[j] = texto[i+j];
      j++;

    }
    
    conteudoToken = realloc(conteudoToken, (j + 1) * sizeof(char));
    conteudoToken[j] = '\0';
    tok = construirToken(conteudoToken,WORD);
  }else if (texto[i]=='0'){
    
    // Se o primeiro caracter for 0, pode ser hexadecimal 0x ou decimal

    if(texto[i+j]=='x' || texto[i+j]=='X'){

      // Se o segundo digito for x ou X o número é hexadecimal

      j++;

      while(isxdigit(texto[i+j])){

        conteudoToken = realloc(conteudoToken, (j-1) * sizeof(char));
        conteudoToken[j-2] = texto[i+j];
	j++;
	
      }

      conteudoToken = realloc(conteudoToken, (j-1) * sizeof(char));
      conteudoToken[j-2] = '\0';

      if (isalpha(texto[i+j]) || texto[i+j] == '_') { // Verifica se tinha continuacao errada do hex
        tok = construirToken(conteudoToken, ERRONUM);
      } else {
	tok = construirToken(conteudoToken, NUM_HEX);
      }
      
    } else {

      // Se o segundo digito for um número, sera um decimal
      while (isdigit(texto[i+j])) {
        conteudoToken = realloc(conteudoToken, (j + 1) * sizeof(char));
        conteudoToken[j] = texto[i+j];
        j++;
      }

      conteudoToken = realloc(conteudoToken, (j + 1) * sizeof(char));
      conteudoToken[j] = '\0';

      if (isalpha(texto[i+j]) || texto[i+j] == '_') { // Verifica se tinha erro no decimal
	tok = construirToken(conteudoToken, ERRONUM);
      } else {
	tok = construirToken(conteudoToken, NUM_DEC);
      }
    }
  }

  else if (isdigit(texto[i])){

    // Se o primeiro digito for numero diferente de 0, assume-se que deva ser decimal
    while (isdigit(texto[i+j])) {
      conteudoToken = realloc(conteudoToken, (j + 1) * sizeof(char));
      conteudoToken[j] = texto[i+j];
      j++;
    }

    conteudoToken = realloc(conteudoToken, (j + 1) * sizeof(char));
    conteudoToken[j] = '\0';

    if (isalpha(texto[i+j]) || texto[i+j] == '_') { // Verifica se tinha erro no decimal
      tok = construirToken(conteudoToken, ERRONUM);
    } else {
      tok = construirToken(conteudoToken, NUM_DEC);
    }
      
  }

  else if (texto[i] == ' ' || texto[i] == '\t'){

    // Se for espaco ou \t, o token sera de espaco (junta todos espacos em um token so de espaco)
    while (texto[i+j] == ' ' || texto[i+j] == '\t') { j++; }
    tok = construirToken(" ",ESP);

  }

  else if (texto[i]==';'){

    // Se for ; sera comentario, e sera ignorado todo o comentario
    while (texto[i+j] != '\n' && texto[i+j] != '\0') { j++; }
    tok = construirToken(";",PTVIR);

  }
  else {
    switch (texto[i]) {
    case ',' :
      tok = construirToken(",",VIR);
      break;
    case ':' :
      tok = construirToken(":",DOISPTS);
      break;
    case '+' :
      tok = construirToken("+",MAIS);
      break;
    case '-' :
      tok = construirToken("-",MENOS);
      break;
    case '\n' :
      tok = construirToken("\n",FIM_LINHA);
      break;
    case '\0' :
      tok = construirToken("\0",FIM);
      break;
    // Caso nao seja nenhuma das anteriores, o caractere eh invalido
    default:
      conteudoToken = realloc(conteudoToken, 2 * sizeof(char));
      conteudoToken[1] = '\0';
      tok = construirToken(conteudoToken, ERROCHAR);
      break;
    }
  }

  *posicaotexto = i + j;

  return tok;

}

line construirLinha (int *posicaoTexto, char *texto, int numeroDaLinha) {
  
  line linha;
  int posicao = *posicaoTexto;
  token tok;
  token *toks;
  int posToken = 0;

  toks = (token*) malloc(sizeof(token));

  tok = scanner(texto, posicaoTexto);
  temErroLexico(tok, numeroDaLinha);

  while (tok.tipo!=FIM_LINHA && tok.tipo!=FIM){
    
    if (tok.tipo != ESP && tok.tipo != PTVIR) {
      
      toks = realloc(toks, (posToken + 1) * sizeof(token));
      toks[posToken] = tok;
      posToken++;

    }
    
    tok = scanner(texto, posicaoTexto);
    temErroLexico(tok, numeroDaLinha);

  }


  toks = realloc(toks, (posToken + 1) * sizeof(token));
  toks[posToken] = tok;

  linha.num = numeroDaLinha;
  linha.tokens = toks;
      
  return linha;

}

listaLinhas *limpaLista(listaLinhas *head) { // Tira linhas vazias
  
  listaLinhas *aux = head;
  listaLinhas *ant = NULL;

  while (aux->prox != NULL) {

    if (aux->linha.tokens[0].tipo == FIM_LINHA) {
    
      ant->prox = aux->prox;
      aux = ant->prox;
  
    } else {

      ant = aux;
      aux = aux->prox;
    
    }

  }

  return head;

}

listaLinhas *construirListaLinhas(char *texto){

  int posTexto = 0;
  int numeroLinha = 1;

  listaLinhas *head = (listaLinhas*) malloc(sizeof(listaLinhas));
  listaLinhas *aux  = head;

  line l = construirLinha(&posTexto,texto,numeroLinha);

  while(l.tokens[0].tipo != FIM){
  
    aux->linha = l;

    numeroLinha++;
    l = construirLinha(&posTexto,texto,numeroLinha);

    aux->prox = (listaLinhas*) malloc(sizeof(listaLinhas));
    aux = aux->prox;
  
  }

  aux->prox = NULL;
  free(texto);

  return limpaLista(head);

}

void imprimeLinhas(listaLinhas *list){

  listaLinhas *aux = list;
  int i;

  printf("\n");
  while(aux->prox!=NULL){
    i=0;
    while((aux->linha.tokens[i].tipo!=FIM)&&(aux->linha.tokens[i].tipo!=FIM_LINHA)){
      printf("%s ",aux->linha.tokens[i].conteudo);
      i++;
    }
    printf("\n");
    aux = aux->prox;
  }
}
