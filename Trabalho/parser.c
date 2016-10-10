//Parser

void temErroSintatico(erroSintatico erro, int ln) {

  switch (erro) {

  case COMANDO_NAO_EXISTE :
    
    printf("Linha %d : Comando não existe.", ln);
    exit(1);
    
  case MA_FORMACAO_DE_LABEL :
    
    printf("Linha %d : Label má formado.", ln);
    exit(1);
    
  case NUMERO_DE_OPERADORES_ERRADO :
    
    printf("Linha %d : Número de operadores errado.", ln);
    exit(1);

  case MA_FORMACAO_DOS_OPERADORES :
    
    printf("Linha %d : Operador mal formado.", ln);
    exit(1);

  case COMANDO_MAL_FORMADO :
    
    printf("Linha %d : Comando mal formado.", ln);
    exit(1);

  case SEM_ERROS_SINTATICOS : break;

  }

}


int numeroDoComando(comando comand) {
  int i;
  for (i = 0; i < 21; i++) {
    if (comand == comandos[i]) 
      return i;
  } 
  return -1; // comando não existe
}

bool comandoExiste(char *comand, int *n) { // n eh o numero do comando
  int i;
  for (i = 0; i < 21; i++) {
    if (strcmp(comand,comandString[i]) == 0) {
      *n = i;
      return TRUE;
    }
  }
  *n = -1;
  return FALSE;
}

operando pegarOperador(token *toks, int *c, int ln) {

  operando op;
  erroSintatico erro=SEM_ERROS_SINTATICOS;
  tipoToken t1, t2, t3;
  int i = *c;

  op.temOp = TRUE;
  op.temInc = FALSE;
  op.sinal = POS;

  t1 = toks[i].tipo;

  if (t1 == WORD || t1 == NUM_DEC || t1 == NUM_HEX) {
    
    op.tipo =
      t1 == WORD ? PALAVRA : NUMERO;

    op.conteudoChar = toks[i].conteudo;
    op.conteudoInt =
      t1 == WORD ? 0 : (t1== DEC_NUM ? (int)strtol(toks[i].conteudo,NULL,10) : (int)strtol(toks[i].conteudo,NULL,16));

    t2 = toks[i+1].tipo;

    if (t2 == FIM_LINHA || t2 ==  FIM)
      
      *c = i + 1;

    else if (t2 == VIR)

      *c = i + 2;

    else if (t2 != MAIS && t2 != MENOS)
      
      erro = MA_FORMACAO_DOS_OPERADORES;

    else {

      t3 = toks[i+2].tipo;
      
      if (t3 == NUM_DEC || t3 == NUM_HEX || t3 == WORD) {
      
        *c = i + 3;
        op.temIncremento = TRUE;
        op.sinalDoIncremento = t2 == MAIS ? POS : NEG;
        op.tipoDeIncremento = t3 == WORD ? PALAVRA : NUM;
        op.incrementoChar = toks[i+2].conteudo;
	op.incrementoInt =
	  t3 == WORD ? 0 : (t3== DEC_NUM ? (int)strtol(toks[i+2].conteudo,NULL,10) : (int)strtol(toks[i+2].conteudo,NULL,16));

      } else erro = MA_FORMACAO_DOS_OPERADORES;

    }

  } else if (t1 == MAIS || t1 == MENOS) {
    
    op.sinal = t1 == MAIS ? POS : NEG;
    i++;
    t1 = toks[i].tipo;

    if (t1 == WORD || t1 == NUM_DEC || t1 == NUM_HEX) {
      
      op.tipo = t1 == WORD ? PALAVRA : NUM;
      
      op.conteudoChar = toks[i].conteudo;
      op.conteudoInt =
	t1 == WORD ? 0 : (t1== DEC_NUM ? (int)strtol(toks[i].conteudo,NULL,10) : (int)strtol(toks[i].conteudo,NULL,16));
      t2 = toks[i+1].tipo;

      if (t2 == FIM_LINHA || t2 ==  FIM)
      
        *c = i + 1;

      else if (t2 == VIRGULA)
      
        *c = i + 2;
    
      else if (t2 != MAIS && t2 != MENOS)
      
        erro = MA_FORMACAO_DOS_OPERADORES;
    
      else {
      
        t3 = toks[i+2].tipo;
      
        if (t3 == NUM_DEC || t3 == NUM_HEX) {
        
          *c = i + 3;
          op.temIncremento = TRUE;
          op.sinalDoIncremento = t2 == MAIS ? POS : NEG;
          op.tipoDeIncremento =
            t3 == WORD ? PALAVRA : NUM;
          op.incrementoChar = toks[i+2].conteudo;
	  op.incrementoInt =
	    t3 == WORD ? 0 : (t3== DEC_NUM ? (int)strtol(toks[i+2].conteudo,NULL,10) : (int)strtol(toks[i+2].conteudo,NULL,16));
        
        } else erro = MA_FORMACAO_DOS_OPERADORES;

      }

    } else MA_FORMACAO_DOS_OPERADORES;
    
  } else {

    op.operadorPresente = FALSE;

  }

  temErroSintatico(erro,ln);
  return op;

}

