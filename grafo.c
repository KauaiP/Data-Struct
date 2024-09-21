#include <stdlib.h>
#include <stdio.h>

typedef struct lista
{
    int dest;
    int custo;
    struct lista *prox;
} lista;

void inicializar(lista **g, int n)
{
    for (int i = 0; i <= n; i++)
    {
        g[i] = NULL;
    }
}

lista *inserirLista(lista *l, int dest, int custo)
{
    lista *new_node = (lista *)malloc(sizeof(lista));
    new_node->dest = dest;
    new_node->custo = custo;
    new_node->prox = l; // estamos colocando no começo da lista
    return new_node;
}

void inserirAresta(lista **g, int origem, int dest, int custo)
{
    g[origem] = inserirLista(g[origem], dest, custo);
}

lista *removerLista(lista *l, int dest)
{
    if (l->dest == dest)
    {
        free(l);
        return NULL;
    }

    lista *aux = l->prox;
    lista *new_list = l;

    while (aux != NULL)
    {
        if (aux->dest == dest)
        {
            new_list->prox = aux->prox;
            free(aux);
            break;
        }
        aux = aux->prox;
        new_list = new_list->prox;
    }
    return new_list;
}

void removerAresta(lista **g, int origem, int dest)
{
    g[origem] = removerLista(g[origem], dest);
}

void imprimirLista(lista *l)
{
    if (l != NULL)
    {
        printf("-(%d, %d)", l->dest, l->custo);
        imprimirLista(l->prox);
    }
}

void imprimirGrafo(lista **g, int n)
{
    printf("Grafo:\n");
    for (int i = 1; i < n; i++)
    {
        printf("\n\t%d", i);
        imprimirLista(g[i]);
        printf("\n");
    }
}

int grausSaida(lista *l)
{
    if (l == NULL)
    {
        return 0;
    }

    return 1 + grausSaida(l->prox);
}

int grausEntrada(lista *l, int vertice)
{
    if (l == NULL)
    {
        return 0;
    }
    else
    {
        if (l->dest == vertice)
        {
            return 1 + grausEntrada(l->prox, vertice);
        }
        else
        {
            return grausEntrada(l->prox, vertice);
        }
    }
}

void graus(lista **g, int n, int vertice)
{
    int cont = 0;
    for (int i = 1; i < n; i++)
    {
        cont = cont + grausEntrada(g[i], vertice);
    }

    printf("\tgrau de entrada: %d // grau de saida: %d\n", cont, grausSaida(g[vertice]));
}

void completo(lista **g, int n)
{
    int cont = 0;
    int size = n - 1;
    int quantidade;
    for (int i = 1; i < n; i++)
    {
        quantidade = grausSaida(g[i]); // retorna a quantidade de arestas de um vertice
        cont = cont + quantidade;
    }

    if (cont == size * (size - 1))
    {
        printf("O grafo eh completo\n");
    }
    else
    {
        printf("O grafo nao eh completo");
    }
}

int existe(int *vet, int valor, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (vet[i] == valor)
        {
            return 1;
        }
    }
    return 0;
}

void caminhos(lista **g, int b, int *vet, int pos)
{
    if (vet[pos - 1] == b)
    {
        int i;
        printf("\n");
        for (i = 0; i < pos; i++)
        {
            printf("%d ", vet[i]);
        }
    }
    else
    {
        lista *p = g[vet[pos - 1]];
        while (p != NULL)
        {
            if (!existe(vet, p->dest, pos))
            {
                vet[pos] = p->dest;
                caminhos(g, b, vet, pos + 1);
            }
            p = p->prox;
        }
    }
}

void caminhos_curto(lista **g, int b, int *vet, int pos, int *vetAux, int *menor)
{
    if (vet[pos - 1] == b)
    {
        int size = pos;
        if (size < *menor)
        {
            *menor = size;
            for (int i = 0; i < pos; i++)
            {
                vetAux[i] = vet[i];
            }
        }
    }

    else
    {
        lista *p = g[vet[pos - 1]];
        while (p != NULL)
        {
            if (!existe(vet, p->dest, pos))
            {
                vet[pos] = p->dest;
                caminhos_curto(g, b, vet, pos + 1, vetAux, menor);
            }
            p = p->prox;
        }
    }
}

void caminhos_menor(lista **g, int b, int *vet, int pos, int *vetAux, int *menorCusto, int soma, int *len)
{
    if (vet[pos - 1] == b)
    {
        if (soma < *menorCusto)
        {
            *menorCusto = soma;
            for (int i = 0; i < pos; i++)
            {
                vetAux[i] = vet[i];
                *len = pos;
            }
        }
    }
    else
    {
        lista *p = g[vet[pos - 1]];
        while (p != NULL)
        {
            if (!existe(vet, p->dest, pos))
            {
                vet[pos] = p->dest;
                soma = soma + p->custo;
                caminhos_menor(g, b, vet, pos + 1, vetAux, menorCusto, soma, len);
            }
            p = p->prox;
        }
    }
}

