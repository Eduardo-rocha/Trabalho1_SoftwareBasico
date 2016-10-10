#define LEN 9
#define HLEN 12
#define COMANDOS 21

const char chars[LEN] = 
  { ',', ':', '+', '-', ';', ' ', '\t', '\n', '\0' };

const tipoDeToken tokenOfCharTable[LEN] = 
  { VIRGULA, DOIS_PONTOS, MAIS, MENOS, PONTO_VIRGULA, ESPACO, ESPACO, FIM_DE_LINHA, FIM };

char *tipoDeToken_str[] = {
  "WORD",
  "DEC_NUM",
  "HEX_NUM",
  "ESPACO",
  "VIRGULA",
  "DOIS_PONTOS",
  "MAIS",
  "MENOS",
  "PONTO_VIRGULA",
  "FIM_DE_LINHA",
  "FIM"
};

char H[] = {'A', 'B', 'C', 'D', 'E', 'F'};

char *comandos_str[] = {
  "ADD",
  "SUB",
  "MULT",
  "DIV",
  "JMP",
  "JMPN",
  "JMPP",
  "JMPZ",
  "COPY",
  "LOAD",
  "STORE",
  "INPUT",
  "OUTPUT",
  "STOP",
  "SECTION",
  "SPACE",
  "CONST",
  "EQU",
  "IF",
  "MACRO",
  "ENDMACRO"
};

comando comandos[] = {
  ADD,
  SUB,
  MULT,
  DIV,
  JMP,
  JMPN,
  JMPP,
  JMPZ,
  COPY,
  LOAD,
  STORE,
  INPUT,
  OUTPUT,
  STOP,
  SECTION,
  SPACE,
  CONST,
  EQU,
  IF,
  MACRO,
  ENDMACRO
};

int numeroDeOperadores[] = {
  1,1,1,1,
  1,1,1,1,
  2,1,1,1,
  1,0,1,-1,
  1,1,1,0,0
};
