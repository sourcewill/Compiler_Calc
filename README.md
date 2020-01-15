# FrontEnd e motor de execução de um compilador.
Desenvolvido por William Rodrigues. 2019.
Compreende análise léxica, sintática, semântica e motor de execução.

## Pré-requisitos
- Flex
- Bison
- GCC

## Montando:

### Montando com shell script:
```
./montar_frontend.sh
```

### Montando manualmente:
```
bison -d sintatico.y
flex lexico.l -o lex.c
gcc -o calculadora lex.c sintatico.tab.c sintatico.tab.h arvore.c semantico.c -lfl -lm -w
```

## Executando:
Após montar, execute com o seguinte comando:
```
./calculadora <arquivo_entrada>
```

**Obs:** Por padrão o arquivo de entrada para o compilador é setado como "entrada.calc".
Para passar outro arquivo de entrada passe-o como parâmetro de entrada na execução.

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
