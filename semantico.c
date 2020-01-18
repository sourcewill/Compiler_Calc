#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include "arvore.h"
#include "semantico.h"
#include "sintatico.tab.h"

/* GLOBAL */
struct variavel* inicio_lista_vars = NULL;
struct variavel* fim_lista_vars = NULL;
extern FILE* arq_saida;
int ID_REG = 1;


/* Insere inicio do arquivo de saida */
void insere_inicio_saida(){

	char* comentario = "; Codigo LLVM intermediario gerado pelo compilador Calc.\n; Desenvolvido por William Rodrigues.";
	char* print_int = "\n\n@.str = private unnamed_addr constant [4 x i8] c\"%d\\0A\\00\", align 1";
	char* print_float = "\n@.str.1 = private unnamed_addr constant [6 x i8] c\"%.2f\\0A\\00\", align 1";
	char* inicio_main = "\n\ndefine i32 @main() #0 {";

	fprintf(arq_saida, "%s", comentario);
	fprintf(arq_saida, "%s", print_int);
	fprintf(arq_saida, "%s", print_float);
	fprintf(arq_saida, "%s", inicio_main);
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

	char* fim_main = "\n  ret i32 0\n}";
	char* declare_pow = "\n\ndeclare double @pow(double, double) #1";
	char* declare_printf = "\n\ndeclare i32 @printf(i8*, ...) #2";

	fprintf(arq_saida, "%s", fim_main);
	fprintf(arq_saida, "%s", declare_pow);
	fprintf(arq_saida, "%s", declare_printf);
}

/* Insere alocacao de variavel INT no arquivo de saida */
void insere_alloca_INT_saida(int registrador){
	char* alloca_inicio = "\n  %";
	char* alloca_fim = " = alloca i32, align 4";

	fprintf(arq_saida, "%s", alloca_inicio);
	fprintf(arq_saida, "%d", registrador);
	fprintf(arq_saida, "%s", alloca_fim);
}

/* Insere instrucao store (valor -> registrador) */
void insere_store_INT_saida(int valor, int registrador){
	char* store_inicio = "\n  store i32 ";
	char* store_meio = ", i32* %";
	char* store_fim = ", align 4";

	fprintf(arq_saida, "%s", store_inicio);
	fprintf(arq_saida, "%d", valor);
	fprintf(arq_saida, "%s", store_meio);
	fprintf(arq_saida, "%d", registrador);
	fprintf(arq_saida, "%s", store_fim);
}

/* Insere instrucao store (registrador -> registrador) */
void insere_store_INT_regs_saida(int reg_origem, int reg_destino){
	char* store_inicio = "\n  store i32 %";
	char* store_meio = ", i32* %";
	char* store_fim = ", align 4";

	fprintf(arq_saida, "%s", store_inicio);
	fprintf(arq_saida, "%d", reg_origem);
	fprintf(arq_saida, "%s", store_meio);
	fprintf(arq_saida, "%d", reg_destino);
	fprintf(arq_saida, "%s", store_fim);
}

/* Insere instrucao load para int */
void insere_load_INT_saida(int reg_origem, int reg_destino){
	char* load_inicio = "\n  %";
	char* load_meio = " = load i32, i32* %";
	char* load_fim = ", align 4";

	fprintf(arq_saida, "%s", load_inicio);
	fprintf(arq_saida, "%d", reg_destino);
	fprintf(arq_saida, "%s", load_meio);
	fprintf(arq_saida, "%d", reg_origem);
	fprintf(arq_saida, "%s", load_fim);
}

/* Insere instrucao sitofp (converte inteiro para double)*/
void insere_sitofp_saida(int reg_origem, int reg_destino){
	char* sitofp_inicio = "\n  %";
	char* sitofp_meio = " = sitofp i32 %";
	char* sitofp_fim = " to double";

	fprintf(arq_saida, "%s", sitofp_inicio);
	fprintf(arq_saida, "%d", reg_destino);
	fprintf(arq_saida, "%s", sitofp_meio);
	fprintf(arq_saida, "%d", reg_origem);
	fprintf(arq_saida, "%s", sitofp_fim);
}

/* Insere instrucao fptosi (converte double para inteiro)*/
void insere_fptosi_saida(int reg_origem, int reg_destino){
	char* fptosi_inicio = "\n  %";
	char* fptosi_meio = " = fptosi double %";
	char* fptosi_fim = " to i32";

	fprintf(arq_saida, "%s", fptosi_inicio);
	fprintf(arq_saida, "%d", reg_destino);
	fprintf(arq_saida, "%s", fptosi_meio);
	fprintf(arq_saida, "%d", reg_origem);
	fprintf(arq_saida, "%s", fptosi_fim);
}

