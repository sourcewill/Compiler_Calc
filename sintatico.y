/* Desenvolvido por: William Rodrigues @ UEM 2020. */

%{

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
	fprintf(stderr, "\nUSO:\n  ./calc <arquivo_entrada> -o <arquivo_saida>\n\n");
	fprintf(stderr, "\nEXEMPLO DE USO:\n  ./calc entrada.calc -o saida\n\n");
	fprintf(stderr, "\nDesenvolvido por: William Rodrigues @ UEM 2020.\n\n");
	exit(0);
}

int main(int argc, char* argv[]) {

	char nome_arq_entrada[254] = "";
	char nome_arq_saida[254] = "";
	char nome_arq_saida_llvm[254];
	int opt, indice;


	//if ( argc < 2 ) mostra_help() ;

	while( (opt = getopt(argc, argv, "ho:")) > 0 ) {
		switch ( opt ) {
			case 'h':
				mostra_help() ;
				break ;
			case 'o':
				strcpy(nome_arq_saida, optarg);
				break ;
			default:
				fprintf(stderr, "Opcao invalida ou faltando argumento: `%c'\n\n", optopt) ;
				return -1 ;
		}
	}

	for (indice = optind; indice < argc; indice++){
		strcpy(nome_arq_entrada, argv[indice]);
	}

	if(strcmp(nome_arq_entrada, "") == 0){
		fprintf(stderr, "\nErro: Arquivo de entrada nao informado.");
		fprintf(stderr, "\nCalc: Utilize a opcao -h para ajuda.\n\n");
		exit(1);
	}
	if(strcmp(nome_arq_saida, "") == 0){
		fprintf(stderr, "\nErro: Arquivo de saida nao informado.");
		fprintf(stderr, "\nCalc: Utilize a opcao -h para ajuda.\n\n");
		exit(1);
	}

	yyin = fopen(nome_arq_entrada, "r");
	if(!yyin){
		fprintf(stderr, "\nErro ao abrir arquivo de entrada: '%s'\n\n", nome_arq_entrada);
		exit(1);
	}

	printf("\nArquivo de entrada: %s\n", nome_arq_entrada);

	printf("\nExecutando FrontEnd...");

	yyparse();
	fclose(yyin);

	analise_semantica( get_inicio_lista_arvore() );

	strcpy((char*)nome_arq_saida_llvm, (char*)nome_arq_saida);
	strcat(nome_arq_saida_llvm, ".ll");

	arq_saida = fopen(nome_arq_saida_llvm, "w");
	if(!arq_saida){
		fprintf(stderr, "\nErro ao abrir arquivo de saida: '%s'\n\n", nome_arq_saida_llvm);
		exit(1);
	}

	printf("\nExecutando BackEnd...\n");
	backend( get_inicio_lista_arvore() );

	fclose(arq_saida);

	/* Forma e executa comando clang para compilar o codigo intermediario gerado*/
	char comando[254] = "clang ";
	strcat(comando, nome_arq_saida_llvm);
	strcat(comando, " -o ");
	strcat(comando, nome_arq_saida);
	strcat(comando, " -lm");
	system(comando);

	printf("\nArquivo intermediario '%s' criado no atual diretorio.", nome_arq_saida_llvm);
	printf("\nArquivo de saida '%s' criado no atual diretorio.", nome_arq_saida);
	printf("\nExecute usando o comando:\n./%s\n\n", nome_arq_saida);
	return 0;
}

void yyerror(char *s) {
	fprintf(stderr, "\nAnalisador sintatico: (linha %d, coluna %d)\nErro sintatico.\n", linha, coluna-1);
	exit(1);
}
