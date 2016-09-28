//** Lexer.h **//

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