char *pegarLabel(line l, bool *temLabel) {

  int i = 0;
  token *toks;
  bool tem2Pontos = FALSE;
  erroSintatico erro = SEM_ERROS_SINTATICOS;
  char *label;

  toks = l.tokens;
  
  // ver se a linha tem dois pontos (:)
  while (toks[i].tipo != FIM_DE_LINHA) {
    if (toks[i].tipo == DOIS_PONTOS) {
      tem2Pontos = TRUE;
      break;
    }
    i++;
  }
  
  if (tem2Pontos) {
    // se tiver, os : devem estar na posição 1
    if (i == 1) {

      *temLabel = TRUE;
      label = toks[0].conteudo;

    } else {

      *temLabel = FALSE;
      erro = MA_FORMACAO_DE_LABEL;

    }

  } else *temLabel = FALSE;

  erroSintaticoSe(erro,l.num);

  return label;

}

comando pegarInstrucao (line l, int start) {

  token *toks;
  token instr;
  erroSintatico erro = SEM_ERROS_SINTATICOS;
  bool existe;
  int numero;

  toks  = l.tokens;
  instr = toks[start];

  if (instr.tipo != WORD) erro = COMANDO_MAL_FORMADO;

  existe = comandoExiste(instr.conteudo,&numero);

  if (!existe) erro = COMANDO_NAO_EXISTE;

  erroSintaticoSe(erro,l.num);

  return numero;
}

instrucao pegarOperandos (line l, int numOperadores, int start) {

  instrucao ins;
  erroSintatico erro = SEM_ERROS_SINTATICOS;
  token *toks;
  operando op;
  int j = start;

  ins.numeroLinha = l.num;
  toks = l.tokens;

  switch(numOperadores) {

  case 0 :
    
    if (toks[j].tipo == FIM_LINHA)
      ins.numeroDeOperandos = 0;
    else
      erro = NUMERO_DE_OPERADORES_ERRADO;
    
    break;

  case 1 :

    op = pegarOperador(toks,&j,l.num);

    if (!op.temOp || toks[j].tipo != FIM_DE_LINHA)
      erro = NUMERO_DE_OPERADORES_ERRADO;

    ins.primeiro = op;
    ins.numeroDeOperandos = 1;

    break;
    
  case 2 :
    
    op = pegarOperador(toks,&j,l.num);

    if (!op.operadorPresente)
      erro = NUMERO_DE_OPERADORES_ERRADO;

    res.primeiro = op;

    op = pegarOperador(toks,&j,l.num);
    
    if (!op.operadorPresente || toks[j].tipo != FIM_DE_LINHA)
      erro = NUMERO_DE_OPERADORES_ERRADO;
    
    res.segundo = op;
    res.numeroDeOperandos = 2;

    break;

  case -1 : // caso SPACE (argumento opcional)
    
    op = pegarOperador(toks,&j,l.num);
    
    if (!ins.temOp)

      res.numeroDeOperandos = 0;

    else {

      if (toks[j].tipo != FIM_LINHA)
        erro = NUMERO_DE_OPERADORES_ERRADO;

      ins.numeroDeOperandos = 1;
      ins.primeiro = op;

    }

    break;

  default :

    printf("ERRO.");
    exit(1);

  }

  erroSintaticoSe(erro,l.num);

  return ins;

}

instrucao linhaParaInstrucao(line l) {

  instrucao ins;
  instrucao ops;
  token *toks;
  bool temLabel;
  char *label;
  int instrStart;
  int comando;

  ins.numeroDaLinha = l.num;
  toks = l.tokens;

  label = getLabel(l, &temLabel);

  instrStart = temLabel ? 2 : 0; // se tem label pula a label e os 2 pontos

  comando = getInstrucao(l, instrStart);

  ops = 
    getOperandos(l, numeroDeOperadores[com],instrStart + 1);
  
  ins.temLabel = temLabel;
  ins.label    = label;
  ins.comando  = comando;
  ins.numeroDeOperandos = ops.numeroDeOperandos;
  ins.primeiro = ops.primeiro;
  ins.segundo = ops.segundo;

  return ins;

} 

listaDeInstrucoes *linhasParaInstrucoes(listaDeLinhas *head) {
  
  
  listaDeLinhas *aux;
  line l;
  instrucao instr;

  listaDeInstrucoes *res_head
    = (listaDeInstrucoes*) malloc(sizeof(listaDeInstrucoes));;
  listaDeInstrucoes *res = res_head;

  aux = head;

  while (aux->next != NULL) {

    l = aux->linha;

    instr = linhaParaInstrucao(l);
    
    res->instr = instr;
    res->next  =
      (listaDeInstrucoes*) malloc(sizeof(listaDeInstrucoes));

    res = res->next;
    aux = aux->next;

  }
  
  return res_head;

}
