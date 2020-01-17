#!/bin/bash

# Montando FrontEnd para calculadora

bison -d sintatico.y #Executa bison
flex lexico.l #Executa flex
gcc -o calculadora lex.yy.c sintatico.tab.c sintatico.tab.h arvore.c semantico.c -lfl -lm -w #Compila o arquivo c gerado pelo flex

echo $'\nCompilador para Calculadora montado.\nExecute com o seguinte comando:\n./calculadora <arquivo_entrada> <arquivo_saida>\n'

echo $'\nExecutando compilador...'
./calculadora entrada.calc saida.ll

echo $'\nCompilando codigo intermediario gerado...'
clang -o saida saida.ll -w -lm

echo $'\nExecutando saida...\n'
./saida
