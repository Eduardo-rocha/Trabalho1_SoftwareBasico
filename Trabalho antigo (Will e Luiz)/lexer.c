
/** Lexer **/

char *textoDoArquivo(char* arquivo_entrada){

  FILE *arq  = fopen(arquivo_entrada,"r");
  char *text = (char*) malloc(sizeof(char));
  char c;
  int size = 1;
  
  while( EOF != (c = fgetc(arq))) {

    text = realloc(text, sizeof(char)*size);
    text[size-1] = c;
    size++;

  }

  text = realloc(text, sizeof(char)*size);
  text[size-1] = '\0';

  fclose(arq);
  return text;

}

token construirToken(char *conteudo, tipoDeToken tt) {

  token t;

  t.tipo = tt;
  t.conteudo = conteudo;

  return t;

}

tokenOuErro tokenOfChar(char *var) {

  tokenOuErro res;
  token t; int i;
  bool found = FALSE;

  for (i = 0; i < LEN; i++) {
    if (chars[i] == var[0]) { found = TRUE; break; }
  }

  if (found) {

    tipoDeToken tipo = tokenOfCharTable[i];
    
    res.token.tipo     = tipo;
    res.token.conteudo = var;
    res.erro           = SEM_ERROS;

  } else {
    
    res.erro = CARACTER_INVALIDO;

  }

  return res;

}

bool isL(char c) { return isalpha(c); }

bool isD(char c) { return isdigit(c); }

bool isH(char c) {

  int i;

  for (i = 0; i < HLEN; i++)
    if (toupper(c) == H[i]) return TRUE;

  return FALSE;

}

tipoDeTokenEsperado estadoEsperado(char c) {

  if (c == '0')
    return EXP_NUM;
  
  if (isD(c))
    return EXP_DEC_NUM;
  
  if (c == ' ' || c == '\t')
    return EXP_SPACE;
  
  if (c == ';') 
    return EXP_COMEN;
  
  if (isL(c) || c == '_')
    return EXP_WORD;
  
  return NOT_EXP;

}

tokenOuErro scanner(int *count, char *text) {

  int i, j;
  tipoDeTokenEsperado exp;
  char *var = (char*) malloc(sizeof(char));
  tokenOuErro res;
  token tok;
  erroLexico err = SEM_ERROS;

  i      = *count;
  j      = 1;
  exp    = estadoEsperado(text[i]);
  var[0] = text[i];

  switch (exp) {

  case EXP_WORD:
        
    while (isL(text[i+j]) 
           || text[i+j] == '_' 
           || isD(text[i+j])) {
      var = realloc(var, (j + 1) * sizeof(char));
      var[j] = text[i+j];
      j++;
    }

    var = realloc(var, (j + 1) * sizeof(char));
    var[j] = '\0';
    
    tok = construirToken(var,WORD);

    break;

  case EXP_DEC_NUM :

    while (isD(text[i+j])) {
      var = realloc(var, (j + 1) * sizeof(char));
      var[j] = text[i+j];
      j++;
    }

    if (isL(text[i+j]) || text[i+j] == '_') {
      err = NUMERO_MAL_FORMADO;
      break;
    }

    var = realloc(var, (j + 1) * sizeof(char));
    var[j] = '\0';

    tok = construirToken(var,DEC_NUM);

    break;

  case EXP_NUM :

    if (text[i+j] == 'x' || text[i+j] == 'X') {

      j++;

      while (isD(text[i+j]) || isH(text[i+j])) {
        var = realloc(var, (j - 1) * sizeof(char));
        var[j-2] = text[i+j];
        j++;
      }

      if (isL(text[i+j]) || text[i+j] == '_') {
        err = NUMERO_MAL_FORMADO;
        break;
      }

      var = realloc(var, (j - 1) * sizeof(char));
      var[j-2] = '\0';

      tok = construirToken(var,HEX_NUM);

    } else {

      while (isD(text[i+j])) {
        var = realloc(var, (j + 1) * sizeof(char));
        var[j] = text[i+j];
        j++;
      }

      if (isL(text[i+j]) || text[i+j] == '_') {
        err = NUMERO_MAL_FORMADO;
        break;
      }

      var = realloc(var, (j + 1) * sizeof(char));
      var[j] = '\0';

      tok = construirToken(var,DEC_NUM);

    }

    break;

  case EXP_SPACE :

    while (text[i+j] == ' ' || text[i+j] == '\t') { j++; }
    tok = construirToken(" ",ESPACO);

    break;

  case EXP_COMEN :
    
    // ignorando comentários
    while (text[i+j] != '\n' && text[i+j] != '\0') { j++; }
    tok = construirToken(";",PONTO_VIRGULA);

    break;

  case NOT_EXP :

    var = realloc(var, 2 * sizeof(char));
    var[1] = '\0';

    *count = i + j;

    return tokenOfChar(var);

  }

  *count = i + j;
  res.token = tok;
  res.erro  = err;
  
  return res;

}

