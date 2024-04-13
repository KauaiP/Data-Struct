#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Tree
{
    int info;
    int FB;
    struct Tree *left;
    struct Tree *right;
} Tree;

typedef struct Queue
{ // estrutura de fila para aux na impressão por largura pelo método BFS
    Tree *node;
    struct Queue *next;
} Queue;

void enqueue(Queue **front, Queue **rear, Tree *t)
{
    Queue *temp = malloc(sizeof(Queue));
    temp->node = t;
    temp->next = NULL;

    if (*front == NULL)
    {
        *front = *rear = temp;
    }
    else
    {
        (*rear)->next = temp;
        *rear = temp;
    }
}

Tree *dequeue(Queue **front, Queue **rear)
{
    if (*front == NULL)
        return NULL;

    Queue *temp = *front;
    Tree *t = temp->node;

    if (*front == *rear)
    {
        *front = *rear = NULL;
    }
    else
    {
        *front = (*front)->next;
    }

    free(temp);
    return t;
}
// fim da queue e dequeue para auxliar na impressão da fila por ordem

Tree *read_file(FILE *arq)
{
    char c;
    int num;

    fscanf(arq, "%c", &c);
    fscanf(arq, "%d", &num);

    if (num == -1)
    {
        fscanf(arq, "%c", &c);
        return NULL;
    }
    else
    {
        Tree *t = malloc(sizeof(Tree));
        t->info = num;
        t->left = read_file(arq);
        t->right = read_file(arq);
        fscanf(arq, "%c", &c);
        return t;
    }
}

int height(Tree *t)
{
    if (t == NULL)
    {
        return 0;
    }
    else
    {
        int hl = height(t->left);
        int hr = height(t->right);
        if (hl > hr)
        {
            return hl + 1;
        }
        else
        {
            return hr + 1;
        }
    }
}

void adjust_FB(Tree *t)
{
    if (t != NULL)
    {
        int hl = height(t->left);
        int hr = height(t->right);
        t->FB = abs(hr - hl);

        adjust_FB(t->left);
        adjust_FB(t->right);
    }
}

Tree *read_file_Tree(FILE *arq)
{
    Tree *t = read_file(arq);
    adjust_FB(t);
    return t;
}

void print_pre(Tree *t)
{
    if (t != NULL)
    {
        printf("%d ", t->info);
        print_pre(t->left);
        print_pre(t->right);
    }
}

void print_in(Tree *t)
{
    if (t != NULL)
    {
        print_in(t->left);
        printf("%d ", t->info);
        print_in(t->right);
    }
}

void print_pos(Tree *t)
{
    if (t != NULL)
    {
        print_pos(t->left);
        print_pos(t->right);
        printf("%d ", t->info);
    }
}

void printLevelOrder(Queue *front, Queue *rear)
{
    if (front == NULL)
        return;

    Tree *t = dequeue(&front, &rear);
    printf("%d ", t->info);

    if (t->left != NULL)
        enqueue(&front, &rear, t->left);
    if (t->right != NULL)
        enqueue(&front, &rear, t->right);

    printLevelOrder(front, rear);
}

void printBFS(Tree *t)
{
    if (t == NULL)
        return;

    Queue *front = NULL;
    Queue *rear = NULL;
    enqueue(&front, &rear, t);

    printLevelOrder(front, rear);
}

int exist(Tree *t, int x)
{
    if (t == NULL)
    {
        return 0;
    }
    else if (t->info == x)
    {
        return 1;
    }
    else if (x <= t->info)
    {
        return exist(t->left, x);
    }
    else
    {
        return exist(t->right, x);
    }
}

void print_leaves(Tree *t, int x)
{
    if (t != NULL)
    {
        if (t->info < x)
        {
            if (t->left == NULL && t->right == NULL)
            {
                printf("%d ", t->info);
            }

            print_leaves(t->left, x);
            print_leaves(t->right, x);
        }
        else
        {
            print_leaves(t->left, x);
        }
    }
}

int level_node(Tree *t, int cont, int x)
{
    if (t == NULL)
    {
        return -1;
    }

    if (t->info == x)
    {
        return cont;
    }
    if (x <= t->info)
    {
        int level_left = level_node(t->left, cont + 1, x);
    }
    else
    {
        int level_right = level_node(t->right, cont + 1, x);
    }
}

Tree *rotacaoleftSimples(Tree *t)
{

    Tree *aux = t;
    Tree *aux2 = aux->right;

    aux->right = aux2->left;
    aux2->left = aux;

    if (aux2->FB == 1)
    {
        aux->FB = 0;
        aux2->FB = 0;
    }
    else
    {
        aux->FB = 1;
        aux2->FB = -2;
    }

    t = aux2;
    return t;
}

