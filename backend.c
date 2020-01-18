#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "arvore.h"
#include "semantico.h"
#include "sintatico.tab.h"
#include "backend.h"

/* GLOBAL */
extern FILE* arq_saida;
int ID_REG = 1;
extern struct variavel* inicio_lista_vars;
extern struct variavel* fim_lista_vars;

/* Insere inicio do arquivo de saida */
void insere_inicio_saida(){

	fprintf(arq_saida, "; Codigo LLVM intermediario gerado pelo compilador Calc.\n; Desenvolvido por William Rodrigues.");
	fprintf(arq_saida, "\n\n@.str = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\", align 1");
	fprintf(arq_saida, "\n@.str.1 = private unnamed_addr constant [6 x i8] c\"%%.2f\\0A\\00\", align 1");
	fprintf(arq_saida, "\n\ndefine i32 @main() #0 {");
}

/* Percorre alista de variaveis e alloca cada uma no arquivo de saida */
void alloca_variaveis(){

	struct tipo_valor tipo_valor;
	struct variavel* var = inicio_lista_vars;
	
	while(var){

		var->id = ID_REG++;

		switch(var->tipo){
			case NUM_INT:
				insere_alloca_INT_saida(var->id);
				break;
			case NUM_FLOAT:
				insere_alloca_FLOAT_saida(var->id);
				break;
			default:
				printf("\nErro interno: tipo de no desconhecido");
			exit(1);
		}
		var = var->next;
	}
}

/* Insere fim do arquivo de saida */
void insere_fim_saida(){

	fprintf(arq_saida, "\n  ret i32 0\n}");
	fprintf(arq_saida, "\n\ndeclare double @pow(double, double) #1");
	fprintf(arq_saida, "\n\ndeclare i32 @printf(i8*, ...) #2");
}


/*======================== OPERACOES COM INT ===============================*/


/* Insere alocacao de variavel INT no arquivo de saida */
void insere_alloca_INT_saida(int registrador){

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", registrador);
	fprintf(arq_saida, " = alloca i32, align 4");
}

/* Insere instrucao store (valor -> registrador) */
void insere_store_INT_saida(int valor, int registrador){

	fprintf(arq_saida, "\n  store i32 ");
	fprintf(arq_saida, "%d", valor);
	fprintf(arq_saida, ", i32* %%");
	fprintf(arq_saida, "%d", registrador);
	fprintf(arq_saida, ", align 4");
}

/* Insere instrucao store (registrador -> registrador) */
void insere_store_INT_regs_saida(int reg_origem, int reg_destino){

	fprintf(arq_saida, "\n  store i32 %%");
	fprintf(arq_saida, "%d", reg_origem);
	fprintf(arq_saida, ", i32* %%");
	fprintf(arq_saida, "%d", reg_destino);
	fprintf(arq_saida, ", align 4");
}

/* Insere instrucao load para int */
void insere_load_INT_saida(int reg_origem, int reg_destino){

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", reg_destino);
	fprintf(arq_saida, " = load i32, i32* %%");
	fprintf(arq_saida, "%d", reg_origem);
	fprintf(arq_saida, ", align 4");
}

/* Insere instrucao sitofp (converte inteiro para double)*/
void insere_sitofp_saida(int reg_origem, int reg_destino){

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", reg_destino);
	fprintf(arq_saida, " = sitofp i32 %%");
	fprintf(arq_saida, "%d", reg_origem);
	fprintf(arq_saida, " to double");
}

/* Insere instrucao fptosi (converte double para inteiro)*/
void insere_fptosi_saida(int reg_origem, int reg_destino){

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", reg_destino);
	fprintf(arq_saida, " = fptosi double %%");
	fprintf(arq_saida, "%d", reg_origem);
	fprintf(arq_saida, " to i32");
}

/* Insere instrucao fpext (converte float para double)*/
void insere_fpext_saida(int reg_origem, int reg_destino){

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", reg_destino);
	fprintf(arq_saida, " = fpext float %%");
	fprintf(arq_saida, "%d", reg_origem);
	fprintf(arq_saida, " to double");
}

