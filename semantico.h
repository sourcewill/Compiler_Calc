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

/* Estrutura para encapsular dois retornos*/
struct tipo_registrador{
	int tipo;
	int registrador;
};


/* Valida uma expressao */
struct tipo_valor valida_exp(struct no *);

/* Valida uma atribuicao */
void valida_atr(char*, struct no *);

/* Obtem tipo de uma variavel, buscando por seu nome */
struct tipo_valor get_tipo_valor_variavel(char*);

/* Altera o valor de uma variavel inteira, buscando por seu nome */
void set_valor_variavel_int(char*, int);

/* Altera o valor de uma variavel real, buscando por seu nome */
void set_valor_variavel_real(char*, float);

/* Analisador semantico */
void analise_semantica(struct arvore_sintatica *);

/* Insere nova variavel na lista*/
struct variavel* nova_variavel(int, char*, union numero, struct variavel*);

/* Backend */
void Backend(struct arvore_sintatica *);
