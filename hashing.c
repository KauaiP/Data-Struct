#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SIZE 17

// código feito para uma tabela de dispersão para 17 elementos

typedef struct hashing
{
    int matricula;
    char nome[20];
    char curso[20];
    int disponibilidade;
} aluno;

void inicializar(char *arqName)
{
    FILE *arq = fopen(arqName, "wb");
    aluno a;
    a.disponibilidade = 1;
    for (int i = 0; i < SIZE; i++)
    {
        fwrite(&a, sizeof(aluno), 1, arq);
    }
    fclose(arq);
}

int hash(int key)
{
    key = key % 1000;
    return key % SIZE;
}

int searchPos(char *arqName, int mat)
{
    int pos = hash(mat);
    aluno a;
    FILE *arq = fopen(arqName, "rb");
    fseek(arq, pos * sizeof(aluno), SEEK_SET);
    fread(&a, sizeof(aluno), 1, arq);
    while (a.disponibilidade == 0)
    {
        pos = (pos + 1) % SIZE;
        fseek(arq, pos * sizeof(aluno), SEEK_SET);
        fread(&a, sizeof(aluno), 1, arq);
    }
    fclose(arq);
    return pos;
}

void insert(char *arqName, int mat, char *name, char *course)
{
    int pos = searchPos(arqName, mat);
    FILE *arq = fopen(arqName, "r+b");
    aluno a;
    a.matricula = mat;
    strcpy(a.nome, name);
    strcpy(a.curso, course);
    a.disponibilidade = 0;
    fseek(arq, pos * sizeof(aluno), SEEK_SET);
    fwrite(&a, sizeof(aluno), 1, arq);
    fclose(arq);
}

void searchAndPrint(char *arqName, int mat)
{
    FILE *arq = fopen(arqName, "rb");
    int pos = hash(mat);
    aluno a;
    fseek(arq, pos * sizeof(aluno), SEEK_SET);
    fread(&a, sizeof(aluno), 1, arq);

    while (a.matricula != mat && a.matricula != 0)
    {
        puts("estamos aqui");
        pos = (pos + 1) % SIZE;
        fseek(arq, pos * sizeof(aluno), SEEK_SET);
        fread(&a, sizeof(aluno), 1, arq);
    }

    if (a.matricula == mat)
    {
        printf("Matricula: %d\n", a.matricula);
        printf("Nome: %s\n", a.nome);
        printf("Curso: %s\n", a.curso);
    }
    else
    {
        puts("Não foi encontrada a matricula");
    }
    fclose(arq);
}

void printHashing(char *arqName)
{
    FILE *arq = fopen(arqName, "rb");
    aluno a;
    for (int i = 0; i < SIZE; i++)
    {
        fread(&a, sizeof(aluno), 1, arq);
        printf("posicao %d   matricula: %d\n", i, a.matricula);
    }
    fclose(arq);
}

int main()
{
    char *arqName = "arq";
    inicializar(arqName);

    int cond;
    while (1)
    {
        printf("\n");
        puts("1 - Inserir um novo aluno (matrícula, nome, curso)");
        puts("2 - Imprimir as informacoes de um determinado aluno");
        puts("3 - Imprimir a tabela de dispersao");
        puts("4 - Sair");
        scanf("%d", &cond);
        getchar();

        if (cond == 4)
        {
            break;
        }

        switch (cond)
        {
        case 1:
            system("clear");
            puts("1 - Inserir um novo aluno");
            printf("\n");
            printf("digite a matricula do aluno: ");
            int mat;
            scanf("%d", &mat);
            system("clear");
            printf("digite o nome do aluno: ");
            char name[20];
            scanf("%19s", name);
            system("clear");
            printf("digite o curso do aluno: ");
            char course[20];
            scanf("%19s", course);
            insert(arqName, mat, name, course);
            system("clear");
            break;

        case 2:
            system("clear");
            puts("2 - Imprimir as informacoes de um determinado aluno");
            printf("\nDigite a matricula do aluno desejado: ");
            int x;
            scanf("%d", &x);
            searchAndPrint(arqName, x);
            break;

        case 3:
            system("clear");
            puts("3 - Imprimir a tabela de dispersao");
            printHashing(arqName);
            printf("\n");
            break;

        default:
            break;
        }
    }
    printf("\nPrograma encerrado\n");
    return 0;
}