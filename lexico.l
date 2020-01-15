
%{

#include <stdio.h>
#include <string.h>
#include "sintatico.tab.h"
#include "arvore.h"

extern int linha;
extern int coluna;

%}

%%

"//"[^\n]* { /* Comenta(ignora) toda linha a partir do simbolo // */ }
[ \t]	{ coluna += strlen(yytext); } /* Ignora espacos em branco e tabulacoes */
\n { linha++; }

[a-zA-Z][a-zA-Z0-9_]* if(strcmp(yytext, "print") == 0){ coluna += 5; return PRINT;}else{ coluna += strlen(yytext); yylval.string = strdup(yytext); return ID;}

[0-9]+		{ coluna += strlen(yytext); yylval.value_int = atoi(yytext); return NUM_INT; }
-[0-9]+		{ coluna += strlen(yytext); yylval.value_int = atoi(yytext); return NUM_INT; }
[0-9]+\.[0-9]+ 	{ coluna += strlen(yytext); yylval.value_float = atof(yytext); return NUM_FLOAT; }
-[0-9]+\.[0-9]+ { coluna += strlen(yytext); yylval.value_float = atof(yytext); return NUM_FLOAT; }

"+"	{ coluna++; return ADD; }
"-"	{ coluna++; return SUB; }
"*"	{ coluna++; return MUL; }
"/"	{ coluna++; return DIV; }
"^"	{ coluna++; return POW; }

"="	{ coluna++; return ATR; }

"(" 	{ coluna++; return OPEN; }
")" 	{ coluna++; return CLOSE; }

";"	{ return END_LINE; }
.	{ fprintf(stderr, "\nAnalisador lexico: (linha %d, coluna %d)\nCaractere '%c' nao reconhecido pela linguagem.\n", linha, coluna, *yytext); exit(1);}

%%