void freeList(lista *l)
{
    if (l != NULL)
    {
        freeList(l->prox);
        free(l);
    }
}

void freeGrafo(lista **g, int n)
{
    for (int i = 0; i < n; i++)
    {
        freeList(g[i]);
    }
}

int main()
{
    int n = 5;
    lista *g[n];
    inicializar(g, n);
    int escolha = 0;
    int origem, dest, custo;
    while (escolha != 9)
    {
        printf("\n");
        printf("1. Inserir uma aresta no grafo\n");
        printf("2. Remover uma aresta do grafo\n");
        printf("3. Imprimir grafo\n");
        printf("4. Imprimir os graus de entrada e saida de um vertice\n");
        printf("5. Verificra se um grafo eh completo\n");
        printf("6. Imprimir todos os caminhos entre uma origem e um destino\n");
        printf("7. Imprimir o caminho mais curto\n");
        printf("8. Imprimir o caminho com menos custo\n");
        printf("9. Sair");
        printf("\n");

        scanf("%d", &escolha);
        getchar();

        switch (escolha)
        {
        case 1:
            system("cls");
            printf("1. Inserir uma aresta no grafo\n");
            printf("\n");

            printf("Digite o vertice origem {1, 2, 3, 4}\n");
            scanf("%d", &origem);
            printf("Digite o vertice destino\n");
            scanf("%d", &dest);
            printf("Digite o custo dessa aresta\n");
            scanf("%d", &custo);

            inserirAresta(g, origem, dest, custo);
            break;
        case 2:
            system("cls");
            printf("2. Remover uma aresta do grafo\n");
            printf("\n");

            printf("Digite a origem do vertice\n");
            scanf("%d", &origem);
            printf("Digite o vertice destino\n");
            scanf("%d", &dest);

            removerAresta(g, origem, dest);
            break;
        case 3:
            system("cls");
            printf("3. Imprimir grafo\n");
            printf("\n");

            imprimirGrafo(g, n);
            break;
        case 4:
            system("cls");
            printf("4. Imprimir os graus de entrada e saida de um vertice\n");
            printf("\n");
            printf("digite um vertice: ");

            int vertice;

            scanf("%d", &vertice);

            graus(g, n, vertice);
            break;
        case 5:
            system("cls");
            printf("5. Verificar se um grafo eh completo");
            printf("\n");

            completo(g, n);
            break;
        case 6:
            system("cls");
            printf("6. Imprimir todos os caminhos entre uma origem e um destino\n");
            printf("\n");

            int *vet;

            printf("Digite a origem que desja: ");
            scanf("%d", &origem);
            printf("Digite o destino que deseja: ");
            scanf("%d", &dest);

            vet = (int *)malloc(n * sizeof(int));
            vet[0] = origem;

            caminhos(g, dest, vet, 1);
            printf("\n");

            free(vet);
            break;
        case 7:
            system("cls");
            printf("7. Imprimir o caminho mais curto\n");
            printf("\n");

            int menor = n * 2;
            int *vetAux, *vet2;

            printf("Digite a origem que deseja: ");
            scanf("%d", &origem);
            printf("Digite o destino que deseja: ");
            scanf("%d", &dest);

            vet2 = (int *)malloc(n * sizeof(int));
            vet2[0] = origem;
            vetAux = (int *)malloc(n * sizeof(int));

            caminhos_curto(g, dest, vet2, 1, vetAux, &menor);
            for (int i = 0; i < menor; i++)
            {
                printf("%d ", vetAux[i]);
            }

            printf("\n");

            free(vet2);
            free(vetAux);
            break;
        case 8:
            system("cls");
            printf("8. Imprimir o caminho com menos custo\n");

            int *vetAux2, *vet3;
            int menorCusto = 1000;
            int len;

            printf("Digite a origem que deseja: ");
            scanf("%d", &origem);
            printf("Digite o destino que deseja: ");
            scanf("%d", &dest);

            vet3 = (int *)malloc(n * sizeof(int));
            vet3[0] = origem;
            vetAux2 = (int *)malloc(n * sizeof(int));
            caminhos_menor(g, dest, vet3, 1, vetAux2, &menorCusto, 0, &len);
            for (int i = 0; i < len; i++)
            {
                printf("%d ", vetAux2[i]);
            }

            printf("\n");
            free(vet3);
            free(vetAux2);
        default:
            break;
        }
    }

    freeGrafo(g, n);
}