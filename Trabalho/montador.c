/** Montador **/

// labelExiste e uma funcao que verifica se uma label existe na tabela de simbolos

bool temText = FALSE;
bool temData = FALSE;

void sprintf_(char **buf, char *str) {
  
  int n, m;
  n = strlen(str);
  m = strlen(*buf);
  
  *buf = realloc(*buf, sizeof(char) * (m + n + 1));
  strcat(*buf,str);

}

bool labelExiste (tabelaDeSimbolos *tab, char *label, int *valor){

  tabelaDeSimbolos *aux = tab;

  while(aux != NULL){

    if(strcmp(aux->simbolo.label,label)==0){

      *valor = aux->simbolo.endereco;
      return TRUE;

    }

    aux = aux->next;

  }

  return FALSE;

}

// incrementaGP retorna o tamanho de memoria utilizado pelo comando
int incrementaGP (instrucao instr){ //incrementa global ponter
  int i=0;
  switch(instr.comando){
  case COPY:
    i = i + 3;
    break;
  case STOP:
    i = i + 1;
    break;
  case SECTION:
    break;
  case CONST:
    i = i+1;
    break;
  case SPACE:
    if(instr.numeroDeOperandos==0){ // se for so space, reserva um na memória
      i = i + 1;
    } else{
      if(!instr.primeiro.temInc){
	if (instr.primeiro.sinal==NEG){
	  printf("\nErro semantico na linha %d: Nao e aceito numero negativo como parametro do space\n", instr.numeroLinha);
	  ERRO_EXEC = TRUE;
	}
	if (instr.primeiro.tipo==NUM)
	  i = i + instr.primeiro.conteudoInt;
	else if (instr.primeiro.tipo==PALAVRA){
	  printf("\nErro semantico na linha %d: Nao e aceito string como parametro do space\n", instr.numeroLinha);
	  ERRO_EXEC = TRUE;
	}
      } else {
	if ((instr.primeiro.tipo==PALAVRA)||(instr.primeiro.tipoInc==PALAVRA)){
	  printf("\nErro semantico na linha %d: Nao e aceito string como parametro do space\n", instr.numeroLinha);
	  ERRO_EXEC = TRUE;
	} else {
	  if((instr.primeiro.sinal!=NEG)&&(instr.primeiro.sinalInc==POS)){
	    i = instr.primeiro.conteudoInt + instr.primeiro.incrementoInt;
	  }
	  else if((instr.primeiro.sinal!=NEG)&&(instr.primeiro.sinalInc==NEG))
	    i = instr.primeiro.conteudoInt - instr.primeiro.incrementoInt;
	  else if(((instr.primeiro.sinal==NEG)&&(instr.primeiro.sinalInc==POS))&&((instr.primeiro.incrementoInt)>(instr.primeiro.conteudoInt)))
	    i = instr.primeiro.incrementoInt - instr.primeiro.conteudoInt;
	  else if((instr.primeiro.sinal==NEG)&&(instr.primeiro.sinalInc==NEG)){
	    printf("\nErro semantico na linha %d: Nao e aceito numero negativo como parametro do space\n", instr.numeroLinha);
	    ERRO_EXEC = TRUE;	      
	  }
	}
      }
    }
    break;
  default:
    i = i + 2;
  }
  return i;
}

// Dado uma string, esta funcao procura a label na tabela de simbolos e retorna seu endereco
// se a label nao estiver na tabela de simbolos, o programa gera erro
int enderecoLabel(tabelaDeSimbolos *tab, char *label, int numeroLinha){
  int valor = 0;
  tabelaDeSimbolos *aux = tab;

  while(aux != NULL){
    if(strcmp(aux->simbolo.label,label)==0){
      valor = aux->simbolo.endereco;
      return valor;
    }
    aux = aux->next;
  }

  printf("\nERRO semantico na linha %d: a label %s nao foi declarada\n",numeroLinha, label);
  ERRO_EXEC = TRUE;
  return 0;
}

// esta funcao detecta se o endereco e o endereco de uma constante ou se esta fora da memoria alocada no programa

void verificaEndereco(int endereco, tabelaDeSimbolos *tab, int ultimoEndereco, int numeroLinha){
  if(endereco>ultimoEndereco){
    printf("\n ERRO semantico na linha %d: tentativa de armazenar dado em espaco nao alocado\n", numeroLinha); // Erro se a label ja existir
    ERRO_EXEC = TRUE;
  }
  tabelaDeSimbolos *aux=tab;
  while(aux!=NULL){
    if(aux->simbolo.constante==TRUE){
      if((endereco>=aux->simbolo.endereco)&&(endereco<(aux->simbolo.endereco+aux->simbolo.tamanho))){
	printf("\n ERRO semantico na linha %d: tentativa de armazenar dado em espaco alocado com uma constante\n", numeroLinha); // Erro se a label ja existir
	ERRO_EXEC = TRUE;
      }
    }
    aux=aux->next;
  }
}