/* Insere instrucao fpext (converte float para double)*/
void insere_fpext_saida(int reg_origem, int reg_destino){
	char* fpext_inicio = "\n  %";
	char* fpext_meio = " = fpext float %";
	char* fpext_fim = " to double";

	fprintf(arq_saida, "%s", fpext_inicio);
	fprintf(arq_saida, "%d", reg_destino);
	fprintf(arq_saida, "%s", fpext_meio);
	fprintf(arq_saida, "%d", reg_origem);
	fprintf(arq_saida, "%s", fpext_fim);
}

/* Insere instrucao de soma e retorna o registrador que contem o resultado */
int insere_add_INT_saida(int reg1, int reg2){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;
	int novo_reg3 = ID_REG++;
	int reg_resultado = ID_REG++;

	insere_load_INT_saida(reg1, novo_reg1);
	insere_load_INT_saida(reg2, novo_reg2);

	char* add_inicio = "\n  %";
	char* add_meio = " = add nsw i32 %";
	char* add_fim = ", %";

	fprintf(arq_saida, "%s", add_inicio);
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, "%s", add_meio);
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, "%s", add_fim);
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

	char* sub_inicio = "\n  %";
	char* sub_meio = " = sub nsw i32 %";
	char* sub_fim = ", %";

	fprintf(arq_saida, "%s", sub_inicio);
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, "%s", sub_meio);
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, "%s", sub_fim);
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

	char* mul_inicio = "\n  %";
	char* mul_meio = " = mul nsw i32 %";
	char* mul_fim = ", %";

	fprintf(arq_saida, "%s", mul_inicio);
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, "%s", mul_meio);
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, "%s", mul_fim);
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

	char* mul_inicio = "\n  %";
	char* mul_meio = " = sdiv i32 %";
	char* mul_fim = ", %";

	fprintf(arq_saida, "%s", mul_inicio);
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, "%s", mul_meio);
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, "%s", mul_fim);
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

	char* pow_inicio = "\n  %";
	char* pow_meio = " = call double @pow(double %";
	char* pow_meio2 = ", double %";
	char* pow_fim = ") #3";

	fprintf(arq_saida, "%s", pow_inicio);
	fprintf(arq_saida, "%d", novo_reg5);
	fprintf(arq_saida, "%s", pow_meio);
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, "%s", pow_meio2);
	fprintf(arq_saida, "%d", novo_reg4);
	fprintf(arq_saida, "%s", pow_fim);

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

	char* add_inicio = "\n  %";
	char* add_meio = " = add nsw i32 %";
	char* add_fim = ", 0";

	fprintf(arq_saida, "%s", add_inicio);
	fprintf(arq_saida, "%d", novo_reg2);
	fprintf(arq_saida, "%s", add_meio);
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, "%s", add_fim);

	insere_store_INT_regs_saida(novo_reg2, reg_destino);
}

/* Insere instrucoes para print de um INT*/
void insere_print_INT_saida(int registrador){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;

	insere_load_INT_saida(registrador, novo_reg1);

	char* call_inicio = "\n  %";
	char* call_meio = " = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %";
	char* call_fim = ")";

	fprintf(arq_saida, "%s", call_inicio);
	fprintf(arq_saida, "%d", novo_reg2);
	fprintf(arq_saida, "%s", call_meio);
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, "%s", call_fim);
}


/*======================== FLOATS ===============================*/

/* Insere alocacao de variavel FLOAT no arquivo de saida */
void insere_alloca_FLOAT_saida(int registrador){
	char* alloca_inicio = "\n  %";
	char* alloca_fim = " = alloca double, align 4";

	fprintf(arq_saida, "%s", alloca_inicio);
	fprintf(arq_saida, "%d", registrador);
	fprintf(arq_saida, "%s", alloca_fim);
}

/* Insere instrucao load */
void insere_load_FLOAT_saida(int reg_origem, int reg_destino){
	char* load_inicio = "\n  %";
	char* load_meio = " = load double, double* %";
	char* load_fim = ", align 4";

	fprintf(arq_saida, "%s", load_inicio);
	fprintf(arq_saida, "%d", reg_destino);
	fprintf(arq_saida, "%s", load_meio);
	fprintf(arq_saida, "%d", reg_origem);
	fprintf(arq_saida, "%s", load_fim);
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

	char* store_inicio = "\n  store double ";
	char* store_meio = ", double* %";
	char* store_fim = ", align 4";

	fprintf(arq_saida, "%s", store_inicio);
	fprintf(arq_saida, "%s", fpString);
	fprintf(arq_saida, "%s", store_meio);
	fprintf(arq_saida, "%d", registrador);
	fprintf(arq_saida, "%s", store_fim);
}

