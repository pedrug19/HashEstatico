
/*
 * Projeto:   Trabalho 1 Lab ED2
 * Autores:   Pedrenrique G. Guimarães
 *            Gustavo Molina
 *            Luis Marcello
 * Implementação de Hash
 * Compilado e testado em Arch Linux e Windows 10
 */

#ifndef TAD_H
#define TAD_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * DECLARAÇÕES DE ESTRUTURAS
 */
typedef struct node {
    /*TODO: ADICIONAR POSSIVEIS DADOS*/
    int matricula;
    char nome[100];
    int CPF;
    struct node *prox;
} NODE;

typedef struct hash {
    struct node *inicio;
    int contador;
} Hash;

typedef struct paginas {
    int pin_count;
    int dirty;
    struct hash *inicio;
    struct paginas *prox;
    struct paginas *ant;
} PAG;

typedef struct quadros {
    struct paginas *inicio;
    struct quadros *prox;
    struct quadros *ant;
} QUAD;

typedef struct pool {
    struct quadros *livres;
    struct quadros *usados;
} POOL;

/*
 * PROTÓTIPOS DE FUNÇÕES
 */
NODE *criar_No(int matricula, char *nome, int CPF);
Hash *cria_Tabela(int m, Hash tabela[]);
PAG *cria_Pagina(int m, Hash tabela[]);
void LRU();
Hash *inserir_Hash(Hash tabela[], int m);
Hash *deletar_Hash(Hash tabela[], int matricula, int m);
Hash *modifica_Hash(Hash tabela[], int m);
int procura_Hash(Hash tabela[], int matricula, int m, int modifica);
void imprime_No(NODE *no);
void imprime_Hash();


#ifdef __cplusplus
}
#endif

#endif /* TAD_H */
