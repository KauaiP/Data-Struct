#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Tree
{
    int info;
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

Tree *insert(Tree *t, int x)
{
    if (t == NULL)
    {
        t = (Tree *)malloc(sizeof(Tree));
        t->info = x;
        t->left = NULL;
        t->right = NULL;
    }
    else if (x <= t->info)
    {
        t->left = insert(t->left, x);
    }
    else
    {
        t->right = insert(t->right, x);
    }

    return t;
}

Tree *remover(Tree *t, int x)
{
    if (t != NULL)
    {
        if (t->info == x)
        {
            if (t->left == NULL && t->right == NULL)
            {
                free(t);
                return NULL;
            }
            else if (t->left == NULL)
            {
                Tree *aux = t->right;
                free(t);
                return aux;
            }
            else if (t->right == NULL)
            {
                Tree *aux = t->left;
                free(t);
                return aux;
            }
            else
            {
                Tree *aux = t->left;
                while (aux->right != NULL)
                {
                    aux = aux->right;
                }
                t->info = aux->info;
                t->left = remover(t->left, aux->info);
            }
        }
        else if (x < t->info)
        {
            t->left = remover(t->left, x);
        }
        else
        {
            t->right = remover(t->right, x);
        }

        return t;
    }
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
    while (condicao != 8)
    {
        printf("\n");
        puts("1 - Ler a arvore de um arquivo fornecido pelo usuario");
        puts("2 - Imprimir a arvore");
        puts("3 - Verificar se um elemento x existe na arvore");
        puts("4 - Imprimir o nivel de um no x");
        puts("5 - Imprimir os nos folhas menores que um valor x");
        puts("6 - inserir um nó x na arvore");
        puts("7 - removerr um no x da arvore");
        puts("8 - Sair");

        scanf("%d", &condicao);
        getchar();

        if (condicao == 1)
        {
            system("clear");
            puts("1 - Ler a arvore de um arquivo fornecido pelo usuario");
            printf("\n");
            printf("digite o nome do arquivo: ");
            char file_name[20];
            scanf("%19s", file_name);
            FILE *f = fopen(file_name, "r");
            t = read_file(f);
            fclose(f);
        }
        else if (condicao == 2)
        {
            system("clear");
            puts("2 - Imprimir a arvore");
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
            puts("3 - Verificar se um elemento x existe na arvore");
            printf("\n");
            printf("Digite o elemento que deseja verificar: ");
            int elemento;
            scanf("%d", &elemento);
            if (exist(t, elemento))
            {
                puts("O elemento digitado existe na arvore");
            }
            else
            {
                puts("O elemento digitado nao existe na arvore");
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
                puts("o no digitado nao existe na arvore");
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
            puts("6 - inserir um nó x na arvore");
            printf("\n");
            printf("Qual valor que deseja inserir: ");
            int x;
            scanf("%d", &x);
            t = insert(t, x);
        }

        else if (condicao == 7)
        {
            system("clear");
            puts("7 - removerr um no x da arvore");
            printf("\n");
            printf("Digite o no que deseja removerr: ");
            int x;
            scanf("%d", &x);
            t = remover(t, x);
        }
    }
    printf("\nPrograma encerrado\n");
    free_tree(t);
    return 0;
}
