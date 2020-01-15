#!/bin/bash

# Montando FrontEnd para calculadora

bison -d sintatico.y #Executa bison
flex lexico.l #Executa flex
gcc -o calculadora lex.yy.c sintatico.tab.c sintatico.tab.h arvore.c semantico.c -lfl -lm -w #Compila o arquivo c gerado pelo flex

echo $'\nFrontEnd para Calculadora montado.\nExecute com o seguinte comando:\n./calculadora <arquivo_entrada>\n'
