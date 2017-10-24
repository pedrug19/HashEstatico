/* 
 * Projeto:   Trabalho 1 Lab ED2
 * 
 * Autores:   Pedrenrique G. Guimarães
 *            Gustavo Molina
 *            Luis Marcello
 * 
 * Implementação de Hashing estático
 * Compilado e testado em Arch Linux e Windows 10
 */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
//#include "tad.h" //Arquivo com as declarações de estruturas e protÃ³tipos
#include "locale.h"

int m = 0;
/*
 * DECLARAÇÕES DE ESTRUTURAS
 */
//Teremos basicamente 8 structs, separadas em 3 Estruturas independentes, veja:
typedef struct node {
    //é o que a tabela HASH possui
    int matricula;
    int CPF;
    char nome[100];
    char sexo[2];
    struct node *prox;
} ALUNO;

typedef struct nodehash {
    int posi;
    ALUNO *apontaComeco;
    struct nodehash *prox;
} NoDeHash;

typedef struct hash {
    NoDeHash *inicio;
    NoDeHash *fim;
} Hash;

typedef struct registro {
    int id;
} REG;

typedef struct paginas {
    REG reg[3];
    struct paginas *prox;
    struct paginas *ant;
} PAG;

typedef struct lista {
    PAG *inicio;
    PAG *fim;
} LISTA;

typedef struct noDaPool {
    int pin_count;
    int dirty;
    int id;
    int maisAntigo;//use para fazer o LRU
    PAG *pag;
    struct noDaPool *prox;
} NoPool;

typedef struct pool {
    NoPool *inicio;
    NoPool *fim;
} POOL;


/*
 * PROTÓTIPOS DE FUNÇÕES
 */

/*FUNÇÕES PRINCIPAIS*/

POOL *criaPool(POOL *pool) {

    NoPool *primeiro = (NoPool *) malloc(sizeof(NoPool));
    NoPool *segundo = (NoPool *) malloc(sizeof(NoPool));
    NoPool *terceiro = (NoPool *) malloc(sizeof(NoPool));
    NoPool *quarto = (NoPool *) malloc(sizeof(NoPool));

    pool->inicio = primeiro;
    pool->fim = quarto;

    primeiro->dirty = 0;
    primeiro->pin_count = 0;
    primeiro->pag = NULL;
    primeiro->id = 1;
    primeiro->maisAntigo = -1;// se estiver vazia é -1
    primeiro->prox = segundo;

    segundo->dirty = 0;
    segundo->pin_count = 0;
    segundo->pag = NULL;
    segundo->id = 2;
    segundo->maisAntigo = -1;// se estiver vazia é -1
    segundo->prox = terceiro;

    terceiro->dirty = 0;
    terceiro->pin_count = 0;
    terceiro->pag = NULL;
    terceiro->id = 3;
    terceiro->maisAntigo = -1;// se estiver vazia é -1
    terceiro->prox = quarto;

    quarto->dirty = 0;
    quarto->pin_count = 0;
    quarto->pag = NULL;
    quarto->id = 4;
    quarto->maisAntigo = -1;// se estiver vazia é -1
    quarto->prox = primeiro;

    return pool;
}

LISTA *inicializaLRU(LISTA *lru) {
    lru->inicio = NULL;
    lru->fim = NULL;
}

LISTA *funcaoLRU(LISTA *lru, POOL *pool){
    NoPool *aux = pool->inicio;

    if(aux->maisAntigo == -1) {
        //LISTA
    }
}
/* Acho que tem algum erro nessas estruturas*/

/*TODO: RESOLVER ESSES ERROS E DISCUTIR*/

POOL *procuraPool(POOL *pool, Hash *tabela, int id, PAG *pag){
    int achou = 0;
    NoPool *aux = pool->inicio;

    /*POOL precisa da Página*/
    while(aux != NULL) {
        if(aux->id == id) {
            achou = 1;
        }

        aux = aux->prox;
    }

    if(achou) {
        aux->pin_count++;
    } else {
        printf("Não se encontra esse registro\n\n");
    }

}

Hash *cria_Tabela(Hash *tabela, int i) {

    NoDeHash *novo = (NoDeHash *) malloc(sizeof(NoDeHash));
    novo->apontaComeco = NULL;
    novo->posi = i;
    if (tabela->inicio == NULL) {
        tabela->inicio = novo;
        tabela->fim = novo;
        novo->prox = NULL;
    } else {
        tabela->fim->prox = novo;
        tabela->fim = novo;
        novo->prox = NULL;
    }

    return tabela;
}

