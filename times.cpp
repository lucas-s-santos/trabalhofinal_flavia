#include <stdio.h>
#include <string.h>

struct team {
    char nome[50];
    int anoFun;
    char status;
    char liga[50];
};

int tam(FILE *);
void cad(FILE *);
void con(FILE *);
void ex(FILE *);
void gerarArquivo(FILE *);

int main() {
    int op;
    FILE *arq;

    if (!(arq = fopen("times.dat", "rb+"))) {
        if (!(arq = fopen("times.dat", "wb+"))) {
            printf("Falha ao abrir o arquivo!\n");
            return 1;
        }
    }

    do {
        printf("\n       GESTAO DE TIMES       \n \n");
        printf("1- Cadastrar time\n2- Consultar time\n3- Excluir time\n4- Gerar arquivo\n5- Sair\n        TIMES CADASTRADOS: %d\n \nEscolha uma opcao: ", tam(arq));
        scanf("%d", &op);

        switch (op) {
            case 1: cad(arq); break;
            case 2: con(arq); break;
            case 3: ex(arq); break;
            case 4: gerarArquivo(arq); break;
            case 5: fclose(arq); break;
        }
    } while (op != 5);

    return 0;
}

void cad(FILE *arq) {
    struct team novoTime;
    char confirma[2];

    novoTime.status = ' ';

    printf("Cadastrando novo time:\nNome do time: ");
    getchar();
    fgets(novoTime.nome, 50, stdin);
    novoTime.nome[strcspn(novoTime.nome, "\n")] = 0; 

    printf("Ano de fundacao: "); scanf("%d", &novoTime.anoFun);
    printf("Liga: ");
    getchar(); 
    fgets(novoTime.liga, 50, stdin);
    novoTime.liga[strcspn(novoTime.liga, "\n")] = 0;

    printf("\nConfirma cadastro? <s/n>: "); scanf("%1s", confirma);

    if (confirma[0] == 's' || confirma[0] == 'S') {
        fseek(arq, 0, SEEK_END);
        fwrite(&novoTime, sizeof(struct team), 1, arq);
        printf("\nTime cadastrado com sucesso!\n");
    } else {
        printf("\nCadastro cancelado.\n");
    }
}

void con(FILE *arq) {
    struct team consultTime;
    int cod;

    printf("\nConsultar time\nInforme o codigo do time: "); scanf("%d", &cod);

    if ((cod <= tam(arq)) && (cod > 0)) {
        fseek(arq, (cod - 1) * sizeof(struct team), SEEK_SET);
        fread(&consultTime, sizeof(struct team), 1, arq);

        if (consultTime.status == ' ') {
            printf("\nNome do time: %s\nAno de fundacao: %d\nLiga: %s\n", consultTime.nome, consultTime.anoFun, consultTime.liga);
        } else {
            printf("\nTime nao existe ou foi excluido!\n");
        }
    } else {
        printf("\nCodigo de time invalido!\n");
    }
}

void ex(FILE *arq) {
    struct team delTime;
    char confirma[2];
    int cod;

    printf("\nExcluir time\nInforme o codigo do time a ser excluido: "); scanf("%d", &cod);

    if ((cod <= tam(arq)) && (cod > 0)) {
        fseek(arq, (cod - 1) * sizeof(struct team), SEEK_SET);
        fread(&delTime, sizeof(struct team), 1, arq);

        if (delTime.status == ' ') {
            printf("\nNome do time: %s\nAno de fundacao: %d\nLiga: %s\nConfirma exclusao? <s/n>: ", delTime.nome, delTime.anoFun, delTime.liga);
            scanf("%1s", confirma);

            if (confirma[0] == 's' || confirma[0] == 'S') {
                fseek(arq, (cod - 1) * sizeof(struct team), SEEK_SET);
                delTime.status = '*';
                fwrite(&delTime, sizeof(struct team), 1, arq);
                printf("\nTime excluido com sucesso!\n");
            }
        } else {
            printf("Time inexistente!\n");
        }
    } else {
        printf("\nCodigo de time invalido!\n");
    }
}

void gerarArquivo(FILE *arq) {
    FILE *arqTexto;
    struct team consultTime;
    int numTimes = tam(arq);

    if (numTimes == 0) {
        printf("\nNenhum time cadastrado para gerar arquivo!\n");
        return;
    }

    if (!(arqTexto = fopen("times.txt", "w"))) {
        printf("Falha ao criar o arquivo de texto!\n");
        return;
    }

    fprintf(arqTexto, "LISTA DE TIMES CADASTRADOS:\n\n");

    for (int i = 0; i < numTimes; ++i) {
        fseek(arq, i * sizeof(struct team), SEEK_SET);
        fread(&consultTime, sizeof(struct team), 1, arq);

        if (consultTime.status == ' ') {
            fprintf(arqTexto, "Time %d:\n", i + 1);
            fprintf(arqTexto, "Nome: %s\n", consultTime.nome);
            fprintf(arqTexto, "Ano de fundacao: %d\n", consultTime.anoFun);
            fprintf(arqTexto, "Liga: %s\n\n", consultTime.liga);
        }
    }

    fclose(arqTexto);
    printf("\nArquivo 'times.txt' gerado com sucesso!\n");
}

int tam(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(struct team);
}
