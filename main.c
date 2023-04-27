/*
  Implemente um prorama que apresente na tela o seguinte menu de opções: 
  1 - Ler uma árvore de um araquivo fornecido pelo usuário        ok
  2 - Imprimir a árvore (pré-ordem,em-ordem,pós-ordem,largura)    ok
  3 - Verificar se um elemento x existe na árvore                 ok
  4 - Imprimir o nível de um nó x                                 ok
  5 - Imprimir valores menor que x                                ok
  6 - Inserir um nó x na árvore
  7 - Remover um nó x na árvore
  8 - Sair                                                        ok
*/

#include "stdio.h"
#include "stdlib.h"
#include "locale.h"

typedef struct arvore{

    int info;
    struct arvore *esq;
    struct arvore *dir;
}arvore;

arvore *lerArvore(FILE *arq) {

    arvore *a = NULL;
    char c;
    int num;

    fscanf(arq,"%c",&c);
    fscanf(arq,"%d",&num);
    
    if (num==-1) {
        fscanf(arq,"%c",&c);
        return NULL;
    }
    else {
        a = (arvore*)malloc(sizeof(arvore));
        a->info = num;
        a->esq = lerArvore(arq);
        a->dir = lerArvore(arq);
        fscanf(arq,"%c",&c);
        return a;
    }
}

void imprimirPreOrdem(arvore *a){

    if(a != NULL){
      printf("%d ", a->info);
      imprimirPreOrdem(a->esq);
      imprimirPreOrdem(a->dir);  
    }
}

void imprimirPosOrdem(arvore *a){

    if(a != NULL){
      imprimirPosOrdem(a->esq);
      imprimirPosOrdem(a->dir);
      printf("%d ", a->info);
    }
}

void imprimirEmOrdem(arvore *a){

    if(a != NULL){
      imprimirEmOrdem(a->esq);
      printf("%d ", a->info);
      imprimirEmOrdem(a->dir);
    }
}

int altura(arvore *a){

  if (a == NULL) return 0;
  else{
    int he = altura(a->esq);
    int hd = altura(a->dir);
    if(he > hd) return he + 1;
    else return hd + 1;
  }
}

void imprimirNivel(arvore *a, int cont, int n){

  if(a != NULL){
    if(cont == n) 
      printf("%d ", a->info);
    else{
      imprimirNivel(a->esq, cont + 1, n);
      imprimirNivel(a->dir, cont + 1, n);
    }
  }
}


void imprimirEmLargura(arvore *a){

  int h = altura(a);
  for(int i = 1; i <= h; i++){
    imprimirNivel(a, 1, i);
  }
}


int existir(arvore *a, int x){

  if(a == NULL)
    return 0;

  else if(x == a->info)
    return 1;
  
  else if(x < a->info)
    return existir(a->esq, x);

  else 
    return existir(a->dir, x);

}

void imprimirMenores(arvore *a, int x){

  if(a != NULL){
    if(a->info < x){
      printf("%d", a->info);
      imprimirMenores(a->esq,x);
      imprimirMenores(a->dir,x);
    }
    else
      imprimirMenores(a->esq,x);
  }
}

void imprimirFolhas(arvore *a){

  if(a != NULL){
    if(a->dir == NULL && a->esq == NULL)
      printf("%d ", a->info);
  
  imprimirFolhas(a->esq);
  imprimirFolhas(a->dir);
}
}

int procurarElemento(arvore *a, int x){

    if(a == NULL){
        return 0;
    }
    else if(a->info == x){
        return 1;
    }
    else{
        return procurarElemento(a->esq, x) || procurarElemento(a->dir, x);
    }
}

int contarElementos(arvore *a){

  if(a == NULL){
    return 0;
  }
  else
    return 1 + contarElementos(a->esq) + contarElementos(a->dir);
}

int nivelDoNo(arvore *a, int x, int nivel){

  if(a == NULL) return -1; //menos 1, porque nesse caso o nivel pode ser 0

  if(a->info == x) return nivel;

  int nivel1 = nivelDoNo(a->esq, x, nivel+1);
  if(nivel1 == -1){ //se nao estiver na esquerda procura na direita
    nivel1 = nivelDoNo(a->dir, x, nivel+1);
  }

  return nivel1;
}

arvore *inserir(arvore *a, int x){

  if(a == NULL){
    a = (arvore *)malloc(sizeof(arvore));
    a->info = x;
    a->esq = NULL;
    a->dir = NULL;
    return a;
  }

  else if(x <= a->info)
    a->esq = inserir(a->esq, x);

  else
   a->dir = inserir(a->dir, x);
  
  return a;
}

