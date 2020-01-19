/* Desenvolvido por: William Rodrigues @ UEM 2020. */

/* Definicao de numero */
union numero{
	int inteiro;
	float real;
};

/* Definicao de variavel */
struct variavel{
	int tipo;
	int id;
	char* nome;
	union numero valor;
	struct variavel * next;
};

/* Estrutura para encapsular dois retornos*/
struct tipo_valor{
	int tipo;
	union numero valor;
};


/* Valida uma expressao */
struct tipo_valor valida_exp(struct no *);

/* Obtem tipo de uma variavel, buscando por seu nome */
struct tipo_valor get_tipo_valor_variavel(char*);
