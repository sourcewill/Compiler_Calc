#!/bin/bash

# Montando Compilador Calc

bison -d sintatico.y #Executa bison
flex lexico.l #Executa flex
gcc -o calc lex.yy.c sintatico.tab.c sintatico.tab.h arvore.c semantico.c backend.c -lfl -lm -w #Compila o arquivo c gerado pelo flex

echo $'\nCompilador para linguagem Calc montado.\nExecute com o seguinte comando:\n./calc <arquivo_entrada> -o <arquivo_saida>\n'
