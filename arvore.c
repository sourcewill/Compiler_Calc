#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"
#include "sintatico.tab.h"

void yyerror(char *);

/* Cria novo No*/
struct no *novo_no(int tipo, struct no *l, struct no *r, int linha, int coluna){

	struct no *no = malloc(sizeof(struct no));
	if (!no) {
		yyerror("out of space");
		exit(0);
	}
	no->tipo = tipo;
	no->l = l;
	no->r = r;
	no->linha = linha;
	no->coluna = coluna;
	return no;
}

struct no *novo_no_folha_int(int valor, int linha, int coluna){

	struct no_folha *no = malloc(sizeof(struct no_folha));
	if (!no) {
		yyerror("out of space");
		exit(0);
	}
	no->tipo = NUM_INT;
	no->valor.inteiro = valor;
	no->linha = linha;
	no->coluna = coluna;
	return (struct no *)no;
}

struct no *novo_no_folha_float(float valor, int linha, int coluna){

	struct no_folha *no = malloc(sizeof(struct no_folha));
	if (!no) {
		yyerror("out of space");
		exit(0);
	}
	no->tipo = NUM_FLOAT;
	no->valor.real = valor;
	no->linha = linha;
	no->coluna = coluna;
	return (struct no *)no;
}

struct no *novo_no_folha_id(char* valor, int linha, int coluna){
	struct no_folha *no = malloc(sizeof(struct no_folha));
	if (!no) {
		yyerror("out of space");
		exit(0);
	}
	no->tipo = ID;
	no->valor.string = valor;
	no->linha = linha;
	no->coluna = coluna;
	return (struct no *)no;
}

/* Print No */
void print_no(struct no *no){

	switch(no->tipo) {
		case ADD:
			print_no(no->l);
			printf("+");
			print_no(no->r);
			break;
		case SUB:
			print_no(no->l);
			printf("-");
			print_no(no->r);
			break;
		case MUL:
			print_no(no->l);
			printf("*");
			print_no(no->r);
			break;
		case DIV:
			print_no(no->l);
			printf("/");
			print_no(no->r);
			break;
		case POW:
			print_no(no->l);
			printf("^");
			print_no(no->r);
			break;
		case NUM_INT:
			printf("%d", ((struct no_folha *)no)->valor.inteiro);
			break;
		case NUM_FLOAT:
			printf("%.2f", ((struct no_folha *)no)->valor.real);
			break;
		case ID:
			printf("%s", ((struct no_folha *)no)->valor.string);
			break;
		default:
			printf("\nErro interno: tipo de no desconhecido");

	}
}

/* Arvore sintatica (lista de nos) */

struct arvore_sintatica * novo_arvore_sintatica(int tipo, struct no* exp, char* id, struct arvore_sintatica* anterior){
	
	struct arvore_sintatica * novo = malloc(sizeof(struct arvore_sintatica));
	novo->tipo = tipo;
	novo->exp = exp;
	if(tipo == 3){
		novo->id = id;
	}
	novo->next = NULL;
	if (anterior){
		anterior->next = novo;
	}else{
		inicio_lista_arvore = novo;	
	}
	return novo;
}

struct arvore_sintatica * get_inicio_lista_arvore(){
	return inicio_lista_arvore;
}



