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
    else
    {
        int aux = exist(t->left, x);
        if (aux == 1)
        {
            return 1;
        }

        return exist(t->right, x);
    }
}

int quantity(Tree *t)
{
    if (t == NULL)
    {
        return 0;
    }

    return 1 + quantity(t->left) + quantity(t->right);
}

void print_leaves(Tree *t)
{
    if (t != NULL)
    {
        if (t->left == NULL && t->right == NULL)
        {
            printf("%d ", t->info);
        }
        print_leaves(t->left);
        print_leaves(t->right);
    }
}

int height(Tree *t) // função auxiliar
{
    if (t == NULL)
    {
        return 0;
    }
    else
    {
        int he = height(t->left);
        int hd = height(t->right);
        if (he > hd)
        {
            return he + 1;
        }
        else
        {
            return hd + 1;
        }
    }
}

int is_full(Tree *t)
{
    int quantity_node = quantity(t);
    double h = height(t);
    double pot = pow(2, h);
    if (quantity_node == pot - 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_balanced(Tree *t)
{
    if (t == NULL)
    {
        return 0;
    }
    int he = height(t->left);
    int hd = height(t->right);
    int dif = abs(he - hd);

    if (dif > 1)
    {
        return 0;
    }

    int cond_left = is_balanced(t->left);
    int cond_right = is_balanced(t->right);

    return 1;
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
    int level_left = level_node(t->left, cont + 1, x);
    int level_right = level_node(t->right, cont + 1, x);
    if (level_left == -1)
    {
        return level_right;
    }
    else
    {
        return level_left;
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
    while (condicao != 9)
    {
        printf("\n");
        puts("1 - Ler a arvore de um arquivo fornecido pelo usuario");
        puts("2 - Imprimir a arvore");
        puts("3 - Verificar se um elemento x existe na arvore");
        puts("4 - Contar o numero de elementos da arvore");
        puts("5 - Imprimir os nos folhas da arvore");
        puts("6 - verificar se uma arvore esta balanceada");
        puts("7 - Verificar se uma arvore e cheia");
        puts("8 - Imprimir o nivel de um no x");
        puts("9 - Sair");

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
            if (exist(t, elemento) == 1)
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
            puts("4 - Contar o numero de elementos da arvore");
            printf("\n");
            int quant = quantity(t);
            printf("A arvore possui %d elementos.", quant);
            printf("\n");
        }

        else if (condicao == 5)
        {
            system("clear");
            puts("5 - Imprimir os nos folhas da arvore");
            printf("\n");
            print_leaves(t);
            printf("\n");
        }

        else if (condicao == 6)
        {
            system("clear");
            puts("6 - verificar se uma arvore esta balanceada");
            printf("\n");
            int cond = is_balanced(t);
            if (cond == 1)
            {
                printf("A arvore esta balanceada");
            }
            else
            {
                printf("A arvore nao esta balanceada");
            }
            printf("\n");
        }

        else if (condicao == 7)
        {
            system("clear");
            puts("7 - Verificar se uma arvore e cheia");
            printf("\n");
            int cond = is_full(t);
            if (cond == 1)
            {
                printf("A arvore e cheia");
            }
            else
            {
                printf("A arvore nao e cheia");
            }
            printf("\n");
        }

        else if (condicao == 8)
        {
            system("clear");
            puts("8 - Imprimir o nivel de um no x");
            printf("\n");
            printf("Digite o nó que deseja ver o nível: ");
            int no;
            scanf("%d", &no);
            int nivel = level_node(t, 0, no);
            printf("O nivel do no digitado é: %d", nivel);
            printf("\n");
        }
    }
    printf("\nPrograma encerrado\n");
    free_tree(t);
    return 0;
}
