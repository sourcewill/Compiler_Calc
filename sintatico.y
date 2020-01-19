/* Desenvolvido por: William Rodrigues @ UEM 2020. */

%{

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "arvore.h"
#include "semantico.h"
#include "backend.h"

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

void mostra_help(){
	fprintf(stderr, "\nVISAO GERAL:\n  Compilador para linguagem de programacao Calc.\n");
	fprintf(stderr, "\nENTRADA:\n  Programa na linguagem Calc.\n");
	fprintf(stderr, "\nSAIDA:\n  Bitcode (codigo intermediario LLVM).\n");
	fprintf(stderr, "\nOPCOES:\n  -o <arquivo_saida>\n  Especifica o arquivo de saida para o compilador.\n  -h\n  Exibe mensagem informativa sobre o compilador.\n");
	fprintf(stderr, "\nEXEMPLO DE USO:\n  ./calc entrada.calc -o saida.ll\n\n");
	fprintf(stderr, "\nDesenvolvido por: William Rodrigues @ UEM 2020.\n\n");
	exit(0);
}

int main(int argc, char* argv[]) {

	char* nome_arq_entrada;
	char* nome_arq_saida;
	int opt, indice;
	nome_arq_entrada = "entrada.calc";
	nome_arq_saida = "saida_calc.ll";

	if ( argc < 2 ) mostra_help() ;

	while( (opt = getopt(argc, argv, "ho:")) > 0 ) {
		switch ( opt ) {
			case 'h':
				mostra_help() ;
				break ;
			case 'o':
				nome_arq_saida = optarg ;
				break ;
			default:
				fprintf(stderr, "Opcao invalida ou faltando argumento: `%c'\n\n", optopt) ;
				return -1 ;
		}
	}

	for (indice = optind; indice < argc; indice++){
		nome_arq_entrada = argv[indice];
	}

	yyin = fopen(nome_arq_entrada, "r");
	if(!yyin){
		fprintf(stderr, "\nErro ao abrir arquivo de entrada: '%s'\n\n", nome_arq_entrada);
		exit(1);
	}

	printf("\nExecutando FrontEnd e Motor de Execucao...\n\n");

	yyparse();
	fclose(yyin);

	analise_semantica( get_inicio_lista_arvore() );

	arq_saida = fopen(nome_arq_saida, "w");
	if(!arq_saida){
		fprintf(stderr, "\nErro ao abrir arquivo de saida: '%s'\n\n", nome_arq_saida);
		exit(1);
	}

	printf("\nExecutando BackEnd...\n");
	backend( get_inicio_lista_arvore() );

	fclose(arq_saida);

	printf("\nArquivo de saida '%s' criado no atual diretorio.", nome_arq_saida);
	printf("\nPara gerar um executavel use o seguinte comando:\nclang %s -o <nome_executavel> -lm\n\n", nome_arq_saida);
	return 0;
}

void yyerror(char *s) {
	fprintf(stderr, "\nAnalisador sintatico: (linha %d, coluna %d)\nErro sintatico.\n", linha, coluna-1);
	exit(1);
}