/* Insere instrucao de soma e retorna o registrador que contem o resultado */
int insere_add_INT_saida(int reg1, int reg2){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;
	int novo_reg3 = ID_REG++;
	int reg_resultado = ID_REG++;

	insere_load_INT_saida(reg1, novo_reg1);
	insere_load_INT_saida(reg2, novo_reg2);

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, " = add nsw i32 %%");
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, ", %%");
	fprintf(arq_saida, "%d", novo_reg2);

	insere_alloca_INT_saida(reg_resultado);

	insere_store_INT_regs_saida(novo_reg3, reg_resultado);

	return reg_resultado;
}

/* Insere instrucao de subtracao e retorna o registrador que contem o resultado */
int insere_sub_INT_saida(int reg1, int reg2){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;
	int novo_reg3 = ID_REG++;
	int reg_resultado = ID_REG++;

	insere_load_INT_saida(reg1, novo_reg1);
	insere_load_INT_saida(reg2, novo_reg2);

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, " = sub nsw i32 %%");
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, ", %%");
	fprintf(arq_saida, "%d", novo_reg2);

	insere_alloca_INT_saida(reg_resultado);

	insere_store_INT_regs_saida(novo_reg3, reg_resultado);

	return reg_resultado;
}

/* Insere instrucao de multiplicacao e retorna o registrador que contem o resultado */
int insere_mul_INT_saida(int reg1, int reg2){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;
	int novo_reg3 = ID_REG++;
	int reg_resultado = ID_REG++;

	insere_load_INT_saida(reg1, novo_reg1);
	insere_load_INT_saida(reg2, novo_reg2);

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, " = mul nsw i32 %%");
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, ", %%");
	fprintf(arq_saida, "%d", novo_reg2);

	insere_alloca_INT_saida(reg_resultado);

	insere_store_INT_regs_saida(novo_reg3, reg_resultado);

	return reg_resultado;
}

/* Insere instrucao de divisao e retorna o registrador que contem o resultado */
int insere_div_INT_saida(int reg1, int reg2){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;
	int novo_reg3 = ID_REG++;
	int reg_resultado = ID_REG++;

	insere_load_INT_saida(reg1, novo_reg1);
	insere_load_INT_saida(reg2, novo_reg2);

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, " = sdiv i32 %%");
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, ", %%");
	fprintf(arq_saida, "%d", novo_reg2);

	insere_alloca_INT_saida(reg_resultado);

	insere_store_INT_regs_saida(novo_reg3, reg_resultado);

	return reg_resultado;
}

/* Insere instrucao de potencia e retorna o registrador que contem o resultado */
int insere_pow_INT_saida(int reg1, int reg2){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;
	int novo_reg3 = ID_REG++;
	int novo_reg4 = ID_REG++;
	int novo_reg5 = ID_REG++;
	int novo_reg6 = ID_REG++;
	int reg_resultado = ID_REG++;

	insere_load_INT_saida(reg1, novo_reg1);
	insere_load_INT_saida(reg2, novo_reg2);
	insere_sitofp_saida(novo_reg1, novo_reg3);
	insere_sitofp_saida(novo_reg2, novo_reg4);

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", novo_reg5);
	fprintf(arq_saida, " = call double @pow(double %%");
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, ", double %%");
	fprintf(arq_saida, "%d", novo_reg4);
	fprintf(arq_saida, ") #3");

	insere_fptosi_saida(novo_reg5, novo_reg6);
	insere_alloca_INT_saida(reg_resultado);
	insere_store_INT_regs_saida(novo_reg6, reg_resultado);

	return reg_resultado;
}


/* Insere instruao de atribuicao de int*/
void insere_atribuicao_INT_saida(int reg_origem, int reg_destino){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;

	insere_load_INT_saida(reg_origem, novo_reg1);

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", novo_reg2);
	fprintf(arq_saida, " = add nsw i32 %%");
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, ", 0");

	insere_store_INT_regs_saida(novo_reg2, reg_destino);
}

/* Insere instrucoes para print de um INT*/
void insere_print_INT_saida(int registrador){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;

	insere_load_INT_saida(registrador, novo_reg1);

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", novo_reg2);
	fprintf(arq_saida, " = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %%");
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, ")");
}