void erroLexicoSe(erroLexico err, int line) {

  switch (err) {

  case CARACTER_INVALIDO :

    printf("Linha %d : Caracter inválido.", line);
    exit(1);

  case NUMERO_MAL_FORMADO :

    printf("Linha %d : Número mal formado.", line);
    exit(1);

  case SEM_ERROS : break;
  
  }

}

line getLine(int *count, char *texto, int linenumber) {
  
  line linha;
  linha.fim = FALSE;
  tokenOuErro res;
  
  int countAnt = *count;
  int tn = 0;
  token tokaux;
  token *toks; 

  toks = (token*) malloc(sizeof(token));
  
  if ( toks == NULL ) {
    printf("Erro de alocação de memória!\n");
    exit(1);
  }

  res = scanner(count, texto);

  erroLexicoSe(res.erro, linenumber);

  if (res.token.tipo == FIM) {
    linha.num = linenumber;
    linha.fim = TRUE;
    return linha;
  }

  while (res.token.tipo != FIM && res.token.tipo != FIM_DE_LINHA) {

    if (res.token.tipo != ESPACO &&
        res.token.tipo != PONTO_VIRGULA) {
      
      toks = realloc(toks, (tn + 1) * sizeof(token));
      toks[tn] = res.token;
      tn++;

    }

    countAnt = *count;
    res = scanner(count, texto);

    erroLexicoSe(res.erro, linenumber);

  }

  tokaux.conteudo = "\n";
  tokaux.tipo    = FIM_DE_LINHA;
  toks = realloc(toks, (tn + 1) * sizeof(token));
  toks[tn] = tokaux;
  
  linha.num = linenumber;
  linha.tokens = toks;

  if (res.token.tipo == FIM) {
    *count = countAnt;
  }

  return linha;

}

listaDeLinhas *filterEmptyLines(listaDeLinhas *head) {
  
  listaDeLinhas *aux = head;
  listaDeLinhas *ant = NULL;

  while (aux->next != NULL) {

    if (aux->linha.tokens[0].tipo == FIM_DE_LINHA) {
    
      ant->next = aux->next;
      aux = ant->next;
  
    } else {

      ant = aux;
      aux = aux->next;
    
    }

  }

  return head;

}

listaDeLinhas *getLines(char* texto) {
  
  int i  = 0;
  int ln = 1;

  listaDeLinhas *head =
    (listaDeLinhas*) malloc(sizeof(listaDeLinhas));
  listaDeLinhas *aux  = head;

  line l = getLine(&i,texto,ln);

  while (!l.fim) {

    aux->linha = l;

    ln++;
    l = getLine(&i,texto,ln);

    aux->next = (listaDeLinhas*) malloc(sizeof(listaDeLinhas));
    aux = aux->next;

  }

  aux->next = NULL;
  free(texto);

  return filterEmptyLines(head);

}
