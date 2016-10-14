#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include "variaveis.h"
#include "lexer.c"
#include "parser.c"
#include "montador.c"

int *numeroLinhasMacro (){

  FILE *arq=fopen("NumeroDeLinhasM.bin","rb");
  int *linhasMacro = (int*)malloc(sizeof(int));
  int size = 0; 

  if(!arq){
    printf("Erro ao abrir arquivo NumeroDeLinhas.bin\n");
    exit;
  }

  while(!feof(arq)){
    fread(linhasMacro[size],sizeof(int),1,arq);
    linhasMacro = realloc(linhasMacro, sizeof(int)*(size+1));
    size++;
  }
  
  fclose(arq);

  return linhasMacro;
}

char* concatenar3(char *s1, char *s2)
{
  char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int main(int argc, char** argv) {

  // variavel global definida em lexer.h
  // define se houveram ou não erros durante
  // todo o processamento
  ERRO_EXEC = FALSE; 

  if (argc != 4){
    printf("\tNumero de argumentos invalido\n\tSao necessarios 3 argumentos: o nome do arquivo a ser compilado, a opcao e o nome do arquivo objeto\n");
    return 0;
  }

  char *arquivo_entrada = argv[1];
  char *operacao        = argv[2];
  char *arquivo_saida   = argv[3];
  
  if (operacao[0] != '-' || strlen(operacao) != 2){
    printf("\tO segundo argumento esta mal formado:\n\t -p: pre-processado\n\t -m: arquivo apos a resolucao das macros\n\t -o: para o arquivo objeto final\n");
  }

  char *texto = textoDoArquivo(arquivo_entrada);
  char op = operacao[1];  
  listaLinhas *lins;
  listaDeInstrucoes *ins, *ins2;
  char *res;
  int ultimoEndereco;
  
  switch(op) {

  //Processamento de EQUs e IFs
  case 'p' : 
    
    ////////////////////////////////////////// Gera arquivo pre-proc
    char comp[] = "./preproc ";
    char *entp = concat(arquivo_entrada," ");
    char *saidap = concat(arquivo_saida,".p");
    char *comandop = concat(com,entp,saidap);
    system(comandop);
    break;

  
  // Processamento de EQUs, IFs e MACROs
  case 'm' :
    
    //////////////////////////////////////////gera arquivo com macros resolvidas
    char comp[] = "./preproc ";
    char *entp = concat(arquivo_entrada," ");
    char *saidap = concat(arquivo_saida,".p");
    char *comandop = concat(com,entp,saidap);
    system(comandop);
    char comm[] = "./macro ";
    char *entm = concat(saidap," ");
    char *saidam = concat(arquivo_saida,".m");
    char *comandom = concat(comm,ent,saidam);
    system(comando);
    break;

  // Geração de código
  case 'o' : 

    /////////////////////////////////////////// Colocar o pre-proc aqui
    
    char comp[] = "./preproc ";
    char *entp = concat(arquivo_entrada," ");
    char *saidap = concat(arquivo_saida,".p");
    char *comandop = concat(com,entp,saidap);
    system(comandop);

    char comm[] = "./macro ";
    char *entm = concat(saidap," ");
    char *saidam = concat(arquivo_saida,".m");
    char *comandom = concat(comm,ent,saidam);
    system(comandom);

    char *texto = textoDoArquivo(saidam);

    int *linhasMacro = numeroLinhasMacro();

    lins = construirListaLinhas(texto,linhasMacro); //////////////////////////////////////// No lugar de texto colocar string apos o pre-proc e o proc de macros
    //imprimeLinhas(lins);
    ins  = linhasParaInstrucoes(lins);
    //imprimeInstrucoes(ins);

    // geracao de codigo aqui

    tabelaDeSimbolos *tab = primeiraPassagem(ins,&ultimoEndereco);
    //imprimeTabelaSim(tab);
    char *final = geraCodigoFinal(ins,tab,arquivo_saida,ultimoEndereco);
    if (!ERRO_EXEC) {
      sprintf(arquivo_saida,"%s.o",arquivo_saida);
      FILE *fp = fopen(arquivo_saida,"w");
      fprintf(fp,"%s",final);
      fclose(fp);
      printf("\n%s\n",final);
      printf("\nCompilação bem sucedida.\n\n");
    }
    
    break;

  default :

    printf("\tO segundo argumento esta mal formado:\n\t -p: pre-processado\n\t -m: arquivo apos a resolucao das macros\n\t -o: para o arquivo objeto final\n");
    return 0;

  }
      
  return 0;

}