/*======================== OPERACOES COM FLOAT ===============================*/


/* Insere alocacao de variavel FLOAT no arquivo de saida */
void insere_alloca_FLOAT_saida(int registrador){

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", registrador);
	fprintf(arq_saida, " = alloca double, align 4");
}

/* Insere instrucao load */
void insere_load_FLOAT_saida(int reg_origem, int reg_destino){

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", reg_destino);
	fprintf(arq_saida, " = load double, double* %%");
	fprintf(arq_saida, "%d", reg_origem);
	fprintf(arq_saida, ", align 4");
}

/* Funcao para converter float em hex 64 bits*/
void FPtoHexString (char *fpString, double fpVal) {
	sprintf (fpString, "0x%"PRIx64, *((uint64_t *) &fpVal));
}

/* Insere instrucao store (valor -> registrador) */
void insere_store_FLOAT_saida(float valor, int registrador){

	char fpString[64];
	char convertido[64];
	double fpVal = 0.0;
	sprintf(convertido, "%f", valor);
	sscanf (convertido, "%lf", &fpVal);
	FPtoHexString(fpString, fpVal);

	fprintf(arq_saida, "\n  store double ");
	fprintf(arq_saida, "%s", fpString);
	fprintf(arq_saida, ", double* %%");
	fprintf(arq_saida, "%d", registrador);
	fprintf(arq_saida, ", align 4");
}

/* Insere instrucao store (registrador -> registrador) */
void insere_store_FLOAT_regs_saida(int reg_origem, int reg_destino){

	fprintf(arq_saida, "\n  store double %%");
	fprintf(arq_saida, "%d", reg_origem);
	fprintf(arq_saida, ", double* %%");
	fprintf(arq_saida, "%d", reg_destino);
	fprintf(arq_saida, ", align 4");
}

/* Insere instrucao de soma e retorna o registrador que contem o resultado */
int insere_add_FLOAT_saida(int reg1, int reg2){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;
	int novo_reg3 = ID_REG++;
	int reg_resultado = ID_REG++;

	insere_load_FLOAT_saida(reg1, novo_reg1);
	insere_load_FLOAT_saida(reg2, novo_reg2);

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, " = fadd double %%");
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, ", %%");
	fprintf(arq_saida, "%d", novo_reg2);

	insere_alloca_FLOAT_saida(reg_resultado);

	insere_store_FLOAT_regs_saida(novo_reg3, reg_resultado);

	return reg_resultado;
}

/* Insere instrucao de subtracao e retorna o registrador que contem o resultado */
int insere_sub_FLOAT_saida(int reg1, int reg2){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;
	int novo_reg3 = ID_REG++;
	int reg_resultado = ID_REG++;

	insere_load_FLOAT_saida(reg1, novo_reg1);
	insere_load_FLOAT_saida(reg2, novo_reg2);

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, " = fsub double %%");
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, ", %%");
	fprintf(arq_saida, "%d", novo_reg2);

	insere_alloca_FLOAT_saida(reg_resultado);

	insere_store_FLOAT_regs_saida(novo_reg3, reg_resultado);

	return reg_resultado;
}

/* Insere instrucao de multiplicacao e retorna o registrador que contem o resultado */
int insere_mul_FLOAT_saida(int reg1, int reg2){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;
	int novo_reg3 = ID_REG++;
	int reg_resultado = ID_REG++;

	insere_load_FLOAT_saida(reg1, novo_reg1);
	insere_load_FLOAT_saida(reg2, novo_reg2);

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, " = fmul double %%");
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, ", %%");
	fprintf(arq_saida, "%d", novo_reg2);

	insere_alloca_FLOAT_saida(reg_resultado);

	insere_store_FLOAT_regs_saida(novo_reg3, reg_resultado);

	return reg_resultado;
}

