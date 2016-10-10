
typedef enum {
  TRUE  = 1,
  FALSE = 0
} bool;

typedef enum {
  WORD          = 0,
  DEC_NUM       = 1,
  HEX_NUM       = 2,
  ESPACO        = 3,
  VIRGULA       = 4,
  DOIS_PONTOS   = 5,
  MAIS          = 6,
  MENOS         = 7,
  PONTO_VIRGULA = 8,
  FIM_DE_LINHA  = 9,
  FIM           = 10
} tipoDeToken;

typedef enum {
  EXP_WORD      = 0,
  EXP_DEC_NUM   = 1,
  EXP_NUM       = 2,
  EXP_SPACE     = 3,
  EXP_COMEN     = 4,
  NOT_EXP       = 5
} tipoDeTokenEsperado;

typedef struct {
  tipoDeToken tipo;
  char *conteudo;
} token;

typedef enum {
  CARACTER_INVALIDO = 0,
  NUMERO_MAL_FORMADO = 1,
  SEM_ERROS = 2
} erroLexico;

typedef struct {
  token token;
  erroLexico erro;
} tokenOuErro;

typedef struct {
  bool fim;
  token *tokens;
  int num;
} line;

typedef struct node {
  line linha;
  struct node *next;
} listaDeLinhas;

// protótipos

char *textoDoArquivo(char* arquivo_entrada);

token construirToken(char *conteudo, tipoDeToken tt);

tokenOuErro tokenOfChar(char *var);

bool isL(char c);

bool isD(char c);

bool isH(char c);

tipoDeTokenEsperado estadoEsperado(char c);

tokenOuErro scanner(int *count, char *text);

void erroLexicoSe(erroLexico err, int line);

line getLine(int *count, char *texto, int linenumber);

listaDeLinhas *getLines(char* texto);
