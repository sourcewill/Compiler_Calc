# Compilador Calc.
Desenvolvido por: William Rodrigues @ UEM 2020.
Compreende análise léxica, sintática, semântica, motor de execução e BackEnd.

## Entrada
- Programa na linguagem Calc. 

## Saidas
- Codigo intermediario llvm
- Executavel

## Pré-requisitos
- Flex
- Bison
- GCC
- Clang

## Montando:

### Montando com shell script:
```
./montar_compilador.sh
```

### Montando manualmente:
```
bison -d sintatico.y
flex lexico.l
gcc -o calc lex.yy.c sintatico.tab.c sintatico.tab.h arvore.c semantico.c backend.c -lfl -lm -w
```

## Executando Compilador:
Apos montar, execute com o seguinte comando:
```
./calc <arquivo_entrada> -o <arquivo_saida>
```

**Obs:** Para informacoes sobre o compilador utilize a opcao -h.
Por padrao o arquivo de entrada para o compilador é setado como "entrada.calc".
Semelhantemente, por padrão, os arquivos de saída são setados como "saida_calc" (executavel) e "saida_calc.ll" (codigo intermediario llvm).


## Linguagem implementada:

- palavras reservadas: print
- identificadores: nome de variáveis
- números inteiros
- números reais
- operações: + - * / ^
- símbolos: ( )
- atribuição: =
- finalizador de comando: ;
- comentários: //

### Regras semânticas:

- Não deve ser possivel realizar operacao entre tipos diferentes (int/real).
- Não deve ser possivel utilizar um identificador ainda nao definido.
- Não deve ser possivel realizar uma divisao por zero.

## Arquivos:

- lexico.l
Estipula as regras para o Analisador Lexico

- sintatico.y
Estipula as regras para o Analisador Sintatico

- arvore.h
Definicoes e cabecalhos para funcoes de manipulacao da Arvore Sintatica Abstrata

- arvore.c
Funcoes de manipulacao da Arvore Sintatica Abstrata

- semantico.h
Definicoes e cabecalhos para funcoes de manipulacao do Analisador Semantico

- semantico.c
Funcoes de manipulacao do Analisador Semantico

- backend.h
Definicoes e cabecalhos para funcoes do BackEnd.

- backend.c
Funcoes responaveis por gerar o arquivo de saida com codigo intermediario LLVM.

- entrada.calc
Arquivo de entrada para o Compilador.

- montar_compilador.sh
Arquivo contendo comandos necessarios para montar o compilador.