/* Insere instrucao de divisao e retorna o registrador que contem o resultado */
int insere_div_FLOAT_saida(int reg1, int reg2){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;
	int novo_reg3 = ID_REG++;
	int reg_resultado = ID_REG++;

	insere_load_FLOAT_saida(reg1, novo_reg1);
	insere_load_FLOAT_saida(reg2, novo_reg2);

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, " = fdiv double %%");
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, ", %%");
	fprintf(arq_saida, "%d", novo_reg2);

	insere_alloca_FLOAT_saida(reg_resultado);

	insere_store_FLOAT_regs_saida(novo_reg3, reg_resultado);

	return reg_resultado;
}

/* Insere instrucao de potencia e retorna o registrador que contem o resultado */
int insere_pow_FLOAT_saida(int reg1, int reg2){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;
	int novo_reg3 = ID_REG++;
	int reg_resultado = ID_REG++;

	insere_load_FLOAT_saida(reg1, novo_reg1);
	insere_load_FLOAT_saida(reg2, novo_reg2);

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, " = call double @pow(double %%");
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, ", double %%");
	fprintf(arq_saida, "%d", novo_reg2);
	fprintf(arq_saida, ") #3");

	insere_alloca_FLOAT_saida(reg_resultado);
	insere_store_FLOAT_regs_saida(novo_reg3, reg_resultado);

	return reg_resultado;
}

/* Insere instruao de atribuicao de float*/
void insere_atribuicao_FLOAT_saida(int reg_origem, int reg_destino){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;

	insere_load_FLOAT_saida(reg_origem, novo_reg1);

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", novo_reg2);
	fprintf(arq_saida, " = fadd double %%");
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, ", 0x0000000000000000");

	insere_store_FLOAT_regs_saida(novo_reg2, reg_destino);
}

/* Insere instrucoes para print de um FLOAT*/
void insere_print_FLOAT_saida(int registrador){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;

	insere_load_FLOAT_saida(registrador, novo_reg1);

	fprintf(arq_saida, "\n  %%");
	fprintf(arq_saida, "%d", novo_reg2);
	fprintf(arq_saida, " = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), double %%");
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, ")");
}

struct tipo_registrador percorre_expressao(struct no* no){

	int registrador, reg1, reg2;
	struct tipo_registrador tipo_registrador, tipo_registrador_l, tipo_registrador_r;
	struct tipo_valor tipo_valor;