/* Insere instrucao store (registrador -> registrador) */
void insere_store_FLOAT_regs_saida(int reg_origem, int reg_destino){
	char* store_inicio = "\n  store double %";
	char* store_meio = ", double* %";
	char* store_fim = ", align 4";

	fprintf(arq_saida, "%s", store_inicio);
	fprintf(arq_saida, "%d", reg_origem);
	fprintf(arq_saida, "%s", store_meio);
	fprintf(arq_saida, "%d", reg_destino);
	fprintf(arq_saida, "%s", store_fim);
}

/* Insere instrucao de soma e retorna o registrador que contem o resultado */
int insere_add_FLOAT_saida(int reg1, int reg2){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;
	int novo_reg3 = ID_REG++;
	int reg_resultado = ID_REG++;

	insere_load_FLOAT_saida(reg1, novo_reg1);
	insere_load_FLOAT_saida(reg2, novo_reg2);

	char* add_inicio = "\n  %";
	char* add_meio = " = fadd double %";
	char* add_fim = ", %";

	fprintf(arq_saida, "%s", add_inicio);
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, "%s", add_meio);
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, "%s", add_fim);
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

	char* sub_inicio = "\n  %";
	char* sub_meio = " = fsub double %";
	char* sub_fim = ", %";

	fprintf(arq_saida, "%s", sub_inicio);
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, "%s", sub_meio);
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, "%s", sub_fim);
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

	char* mul_inicio = "\n  %";
	char* mul_meio = " = fmul double %";
	char* mul_fim = ", %";

	fprintf(arq_saida, "%s", mul_inicio);
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, "%s", mul_meio);
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, "%s", mul_fim);
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

	char* mul_inicio = "\n  %";
	char* mul_meio = " = fdiv double %";
	char* mul_fim = ", %";

	fprintf(arq_saida, "%s", mul_inicio);
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, "%s", mul_meio);
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, "%s", mul_fim);
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

	char* pow_inicio = "\n  %";
	char* pow_meio = " = call double @pow(double %";
	char* pow_meio2 = ", double %";
	char* pow_fim = ") #3";

	fprintf(arq_saida, "%s", pow_inicio);
	fprintf(arq_saida, "%d", novo_reg3);
	fprintf(arq_saida, "%s", pow_meio);
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, "%s", pow_meio2);
	fprintf(arq_saida, "%d", novo_reg2);
	fprintf(arq_saida, "%s", pow_fim);

	insere_alloca_FLOAT_saida(reg_resultado);
	insere_store_FLOAT_regs_saida(novo_reg3, reg_resultado);

	return reg_resultado;
}

/* Insere instruao de atribuicao de float*/
void insere_atribuicao_FLOAT_saida(int reg_origem, int reg_destino){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;

	insere_load_FLOAT_saida(reg_origem, novo_reg1);

	char* add_inicio = "\n  %";
	char* add_meio = " = fadd double %";
	char* add_fim = ", 0x0000000000000000";

	fprintf(arq_saida, "%s", add_inicio);
	fprintf(arq_saida, "%d", novo_reg2);
	fprintf(arq_saida, "%s", add_meio);
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, "%s", add_fim);

	insere_store_FLOAT_regs_saida(novo_reg2, reg_destino);
}

/* Insere instrucoes para print de um FLOAT*/
void insere_print_FLOAT_saida(int registrador){

	int novo_reg1 = ID_REG++;
	int novo_reg2 = ID_REG++;

	insere_load_FLOAT_saida(registrador, novo_reg1);

	char* call_inicio = "\n  %";
	char* call_meio = " = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), double %";
	char* call_fim = ")";

	fprintf(arq_saida, "%s", call_inicio);
	fprintf(arq_saida, "%d", novo_reg2);
	fprintf(arq_saida, "%s", call_meio);
	fprintf(arq_saida, "%d", novo_reg1);
	fprintf(arq_saida, "%s", call_fim);
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

/*========================================================================================================*/
/*========================================================================================================*/


/* Aloca nova variavel na lista */
struct variavel* nova_variavel(int tipo, char* nome, union numero valor, struct variavel* anterior){

	struct variavel * nova = malloc(sizeof(struct variavel));
	nova->tipo = tipo;
	nova->nome = nome;
	switch(tipo){
		case NUM_INT:
			nova->valor.inteiro = valor.inteiro;
			//insere_alloca_INT_saida();
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


/* Obtem o id de uma variavel, buscando por seu nome */
int get_id_variavel(char* nome){

	struct variavel* var = inicio_lista_vars;

	while(var){
		if( strcmp(nome, var->nome) == 0){
			return var->id;
		}
		var = var->next;
	}
	return -1;
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

	struct tipo_valor tipo_valor;

	while(arvore){

		switch(arvore->tipo){
			case 1:
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
			case 2:
				valida_atr(arvore->id, arvore->exp);
				break;
		}
		arvore = arvore->next;
	}
}