Hash *insereTabela(Hash *tabela) {
    int CPF, matricula;
    char nome[100], sexo[2];

    printf("\nDigite o nome:\n");
    fflush(stdin);
    scanf("%[^\n]", nome);
    printf("\nDigite o sexo [m] ou [f]:\n");
    fflush(stdin);
    scanf("%[^\n]", sexo);
    printf("\nDigite o número da matrícula:\n");
    scanf("%d", &matricula);
    printf("\nDigite o CPF:\n");
    scanf("%d", &CPF);

    ALUNO *novo = (ALUNO *) malloc(sizeof(ALUNO));
    if (novo == NULL) {
        printf("Memória insuficiente!\n");
        exit(1);
    }

    novo->matricula = matricula;
    novo->CPF = CPF;
    fflush(stdin);
    strcpy(novo->sexo, sexo);
    fflush(stdin);
    strcpy(novo->nome, nome);
    novo->prox = NULL;

    NoDeHash *aux;
    aux = tabela->inicio;
    while (aux->posi != matricula % m) {
        aux = aux->prox;
    }
    if (aux->apontaComeco == NULL) {
        aux->apontaComeco = novo;
        novo->prox = NULL;
    } else {
        ALUNO *aux1;
        aux1 = aux->apontaComeco;
        while (aux1->prox != NULL) {
            aux1 = aux1->prox;
        }
        aux1->prox = novo;
    }

    return tabela;
}


Hash *deletar_Hash(Hash *tabela) {

    int achou = 0, aux_mat;

    printf("\nDigite o número da matrícula:\n");
    scanf("%d", &aux_mat);

    NoDeHash *aux = tabela->inicio;
    while (aux->prox != NULL) {
        if (aux->posi == aux_mat % m) {

            break;
        } else
            aux = aux->prox;
    }

    if (aux->apontaComeco == NULL) {
        printf("\nAluno não encontrado!\n");
        system("pause");
        return tabela;
    } else {
        ALUNO *aux1;
        aux1 = aux->apontaComeco;
        while (aux1 != NULL) {
            if (aux1->matricula == aux_mat) {
                if (aux1 == aux->apontaComeco) {
                    aux->apontaComeco = aux->apontaComeco->prox;
                } else {
                    aux1->prox = aux1->prox->prox;
                }
                achou = 1;
                break;
            } else {
                aux1 = aux1->prox;
            }
        }

        if (achou == 1) {
            printf("\nAluno encontrado e removido.\n");
        }
    }
    return tabela;
}


Hash *alterarTabela(Hash *tabela) {

    int CPF;
    char nome[100], sexo[2];
    int achou = 0, aux_mat;

    printf("\nDigite o número da matrícula do aluno para atualizar os dados:\n");
    scanf("%d", &aux_mat);

    NoDeHash *aux = tabela->inicio;
    while (aux != NULL) {
        if (aux->posi == aux_mat % m) {
            break;
        } else
            aux = aux->prox;
    }

    if (aux->apontaComeco == NULL) {
        achou = 0;
        printf("\nAluno não encontrado!\n");
        return tabela;
    } else {
        ALUNO *aux1;
        aux1 = aux->apontaComeco;
        while (aux1 != NULL) {
            if (aux1->matricula == aux_mat) {
                achou = 1;
                break;
            } else {
                aux1 = aux1->prox;
            }
        }
        if (achou == 0) {
            printf("\nAluno não encontrado!\n");
        } else {
            printf("\nAluno encontrado! \n Entre com os novos dados:\n");
            printf("\nDigite o nome:\n");
            fflush(stdin);
            scanf("%[^\n]", nome);
            printf("\nDigite o sexo [m] ou [f]:\n");
            fflush(stdin);
            scanf("%[^\n]", sexo);
            printf("\nDigite o CPF:\n");
            scanf("%d", &CPF);
            aux1->CPF = CPF;
            fflush(stdin);
            strcpy(aux1->sexo, sexo);
            fflush(stdin);
            strcpy(aux1->nome, nome);
        }
    }
    return tabela;
}