	switch(no->tipo){
		case ADD:
			tipo_registrador_l = percorre_expressao(no->l);
			tipo_registrador_r = percorre_expressao(no->r);
			switch(tipo_registrador_l.tipo){
				case NUM_INT:
					tipo_registrador.tipo = NUM_INT;
					tipo_registrador.registrador = insere_add_INT_saida(tipo_registrador_l.registrador, tipo_registrador_r.registrador);
					return tipo_registrador;
				case NUM_FLOAT:
					tipo_registrador.tipo = NUM_FLOAT;
					tipo_registrador.registrador = insere_add_FLOAT_saida(tipo_registrador_l.registrador, tipo_registrador_r.registrador);
					return tipo_registrador;
			}
			break;
		case SUB:
			tipo_registrador_l = percorre_expressao(no->l);
			tipo_registrador_r = percorre_expressao(no->r);
			switch(tipo_registrador_l.tipo){
				case NUM_INT:
					tipo_registrador.tipo = NUM_INT;
					tipo_registrador.registrador = insere_sub_INT_saida(tipo_registrador_l.registrador, tipo_registrador_r.registrador);
					return tipo_registrador;
				case NUM_FLOAT:
					tipo_registrador.tipo = NUM_FLOAT;
					tipo_registrador.registrador = insere_sub_FLOAT_saida(tipo_registrador_l.registrador, tipo_registrador_r.registrador);
					return tipo_registrador;				
			}
			break;
		case MUL:
			tipo_registrador_l = percorre_expressao(no->l);
			tipo_registrador_r = percorre_expressao(no->r);
			switch(tipo_registrador_l.tipo){
				case NUM_INT:
					tipo_registrador.tipo = NUM_INT;
					tipo_registrador.registrador = insere_mul_INT_saida(tipo_registrador_l.registrador, tipo_registrador_r.registrador);
					return tipo_registrador;
				case NUM_FLOAT:
					tipo_registrador.tipo = NUM_FLOAT;
					tipo_registrador.registrador = insere_mul_FLOAT_saida(tipo_registrador_l.registrador, tipo_registrador_r.registrador);
					return tipo_registrador;
			}
			break;
		case POW:
			tipo_registrador_l = percorre_expressao(no->l);
			tipo_registrador_r = percorre_expressao(no->r);
			switch(tipo_registrador_l.tipo){
				case NUM_INT:
					tipo_registrador.tipo = NUM_INT;
					tipo_registrador.registrador = insere_pow_INT_saida(tipo_registrador_l.registrador, tipo_registrador_r.registrador);
					return tipo_registrador;
				case NUM_FLOAT:
					tipo_registrador.tipo = NUM_FLOAT;
					tipo_registrador.registrador = insere_pow_FLOAT_saida(tipo_registrador_l.registrador, tipo_registrador_r.registrador);
					return tipo_registrador;
			}
			break;
		case DIV:
			tipo_registrador_l = percorre_expressao(no->l);
			tipo_registrador_r = percorre_expressao(no->r);
			switch(tipo_registrador_l.tipo){
				case NUM_INT:
					tipo_registrador.tipo = NUM_INT;
					tipo_registrador.registrador = insere_div_INT_saida(tipo_registrador_l.registrador, tipo_registrador_r.registrador);
					return tipo_registrador;
				case NUM_FLOAT:
					tipo_registrador.tipo = NUM_FLOAT;
					tipo_registrador.registrador = insere_div_FLOAT_saida(tipo_registrador_l.registrador, tipo_registrador_r.registrador);
					return tipo_registrador;
			}
			break;
		case NUM_INT:
			registrador = ID_REG++;
			insere_alloca_INT_saida(registrador);
			insere_store_INT_saida( ((struct no_folha*)no)->valor.inteiro, registrador);
			tipo_registrador.tipo = NUM_INT;
			tipo_registrador.registrador = registrador;
			return tipo_registrador;
		case NUM_FLOAT:
			registrador = ID_REG++;
			insere_alloca_FLOAT_saida(registrador);
			insere_store_FLOAT_saida( ((struct no_folha*)no)->valor.real, registrador);
			tipo_registrador.tipo = NUM_FLOAT;
			tipo_registrador.registrador = registrador;
			return tipo_registrador;
		case ID:
			tipo_registrador.registrador = get_id_variavel( ((struct no_folha*)no)->valor.string );
			tipo_valor = get_tipo_valor_variavel( ((struct no_folha*)no)->valor.string );
			switch(tipo_valor.tipo){
				case NUM_INT:
					tipo_registrador.tipo = NUM_INT;
					break;
				case NUM_FLOAT:
					tipo_registrador.tipo = NUM_FLOAT;
					break;
			}
			return tipo_registrador;
		default:
			printf("\nErro interno: tipo de no desconhecido");
			exit(1);
	}

}

/*EM CONSTRUCAO*/
void percorre_arvore(struct arvore_sintatica * arvore){

	while(arvore){

		int registrador_retorno;
		struct tipo_valor tipo_valor;
		struct tipo_registrador tipo_registrador;
		struct no* no = arvore->exp;
		char* var_nome;


		switch(arvore->tipo){
			
			case 1:
				tipo_registrador = percorre_expressao(no);
				switch(tipo_registrador.tipo){
					case NUM_INT:
						insere_print_INT_saida(tipo_registrador.registrador);
						break;
					case NUM_FLOAT:
						insere_print_FLOAT_saida(tipo_registrador.registrador);
						break;
				}
				break;
			case 2:
				tipo_registrador = percorre_expressao(no);
				switch(tipo_registrador.tipo){
					case NUM_INT:
						insere_atribuicao_INT_saida(tipo_registrador.registrador, get_id_variavel(arvore->id));
						break;
					case NUM_FLOAT:
						insere_atribuicao_FLOAT_saida(tipo_registrador.registrador, get_id_variavel(arvore->id));
						break;
				}
				break;
		}

		arvore = arvore->next;
	}
}

/* BACKEND */
void backend(struct arvore_sintatica * arvore){

	insere_inicio_saida();
	alloca_variaveis();
	percorre_arvore(arvore);
	insere_fim_saida();
}