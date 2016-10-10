#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "tabelas.h"
#include "lexer.c"

char *toUpperCase(char *in) {
  int i = 0;
  while (in[i] != '\0') {
    in[i] = toupper(in[i]);
    i++;
  }
  return in;
}

int numeroDoComando(comando comm) {
  int i;
  for (i = 0; i < 21; i++) {
    if (comm == comandos[i]) break;
  }
  if (i == 21) return -1; // comando não existe
  else return i;
}

// tambem retorna o número do comando (se existir)
bool comandoExiste(char *comm, int *c) {
  int i;
  char* comm_ = toUpperCase(comm);
  for (i = 0; i < COMANDOS; i++) {
    if (strcmp(comm_,comandos_str[i]) == 0) {
      *c = i;
      return TRUE;
    }
  }
  *c = -1;
  return FALSE;
}

operando pegarOperador(token *toks, int *c, int ln) {

  operando res;
  erroSintatico erro;
  tipoDeToken t1, t2, t3;
  int i = *c;

  erro = SEM_ERROS_SINTATICOS;
  res.operadorPresente = TRUE;
  res.temIncremento = FALSE;
  res.sinal = MAIS;

  t1 = toks[i].tipo;

  if (t1 == WORD || t1 == DEC_NUM || t1 == HEX_NUM) {
    
    res.tipo =
      t1 == WORD ? WORD : (t1 == DEC_NUM ? DEC : HEX);

    res.conteudo = toks[i].conteudo;

    t2 = toks[i+1].tipo;

    if (t2 == FIM_DE_LINHA || t2 ==  FIM)
      
      *c = i + 1;

    else if (t2 == VIRGULA)

      *c = i + 2;

    else if (t2 != MAIS && t2 != MENOS)
      
      erro = MA_FORMACAO_DOS_OPERADORES;

    else {

      t3 = toks[i+2].tipo;
      
      if (t3 == DEC_NUM || t3 == HEX_NUM || t3 == WORD) {
      
        *c = i + 3;
        res.temIncremento = TRUE;
        res.sinalDoIncremento = t2 == MAIS ? POS : NEG;
        res.tipoDeIncremento =
          t3 == WORD ? PALAVRA : (t3 == HEX_NUM ? HEX : DEC);
        res.incremento = toks[i+2].conteudo;

      } else erro = MA_FORMACAO_DOS_OPERADORES;

    }

  } else if (t1 == MAIS || t1 == MENOS) {
    
    i++;
    t1 = toks[i].tipo;
    res.sinal = t1 == MAIS ? POS : NEG;

    if (t1 == WORD || t1 == DEC_NUM || t1 == HEX_NUM) {
      
      res.tipo =
        t1 == WORD ? WORD : (t1 == DEC_NUM ? DEC : HEX);
      
      res.conteudo = toks[i].conteudo;
    
      t2 = toks[i+1].tipo;

      if (t2 == FIM_DE_LINHA || t2 ==  FIM)
      
        *c = i + 1;

      else if (t2 == VIRGULA)
      
        *c = i + 2;
    
      else if (t2 != MAIS && t2 != MENOS)
      
        erro = MA_FORMACAO_DOS_OPERADORES;
    
      else {
      
        t3 = toks[i+2].tipo;
      
        if (t3 == DEC_NUM || t3 == HEX_NUM) {
        
          *c = i + 3;
          res.temIncremento = TRUE;
          res.sinalDoIncremento = t2 == MAIS ? POS : NEG;
          res.tipoDeIncremento =
            t3 == WORD ? PALAVRA : (t3 == HEX_NUM ? HEX : DEC);
          res.incremento = toks[i+2].conteudo;
        
        } else erro = MA_FORMACAO_DOS_OPERADORES;

      }

    } else MA_FORMACAO_DOS_OPERADORES;

    
  } else {

    res.operadorPresente = FALSE;

  }

  erroSintaticoSe(erro,ln);
  
  return res;

}