// Gera a string traduzida e verifica se no processo de traducao possui erros semanticos
void geraStringFinal (char **buf,instrucao instr, int numeroLinha, bool *comecoData, tabelaDeSimbolos *tab, int ultimoEndereco, bool *temStop){
  
  int comando, op1, op2, inc;
  int i=0; // contador para o numero de spaces
  
  char str [50];
  
  // verifica quantos operados tem, se tem incremento e se esta em hexadecimal ou decimal e gera os inteiros op1 e op2
  if((instr.numeroDeOperandos==1)||((instr.numeroDeOperandos==2))){
    if(instr.primeiro.tipo==NUM)
      op1 = instr.primeiro.conteudoInt;
    else if(instr.primeiro.tipo==PALAVRA){
      if(instr.comando!=SECTION)
	op1 = enderecoLabel(tab,instr.primeiro.conteudoChar,numeroLinha);
    }
    if(instr.primeiro.sinal==NEG)
      op1 = -op1;
    if(instr.primeiro.temInc){
      if(instr.primeiro.tipoInc==NUM)
	inc = instr.primeiro.incrementoInt;
      else
	inc = enderecoLabel(tab,instr.primeiro.incrementoChar,numeroLinha);
      if(instr.primeiro.sinalInc==POS)
	op1 = op1 + inc;
      else
	op1 = op1 - inc;
    }
  }
  if(instr.numeroDeOperandos==2){
    if(instr.segundo.tipo==NUM)
      op2 = instr.segundo.conteudoInt;
    else if(instr.segundo.tipo==PALAVRA){
      op2 = enderecoLabel(tab,instr.segundo.conteudoChar,numeroLinha);
    }
    if(instr.segundo.sinal==NEG)
      op2 = -op2;
    if(instr.segundo.temInc){
      if(instr.segundo.tipoInc==NUM)
	inc = instr.segundo.incrementoInt;
      else if(instr.segundo.tipoInc==PALAVRA)
	inc = enderecoLabel(tab,instr.segundo.incrementoChar,numeroLinha);
      if(instr.segundo.sinalInc==POS)
	op2 = op2 + inc;
      else
	op2 = op2 - inc;
    }
  }

  // verifica se comecou o comeco data e se existe comando na regiao de dados
  if(*comecoData==TRUE){
    if((instr.comando==ADD)||(instr.comando==SUB)||(instr.comando==MULT)||(instr.comando==DIV)||(instr.comando==JMP)||(instr.comando==JMPP)||(instr.comando==JMPN)||(instr.comando==JMPZ)||(instr.comando==COPY)||(instr.comando==LOAD)||(instr.comando==STORE)||(instr.comando==INPUT)||(instr.comando==OUTPUT)||(instr.comando==STOP)){
      printf("\nERRO semantico na linha %d: Existe um comando na secao DATA\n",numeroLinha);
      ERRO_EXEC = TRUE;
    }
  } else{
    if((instr.comando==SPACE)||(instr.comando==CONST)){
      printf("\nERRO semantico na linha %d: Existe alocacao de espaco na secao TEXT\n",numeroLinha);
      ERRO_EXEC = TRUE;
    }
  }

  // coloca na string buf a traducao da instrucao atual, se estiver correto
  switch(instr.comando){
  case ADD:
    sprintf(str,"1 %d ",op1);
    sprintf_(&*buf, str);
    break;
  case SUB:
    sprintf(str,"2 %d ",op1);
    sprintf_(&*buf, str);
    break;
  case MULT:
    sprintf(str,"3 %d ",op1);
    sprintf_(&*buf, str);
    break;
  case DIV:
    if(op1==0){
      printf("\nERRO semantico na linha %d: Divisao por zero\n",numeroLinha);
      ERRO_EXEC = TRUE;
    }
    sprintf(str,"4 %d ",op1);
    sprintf_(&*buf, str);
    break;
  case JMP:
    sprintf(str,"5 %d ",op1);
    sprintf_(&*buf, str);
    break;
  case JMPN:
    sprintf(str,"6 %d ",op1);
    sprintf_(&*buf, str);
    break;
  case JMPP:
    sprintf(str,"7 %d ",op1);
    sprintf_(&*buf, str);
    break;
  case JMPZ:
    sprintf(str,"8 %d ",op1);
    sprintf_(&*buf, str);
    break;
  case COPY:
    verificaEndereco(op1,tab,ultimoEndereco,numeroLinha);
    if(op2>ultimoEndereco){
      printf("\nERRO semantico na linha %d: tentativa de acessar dado de espaco nao alocado\n", numeroLinha); // Erro se a label ja existir
      ERRO_EXEC = TRUE;
    }
    sprintf(str,"9 %d %d ",op1,op2);
    sprintf_(&*buf, str);
    break;
  case LOAD:  
    if(op1>ultimoEndereco){
      printf("\n ERRO semantico na linha %d: tentativa de armazenar dado em espaco nao alocado\n", numeroLinha); // Erro se a label ja existir
      ERRO_EXEC = TRUE;
    }

    sprintf(str,"10 %d ",op1);
    sprintf_(&*buf, str);
    break;
  case STORE:
    verificaEndereco(op1,tab,ultimoEndereco,numeroLinha);
    sprintf(str,"11 %d ",op1);
    sprintf_(&*buf, str);
    break;
  case INPUT:
    verificaEndereco(op1,tab,ultimoEndereco,numeroLinha);
    sprintf(str,"12 %d ",op1);
    sprintf_(&*buf, str);
    break;
  case OUTPUT:
    if(op1>ultimoEndereco){
      printf("\n ERRO semantico na linha %d: tentativa de acessar dado de espaco nao alocado\n", numeroLinha); // Erro se a label ja existir
      ERRO_EXEC = TRUE;
    }
    sprintf(str,"13 %d ",op1);
    sprintf_(&*buf, str);
    break;
  case STOP:
    sprintf(str,"14 ");
    sprintf_(&*buf, str);
    *temStop = TRUE;
    break;
  case SECTION:
    if(strcmp(instr.primeiro.conteudoChar,"DATA")==0){
      if(*comecoData == FALSE){
	*comecoData = TRUE;
	temData = TRUE;
      }
      else{
	printf("\n ERRO semantico na linha %d: duas secoes de DATA\n", numeroLinha);
	ERRO_EXEC = TRUE;
      }
    } else if (strcmp(instr.primeiro.conteudoChar,"TEXT")==0){
      if(temText == FALSE){ 
	temText = TRUE;
      }
      else{
	printf("\n ERRO semantico na linha %d: duas secoes de TEXT\n", numeroLinha);
	ERRO_EXEC = TRUE;
      }
    } else{
      printf("\n ERRO semantico na linha %d: SECTION invalido\n", numeroLinha);
      ERRO_EXEC = TRUE;
    }
  
  break;
  case SPACE:
    if(instr.numeroDeOperandos==1){
      while(i<op1){
	sprintf(str,"0 ");
	sprintf_(&*buf, str);
	i++;
      }
    } else{ // se o space nao tiver operandos, significa que sera alocado apenas um espaco
      sprintf(str,"0 ");
      sprintf_(&*buf, str);
    }
    break;
  case CONST:
    sprintf(str,"%d ",op1);
    sprintf_(&*buf, str);
    break;
  case EQU:
    printf("\nERRO semantico na linha %d: EQU mal posicionado (Deve estar antes da secao TEXT)\n",numeroLinha);
    ERRO_EXEC = TRUE;
    break;
  case IF:
    printf("\nERRO semantico na linha %d: IF mal posicionado\n",numeroLinha);
    ERRO_EXEC = TRUE;
    break;
  case MACRO:
    printf("\nERRO semantico na linha %d: MACRO mal posicionado\n",numeroLinha);
    ERRO_EXEC = TRUE;
    break;
  case ENDMACRO:
    printf("\nERRO semantico na linha %d: ENDMACRO mal posicionado ou formulado\n",numeroLinha);
    ERRO_EXEC = TRUE;
    break;
  }
}

