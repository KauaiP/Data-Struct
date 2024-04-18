#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct heap
{
    int data[30];
    int totalQuant;
} Heap;

// funções aux para heap
int indiceFilhoLeft(int x, Heap *h)
{
    int indice = (2 * x) + 1;
    if (x >= h->totalQuant || indice >= h->totalQuant)
    {
        return -1;
    }
    else
    {
        return indice;
    }
}

int indiceFilhoRight(int x, Heap *h)
{
    int indice = (2 * x) + 2;
    if (x >= h->totalQuant || indice >= h->totalQuant)
    {
        return -1;
    }
    else
    {
        return indice;
    }
}

int indiceDad(Heap *h, int x)
{
    int indice = (int)floor((x - 1) / 2);
    if (x <= 0 || x >= h->totalQuant)
    {
        return -1;
    }
    else
    {
        return indice;
    }
}

void adjustUp(Heap *h, int pos)
{
    if (pos != -1)
    {
        int dad = indiceDad(h, pos);
        if (dad != -1)
        {
            if (h->data[pos] < h->data[dad])
            {
                int aux = h->data[pos];
                h->data[pos] = h->data[dad];
                h->data[dad] = aux;
                adjustUp(h, dad);
            }
        }
    }
}

void insert(Heap *h, int x)
{
    h->data[h->totalQuant] = x;
    h->totalQuant++;

    adjustUp(h, h->totalQuant - 1);
}

void adjustDown(Heap *h, int pos)
{
    if (pos != -1 && indiceFilhoLeft(pos, h) != -1)
    {
        int indiceMenorFilho = indiceFilhoLeft(pos, h);
        if (indiceFilhoRight(pos, h) != -1 &&
            h->data[indiceFilhoRight(pos, h)] < h->data[indiceMenorFilho])
        {
            indiceMenorFilho = indiceFilhoRight(pos, h);
        }

        if (h->data[indiceMenorFilho] < h->data[pos])
        {
            int aux = h->data[pos];
            h->data[pos] = h->data[indiceMenorFilho];
            h->data[indiceMenorFilho] = aux;
            adjustDown(h, indiceMenorFilho);
        }
    }
}

int remover(Heap *h)
{
    if (h->totalQuant == 0)
    {
        return -1;
    }
    else
    {
        int element = h->data[0];
        h->data[0] = h->data[h->totalQuant - 1];
        h->totalQuant--;
        adjustDown(h, 0);
        return element;
    }
}

void printHeap(Heap *h)
{
    for (int i = 0; i < h->totalQuant; i++)
    {
        printf("%d ", h->data[i]);
    }
}

int main()
{
    Heap *h = malloc(sizeof(Heap));
    h->totalQuant = 0;

    int condicao = 0;
    while (condicao != 4)
    {
        printf("\n");
        puts("1 - Inserir um elemento x na heap");
        puts("2 - Remover um elemento");
        puts("3 - Imprimir a heap");
        puts("4 - Sair");
        scanf("%d", &condicao);
        getchar();

        if (condicao == 1)
        {
            system("clear");
            puts("1 - Inserir um elemento x na heap");
            printf("\n");
            printf("digite o elemento x: ");
            int x;
            scanf("%d", &x);
            insert(h, x);
            system("clear");
        }
        else if (condicao == 2)
        {
            system("clear");
            puts("2 - Remover um elemento x da heap");
            printf("\n");
            int x;
            x = remover(h);
            printf("Elmento removido: %d (raiz)", x);
            system("clear");
        }
        else if (condicao == 3)
        {
            system("clear");
            puts("3 - Imprimir a heap");
            printHeap(h);
            printf("\n");
        }
    }
    free(h);
    printf("\nPrograma encerrado\n");
    return 0;
}