Tree *rotacaoleftDupla(Tree *t)
{

    Tree *aux = t;
    Tree *aux2 = aux->right;
    Tree *aux3 = aux2->left;

    aux2->left = aux3->right;
    aux->right = aux3->left;
    aux3->left = aux;
    aux3->right = aux2;

    switch (aux3->FB)
    {
    case -1:
        aux->FB = 0;
        aux2->FB = 1;
        break;
    case 0:
        aux->FB = 0;
        aux2->FB = 0;
        break;
    case +1:
        aux->FB = -1;
        aux2->FB = 0;
        break;
    }

    aux3->FB = 0;
    t = aux3;
    return t;
}

Tree *rotacaoleft(Tree *t)
{
    if (t->right->FB == -1)
        t = rotacaoleftDupla(t);
    else
        t = rotacaoleftSimples(t);

    return t;
}

Tree *rotacaorightSimples(Tree *t)
{

    Tree *aux = t->left;
    Tree *aux2 = t;

    aux2->left = aux->right;
    aux->right = aux2;

    if (aux->FB == -1)
    {
        aux->FB = 0;
        aux2->FB = 0;
    }
    else
    {
        aux->FB = 1;
        aux2->FB = -1;
    }

    t = aux;
    return t;
}

Tree *rotacaorightDupla(Tree *t)
{

    Tree *aux = t;
    Tree *aux2 = aux->left;
    Tree *aux3 = aux2->right;

    aux->left = aux3->right;
    aux2->right = aux3->left;
    aux3->left = aux2;
    aux3->right = aux;

    switch (aux3->FB)
    {
    case -1:
        aux2->FB = 0;
        aux->FB = 1;
        break;
    case 0:
        aux2->FB = 0;
        aux->FB = 0;
        break;
    case +1:
        aux2->FB = -1;
        aux->FB = 0;
        break;
    }

    aux3->FB = 0;
    t = aux3;
    return t;
}

Tree *rotacaoright(Tree *t)
{
    if (t->left->FB == 1)
        t = rotacaorightDupla(t);
    else
        t = rotacaorightSimples(t);

    return t;
}

Tree *inserir(Tree *t, int info, int *hMudou)
{

    if (t == NULL)
    {
        t = (Tree *)malloc(sizeof(Tree));
        t->info = info;
        t->left = NULL;
        t->right = NULL;
        t->FB = 0;
        *hMudou = 1;
    }
    else
    {
        if ((t->info > info) || (t->info == info))
        {
            t->left = inserir(t->left, info, hMudou);

            if (*hMudou == 1)
            {
                switch (t->FB)
                {
                case -1:
                    t = rotacaoright(t);
                    *hMudou = 0;
                    break;
                case 0:
                    t->FB = -1;
                    *hMudou = 1;
                    break;
                case +1:
                    t->FB = 0;
                    *hMudou = 0;
                    break;
                }
            }
        }
        else
        {
            t->right = inserir(t->right, info, hMudou);
            if (*hMudou == 1)
            {
                switch (t->FB)
                {
                case -1:
                    t->FB = 0;
                    *hMudou = 0;
                    break;
                case 0:
                    t->FB = +1;
                    *hMudou = 1;
                    break;
                case +1:
                    t = rotacaoleft(t);
                    *hMudou = 0;
                    break;
                }
            }
        }
    }
    return t;
}

Tree *remover(Tree *t, int info, int *hMudou)
{
    if (t != NULL)
    {
        if (t->info == info)
        {

            if ((t->left == NULL) && (t->right == NULL))
            {
                free(t);
                *hMudou = 1;
                return NULL;
            }
            else if ((t->left == NULL) || (t->right == NULL))
            {
                Tree *aux;
                if (t->left == NULL)
                {
                    aux = t->right;
                }
                else
                    aux = t->left;
                free(t);
                *hMudou = 1;
                return aux;
            }
            else
            {
                Tree *maiorleft = t->left;
                while (maiorleft->right != NULL)
                    maiorleft = maiorleft->right;

                t->info = maiorleft->info;
                t->left = remover(t->left, t->info, hMudou);

                if (*hMudou == 1)
                {
                    switch (t->FB)
                    {
                    case -1:
                        t->FB = 0;
                        *hMudou = 1;
                        break;
                    case 0:
                        t->FB = 1;
                        *hMudou = 0;
                        break;
                    case +1:
                        int aux2 = t->right->FB;
                        t = rotacaoleft(t);

                        if (aux2 == 0)
                            *hMudou = 0;
                        else
                            *hMudou = 1;
                        break;
                    }
                }
            }
        }
        else if (t->info > info)
        {
            t->left = remover(t->left, info, hMudou);

            if (*hMudou == 1)
            {
                switch (t->FB)
                {
                case -1:
                    t->FB = 0;
                    *hMudou = 1;
                    break;
                case 0:
                    t->FB = 1;
                    *hMudou = 0;
                    break;
                case +1:
                    int aux3 = t->right->FB;
                    t = rotacaoleft(t);
                    if (aux3 == 0)
                        *hMudou = 0;
                    else
                        *hMudou = 1;
                    break;
                }
            }
        }
        else
        {
            t->right = remover(t->right, info, hMudou);
            if (*hMudou == 1)
            {
                switch (t->FB)
                {
                case +1:
                    t->FB = 0;
                    *hMudou = 1;
                    break;
                case 0:
                    t->FB = -1;
                    *hMudou = 0;
                    break;
                case -1:
                    int aux4 = t->left->FB;
                    t = rotacaoright(t);

                    if (aux4 == 0)
                        *hMudou = 0;
                    else
                        *hMudou = 1;
                    break;
                }
            }
        }
    }

    return t;
}