void erroSintaticoSe(erroSintatico err, int ln) {

  switch (err) {

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

char *getLabel(line l, bool *temLabel) {

  int i;
  token *toks;
  bool achou2Pontos;
  erroSintatico erro;
  char *label;

  erro = SEM_ERROS_SINTATICOS;
  toks = l.tokens;
  achou2Pontos = FALSE;
  i = 0;
  
  // ver se a linha tem dois pontos (:)
  while (toks[i].tipo != FIM_DE_LINHA) {
    if (toks[i].tipo == DOIS_PONTOS) {
      achou2Pontos = TRUE;
      break;
    }
    i++;
  }
  
  if (achou2Pontos) {
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

comando getInstrucao (line l, int start) {

  token *toks;
  token instr;
  erroSintatico erro;
  bool existe;
  int numero;

  erro  = SEM_ERROS_SINTATICOS;
  toks  = l.tokens;
  instr = toks[start];

  // a instrução tem que ser uma palavra
  if (instr.tipo != WORD)
    erro = COMANDO_MAL_FORMADO;
  
  existe = comandoExiste(instr.conteudo,&numero);

  if (!existe)
    erro = COMANDO_NAO_EXISTE;

  erroSintaticoSe(erro,l.num);

  return numero;

}

instrucao getOperandos (line l, int numOps, int start) {

  instrucao res;
  erroSintatico erro;
  token *toks;
  operando op;
  int j;

  erro = SEM_ERROS_SINTATICOS;
  j = start;
  res.numeroDaLinha = l.num;
  toks = l.tokens;

  switch(numOps) {

  case 0 :
    
    if (toks[j].tipo == FIM_DE_LINHA)
      res.numeroDeOperandos = 0;
    else
      erro = NUMERO_DE_OPERADORES_ERRADO;
    
    break;

  case 1 :

    op = pegarOperador(toks,&j,l.num);

    if (!op.operadorPresente || toks[j].tipo != FIM_DE_LINHA)
      erro = NUMERO_DE_OPERADORES_ERRADO;

    res.primeiro = op;
    res.numeroDeOperandos = 1;

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
    
    if (!op.operadorPresente)

      res.numeroDeOperandos = 0;

    else {

      if (toks[j].tipo != FIM_DE_LINHA)
        erro = NUMERO_DE_OPERADORES_ERRADO;

      res.numeroDeOperandos = 1;
      res.primeiro = op;

    }

    break;

  default :

    printf("Super erro.");
    exit(1);

  }

  erroSintaticoSe(erro,l.num);

  return res;

}

instrucao linhaParaInstrucao(line l) {

  instrucao res;
  instrucao ops;
  token *toks;
  bool temLabel;
  char *label;
  int instrStart;
  int com;

  res.numeroDaLinha = l.num;
  toks = l.tokens;

  label = getLabel(l, &temLabel);

  instrStart = temLabel ? 2 : 0;

  com = getInstrucao(l, instrStart);

  ops = 
    getOperandos(l, numeroDeOperadores[com],instrStart + 1);
  
  res.temLabel = temLabel;
  res.label    = label;
  res.comando  = com;
  res.numeroDeOperandos = ops.numeroDeOperandos;
  res.primeiro = ops.primeiro;
  res.segundo = ops.segundo;

  return res;

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

    /*int j = 0;
    while (aux->linha.tokens[j].tipo != FIM_DE_LINHA) {
      printf("%s : %s\n", aux->linha.tokens[j].conteudo,
             tipoDeToken_str[aux->linha.tokens[j].tipo]);
      j++;      
    }
    printf("\n");*/

    res = res->next;
    aux = aux->next;

  }
  
  return res_head;

}

int main(int argc, char** argv) {

  if (argc != 4) {
    printf("Numero de argumentos inválido.");
    return 1;
  }

  char *arquivo_entrada  = argv[1];
  char *tipo_de_operacao = argv[2];
  char *arquivo_saida    = argv[3];  

  // checar se arquivo_entrada realmente existe

  char *texto = textoDoArquivo(arquivo_entrada);
  listaDeLinhas *res = getLines(texto);
  listaDeInstrucoes *ins = linhasParaInstrucoes(res);
  
  showInstrucoes(ins);

  free(res);

  return 0;

}

void showInstrucoes (listaDeInstrucoes *ins) {
  
  instrucao g;

  while (ins->next != NULL) {

    g = ins->instr;

    if (g.temLabel) {
      printf("%s : ", g.label);
    }

    int i = numeroDoComando(g.comando);
    printf("%s ", comandos_str[i]);
    
    switch(g.numeroDeOperandos) {

    case  0 : default :

      printf("\n"); break;

    case  1:
      
      if (g.primeiro.sinal == NEG)
        printf("-");
      if (g.primeiro.tipo == HEX)
        printf("0x");

      printf("%s", g.primeiro.conteudo);

      if (g.primeiro.temIncremento) {
        if (g.primeiro.sinalDoIncremento == NEG)
          printf("-");
        else
          printf("+");
        if (g.primeiro.tipoDeIncremento == HEX)
          printf("0x");
        printf("%s",g.primeiro.incremento);
      }

      printf("\n");


      break;

    case  2: 
      
      if (g.primeiro.sinal == NEG)
        printf("-");
      if (g.primeiro.tipo == HEX)
        printf("0x");
      
      printf("%s", 
             g.primeiro.conteudo);
      
      if (g.primeiro.temIncremento) {
        if (g.primeiro.sinalDoIncremento == NEG)
          printf("-");
        else
          printf("+");
        if (g.primeiro.tipoDeIncremento == HEX)
          printf("0x");
        printf("%s",g.primeiro.incremento);
      }

      printf(", ");

      if (g.segundo.sinal == NEG)
        printf("-");
      if (g.segundo.tipo == HEX)
        printf("0x");

      printf("%s", 
             g.segundo.conteudo);

      if (g.segundo.temIncremento) {
        if (g.segundo.sinalDoIncremento == NEG)
          printf("-");
        else
          printf("+");
        if (g.segundo.tipoDeIncremento == HEX)
          printf("0x");
        printf("%s",g.segundo.incremento);
      }

      printf("\n");

      break;

    }

    /*int j = 0;
    while (ins->linha.tokens[j].tipo != FIM_DE_LINHA) {
      printf("%s : %s\n",
             ins->linha.tokens[j].conteudo,
             tipoDeToken_tostring[ins->linha.tokens[j].tipo]);
      j++;
    }
    printf("\n");*/

    ins = ins->next;

  }

}