void exibeTabela(Hash *tabela) {

    NoDeHash *aux = tabela->inicio;
    printf("\n \t ---Tamanho da Tabela: %d --- \t\n", m);
    printf("\n--------------------------------------------------------------------------------------------------------------------------------------------\n");
    if (aux == NULL) {
        printf("\nNada ainda!\n");
    } else {
        printf("\n**************************************************************************************************************************\n");
        while (aux != NULL) {
            ALUNO *aux1 = aux->apontaComeco;
            if (aux1 == NULL) {
                printf("   Bucket: %d \n", aux->posi);
            } else {
                printf("   Bucket: %d \n", aux->posi);
                while (aux1 != NULL) {
                    printf("\t");
                    printf(">>Nome:%s ", aux1->nome);
                    aux1 = aux1->prox;

                }
                printf("\n");
                aux1 = aux->apontaComeco;
                while (aux1 != NULL) {
                    printf("\t");
                    printf(">>Sexo:%s ", aux1->sexo);
                    aux1 = aux1->prox;

                }
                printf("\n");
                aux1 = aux->apontaComeco;
                while (aux1 != NULL) {
                    printf("\t");
                    printf(">>Número:%d ", aux1->matricula);
                    aux1 = aux1->prox;

                }
                printf("\n");
                aux1 = aux->apontaComeco;
                while (aux1 != NULL) {
                    printf("\t");
                    printf(">>CPF:%d ", aux1->CPF);
                    aux1 = aux1->prox;

                }
            }
            printf("\n**************************************************************************************************************************\n");
            aux = aux->prox;
        }
    }
    printf("\n--------------------------------------------------------------------------------------------------------------------------------------------\n");
}

Hash *setTabela() {
    Hash *tabela = (Hash *) malloc(sizeof(Hash));

    tabela->fim = NULL;
    tabela->inicio = NULL;

    return tabela;
}

void limpaTabela(Hash *tabela) {
    NoDeHash *aux = tabela->inicio;
    while (tabela->inicio != NULL) {
        tabela->inicio = tabela->inicio->prox;
        free(aux);
        aux = tabela->inicio;
    }
    printf("\nTabela Apagada com sucesso!\n");
}

/*
 * EXECUÇÃO DO PROGRAMA
 */
int main(int argc, char **argv) {
    setlocale(LC_ALL, "Portuguese");
    int i;
    int opc = 0;

    /*Determinando tamanho da Hash*/
    while (m < 1 || m > 10) {
        system("cls");
        printf("Escolha um tamanho para a Hash: \n");
        scanf("%d", &m);
        if (m < 1 || m > 10) {
            printf("\n Entrada incorreta!\n");
        }
    }

    Hash *tabela = setTabela();
    for (i = 0; i < m; i++)
        tabela = cria_Tabela(tabela, i);

    LISTA *disp, *cheia;

    POOL *pool;

    /*Criando a Pool de Buffers*/
    /*
    POOL *pool = (POOL *) malloc(sizeof(POOL));
   	pool = criaPool(pool);
   	*/





    while (opc != -1) {
        system("cls");
        printf("\nSelecione suas opções: \n");
        printf("[1] - Cadastrar aluno:\n");
        printf("[2] - Remover aluno:\n");
        printf("[3] - Alterar informações de aluno:\n");
        printf("[4] - Consultar UM aluno(Na Pool):\n");
        printf("[5] - Exibir a tabela Hash:\n");
        printf("[6] - Exibir a lista de não-cheios:\n");
        printf("[7] - Exibir a lista de cheios:\n");
        printf("[8] - Exibir pool de buffers completa:\n");
        printf("[-1] - Sair:\n");
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                system("cls");
                tabela = insereTabela(tabela);
                //Chamar aqui a função de inserir nas LISTAS! MOLINA
                system("pause");
                break;
            case 2:
                system("cls");
                tabela = deletar_Hash(tabela);
                //Chamar aqui a função de remover nas LISTAS! MOLINA
                system("pause");
                break;
            case 3:
                system("cls");
                tabela = alterarTabela(tabela);
                //Atualizar as listas Aqui! MOLINA
                //Pedren, aqui faça o Dirty=1 para a página atualizada!
                system("pause");
                break;
            case 4:
                system("cls");
                //Pedren, aqui o usuário visualiza apenas um nó da Pool
                system("pause");
                break;
            case 5:
                system("cls");
                exibeTabela(tabela);
                system("pause");
                break;
            case 6:
                system("cls");
                //Molina, funções de exibição da disponivel
                system("pause");
                break;
            case 7:
                system("cls");
                //Molina, funções de exibição da cheio
                system("pause");
                break;
            case 8:
                system("cls");
                //Pedren, criar a exibição da Pool completa aqui
                system("pause");
                break;
            case -1:
                system("cls");
                limpaTabela(tabela);
                //Façam umas funções de limpar
                system("pause");
                break;
            default :
                system("cls");
                printf("\nVocê fez as escolhas erradas! XD \n");
                system("pause");
        }

    }

    return 0;
}
