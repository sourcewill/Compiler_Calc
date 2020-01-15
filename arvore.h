/* Nos da arvore sintatica */
struct no {
	int tipo;
	int linha;
	int coluna;
	struct no *l;
	struct no *r;
};

union terminal{
	int inteiro;
	float real;
	char* string;
};

struct no_folha {
	int tipo;
	int linha;
	int coluna;
	union terminal valor;
};

/* Arvore sintatica (Lista) */
struct arvore_sintatica {
	int tipo;
	struct no *exp;
	char* id;
	struct arvore_sintatica* next;
};

/* Criar novo No */
struct no *novo_no(int, struct no *, struct no *, int, int);
struct no *novo_no_folha_int(int, int, int);
struct no *novo_no_folha_float(float, int, int);
struct no *novo_no_folha_id(char*, int, int);

/* Funcoes sobre Arvore Sintatica */
struct arvore_sintatica * novo_arvore_sintatica(int, struct no*, char*, struct arvore_sintatica*);
struct arvore_sintatica * get_inicio_lista_arvore();

/* Print No */
void print_no(struct no *);

/* Global */
struct arvore_sintatica* inicio_lista_arvore;

