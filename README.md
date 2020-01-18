# FrontEnd e motor de execução de um compilador.
Desenvolvido por William Rodrigues. 2019.
Compreende análise léxica, sintática, semântica e motor de execução.

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
Após montar, execute com o seguinte comando:
```
./calc <arquivo_entrada> -o <arquivo_saida>
```

**Obs:** Por padrão o arquivo de entrada para o compilador é setado como "entrada.calc".
Para passar outro arquivo de entrada passe-o como parâmetro de entrada na execução.

## Gerando executável
Após executar o compilador, o mesmo gera um arquivo de saída traduzindo a entradda para código intermediário LLVM.
Gere o executavel com o seguinte comando:
```
clang saida.ll -o <nome_executavel> -lm
```
**Obs:** O compilador faz uso da funcao pow pertencente à biblioteca math, portanto se faz necessário uso da opcao -lm para gerar o executavel com Clang.

## Linguagem implementada:

- palavras reservadas: print
- identificadores: nome de variáveis
- números inteiros
- números reais
- operações: + - * / ^
- símbolos: ( )
- atribuição: =
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

- entrada.calc
Arquivo de entrada para o FrontEnd.

- montar_frontend.sh
Arquivo contendo comandos necessarios para montar o FrontEnd.
