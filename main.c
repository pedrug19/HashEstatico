/* 
 * Projeto:   Trabalho 1 Lab ED2
 * 
 * Autores:   Pedrenrique G. Guimarães
 *            Gustavo Molina
 *            Luis Marcello
 * 
 * Implementação de Hashing estático
 * Compilado e testado em Arch Linux, Fedora e Windows 10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tad.h" //Arquivo com as declarações de estruturas e protótipos
#include <locale.h>

/*FUNÇÕES PRINCIPAIS*/
PAG *cria_Pagina(int m, Hash tabela[]) {
  /*
   *
   *  FUNÇÃO QUE CRIA PÁGINAS
   *
   */
  int i;
  PAG *aux = inicioPag;

  for(i = 0; i < 3; i++) {
    PAG *novo = (PAG *) malloc(sizeof(PAG));
    if(novo == NULL) {
      printf("Memória insuficiente!\n");
      exit(1);
    }

    novo->dirty = 0;
    novo->pin_count = 0;
    novo->prox = NULL;
    novo->inicio = cria_Tabela(m, tabela);
    if(aux == NULL) {
      aux->ant = NULL;
      inicioPag = novo;
      aux = novo;
    } else {
      if(aux->ant = NULL) {
        novo->ant = aux;
        aux->prox = novo;
        novo->prox = NULL;
        aux = novo;
        /*TODO: CONTINUAR A FAZER A LOGICA DISSO*/
      } else {
        /*TODO: FAZER O RESTO*/
      }
    }
  }

}

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
  NODE *novo = (NODE *) malloc(sizeof(NODE));
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
   * Função responsável por inserir elementos na Hash
   *      
   */
  int CPF, matricula;
  char nome[100];

  printf("Digite o numero da matricula: ");
  scanf("%d", &matricula);
  printf("Digite o nome: ");
  scanf("%[^\n]", nome);
  printf("Digite o valor do CPF: ");
  scanf("%d", &CPF);

  NODE *novo = criar_No(matricula, nome, CPF);

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
   * FUNÇÃO QUE ENCONTRA O BUCKET USANDO A CHAVE E DELETA O CONTEÚDO
   * 
   */
  int indiceHash = matricula % m;

  NODE *aux;

  aux = tabela[indiceHash].inicio;
  if (aux == NULL) {
    printf("Esses dados não estão na tabela!\n");
  } else {
    NODE *aux2 = aux; //auxiliar para não perder referência
    /*Procura os dados na tabela*/
    while (aux != NULL) {
      if (aux->matricula == matricula) {
        if (aux == tabela[indiceHash].inicio) {
          tabela[indiceHash].inicio = aux->prox;
        } else {
          aux2->prox = aux->prox;
        }
        /*Diminui o contador de elementos da tabela e libera os dados*/
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
  /*
   *
   *   Função para imprimir os dados do nó
   *
   */
  printf("Nome: %s\n", no->nome);
  printf("Numero Matricula? %d\n", no->matricula);
  printf("CPF: %d\n", no->CPF);
}

int procura_Hash(Hash tabela[], int matricula, int m, int modifica) {
  /*
   *
   *  FUNÇÃO PARA PROCURAR NA HASH UM VALOR
   * 
   */
  int indiceHash = matricula % m;
  int achou = 0; //variavel de controle
  int contador = 0; //retorna a posição onde foi encontrado o elemento

  NODE *aux = tabela[indiceHash].inicio;
  if (aux == NULL) {
    printf("Não há dados correspondentes na tabela!\n");
  } else {
    while (aux != NULL) {
      if (aux->matricula == matricula) {
        imprime_No(aux);
        achou = 1;
      } else {
        aux = aux->prox;
        if (modifica == 1) {
          /*  O valor de modifica vai determinar se o elemento será modificado
           *   pela função modifica
           */
          contador++;
        }
      }
    }

    if (!achou) {
      printf("Dados nao foram encontrados com esse numero de matricula!\n");
      return -1;
    }
  }

  return achou;
}

Hash *modifica_Hash(Hash tabela[], int m) {
  /*
   *
   *  FUNÇÃO QUE MODIFICA O VALOR DE UM ELEMENTO NA HASH
   * 
   */
  int CPF, matricula, x;
  char nome[100];

  printf("Digite o numero da matricula a ser procurada: ");
  scanf("%d", &matricula);

  /*Recebe o valor de procura*/
  x = procura_Hash(tabela, matricula, m, 1);

  int indiceHash = matricula % m;
  NODE *aux = tabela[indiceHash].inicio;

  if (x == -1) {
    printf("Nao eh possivel modificar essa matricula!\n");
    return tabela;
  }

  printf("Dados serao modificados!\n");
  printf("Digite o nome: ");
  scanf("%[^\n]", nome);
  printf("Digite o valor do CPF: ");
  scanf("%d", &CPF);

  for (int i = 0; i < x - 1; i++) {
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
int main(int argc, char **argv) {
  setlocale(LC_ALL, "Portuguese");
  int m;

  /*Criando o cabeçalho da Pool de Buffers*/
  POOL *cabecalho = (POOL *) malloc(sizeof(POOL));
  cabecalho->livres = NULL;
  cabecalho->usados = NULL;

  /*Determinando tamanho da Hash*/
  printf("Escolha um tamanho para a Hash: \n");
  scanf("%d", &m);

  /*Declarando estrutura da tabela*/
  Hash tabela[m]; //Tabela Hash
  Hash *table = tabela; //apontador para a tabela

  /*O apontador table irá se responsabilizar por passar
   * a tabela para outras funções*/

  table = cria_Tabela(m, tabela); //Preenchendo tabela.inicio com NULL
  int opc = 0;

  while (opc != -1) {
    printf("Selecione suas opções: \n");
    printf("1 - Cadastrar aluno");
    printf("2 - Alterar informações de aluno");
    printf("3 - Remover aluno");
    printf("4 - Exibir informações de aluno");
    printf("5 - Exibir pool de buffers");
    printf("6 - Exibir paginas");
    printf("-1 - Sair");
    scanf("%d", &opc);

    switch(opc) {
      case 1:
        printf("Cadastrando aluno.\n");
        break;
      case 2:

        break;
      case 3:

        break;
      case 4:

        break;
      case 5:

        break;
      case 6:

        break;
      case -1:

        break;

    }

  }

  return (EXIT_SUCCESS);
}