// A primeira passagem gera apenas a tabela de simbolos
tabelaDeSimbolos *primeiraPassagem (listaDeInstrucoes *linst, int *ultimoEndereco){ // Apenas montara a tabela de simbolos
  tabelaDeSimbolos *tab;
  tabelaDeSimbolos *t1, *t2;
  listaDeInstrucoes *aux = linst;
  simbolo sim;
  int i=0;
  int tamanho;
  int end;
  tab = NULL;

  while(aux->next != NULL){
    tamanho = incrementaGP(aux->instr);
    if(aux->instr.temLabel){
      if(labelExiste(tab,aux->instr.label,&end)){ // Verifica se a label ja foi declarada
	printf("\nERRO semantico: A label %s foi declarada mais de uma vez\n", aux->instr.label); // Erro se a label ja existir
	ERRO_EXEC = TRUE;
      }
      sim.label = aux->instr.label;
      sim.endereco = i;
      sim.tamanho = tamanho;
      if(aux->instr.comando==CONST) sim.constante = TRUE;
      else sim.constante = FALSE;
      t1 = (tabelaDeSimbolos*)malloc(sizeof(tabelaDeSimbolos));
      t1->simbolo = sim;
      if (tab == NULL){
	tab = t1;
      } else{
	t2->next = t1;
      }
      t2 = t1;
      t1->next=NULL;
    }
    i = i + tamanho;
    aux = aux->next;
  }
  tamanho = incrementaGP(aux->instr);
  if(aux->instr.temLabel){
    if(labelExiste(tab,aux->instr.label,&end)){ // Verifica se a label ja foi declarada
      printf("\nERRO semantico: A label %s foi declarada mais de uma vez\n", aux->instr.label); // Erro se a label ja existir
      exit(1);
    }
    sim.label = aux->instr.label;
    sim.endereco = i;
    sim.tamanho = tamanho;
    if(aux->instr.comando==CONST) sim.constante = TRUE;
    else sim.constante = FALSE;
    t1 = (tabelaDeSimbolos*)malloc(sizeof(tabelaDeSimbolos));
    t1->simbolo = sim;
    if (tab == NULL){
      tab = t1;
    } else{
      t2->next = t1;
    }
    t2 = t1;
    t1->next=NULL;
  }
  int increm = incrementaGP(aux->instr);  
  *ultimoEndereco = i + increm - 1;
  t1 = NULL;
  t2->next = t1;
  return tab;
}

