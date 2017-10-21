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

/*
   POOL *criaPool(POOL *pool);
   ALUNO *criar_No(int matricula, char *nome, int CPF);
   Hash *cria_Tabela(int m, Hash tabela[]);
   PAG *cria_Pagina(int m, Hash tabela[]);
   void LRU();
   Hash *inserir_Hash(Hash tabela[], int m);
   Hash *deletar_Hash(Hash tabela[], int matricula, int m);
   Hash *modifica_Hash(Hash tabela[], int m);
   int procura_Hash(Hash tabela[], int matricula, int m, int modifica);
   void imprime_No(ALUNO *no);
   void imprime_Hash();
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

/*
   PAG *cria_Pagina(int m, Hash tabela[]) {
// FUNÇÃO QUE CRIA PáGINAS
int i;
PAG *aux = NULL;

for(i = 0; i < 3; i++) {
PAG *novo = (PAG *) malloc(sizeof(PAG));
if(novo == NULL) {
printf("MemÃ³ria insuficiente!\n");
exit(1);
}

//novo->dirty = 0;
//novo->pin_count = 0;
if(aux == NULL) {
aux = novo;
aux->ant = NULL;
aux->prox = NULL;
aux->inicio = cria_Tabela(m, tabela);
} else {
if(aux->ant = NULL) {
novo->ant = aux;
aux->prox = novo;


} else {

}
}
}

}
*/

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
  /*
     int indiceHash = matricula % m;

     ALUNO *aux;

     aux = tabela[indiceHash].inicio;
     if (aux == NULL) {
     printf("Esses dados não estão na tabela!\n");
     } else {
     ALUNO *aux2 = aux; //auxiliar para não perder referÃªncia
  /*Procura os dados na tabela*/
  /*
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
     */
  return tabela;
}

/*
   int procura_Hash(Hash tabela[], int matricula, int m, int modifica) {
/*
int indiceHash = matricula % m;
int achou = 0; //variavel de controle
int contador = 0; //retorna a posição onde foi encontrado o elemento

ALUNO *aux = tabela[indiceHash].inicio;
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
*/
/*
   Hash *modifica_Hash(Hash tabela[], int m) {

   int CPF, matricula, idade, x;
   char nome[100];

   printf("Digite o numero da matricula a ser procurada: ");
   scanf("%d", &matricula);


   x = procura_Hash(tabela, matricula, m, 1);

   int indiceHash = matricula % m;
   ALUNO *aux = tabela[indiceHash].inicio;

   if (x == -1) {
   printf("Nao eh possivel modificar essa matricula!\n");
   return tabela;
   }

   printf("Dados serao modificados!\n");
   printf("Digite o nome: ");
   scanf("%[^\n]", nome);
   printf("Digite o valor do CPF: ");
   scanf("%d", &CPF);

   int i;

   for (i = 0; i < x - 1; i++) {
   aux = aux->prox;
   }

   aux->CPF = CPF;
   strcpy(aux->nome, nome);
   printf("Dados modificados!");
   imprime_No(aux);

   return tabela;
   }
   */


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

  POOL *pool;

  /*Criando a Pool de Buffers*/
  /*
     POOL *pool = (POOL *) malloc(sizeof(POOL));
     pool = criaPool(pool);
     */

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
