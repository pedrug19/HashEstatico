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
#include "tad.h" //Arquivo com as declarações de estruturas e protótipos
#include <locale.h>

/*ESTRUTURA INICIAL DA HASH*/
//Hash *tabela = NULL; //Criando o apontador da tabela para nulo
//int contador_de_elementos = 0;

/*FUNÇÕES PRINCIPAIS*/
Hash *cria_Tabela(int m, Hash tabela[]) {
    /*
     *
     * Função responsável pela criação e preenchimento da tabela Hash
     * 
     */
    int i;

    /*Fazendo com que todos os espaços da tabela apontem para NULL*/
    for (i = 0; i < m; i++) {
        tabela[i].inicio = NULL;
        tabela[i].contador = 0; //o numero de dados nesse bucket é 0
    }

    return tabela;
}

NODE *criar_No(int matricula, char *nome, int CPF) {
    /*
     * 
     * Função responsável para criar os nós da tabela
     *
     */

    NODE *novo = (NODE *) malloc(sizeof (NODE));
    if (novo == NULL) { // Verifica se o nó foi criado com sucesso
        printf("Memória insuficiente!\n"); // Não há espaço de memória
        exit(1);
    }

    /*Alocando elementos no novo nó*/
    novo->matricula = matricula;
    novo->CPF = CPF;
    strcpy(novo->nome, nome);
    novo->prox = NULL;
    return novo;
}

Hash *inserir_Hash(Hash tabela[], int m) {
    /*
     *
     * Função responsável por inserir elementos na hash
     *      
     */
    int CPF, matricula, idade;
    char nome[100];

    printf("Digite o numero da matricula: ")
    scanf("%d", &matricula);
    printf("Digite o nome: ");
    scanf("%[^\n]", nome);
    printf("Digite o valor do CPF: ");
    scanf("%d", &CPF);

    NODE *novo = criar_No(int matricula, char *nome, int CPF);

    int indiceHash = matricula % m;

    /*Adiciona o node no começo do bucket*/
    novo->prox = (tabela[indiceHash].inicio);
    tabela[indiceHash].inicio = novo;
    tabela[indiceHash].contador++;

    return tabela;
}

Hash *deletar_Hash(Hash tabela[], int matricula, int m) {
    /*
     *
     * FUNÇÃO QUE ENCONTRA O BUCKET USANDO A CHAVE
     * 
     */

    int indiceHash = matricula % m;

    NODE *aux;

    aux = tabela[indiceHash].inicio;
    if (aux == NULL) {
        printf("Esses dados não estão na tabela!\n");
    } else {
        NODE *aux2 = aux; //auxiliar para nao perder referencia

        while (aux != NULL) {
            if (aux->matricula == matricula) {
                if (aux == tabela[indiceHash].inicio) {
                    tabela[indiceHash].inicio = aux->prox;
                } else {
                    aux2->prox = aux->prox;
                }

                tabela[indiceHash].contador--;
                free(aux);
                printf("Deletado com sucesso!\n");
                return tabela;
            }

            aux2 = aux;
            aux = aux->prox;
        }

        printf("Dado nao se encontra na tabela!\n");
    }

    return tabela;
}

void imprime_No(NODE *no) {
    /*Funcao para imprimir os dados do no*/
    printf("Nome: %s\n", no->nome);
    printf("Numero Matricula? %d\n", no->matricula);
    printf("CPF: %d\n", no->CPF);

    return;
}

int procura_Hash(Hash tabela[], int matricula, int m, int modifica) {
    /*
     *
     *  FUNÇÃO PARA PROCURAR NA HASH UM VALOR
     * 
     */
    int indiceHash = matricula % m;
    int achou = 0;
    int contador = 0;

    NODE *aux = tabela[indiceHash].inicio;
    if (aux = NULL) {
        printf("Não há dados correspondentes na tabela!\n");
    } else {
        while (aux != NULL) {
            if (aux->matricula == matricula) {
                imprime_No(aux);
                achou = 1;
            } else {
                aux = aux->prox;
                if (modifica == 1) {
                    contador++;
                }
            }
        }

        if (!achou) {
            printf("Dados nao foram encontrados com esse numero de matricula!\n");
            return -1;
        }
    }

    return contador;
}

Hash *modifica_Hash(Hash tabela[], int m) {
    int CPF, matricula, idade, x;
    char nome[100];
    

    printf("Digite o numero da matricula a ser procurada: ")
    scanf("%d", &matricula);
    
    x = procura_Hash(tabela[], matricula, m, 1);
    
    int indiceHash = matricula % m;
    NODE *aux = tabela[indiceHash].inicio;
    
    if(x == -1) {
        printf("Nao eh possivel modificar essa matricula!\n");
        return tabela;
    }
    
    printf("Dados serao modificados!\n");
    printf("Digite o nome: ");
    scanf("%[^\n]", nome);
    printf("Digite o valor do CPF: ");
    scanf("%d", &CPF);
    
    for(int i = 0; i < x-1; i++) {
        aux = aux->prox;
    }
    
    aux->CPF = CPF;
    strcpy(aux->nome, nome);
    printf("Dados modificados!");
    imprime_No(aux);
    
    return tabela;
}

/*
 * EXECUÇÃO DO PROGRAMA
 */
int main(int argc, char** argv) {
    int m, i;

    /*Determinando tamanho da Hash*/
    printf("Escolha um tamanho para a Hash: \n");
    scanf("%d", &m);

    /*Declarando estrutura da tabela*/
    Hash tabela[m]; //Tabela Hash
    Hash *table; //apontador para a tabela

    /*O apontador table irá se responsabilizar por passar
     * a tabela para outras funções*/

    table = cria_Tabela(m, tabela); //Preenchendo tabela.inicio com NULL
    int opc = 0;

    /*while (opc != -1) {
        printf("Selecione suas opções: \n");


    }*/

    return (EXIT_SUCCESS);
}