// Esta e a segunda passagem, que realiza a traducao a partir das instrucoes e tabela de simbolos
// Ele tambem escreve no arquivo objeto
char *geraCodigoFinal (listaDeInstrucoes *linst,tabelaDeSimbolos *tab, char *nomeArquivoObjeto, int ultimoEndereco){
  listaDeInstrucoes *auxL = linst;
  tabelaDeSimbolos *auxT = tab;
  int instrucao,primeiroOp, segundoOp;
  bool comecoData = FALSE;
  bool temStop = FALSE;
  
  char *buf;
  
  buf = (char*)calloc(0,sizeof(char));
  
  
  // A primeira coisa que o programa tem que pegar eh o section text
  
  if (auxL->instr.comando!=SECTION){
    printf("\nErro semantico: O programa deve conter SECTION como primeira secao");
    ERRO_EXEC = TRUE;
  }

  // vai incrementando a string ate chegar na penultima linha

  while(auxL->next!=NULL){
    geraStringFinal(&buf,auxL->instr,auxL->instr.numeroLinha,&comecoData,tab,ultimoEndereco,&temStop);
    auxL = auxL->next;
  }

  //geraStringFinal(&buf,auxL->instr,auxL->instr.numeroLinha,&comecoData,tab,ultimoEndereco); //printa a ultima linha na string
  

  // se nao apresentou secao data, deve ser dado um erro
  if(comecoData==FALSE){
    printf("\nErro semantico: Programa nao apresentou SECTION DATA\n");
    ERRO_EXEC = TRUE;
  }
  if(temText==FALSE){
    printf("\nErro semantico: Programa nao apresentou SECTION TEXT\n");
    ERRO_EXEC = TRUE;
  }
  if(temStop==FALSE){
    printf("\nErro semantico: Programa nao apresentou STOP\n");
    ERRO_EXEC = TRUE;
  }
  
  // caso tudo ocorra corretamente, a funcao retorna a string contendo a traducao do codigo
  return buf;
}

listaDeInstrucoes *colocaTextInicio(listaDeInstrucoes *linst){
  listaDeInstrucoes *aux = linst;
  listaDeInstrucoes *pre;
  listaDeInstrucoes *inicio;
  listaDeInstrucoes *inicioData;
  if((aux->instr.comando == SECTION)&&(strcmp(aux->instr.primeiro.conteudoChar,"DATA")==0)){
    inicioData = aux;
    while((strcmp(aux->instr.primeiro.conteudoChar,"TEXT")!=0)&&(aux->next!=NULL)){
      pre=aux;
      aux = aux->next;
    }
    pre->next = NULL;
    inicio = aux;
    while((aux->next)->next!=NULL){
      aux = aux->next;
    }
    aux->next = inicioData;
  }
  return inicio;
}

void imprimeTabelaSim(tabelaDeSimbolos *simtab){
  
  tabelaDeSimbolos *auxsimtab = simtab;
  simbolo aux;
  printf("\n\n----------\n\n");

  while(auxsimtab->next!=NULL){
    aux = auxsimtab->simbolo;
    printf("%s\t%d\t%d",aux.label,aux.endereco,aux.tamanho);
    if(aux.constante) printf("\tconst\n");
    else printf("\tnao const\n");
    auxsimtab=auxsimtab->next;
  }
  aux = auxsimtab->simbolo;
  printf("%s\t%d\t%d",aux.label,aux.endereco,aux.tamanho);
  if(aux.constante) printf("\tconst\n");
  else printf("\tnao const\n");
    
  printf("\n----------\n\n");

}

