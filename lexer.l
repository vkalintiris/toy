%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"

static char *string_value;
static int int_value;
static float float_value;

%}

%%

 /* math operators */
"+"      { return OP_ADD;     }
"-"      { return OP_SUB;     }
"*"      { return OP_MUL;     }
"/"      { return OP_DIV;     }

 /* comparison operators */
">"      { return OP_GT;      }
"<"      { return OP_LT;      }
"=="     { return OP_EQ;      }
"!="     { return OP_NE;      }
"<="     { return OP_LE;      }
">="     { return OP_GE;      }

 /* assignment operator */
"="      { return OP_ASSIGN;  }

 /* statement terminator */
";"      { return SEMICOLON;  }

 /* grouping operators */
"{"      { return LBRACE;     }
"}"      { return RBRACE;     }
"("      { return LPAREN;     }
")"      { return RPAREN;     }

 /* data type keywords */
"int"    { return KW_INT;     }
"float"  { return KW_FLOAT;   }

 /* control flow keywords */
"if"     { return KW_IF;      }
"else"   { return KW_ELSE;    }
"while"  { return KW_WHILE;   }

 /* regular expression for identifier names */
[_a-zA-Z][_a-zA-Z0-9]* {
   string_value = strdup(yytext);
   return IDENTIFIER;
}

 /* regular expression for integer constants */
[0-9]+ {
   int_value = atoi(yytext);
   return INT_CONSTANT;
}
[0-9]+\.[0-9]+ {
   float_value = (float) atof(yytext);
   return FLOAT_CONSTANT;
}

 /* ignore whitespace */
[ \r\t]+ { }

 /* ignore newlines */
\n       { }

 /* ignore unknown characters */
.        { }

%%

static void
print_token(int token) {
  switch (token) {
    case OP_ADD: printf("OP_ADD"); break;
    case OP_SUB: printf("OP_SUB"); break;
    case OP_MUL: printf("OP_MUL"); break;
    case OP_DIV: printf("OP_DIV"); break;

    case OP_GT: printf("OP_GT"); break;
    case OP_LT: printf("OP_LT"); break;
    case OP_EQ: printf("OP_EQ"); break;
    case OP_NE: printf("OP_NE"); break;
    case OP_LE: printf("OP_LE"); break;
    case OP_GE: printf("OP_GE"); break;

    case LBRACE: printf("LBRACE"); break;
    case RBRACE: printf("RBRACE"); break;
    case LPAREN: printf("LPAREN"); break;
    case RPAREN: printf("RPAREN"); break;

    case KW_INT: printf("KW_INT"); break;
    case KW_FLOAT: printf("KW_FLOAT"); break;
    case KW_IF: printf("KW_IF"); break;
    case KW_ELSE: printf("KW_ELSE"); break;
    case KW_WHILE: printf("KW_WHILE"); break;

    case OP_ASSIGN: printf("OP_ASSIGN"); break;
    case SEMICOLON: printf("SEMICOLON"); break;

    case IDENTIFIER:
      printf("IDENTIFIER %s", string_value);
      break;

    case INT_CONSTANT:
      printf("INT_CONSTANT %d", int_value);
      break;

    case FLOAT_CONSTANT:
      printf("FLOAT_CONSTANT %f", float_value);
      break;

    default:
      fprintf(stderr, "Unknokwn token: %d", token);
      break;
  }

  printf("\n");
}

int main(int argc, char *argv[]) {
  int token;

  while ((token = yylex()) > 0)
    print_token(token);

  return 0;
}
