#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "arvore.h"
#include "semantico.h"
#include "sintatico.tab.h"

/* GLOBAL */
struct variavel* inicio_lista_vars = NULL;
struct variavel* fim_lista_vars = NULL;
extern FILE* arq_saida;


/* Aloca nova variavel na lista */
struct variavel* nova_variavel(int tipo, char* nome, union numero valor, struct variavel* anterior){

	struct variavel * nova = malloc(sizeof(struct variavel));
	nova->tipo = tipo;
	nova->nome = nome;
	switch(tipo){
		case NUM_INT:
			nova->valor.inteiro = valor.inteiro;
			break;
		case NUM_FLOAT:
			nova->valor.real = valor.real;
			break;
		default:
			printf("\nErro interno: tipo indefinido.");
			exit(1);
	}
	nova->next = NULL;
	if (anterior){
		anterior->next = nova;
	}else{
		inicio_lista_vars = nova;	
	}
	return nova;
}


/* Obtem tipo e valor de uma variavel, buscando por seu nome */
struct tipo_valor get_tipo_valor_variavel(char* nome){

	struct tipo_valor tipo_valor;
	struct variavel* var = inicio_lista_vars;
	while(var){
		if( strcmp(nome, var->nome) == 0){
			tipo_valor.tipo = var->tipo;
			switch(var->tipo){
				case NUM_INT:
					tipo_valor.valor.inteiro = var->valor.inteiro;
					break;
				case NUM_FLOAT:
					tipo_valor.valor.real = var->valor.real;
					break;
				default:
					printf("\nErro interno: tipo indefinido.");
					exit(1);
			}
			return tipo_valor;
		}
		var = var->next;
	}
	tipo_valor.tipo = -1;
	return tipo_valor;
}

/* Altera o valor de uma variavel inteira, buscando por seu nome */
void set_valor_variavel_int(char *nome, int novo_valor){
	
	struct variavel* var = inicio_lista_vars;

	while(var){
		if( strcmp(nome, var->nome) == 0){
			var->valor.inteiro = novo_valor;
			return;
		}
		var = var->next;
	}
	printf("\nErro interno: variavel nao alocada.");
	exit(1);
}

/* Altera o valor de uma variavel real, buscando por seu nome */
void set_valor_variavel_real(char *nome, float novo_valor){
	
	struct variavel* var = inicio_lista_vars;

	while(var){
		if( strcmp(nome, var->nome) == 0){
			var->valor.real = novo_valor;
			return;
		}
		var = var->next;
	}
	printf("\nErro interno: variavel nao alocada.");
	exit(1);
}


/* Valida uma expressao */
struct tipo_valor valida_exp(struct no *no){

	struct tipo_valor tipo_valor1, tipo_valor2, tipo_valor3;
	