arvore *remover(arvore *a, int x){

if(a == NULL)
return NULL;

if(x < a->info)
a->esq = remover(a->esq, x);

else if(x > a->info)
a->dir = remover(a->dir, x);

else{
if(a->esq == NULL && a->dir == NULL){ //caso 1: folha
free(a);
a = NULL;
}
else if(a->esq == NULL){ //caso 2: só tem filho a direita
arvore *temp = a;
a = a->dir;
free(temp);
}
else if(a->dir == NULL){ //caso 2: só tem filho a esquerda
arvore *temp = a;
a = a->esq;
free(temp);
}
else{ //caso 3: tem ambos os filhos
arvore *temp = a->esq;
while(temp->dir != NULL){ //busca o maximo da subarvore esquerda
temp = temp->dir;
}
a->info = temp->info; //copia o valor maximo da esquerda para o no que queremos remover
temp->info = x; //troca o valor que queremos remover pelo valor maximo da esquerda
a->esq = remover(a->esq, x); //remove o valor trocado (que agora está na esquerda)
}
}
return a;
}

void freeArvore(arvore *a) {

    if (a != NULL) {
        freeArvore(a->esq);
        freeArvore(a->dir);
        free(a);
    }
}

int main(){
  setlocale(LC_ALL,"Portuguese");
  int opcao, elemento, nivel;
  char nomeArq[50]; 
  arvore *a = NULL;
  FILE *arq = NULL;

  do{
    printf("\nMENU:\n");
    printf("1 - Ler uma árvore de um arquivo fornecido pelo usuário\n");
    printf("2 - Imprimir a árvore (pré-ordem, em-ordem, pós-ordem, largura)\n");
    printf("3 - Verificar se um elemento x existe na árvore\n");
    printf("4 - Imprimir o nível de um nó x\n");
    printf("5 - Imprimir valores menores que x\n");
    printf("6 - Inserir um nó x na árvore\n");
    printf("7 - Remover um nó x na árvore\n");
    printf("8 - Sair\n");

    printf("\nDigite a opção: ");
    scanf("%d",&opcao);

    switch(opcao){

      case 1:
       
        printf("Digite o nome do arquivo que deseja ler: ");
        scanf("%s", nomeArq);
        arq = fopen(nomeArq, "r");
        if(arq == NULL) {
          printf("Erro ao abrir arquivo!\n\n");
          break;
        }
        a = lerArvore(arq);
        printf("Arvore lida com sucesso!\n");
        fclose(arq);
        break;

      case 2:
        printf("\nÁrvore em pré-ordem: ");
        imprimirPreOrdem(a);
        printf("\nÁrvore em em-ordem: ");
        imprimirEmOrdem(a);
        printf("\nÁrvore em pós-ordem: ");
        imprimirPosOrdem(a);
        printf("\nÁrvore em largura: ");
        imprimirEmLargura(a);
        printf("\n");
        break;

      case 3:
        printf("\nDigite o elemento a ser procurado: ");
        scanf("%d",&elemento);
        if(existir(a,elemento))
          printf("O elemento %d está na árvore.\n",elemento);
        else  
          printf("O elemento %d não está na árvore.\n",elemento);
        break;

      case 4:
        printf("\nDigite o elemento para saber o nível: ");
        scanf("%d",&elemento);
        nivel = nivelDoNo(a, elemento, 0);
        if(nivel == -1)
          printf("O elemento %d não está na árvore.\n",elemento);
        else
          printf("O nível do elemento %d é %d.\n",elemento,nivel);
        break;

      case 5:
        printf("\nDigite o valor de referência para imprimir menores: ");
        scanf("%d",&elemento);
        printf("Os valores menores que %d são: ",elemento);
        imprimirMenores(a,elemento);
        printf("\n");
        break;

      case 6:
        printf("\nDigite o elemento a ser inserido: ");
        scanf("%d",&elemento);
        a = inserir(a,elemento);
        printf("O elemento %d foi inserido na árvore.\n",elemento);
        break;

      case 7:
        printf("\nDigite o elemento a ser removido: ");
        scanf("%d",&elemento);
        a = remover(a,elemento);
        printf("O elemento %d foi removido da árvore.\n",elemento);
        break;

      case 8:
        printf("\nSaindo...\n");
        break;

      default:
        printf("\nOpção inválida!\n");
    }
  }while(opcao != 8);

  freeArvore(a);

  return 0;
}
