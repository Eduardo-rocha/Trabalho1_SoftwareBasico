// comandos possíveis
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

// operando

typedef enum {
  PALAVRA, DEC, HEX
} tipoDeOperador;

typedef enum {
  POS, NEG
} sinal;

//
typedef struct {

  bool operadorPresente;

  tipoDeOperador tipo;
  char *conteudo;

  sinal sinal;

  bool temIncremento;
  sinal sinalDoIncremento;
  tipoDeOperador tipoDeIncremento;
  char *incremento;

} operando;

typedef struct {
  
  bool temLabel;
  char *label;

  comando comando;

  int numeroDeOperandos;
  operando primeiro;
  operando segundo;

  int numeroDaLinha;

} instrucao;

// lista de intrucoes
typedef struct instrs {
  instrucao instr;
  struct instrs *next;
} listaDeInstrucoes; //instructions;

typedef struct {
  instrucao instr;
  erroSintatico erro;
} instrucaoOuErro;

char *toUpperCase(char *in);

int numeroDoComando(comando comm);

bool comandoExiste(char *comm, int *c);

operando pegarOperador(token *toks, int *c, int ln);

void erroSintaticoSe(erroSintatico err, int ln);

instrucao linhaParaInstrucao(line l);

listaDeInstrucoes *linhasParaInstrucoes(listaDeLinhas *head);

void showInstrucoes (listaDeInstrucoes *ins);