	switch(no->tipo) {
		case ADD:
			tipo_valor1 = valida_exp(no->l);
			tipo_valor2 = valida_exp(no->r);
			if(tipo_valor1.tipo == tipo_valor2.tipo){
				tipo_valor3.tipo = tipo_valor1.tipo;
				switch(tipo_valor3.tipo){
					case NUM_INT:
						tipo_valor3.valor.inteiro = tipo_valor1.valor.inteiro + tipo_valor2.valor.inteiro;
						break;
					case NUM_FLOAT:
						tipo_valor3.valor.real = tipo_valor1.valor.real + tipo_valor2.valor.real;
						break;
					default:
						printf("\nErro interno: tipo indefinido.");
						exit(1);
				}
				return tipo_valor3;
			}
			printf("\nAnalisador semantico: (linha %d, coluna %d)\nOperacao sobre tipos diferentes: '", no->linha, no->coluna);
			print_no(no); printf("'\n");
			exit(1);
		case SUB:
			tipo_valor1 = valida_exp(no->l);
			tipo_valor2 = valida_exp(no->r);
			if(tipo_valor1.tipo == tipo_valor2.tipo){
				tipo_valor3.tipo = tipo_valor1.tipo;
				switch(tipo_valor3.tipo){
					case NUM_INT:
						tipo_valor3.valor.inteiro = tipo_valor1.valor.inteiro - tipo_valor2.valor.inteiro;
						break;
					case NUM_FLOAT:
						tipo_valor3.valor.real = tipo_valor1.valor.real - tipo_valor2.valor.real;
						break;
					default:
						printf("\nErro interno: tipo indefinido.");
						exit(1);
				}
				return tipo_valor3;
			}
			printf("\nAnalisador semantico: (linha %d, coluna %d)\nOperacao sobre tipos diferentes: '", no->linha, no->coluna);
			print_no(no); printf("'\n");
			exit(1);
		case MUL:
			tipo_valor1 = valida_exp(no->l);
			tipo_valor2 = valida_exp(no->r);
			if(tipo_valor1.tipo == tipo_valor2.tipo){
				tipo_valor3.tipo = tipo_valor1.tipo;
				switch(tipo_valor3.tipo){
					case NUM_INT:
						tipo_valor3.valor.inteiro = tipo_valor1.valor.inteiro * tipo_valor2.valor.inteiro;
						break;
					case NUM_FLOAT:
						tipo_valor3.valor.real = tipo_valor1.valor.real * tipo_valor2.valor.real;
						break;
					default:
						printf("\nErro interno: tipo indefinido.");
						exit(1);
				}
				return tipo_valor3;
			}
			printf("\nAnalisador semantico: (linha %d, coluna %d)\nOperacao sobre tipos diferentes: '", no->linha, no->coluna);
			print_no(no); printf("'\n");
			exit(1);
		case POW:
			tipo_valor1 = valida_exp(no->l);
			tipo_valor2 = valida_exp(no->r);
			if(tipo_valor1.tipo == tipo_valor2.tipo){
				tipo_valor3.tipo = tipo_valor1.tipo;
				switch(tipo_valor3.tipo){
					case NUM_INT:
						tipo_valor3.valor.inteiro = pow(tipo_valor1.valor.inteiro, tipo_valor2.valor.inteiro);
						break;
					case NUM_FLOAT:
						tipo_valor3.valor.real = pow(tipo_valor1.valor.real, tipo_valor2.valor.real);
						break;
					default:
						printf("\nErro interno: tipo indefinido.");
						exit(1);
				}
				return tipo_valor3;
			}
			printf("\nAnalisador semantico: (linha %d, coluna %d)\nOperacao sobre tipos diferentes: '", no->linha, no->coluna);
			print_no(no); printf("'\n");
			exit(1);
		case DIV:
			tipo_valor1 = valida_exp(no->l);
			tipo_valor2 = valida_exp(no->r);
			if(tipo_valor1.tipo == tipo_valor2.tipo){
				switch(tipo_valor1.tipo){
					case NUM_INT:
						if(tipo_valor2.valor.inteiro == 0){
							printf("\nAnalisador semantico: (linha %d, coluna %d)\nDivisao por zero: '", no->linha, no->coluna-1);
							print_no(no); printf("'\n");
							exit(1);
						}
						tipo_valor3.valor.inteiro = tipo_valor1.valor.inteiro / tipo_valor2.valor.inteiro;
						tipo_valor3.tipo = NUM_INT;
						break;
					case NUM_FLOAT:
						if(tipo_valor2.valor.real == 0.0){
							printf("\nAnalisador semantico: (linha %d, coluna %d)\nDivisao por zero: '", no->linha, no->coluna-1);
							print_no(no); printf("'\n");
							exit(1);
						}
						tipo_valor3.valor.real = tipo_valor1.valor.real / tipo_valor2.valor.real;
						tipo_valor3.tipo = NUM_FLOAT;
						break;
					default:
						printf("\nErro interno: tipo indefinido.");
						exit(1);
				}
				return tipo_valor3;
			}
			printf("\nAnalisador semantico: (linha %d, coluna %d)\nOperacao sobre tipos diferentes: '", no->linha, no->coluna);
			print_no(no); printf("'\n");
			exit(1);
		case NUM_INT:
			tipo_valor1.tipo = NUM_INT;
			tipo_valor1.valor.inteiro = ((struct no_folha*)no)->valor.inteiro;
			return tipo_valor1;
		case NUM_FLOAT:
			tipo_valor1.tipo = NUM_FLOAT;
			tipo_valor1.valor.real = ((struct no_folha*)no)->valor.real;
			return tipo_valor1;
		case ID:
			tipo_valor1 = get_tipo_valor_variavel( ((struct no_folha*)no)->valor.string );
			if(tipo_valor1.tipo == -1){
				printf("\nAnalisador semantico: (linha %d, coluna %d)\nVariavel '%s' usada antes da definicao: '", no->linha, no->coluna-1, ((struct no_folha*)no)->valor.string );
				print_no(no); printf("'\n");
				exit(1);
			}
			return tipo_valor1;
		default:
			printf("\nErro interno: tipo de no desconhecido");
			exit(1);
	}
}


/* Valida uma atribuicao */
void valida_atr(char* id, struct no *no){
	
	struct tipo_valor tipo_valor_exp = valida_exp(no);
	struct tipo_valor tipo_valor_var = get_tipo_valor_variavel(id);

	if(tipo_valor_var.tipo == -1){
		// Alocar nova variavel na lista
		fim_lista_vars = nova_variavel(tipo_valor_exp.tipo, id, tipo_valor_exp.valor, fim_lista_vars);
	}else{
		if(tipo_valor_var.tipo != tipo_valor_exp.tipo){
			printf("\nAnalisador semantico: (linha %d, coluna %d)\nAtribuicao sobre tipos diferentes: '%s = ", no->linha, no->coluna, id);
			print_no(no); printf("'\n");
			exit(1);
		}
		switch(tipo_valor_var.tipo){
			case NUM_INT:
				set_valor_variavel_int(id, tipo_valor_exp.valor.inteiro);
				break;
			case NUM_FLOAT:
				set_valor_variavel_real(id, tipo_valor_exp.valor.real);
				break;
			default:
				printf("\nErro interno: tipo indefinido.");
				exit(1);
		}
	}

	return;
}


/* Analisador semantico */
void analise_semantica(struct arvore_sintatica * arvore){

	int teste = 99;
	fprintf(arq_saida, "%d", teste);

	struct tipo_valor tipo_valor;

	while(arvore){

		switch(arvore->tipo){
			case 1:
				valida_exp(arvore->exp);
				break;
			case 2:
				tipo_valor = valida_exp(arvore->exp);
				switch(tipo_valor.tipo){
					case NUM_INT:
						printf("%d\n", tipo_valor.valor.inteiro);
						break;
					case NUM_FLOAT:
						printf("%.2f\n", tipo_valor.valor.real);
						break;
					default:
						printf("\nErro interno: tipo indefinido.");
						exit(1);
				}
				break;
			case 3:
				valida_atr(arvore->id, arvore->exp);
				break;
		}
		arvore = arvore->next;
	}
}

