
%{

#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#include "semantico.h"

extern FILE* yyin;
FILE* arq_saida;
int yylex();
void yyerror(char *);

struct arvore_sintatica* fim_lista_arvore = NULL;

int linha = 1;
int coluna = 1;

%}

%union {
	struct no *no;
	int value_int;
	float value_float;
	char* string;
}

%token ADD SUB MUL DIV POW END_LINE OPEN CLOSE ATR PRINT

%token <string> ID
%token <value_int> NUM_INT
%token <value_float> NUM_FLOAT

%type <no> exp

%start inicio

%left ADD SUB
%left MUL DIV
%left POW

%%

 /* GRAMATICA */

inicio: item | inicio item

item
: PRINT exp END_LINE	{ fim_lista_arvore = novo_arvore_sintatica(1, $2, NULL, fim_lista_arvore); linha++; coluna=1; }
| ID ATR exp END_LINE	{ fim_lista_arvore = novo_arvore_sintatica(2, $3, $1, fim_lista_arvore); linha++; coluna=1; }
;

exp
: NUM_INT		{ $$ = novo_no_folha_int($1, linha, coluna); }
| NUM_FLOAT		{ $$ = novo_no_folha_float($1, linha, coluna); }
| ID 			{ $$ = novo_no_folha_id($1, linha, coluna); }
| exp ADD exp		{ $$ = novo_no(ADD, $1, $3, linha, coluna); }
| exp SUB exp		{ $$ = novo_no(SUB, $1, $3, linha, coluna); }
| exp MUL exp		{ $$ = novo_no(MUL, $1, $3, linha, coluna); }
| exp DIV exp		{ $$ = novo_no(DIV, $1, $3, linha, coluna); }
| exp POW exp 		{ $$ = novo_no(POW, $1, $3, linha, coluna); }
| OPEN exp CLOSE 	{ $$ = $2; }
;

%%


int main(int argc, char* argv[]) {

	char* nome_arq_entrada;
	char* nome_arq_saida;

	nome_arq_entrada = "entrada.calc";
	nome_arq_saida = "saida.ll";

	yyin = fopen(nome_arq_entrada, "r");
	if(!yyin){
		printf("\nErro ao abrir arquivo de entrada: '%s'\n", nome_arq_entrada);
		exit(1);
	}

	arq_saida = fopen(nome_arq_saida, "w");
	if(!arq_saida){
		printf("\nErro ao abrir arquivo de saida: '%s'\n", nome_arq_saida);
		exit(1);
	}

	printf("\nExecutando FrontEnd e Motor de Execucao.\n\n");

	yyparse();
	fclose(yyin);

	analise_semantica( get_inicio_lista_arvore() );
	fclose(arq_saida);

	printf("\n");
	return 0;
}

void yyerror(char *s) {
	fprintf(stderr, "\nAnalisador sintatico: (linha %d, coluna %d)\nErro sintatico.\n", linha, coluna-1);
	exit(1);
}
