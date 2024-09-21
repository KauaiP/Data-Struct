#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct professor
{
    int matricula;
    char nome[50];
    float salario;
} Professor;

typedef struct aluno
{
    int matricula;
    char nome[50];
    char curso[50];
    int anoIngresso;
} Aluno;

typedef struct pessoa
{
    int tipo;
    void *item;
    struct pessoa *prox;
} Pessoa;

Pessoa *inicializar()
{
    Pessoa *node = (Pessoa *)malloc(sizeof(Pessoa));
    node->prox = NULL;
    return node;
}

void destruir(Pessoa *lista)
{
    if (lista != NULL)
    {
        destruir(lista->prox);
        free(lista->item);
        free(lista);
    }
}

Pessoa *inserir(Pessoa *lista, int tipo, void *info)
{
    Pessoa *new_node = (Pessoa *)malloc(sizeof(Pessoa));

    new_node->tipo = tipo;
    new_node->item = info;
    new_node->prox = lista;

    return new_node;
}

int verifica_cabeca(Pessoa *lista, int matricula)
{
    if (lista->tipo == 2)
    {
        Professor *p = (Professor *)lista->item;
        if (p->matricula == matricula)
        {
            return 1;
        }
    }
    else if (lista->tipo == 1)
    {
        Aluno *a = (Aluno *)lista->item;
        if (a->matricula == matricula)
        {
            return 1;
        }
    }
    return 0;
}

Pessoa *removerMat(int matricula, Pessoa *lista)
{

    if (verifica_cabeca(lista, matricula))
    {
        Pessoa *new_list = lista->prox;
        free(lista);
        return new_list;
    }

    Pessoa *q = lista;
    Pessoa *n = q->prox;
    while (n != NULL)
    {
        if (n->tipo == 2)
        {
            Professor *p = (Professor *)n->item;
            if (p->matricula == matricula)
            {
                q->prox = n->prox;
                free(n);
                break;
            }
        }
        else
        {
            Aluno *a = (Aluno *)n->item;
            if (a->matricula == matricula)
            {
                q->prox = n->prox;
                free(n);
                break;
            }
        }
        q = n;
        n = n->prox;
    }
    return lista;
}

void buscarMat(int matricula, Pessoa *lista)
{
    Pessoa *aux = lista;
    while (aux != NULL)
    {
        if (aux->tipo == 2)
        {
            Professor *p = (Professor *)aux->item;
            if (p->matricula == matricula)
            {
                printf("Professor:\n");
                printf("matricula: %d\n", p->matricula);
                printf("Nome: %s\n", p->nome);
                printf("Salario: %f\n", p->salario);
                return;
            }
        }
        else
        {
            Aluno *a = (Aluno *)aux->item;
            if (a->matricula == matricula)
            {
                printf("Aluno:\n");
                printf("matricula: %d\n", a->matricula);
                printf("Nome: %s\n", a->nome);
                printf("Curso: %s\n", a->curso);
                printf("Ano de ingresso: %d", a->anoIngresso);
                return;
            }
        }
        aux = aux->prox;
    }

    printf("Matricula nao encontrada.\n");
}

int alunosPorCurso(char *curso, Pessoa *lista)
{
    int cont = 0;
    Pessoa *aux = lista;
    while (aux != NULL)
    {
        if (aux->tipo == 1)
        {
            Aluno *a = (Aluno *)aux->item;
            if (strcmp(a->curso, curso) == 0)
            {
                cont++;
            }
        }
        aux = aux->prox;
    }

    return cont;
}

void maiorSalario(Pessoa *lista)
{
    Pessoa *aux = lista;
    int maior = 0;
    while (aux != NULL)
    {
        if (aux->tipo == 2)
        {
            Professor *p = (Professor *)aux->item;
            if (p->salario > maior)
            {
                maior = p->salario;
            }
        }
        aux = aux->prox;
    }

    Pessoa *aux2 = lista;
    while (aux2 != NULL)
    {
        if (aux2->tipo == 2)
        {
            Professor *p = (Professor *)aux2->item;
            if (p->salario == maior)
            {
                printf("Professor %s  salario: %f\n", p->nome, p->salario);
            }
        }
        aux2 = aux2->prox;
    }
}

int main()
{
    Pessoa *lista = inicializar();
    int escolha = 0;
    int mat;
    while (escolha != 6)
    {
        printf("\n");
        printf("1. Inserir uma pessoa na lista\n");
        printf("2. Remover uma pessoa pela matricula\n");
        printf("3. Buscar uma pessoa pela matricula\n");
        printf("4. Contar o numero de alunos de um determinado curso\n");
        printf("5. Imprimir os professores de maior salario\n");
        printf("6. Sair");
        printf("\n");

        scanf("%d", &escolha);
        getchar();

        switch (escolha)
        {
        case 1:
            printf("1. Inserir uma pessoa na lista\n");
            printf("\n");

            printf("Deseja inserir um aluno (1) ou um professor (2)?: ");
            int tipo;
            scanf("%d", &tipo);

            if (tipo == 1)
            {
                Aluno *a = (Aluno *)malloc(sizeof(Aluno));
                printf("Digite a matricula: ");
                scanf("%d", &a->matricula);
                printf("Digite o nome do aluno: ");
                scanf("%s", a->nome);
                printf("Digite o curso do aluno: ");
                scanf("%s", a->curso);
                printf("Digite o ano de ingresso do aluno: ");
                scanf("%d", &a->anoIngresso);
                lista = inserir(lista, 1, a);
            }
            else
            {
                Professor *p = (Professor *)malloc(sizeof(Professor));
                printf("Digite a matricula: ");
                scanf("%d", &p->matricula);
                printf("Digite o nome do professor: ");
                scanf("%s", p->nome);
                printf("Digite o salario do professor: ");
                scanf("%f", &p->salario);
                lista = inserir(lista, 2, p);
            }
            break;

        case 2:
            printf("2. Remover uma pessoa pela matricula\n");
            printf("\n");

            printf("Digite a matricula que deseja remover: ");
            scanf("%d", &mat);
            lista = removerMat(mat, lista);
            break;

        case 3:
            printf("3. Buscar uma pessoa pela matricula\n");
            printf("\n");

            printf("Digite a matricula que deseja buscar: ");
            scanf("%d", &mat);
            buscarMat(mat, lista);
            break;
        case 4:
            printf("4. Contar o numero de alunos de um determinado curso\n");
            printf("\n");

            printf("Digite o curso que deseja analisar: ");
            char curso[50];
            scanf("%s", curso);

            int result = alunosPorCurso(curso, lista);
            printf("quantidade de alunos nesse curso: %d", result);
            break;

        case 5:
            printf("5. Imprimir os professores de maior salario\n");
            printf("\n");

            maiorSalario(lista);
            break;

        default:
            break;
        }
    }

    destruir(lista);
}