void free_tree(Tree *t)
{
    if (t != NULL)
    {
        free_tree(t->left);
        free_tree(t->right);
        free(t);
    }
}

int main()
{
    Tree *t = NULL;
    int condicao = 0;
    int hMudou = 0;
    while (condicao != 8)
    {
        printf("\n");
        puts("1 - Ler a Tree de um arquivo fornecido pelo usuario");
        puts("2 - Imprimir a Tree");
        puts("3 - Verificar se um elemento x existe na Tree");
        puts("4 - Imprimir o nivel de um no x");
        puts("5 - Imprimir os nos folhas menores que um valor x");
        puts("6 - inserir um nó x na Tree");
        puts("7 - remover um no x da Tree");
        puts("8 - Sair");

        scanf("%d", &condicao);
        getchar();

        if (condicao == 1)
        {
            system("clear");
            puts("1 - Ler a Tree de um arquivo fornecido pelo usuario");
            printf("\n");
            printf("digite o nome do arquivo: ");
            char file_name[20];
            scanf("%19s", file_name);
            FILE *f = fopen(file_name, "r");
            t = read_file_Tree(f);
            fclose(f);
        }
        else if (condicao == 2)
        {
            system("clear");
            puts("2 - Imprimir a Tree");
            printf("\n");
            puts("Escolha a maneira que deseja imprimir:");
            puts("pre-order: 1");
            puts("in_order: 2");
            puts("pos-order: 3");
            puts("em largura: 4");
            int escolha;
            scanf("%d", &escolha);
            system("clear");
            printf("Impressão: ");

            if (escolha == 1)
                print_pre(t);
            else if (escolha == 2)
                print_in(t);
            else if (escolha == 3)
                print_pos(t);
            else if (escolha == 4)
                printBFS(t);
            printf("\n");
        }
        else if (condicao == 3)
        {
            system("clear");
            puts("3 - Verificar se um elemento x existe na Tree");
            printf("\n");
            printf("Digite o elemento que deseja verificar: ");
            int elemento;
            scanf("%d", &elemento);
            if (exist(t, elemento))
            {
                puts("O elemento digitado existe na Tree");
            }
            else
            {
                puts("O elemento digitado nao existe na Tree");
            }
            printf("\n");
        }

        else if (condicao == 4)
        {
            system("clear");
            puts("4 - Imprimir o nivel de um no x");
            printf("\n");
            printf("Digite o no x: ");
            int x;
            scanf("%d", &x);
            int level = level_node(t, 0, x);
            if (level == -1)
            {
                puts("o no digitado nao existe na Tree");
            }
            else
            {
                printf("O nivel do no requisitado e: %d", level);
            }

            printf("\n");
        }

        else if (condicao == 5)
        {
            system("clear");
            puts("5 - Imprimir os nos folhas menores que um valor x");
            printf("\n");
            printf("Digite o valor x: ");
            int x;
            scanf("%d", &x);
            print_leaves(t, x);
            printf("\n");
        }

        else if (condicao == 6)
        {
            system("clear");
            puts("6 - inserir um nó x na Tree");
            printf("\n");
            printf("Qual valor que deseja inserir: ");
            int x;
            scanf("%d", &x);
            t = inserir(t, x, &hMudou);
        }

        else if (condicao == 7)
        {
            system("clear");
            puts("7 - removerr um no x da Tree");
            printf("\n");
            printf("Digite o no que deseja remover: ");
            int x;
            scanf("%d", &x);
            t = remover(t, x, &hMudou);
        }
    }
    printf("\nPrograma encerrado\n");
    free_tree(t);
    return 0;
}
