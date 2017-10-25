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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

int m = 0, Ncheio = 0, deletar;
int contadorPool = 4;

/*
 * DECLARAÇÕES DE ESTRUTURAS
 */
typedef struct node {
    //é o que a tabela HASH possui
    int matricula;
    int CPF;
    char nome[100];
    char sexo[100];
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

typedef struct paginas {
    ALUNO reg[3];
    struct paginas *prox;
    struct paginas *ant;
    int posi;
    int quantidade;
} PAG;

typedef struct lista {
    PAG *inicio;
    PAG *fim;
} LISTA;

typedef struct noDaPool {
    int pin_count;
    int dirty;
    int id;
    PAG *pag;
    struct noDaPool *prox;
} NoPool;

typedef struct alglru{
    NoPool *no;
    struct alglru *prox;
} LRU;

typedef struct cabecalho {
    LISTA *disponivel;
    LISTA *cheio;
} CABECALHO;

ALUNO *alterado;

/*VARIAVEIS CONTROLE DA POOL*/
NoPool *inicioPool = NULL;
NoPool *fimPool = NULL;

/*LRU*/
LRU *inicioLRU = NULL;
LRU *fimLRU = NULL;

/*FUNÇÕES PRINCIPAIS*/

void criaPool() {

    for (int i = 0; i < 4; i++) {
        NoPool *novo = (NoPool *) malloc(sizeof(NoPool));
        if (novo == NULL) {
            printf("Erro de memoria!\n");
            exit(1);
        }

        LRU *novoLRU = (LRU *) malloc(sizeof(LRU));
        if (novo == NULL) {
            printf("Erro de memoria!\n");
            exit(1);
        }

        novo->dirty = 0;
        novo->pin_count = 0;
        novo->pag = NULL;
        novo->id = i+1;
        novo->prox = NULL;
        novoLRU->no = novo;
        novoLRU->prox = NULL;

        if (inicioPool == NULL) {
            inicioPool = novo;
            inicioLRU = novoLRU;
            fimPool = novo;
            fimLRU = novoLRU;
        } else {
            fimPool->prox = novo;
            fimLRU->prox = novoLRU;
            fimPool = novo;
            fimLRU = novoLRU;
        }
    }
}

LRU *insereLRU(LRU *novo) {
    if( inicioLRU == NULL) {
        inicioLRU = novo;
        fimLRU = novo;
    } else {
        fimLRU->prox = novo;
        fimLRU = novo;
    }
}

NoPool *procuraeRemove(NoPool *remove) {
    LRU *aux = inicioLRU;
    LRU *aux2 = inicioLRU;
    int achou = 0;

    while(aux != NULL){
        if(aux->no == remove) {
            if(aux == inicioLRU) {
                aux-> no = removeLRU();
                break;
            }

            if(aux == fimLRU) {
                while(aux2->prox != fimLRU) {
                    fimLRU = aux2->prox;
                    aux2->prox = NULL;
                }
            }
        }

        aux = aux->prox;
    }
}

NoPool *removeLRU() {
    LRU *aux = inicioLRU;
    if(inicioLRU == NULL) {
        printf("\nNão há elementos a serem removidos no LRU\n");
    } else {
        if(fimLRU == inicioLRU) {
            fimLRU = inicioLRU->prox;
        }
        inicioLRU = inicioLRU->prox;
    }

    return aux->no;
}

void exibePool() {
    NoPool *aux = inicioPool;
    printf("\n---- Exibindo dados da Pool: ----\n");
    if (aux == NULL) {
        printf("Não há dados na pool!\n\n");
    }
    while (aux != NULL) {
        printf("\n-------------------------------------------------------------------------------------------------\n");
        printf("\nQUADRO %d: \n", aux->id);
        printf(">> PIN_COUNT: %d", aux->pin_count);
        printf(">> DIRTY: %d", aux->dirty);
        printf(">> PÁGINA DO QUADRO: \n");
        printf(">>>> ID: %d\n", aux->pag->posi);
        printf("\n-------------------------------------------------------------------------------------------------\n\n");
        aux = aux->prox;
    }
}

/*LRU, EXIBIR E INSERIR NA POOL*/
NoPool *inserePool(int matricula, CABECALHO *listas) {

    /*Apontadores*/
    NoPool *aux = inicioPool;
    PAG *aux2 = listas->disponivel->inicio;

    int mudanca = 0; //verifica se ja mudou para listas disponiveis ou cheias
    int achou = 0; //Controla se registro foi encontrado na Pool
    int i;
    int pin = 0; //variavel para controlar LRU

    /* Verificando se o objeto esta contido em alguma pagina */
    while (aux != NULL && !achou) {
        if (aux->pag != NULL && !achou) { /* Verificando a presença de página no quadro */
            if (aux->pag->posi == matricula % m) { /* Comparando a posição com a matricula */
                for (i = 0; i < 3; i++) {
                    if (aux->pag->reg[i].matricula == matricula) { /* Se encontrar o registro, mostrará */
                        printf("\nRegistro encontrado!\n");
                        printf("\nIncrementando pin_count!\n");
                        if(aux->pin_count == 0) {
                            aux = procuraeRemove(aux);
                        }
                        aux->pin_count++;
                        printf("\nRegistro:\n");
                        printf("---------------------------------------------------------------------------------------");
                        printf(">> NOME: %s\n", aux->pag->reg[i].nome);
                        printf(">> SEXO: %s\n", aux->pag->reg[i].sexo);
                        printf(">> CPF: %d\n", aux->pag->reg[i].CPF);
                        printf(">> MATRICULA: %d", aux->pag->reg[i].matricula);
                        printf("---------------------------------------------------------------------------------------");
                        achou = 1; //condicao de parada
                        pin = 1;
                        break;
                    } // fim do if aux->pag->reg[i].matricula
                } // fim do for
            } //fim do if (aux->pag->posi == matricula % m)
        } else { //CONDIÇÃO PARA A PAGINA NULA
            if (aux->pag == NULL) { /*Se a página for nula no quadro*/ //CHECAGEM EXTRA
                if (aux2 == NULL) { /* Verificando se a lista de disponíveis é nula */
                    mudanca = 1; /* Para garantir que não troquemos duas vezes, utilizamos essa variavel de controle */
                    aux2 = listas->cheio->inicio; /* Trocando para listas cheias para adicionar */
                    if (aux2 == NULL) { /* Se as duas listas forem vazias, não há registros para adicionar,
                                        * então a pool retorna intacta */
                        printf("\nNão existem registros!\n");
                        return inicioPool; //Retornando intacta
                    }
                } // fim do if(aux2 == NULL)

                /* PROCURANDO NAS LISTAS */
                while (aux2 != NULL && achou == 0) {
                    if (aux2->posi == matricula % m) {
                        for (i = 0; i < 3; i++) { /* Apontando para a página */
                            if (aux2->reg[i].matricula == matricula) {
                                aux->pag = aux2;
                                aux->pin_count++;
                                achou = 1;
                                break;
                            } //fim do if interno
                        } //fim do for
                    } // fim do if

                    aux2 = aux2->prox; //avançando na lista
                } //fim do while(aux2 != NULL)

                if (mudanca == 0) { /* Verificando se ainda não procurou na lista cheia */
                    aux2 = listas->cheio->inicio; //alterando para a lista cheia

                    while (aux2 != NULL && achou == 0) {
                        if (aux2->posi == matricula % m) {
                            for (i = 0; i < 3; i++) { /* Apontando para a página */
                                if (aux2->reg[i].matricula == matricula) {
                                    aux->pag = aux2;
                                    aux->pin_count++;
                                    achou = 1;
                                    break;
                                } //fim do if interno
                            } //fim do for
                        } // fim do if

                        aux2 = aux2->prox;
                    } //fim do while
                } //FIM DO if(mudanca ==0)
            } // FIM DO if(pag == NULL)
        }

        /* Movendo para frente na Pool*/
        aux = aux->prox;
    }

    if (!achou) {
        aux = inicioPool;
        aux2 = listas->disponivel->inicio;
        mudanca = 0;
    }

    if (aux2 == NULL) { /* Verificando se a lista de disponíveis é nula */
        mudanca = 1; /* Para garantir que não troquemos duas vezes, utilizamos essa variavel de controle */
        aux2 = listas->cheio->inicio; /* Trocando para listas cheias para adicionar */
        if (aux2 == NULL) { /* Se as duas listas forem vazias, não há registros para adicionar,
                                        * então a pool retorna intacta */
            printf("\nNão existem registros!\n");
            return inicioPool; //Retornando intacta
        }
    } // fim do if(aux2 == NULL)

    /* Verificando pin_count dessa vez */
    while(aux != NULL && pin == 0) {
        if(aux->pin_count == 0) {
            pin = 1;
            LRU *novo = (LRU *) malloc(sizeof(LRU));
            novo->no = aux;
            novo->prox = NULL;
            inicioLRU = insereLRU(novo);

            /* PROCURANDO NAS LISTAS */
            while (aux2 != NULL && achou == 0) {
                if (aux2->posi == matricula % m) {
                    for (i = 0; i < 3; i++) { /* Apontando para a página */
                        if (aux2->reg[i].matricula == matricula) {
                            aux->pag = aux2;
                            aux->pin_count++;
                            achou = 1;
                            break;
                        } //fim do if interno
                    } //fim do for
                } // fim do if

                aux2 = aux2->prox; //avançando na lista
            } //fim do while(aux2 != NULL)

            if (mudanca == 0) { /* Verificando se ainda não procurou na lista cheia */
                aux2 = listas->cheio->inicio; //alterando para a lista cheia

                while (aux2 != NULL && achou == 0) {
                    if (aux2->posi == matricula % m) {
                        for (i = 0; i < 3; i++) { /* Apontando para a página */
                            if (aux2->reg[i].matricula == matricula) {
                                aux->pag = aux2;
                                aux->pin_count++;
                                achou = 1;
                                break;
                            } //fim do if interno
                        } //fim do for
                    } // fim do if

                    aux2 = aux2->prox;
                } //fim do while
            } //FIM DO if(mudanca ==0)

        }

        aux = aux->prox;
    }//FIM DO WHILE(AUX!=NULL)

    /* PERCORREMOS TODOS OS QUADROS, TODAS AS LISTAS E AINDA NÃO ACHAMOS O QUE QUERÍAMOS */
    if (!achou) {
        printf("\nEsse dado não está carregado na lista ou não existe na tabela.\n");
    }

    if(!pin) {
        printf("\nBuffers sendo utilizados! Decremente o pin_count de um deles para que possa ser usado!\n");
    }

    return inicioPool;
}

void limpaPool() {
    NoPool *aux = inicioPool;
    fimPool = NULL;

    while (aux != NULL) {
        inicioPool = aux->prox;
        free(aux);
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

NoPool *deletaPool(){
    NoPool *aux = inicioPool;
    int achou = 0;

    while(aux != NULL) {
        if(aux->pag->posi == deletar % m) {
            for(int i = 0; i < 3; i++) {
                if(aux->pag->reg[i].matricula == deletar) {
                    aux->pag = NULL;
                    aux->pin_count = 0;
                    aux->dirty = 0;
                    printf("\nRemovido da pool!\n");
                    break;
                }
            }
        }

        aux = aux->prox;
    }

    return inicioPool;
}


Hash *deletar_Hash(Hash *tabela) {

    int achou = 0, aux_mat;

    printf("\nDigite o número da matrícula:\n");
    scanf("%d", &aux_mat);
    deletar = aux_mat;

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

void decrementaPin(int pin) {
    int feito = 0;
    NoPool *aux = inicioPool;

    while (aux != NULL) {
        if (aux->id == pin) {
            if (aux->pin_count > 0) {
                aux->pin_count--;
                if(aux->pin_count == 0) { //insere o quadro no lru
                    LRU *novo = (LRU *) malloc(sizeof(LRU));
                    novo->no = aux;
                    novo->prox = NULL;
                    inicioLRU = insereLRU(novo);
                }
                feito = 1;
                break;
            } else {
                printf("\nPin_count já é 0!\n");
                break;
            }
        } else {
            aux = aux->prox;
        }
    }

    if (feito) {
        printf("\nPin_count decrementado com sucesso!\n");
    } else {
        printf("\nNão foi possivel decrementar o pin_count!\n");
    }
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

            alterado = aux1;
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

PAG *criaPagina(int posi) {
    int i;

    PAG *aux_pag = (PAG *) malloc(sizeof(PAG));
    if (aux_pag == NULL) {
        printf("Memória insuficiente!\n");
        exit(1);
    }

    LRU *novoLRU = (LRU *) malloc(sizeof(LRU));
    aux_pag->prox = NULL;
    aux_pag->ant = NULL;
    aux_pag->posi = posi;
    aux_pag->quantidade = 0;
    novoLRU->no->dirty = 0;
    novoLRU->no->pin_count = 0;
    novoLRU->no->prox = NULL;
    novoLRU->no->id = contadorPool++;

    for (i = 0; i < 3; i++) {
        aux_pag->reg[i].CPF = 0000;
        aux_pag->reg[i].matricula = 0000;
        strcpy(aux_pag->reg[i].nome, "VAZIO");
        strcpy(aux_pag->reg[i].sexo, "VAZIO");
    }

    novoLRU->no->pag = aux_pag;

    inicioLRU = insereLRU(novoLRU);

    return aux_pag;

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

void exibeDisponivel(LISTA *lista) {
    if (lista->inicio == NULL) {
        printf("\nLista vazia!.\n");
    } else {
        PAG *aux = lista->inicio;
        int contador = 0, i, j;
        for (i = 0; i < m; i++) {
            printf("\n---PAGINA %d---\n", i);

            for (j = 0; j < 3; j++) {
                printf("|Nome:%s \t\t", aux->reg[j].nome);
                printf("|Sexo:%s \t\t", aux->reg[j].sexo);
                printf("|CPF:%d \t\t", aux->reg[j].CPF);
                printf("|Matricula:%d \t\t", aux->reg[j].matricula);
                printf("\n");
            }
            aux = aux->prox;
            contador++;
            printf("\n\n");
        }

    }
}

void exibeCheio(LISTA *lista) {
    if (lista->inicio == NULL) {
        printf("\nLista vazia!.\n");
    } else {
        PAG *aux = lista->inicio;
        int contador = 0, i, j;
        for (i = 0; i < Ncheio + 1; i++) {

            printf("\n---PAGINA %d---\n", i);

            for (j = 0; j < 3; j++) {

                printf("|Nome:%s \t\t", aux->reg[j].nome);
                printf("|Sexo:%s \t\t", aux->reg[j].sexo);
                printf("|CPF:%d \t\t", aux->reg[j].CPF);
                printf("|Matricula:%d \t\t", aux->reg[j].matricula);
                printf("\n");
            }
            aux = aux->prox;
            contador++;
            printf("\n\n");
        }

    }
}

CABECALHO *alteraListas(CABECALHO *listas) {
    int i;
    PAG *aux_pag;

    aux_pag = listas->disponivel->inicio;
    printf("Matricula alterado: %d, \n", alterado->matricula);
    do {
        for (i = 0; i < 3; i++) {
            if (aux_pag->reg[i].matricula == alterado->matricula) { //achou
                alterado->prox = aux_pag->reg[i].prox;
                aux_pag->reg[i] = *alterado;
                printf("\n\t||> Dados alterados na lista de Disponivel\n");
                return listas;
            }
        }
        aux_pag = aux_pag->prox;
    } while (aux_pag->prox != NULL);

    aux_pag = listas->cheio->inicio;
    do {
        for (i = 0; i < 3; i++) {
            if (aux_pag->reg[i].matricula == alterado->matricula) { //achou
                alterado->prox = aux_pag->reg[i].prox;
                aux_pag->reg[i] = *alterado;
                printf("\n\t||> Dados alterados na lista de Cheio\n");
                return listas;
            }
        }
        aux_pag = aux_pag->prox;
    } while (aux_pag->prox != NULL);

    return listas;
}

CABECALHO *deletaListas(CABECALHO *listas) {
    int i, achou = 0;
    PAG *aux_pag;

    aux_pag = listas->disponivel->inicio;
    do {
        for (i = 0; i < 3; i++) {
            if (aux_pag->reg[i].matricula == deletar) { //achou
                if (i == 2) {
                    //Deleta a página
                    aux_pag->ant->prox = aux_pag->prox;
                    printf("\n\t||> Pagina deletada da lista de Disponivel!\n");
                    system("pause");
                    achou = 1;
                    return listas;
                    //	free(aux_pag);
                } else { //Deleta o registro
                    aux_pag->reg[i].CPF = 0000;
                    aux_pag->reg[i].matricula = 0000;
                    strcpy(aux_pag->reg[i].nome, "VAZIO");
                    strcpy(aux_pag->reg[i].sexo, "VAZIO");
                    achou = 1;
                    printf("\n\t||> Registro Deletado de pagina da lista de Disponivel!\n");
                    system("pause");
                    return listas;
                }

            }
        }
        aux_pag = aux_pag->prox;
    } while (aux_pag->prox != NULL);

    aux_pag = listas->cheio->inicio;
    do {

        for (i = 0; i < 3; i++) {
            if (aux_pag->reg[i].matricula == deletar) { //achou
                aux_pag->reg[i].CPF = 0;
                aux_pag->reg[i].matricula = 0;
                strcpy(aux_pag->reg[i].nome, "");
                strcpy(aux_pag->reg[i].sexo, "");
                printf("\n\t||> Registro Deletado de pagina da lista de Cheio!\n");
                system("pause");
                return listas;
            }
        }
        aux_pag = aux_pag->prox;
    } while (aux_pag->prox != NULL);

    inicioPool = deletaPool();

    if (achou == 0) printf("\t|#|> Aluno nao encontrado!\n");
    system("pause");
    return listas;
}

void limpaListas(CABECALHO *listas) {
    PAG *aux;

    aux = listas->disponivel->inicio;
    if (listas->disponivel->inicio != NULL) {
        do {
            listas->disponivel->inicio = listas->disponivel->inicio->prox;
            free(aux);
            aux = listas->disponivel->inicio;
        } while (aux->prox != NULL);
        printf("\n\t|-> Lista de Disponivel Apagada com sucesso!\n");
    } else {
        printf("\n\t|-> Lista de Disponivel Vazia!\n");
    }

    if (listas->cheio->inicio != NULL) {
        aux = listas->cheio->inicio;
        do {
            listas->cheio->inicio = listas->cheio->inicio->prox;
            free(aux);
            aux = listas->cheio->inicio;
        } while (aux->prox != NULL);
        printf("\n\t|-> Lista de Cheio Apagada com sucesso!\n");
    } else {
        printf("\n\t|-> Lista de Cheio Vazia!\n");
    }

}

CABECALHO *iniciaCabecalho() {
    int i;

    LISTA *disponivel = (LISTA *) malloc(sizeof(LISTA));
    LISTA *cheia = (LISTA *) malloc(sizeof(LISTA));;

    disponivel->inicio = NULL;
    cheia->inicio = NULL;


    CABECALHO *listas = (CABECALHO *) malloc(sizeof(CABECALHO));
    listas->disponivel = disponivel;
    listas->cheio = cheia;

    //Criar as m páginas:

    PAG *nova_pag, *aux_pag;
    for (i = 0; i < m; i++) { //Disponivel
        nova_pag = criaPagina(i);
        if (listas->disponivel->inicio == NULL) {
            listas->disponivel->inicio = nova_pag;
        } else {
            aux_pag = listas->disponivel->inicio;
            while (aux_pag->prox != NULL) aux_pag = aux_pag->prox; //Achou
            aux_pag->prox = nova_pag;
        }

    }

    for (i = 0; i < m; i++) { //Disponivel
        nova_pag = criaPagina(i);
        if (listas->cheio->inicio == NULL) {
            listas->cheio->inicio = nova_pag;
        } else {
            aux_pag = listas->cheio->inicio;
            while (aux_pag->prox != NULL) aux_pag = aux_pag->prox; //Achou
            aux_pag->prox = nova_pag;
        }

    }
    return listas;
}

CABECALHO *insereListas(CABECALHO *listas, Hash *tabela) {

    NoDeHash *aux_tabela = tabela->inicio;
    ALUNO *aux_aluno;
    PAG *nova_pag, *aux_pag, *aux_pag_cheio;

    int posicao, qtd, i;

    while (aux_tabela->prox != NULL) {
        aux_aluno = aux_tabela->apontaComeco;

        while (aux_aluno != NULL) {
            posicao = aux_aluno->matricula % m; //acha em qual pagina inserir
            printf("\n\t||>Indice da pagina a ser inserido: %d\n", posicao);
            system("pause");

            aux_pag = listas->disponivel->inicio;

            while (aux_pag->posi != posicao) { //Percorre ate achar a pagina
                if (aux_pag == NULL) {
                    printf("\n\t||> Pagina nao encontrada!\n");
                    system("pause");
                    break;
                }
                aux_pag = aux_pag->prox;
            } //achou a página
            if (aux_pag->quantidade == 2) { //Cabe apenas mais 1
                printf("\n\tOBS: Pagina de indice %d lotada e movida para lista de cheio!\n", posicao);
                system("pause");

                //Mover para lista de cheio, inserir, zerar da disponivel e Ncheio ++

                //insere
                qtd = aux_pag->quantidade;
                aux_pag->reg[qtd] = *aux_aluno;
                aux_pag_cheio = listas->cheio->inicio;

                //Move para cheio
                if (Ncheio > m) { //Criar pagina
                    Ncheio++;
                    nova_pag = criaPagina(Ncheio);
                    while (aux_pag_cheio->posi != Ncheio) aux_pag_cheio = aux_pag_cheio->prox; //Achou
                    aux_pag_cheio->prox = nova_pag;
                    nova_pag = aux_pag;
                    nova_pag->posi = Ncheio;
                    printf("\n\tOBS: Aluno inserido na pagina de indice %d da lista de cheio\n", Ncheio);
                    system("pause");

                } else { //Apenas preencher
                    while (aux_pag_cheio->posi != Ncheio) aux_pag_cheio = aux_pag_cheio->prox;
                    printf("Posicao de insercao: %d", aux_pag_cheio->posi);
                    printf("\n\tOBS: Aluno inserido na pagina de indice %d da lista de cheio\n", Ncheio);
                    system("pause");
                    aux_pag_cheio = aux_pag;
                    printf("MAt cheio: %d", aux_pag_cheio->reg[0].matricula);
                    //aux_pag_cheio->posi = Ncheio;
                }

                //Zerar da disponivel
                aux_pag->quantidade = 0;
                for (i = 0; i < 3; i++) {
                    aux_pag->reg[i].matricula = 0;
                    strcpy(aux_pag->reg[i].nome, "");
                    strcpy(aux_pag->reg[i].sexo, "");
                    aux_pag->reg[i].CPF = 0;
                }

            } else {
                qtd = aux_pag->quantidade;
                aux_pag->reg[qtd] = *aux_aluno;
                printf("\n\tOBS: Aluno inserido na pagina de indice %d da lista de disponivel\n", posicao);
                system("pause");
                aux_pag->quantidade = aux_pag->quantidade + 1;
                //aux_aluno = aux_aluno->prox;
                //break;
            }
            aux_aluno = aux_aluno->prox;
            //printf("Matr prox: %d", aux_aluno->matricula);
        }
        aux_tabela = aux_tabela->prox;
    }
    return listas;
}

/*
 * EXECUÇÃO DO PROGRAMA
 * */
int main(int argc, char **argv) {
    setlocale(LC_ALL, "Portuguese");
    int i;
    int opc = 0;
    int pin;

    /*Determinando tamanho da Hash*/
    while (m < 1 || m > 10) {
        system("cls");
        printf("Escolha um tamanho para a Hash: \n");
        scanf("%d", &m);
        if (m < 1 || m > 10) {
            printf("\n Entrada incorreta!\n");
        }
    }
    //Inicialmente a lista cheio tem o tamanho da disponivel

    Hash *tabela = setTabela();
    for (i = 0; i < m; i++) tabela = cria_Tabela(tabela, i);

    CABECALHO *cabecalho;
    cabecalho = iniciaCabecalho(cabecalho); //Listas criadas

    criaPool();

    while (opc != -1) {
        system("cls");
        printf("\n\n|---------------- MENU -----------------| \n\n");
        printf("\nSelecione suas opções: \n");
        printf("[1] - Cadastrar aluno:\n");
        printf("[2] - Remover aluno:\n");
        printf("[3] - Alterar informações de aluno:\n");
        printf("[4] - Consultar UM aluno(Na Pool):\n");
        printf("[5] - Exibir a tabela Hash:\n");
        printf("[6] - Exibir a lista de não-cheios:\n");
        printf("[7] - Exibir a lista de cheios:\n");
        printf("[8] - Exibir pool de buffers completa:\n");
        printf("[9] - Carregar Cadastro nas Listas:\n");
        printf("[10] - Decrementar pin_count do Quadro:\n");
        printf("[-1] - Sair:\n");
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                system("cls");
                tabela = insereTabela(tabela);
                system("pause");
                system("pause");
                break;

            case 2:
                system("cls");
                tabela = deletar_Hash(tabela);
                cabecalho = deletaListas(cabecalho);
                system("pause");
                break;
            case 3:
                system("cls");
                tabela = alterarTabela(tabela);
                cabecalho = alteraListas(cabecalho);
                system("pause");
                break;
            case 4:
                system("cls");
                printf("Digite o numero de matricula do usuario que deseja consultar: \n");
                scanf("%d", &pin);
                inicioPool = inserePool(pin, cabecalho);
                system("pause");
                break;
            case 5:
                system("cls");
                exibeTabela(tabela);
                system("pause");
                break;
            case 6:
                system("cls");
                exibeDisponivel(cabecalho->disponivel);
                system("pause");
                break;
            case 7:
                system("cls");
                exibeCheio(cabecalho->cheio);
                system("pause");
                break;
            case 8:
                system("cls");
                exibePool();
                system("pause");
                break;
            case 9:
                system("cls");
                cabecalho = insereListas(cabecalho, tabela);
                system("pause");
                break;
            case 10:
                system("cls");
                printf("Digite o id do quadro a ter seu pin_count decrementado: \n");
                scanf("%d", &pin);
                decrementaPin(pin);
                break;
            case -1:
                system("cls");
                limpaTabela(tabela);
                limpaListas(cabecalho);//Façam umas funções de limpar
                limpaPool();
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
