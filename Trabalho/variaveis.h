//** variaceis.h **//


// Variaveis do lexer

typedef enum{
    TRUE = 1,
    FALSE = 0
} bool;

typedef enum{
  WORD      = 0,  // Palavra
  NUM_DEC   = 1,  // Numero em decimal
  NUM_HEX   = 2,  // Numero em hexadecimal
  ESP       = 3,  // Espaco ou \t
  VIR       = 4,  // ,
  DOISPTS   = 5,  // Dois pontos
  MAIS      = 6,  // +
  MENOS     = 7,  // -
  PTVIR     = 8,  // ;
  FIM_LINHA = 9,  // \n
  FIM       = 10, // \0
  ERROCHAR  = 11, // Caso tenha caractere errado
  ERRONUM   = 12  // Caso tenha um numero mal formado
} tipoToken;

typedef struct{
  tipoToken tipo;
  char *conteudo;
} token;

typedef struct{
  token *tokens;
  int num;
} line;

typedef struct node{
  line linha;
  struct node *prox;
} listaLinhas;

// Variaveis do parser

typedef enum {
  ADD      = 0,
  SUB      = 1,
  MULT     = 2,
  DIV      = 3,
  JMP      = 4,
  JMPN     = 5,
  JMPP     = 6,
  JMPZ     = 7,
  COPY     = 8,
  LOAD     = 9,
  STORE    = 10,
  INPUT    = 11,
  OUTPUT   = 12,
  STOP     = 13,
  SECTION  = 14,
  SPACE    = 15,
  CONST    = 16,
  EQU      = 17,
  IF       = 18,
  MACRO    = 19,
  ENDMACRO = 20 
} comando;

typedef enum {
  COMANDO_NAO_EXISTE,
  NUMERO_DE_OPERADORES_ERRADO,
  MA_FORMACAO_DE_LABEL,
  COMANDO_MAL_FORMADO,
  SEM_ERROS_SINTATICOS,
  MA_FORMACAO_DOS_OPERADORES
} erroSintatico;

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

char *comandString[] = {
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

int numeroOp[] = {1,1,1,1,1,1,1,1,2,1,1,1,1,0,1,-1,1,1,1,0,0}; // esta na mesma ordem do comando[]

typedef enum {
  PALAVRA, NUM
} tipoDeOperador;

typedef enum {
  POS, NEG
} sinal;

typedef struct {
  bool temOp;
  tipoDeOperador tipo;
  char *conteudoChar;
  int conteudoInt;
  sinal sinal;
  bool temInc;
  sinal sinalInc;
  tipoDeOperador tipoInc;
  char *incrementoChar;
  int incrementoInt;
} operando;

typedef struct {

  bool temLabel;
  char *label;

  comando comando;

  int numeroDeOperandos;
  operando primeiro;
  operando segundo;

  int numeroLinha;

} instrucao;

typedef struct instrs {
  instrucao instr;
  struct instrs *next;
} listaDeInstrucoes; //instructions;

/** Tabela de Simbolos **/

typedef struct{
  char *label;
  int endereco;
  int tamanho;
  bool constante;
}simbolo;

typedef struct tabsim{
  simbolo simbolo;
  struct tabsim *next;
}tabelaDeSimbolos;

bool ERRO_EXEC = FALSE;
