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
#include "locale.h"

int m;
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
}ALUNO;

typedef struct nodehash {
  int posi;
  ALUNO *apontaComeco;
  struct nodehash *prox;
}NoDeHash;

typedef struct hash {
  NoDeHash *inicio;
  NoDeHash *fim;
}Hash;

typedef struct registro{
  int id;
}REG;

typedef struct paginas {
  REG reg[3];
  struct paginas *prox;
  struct paginas *ant;
} PAG;

typedef struct lista{
  PAG *inicio;
  PAG *fim;
}LISTA;

typedef struct noDaPool{
  int pin_count;
  int dirty;
  int id;
  int maisAntigo;//use para fazer o LRU
  PAG *pag;
  struct noDaPool *prox;
}NoPool;

typedef struct pool {
  NoPool *inicio;
  NoPool *fim;
} POOL;


/*
 * PROTÓTIPOS DE FUNÇÕES
 */

/*FUNÇÕES PRINCIPAIS*/
POOL *criaPool(POOL *pool){ //REESCREVENDO A FUNÇÃO DE FORMA MAIS ENXUTA
  pool->inicio = NULL;
  pool->fim = NULL;

  for(int i = 0; i < 4; i++) {
    NoPool *novo = (NoPool *) malloc(sizeof(NoPool));
    if(novo == NULL) {
      printf("Erro de memoria!");
      exit(1);
    }

    novo->dirty = 0;
    novo->pin_count = 0;
    novo->pag = NULL;
    novo->id = 1;
    novo->maisAntigo = -1;
    novo->prox = NULL;

    if(pool->inicio == NULL) {
      pool->inicio = novo;
      pool->fim = novo;
    } else {
      pool->fim->prox = novo;
      pool->fim = novo;
    }
  }

  return pool;
}

PAG *cria_Paginas(PAG *pag){
  int i;
  for(i = 0; i < 3; i++) {
    
  }
}

Hash *cria_Tabela(int m, Hash *tabela) {

  int i;
  for(i=0;i<m;i++){
    NoDeHash *novo = (NoDeHash*) malloc (sizeof(NoDeHash));
    novo->apontaComeco = NULL;
    novo->posi = i;
    if(i==0){
      tabela->inicio = novo;
      tabela->fim = novo;
      novo->prox = NULL;
    }else{
      tabela->fim->prox = novo;
      tabela->fim = novo;
      novo->prox = NULL;
    }
  }

  return tabela;
}

Hash *insereTabela(Hash *tabela) {
  int CPF,matricula;
  char nome[100],sexo[2];

  printf("\nDigite o nome:\n");
  fflush(stdin);
  scanf("%s",nome);
  printf("\nDigite o sexo [m] ou [f]:\n");
  fflush(stdin);
  scanf("%s",sexo);
  printf("\nDigite o número da matrícula:\n");
  scanf("%d",&matricula);
  printf("\nDigite o CPF:\n");
  scanf("%d",&CPF);

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
  while(aux->posi!=matricula%m){
    aux = aux->prox;
  }
  if(aux->apontaComeco == NULL){
    aux->apontaComeco = novo;
  }else{
    ALUNO *aux1;
    aux1 = aux->apontaComeco;
    while(aux1->prox!=NULL){
      aux1 = aux1->prox;
    }
    aux1->prox = novo;
  }

  return tabela;
}

Hash *inserir_Hash(Hash *tabela, int m) {
  int i;

  return tabela;
}

Hash *deletar_Hash(Hash *tabela, int matricula, int m) {
  /*
   *
   * FUNÇÃO QUE ENCONTRA O BUCKET USANDO A CHAVE E DELETA O CONTEÚDO
   *
   */
  return tabela;
}


/*
 * EXECUÇÃO DO PROGRAMA
 */
int main(int argc, char **argv) {
  setlocale(LC_ALL, "Portuguese");
  int i;
  int opc = 0;

  /*Determinando tamanho da Hash*/
  printf("Escolha um tamanho para a Hash: \n");
  scanf("%d", &m);

  Hash *tabela;
  tabela = cria_Tabela(m, tabela); 

  LISTA *disp,*cheia;
  POOL *pool = NULL;
  pool = criaPool(pool);

  /*Criando a Pool de Buffers*/
  while (opc != -1) {
    printf("\nSelecione suas opções: \n");
    printf("\n[1] - Cadastrar aluno:\n");
    printf("\n[2] - Remover aluno:\n");
    printf("\n[3]- Alterar informações de aluno:\n");
    printf("\n[4] - Consultar UM aluno(Na Pool):\n");
    printf("\n[5] - Exibir a tabela Hash:\n");
    printf("\n[6] - Exibir a lista de não-cheios:\n");
    printf("\n[7] - Exibir a lista de cheios:\n");
    printf("\n[8] - Exibir pool de buffers completa:\n");
    printf("\n[-1] - Sair:\n");
    scanf("%d", &opc);

    switch(opc) {
      case 1:

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
      case 7:

        break;
      case 8:

        break;
      case -1:

        break;

    }

  }

  return 0;
}
