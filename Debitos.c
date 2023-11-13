#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

struct Transacao {
    char cliente[30];
    char data[30];
    float valor;
    char frentista[30];
};


void salvarTransacao(const struct Transacao *transacao);
void listarArquivos();
float somarValores(const char *cliente);

int main(void) {
    struct Transacao transacao;
    int opcao;

    do {
        printf("\n1. Nova transacao\n");
        printf("2. Listar arquivos\n");
        printf("3. Somar valores para cliente\n");
        printf("0. Sair\n");
        printf("Escolha a opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Enter cliente name (enter 0 to stop): ");
                scanf("%s", transacao.cliente);

                while (strcmp(transacao.cliente, "0") != 0) {
                    printf("Enter data: ");
                    scanf("%s", transacao.data);
                    printf("Enter valor: ");
                    scanf("%f", &transacao.valor);
                    printf("Enter frentista: ");
                    scanf("%s", transacao.frentista);

                    salvarTransacao(&transacao);

                    printf("Enter cliente name (enter 0 to stop): ");
                    scanf("%s", transacao.cliente);
                }
                break;

            case 2:
                listarArquivos();
                break;

            case 3:
                printf("Enter cliente name: ");
                scanf("%s", transacao.cliente);
                printf("\n/////////////////////////////////////////////\nSoma dos valores para o cliente %s: R$ %.2f\n", transacao.cliente, somarValores(transacao.cliente));
                break;

            case 0:
                printf("Saindo do programa.\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

void salvarTransacao(const struct Transacao *transacao) {
    char filename[50];
    sprintf(filename, "pathdiretorio\\%s.txt", transacao->cliente);

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(file, "\n~////////////////////////~ \n\nCliente: %s\n", transacao->cliente);
    fprintf(file, "Data: %s\n", transacao->data);
    fprintf(file, "Valor: R$ %.2f\n", transacao->valor);
    fprintf(file, "Frentista: %s\n\n~////////////////////////~ ", transacao->frentista);

    fclose(file);
}

void listarArquivos() {
#ifdef _WIN32
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("pathdiretorio\\*.txt", &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error opening directory\n");
        exit(1);
    }

    do {
        printf("%s\n", findFileData.cFileName);
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
#else
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir("pathdiretorio")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt") != NULL) {
                printf("%s\n", ent->d_name);
            }
        }
        closedir(dir);
    } else {
        perror("Error opening directory");
        exit(1);
    }
#endif
}

float somarValores(const char *cliente) {
    float soma = 0.0;
#ifdef _WIN32
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("pathdiretorio\\*.txt", &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error opening directory\n");
        exit(1);
    }

    do {
        char filename[256];
        sprintf(filename, "pathdiretorio\\%s", findFileData.cFileName);
        FILE *file = fopen(filename, "r");

        if (file != NULL) {
            char line[256];
            while (fgets(line, sizeof(line), file) != NULL) {
                if (strstr(line, "Cliente:")) {
                    char transacaoCliente[30];
                    sscanf(line, "Cliente: %s", transacaoCliente);

                    if (strcmp(transacaoCliente, cliente) == 0) {
                        while (fgets(line, sizeof(line), file) != NULL) {
                            if (strstr(line, "Valor:")) {
                                float valor;
                                sscanf(line, "Valor: R$ %f", &valor);
                                soma += valor;
                            }
                        }
                    }

                    break;  
                }
            }

            fclose(file);
        }

    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    return soma;
#else
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir("pathdiretorio")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt") != NULL) {
                char filename[256];
                sprintf(filename, "pathdiretorio\\%s", ent->d_name);
                FILE *file = fopen(filename, "r");

                if (file != NULL) {
                    char line[256];
                    while (fgets(line, sizeof(line), file) != NULL) {
                        if (strstr(line, "Cliente:")) {
                            char transacaoCliente[30];
                            sscanf(line, "Cliente: %s", transacaoCliente);

                            if (strcmp(transacaoCliente, cliente) == 0) {
                                while (fgets(line, sizeof(line), file) != NULL) {
                                    if (strstr(line, "Valor:")) {
                                        float valor;
                                        sscanf(line, "Valor: R$ %f", &valor);
                                        soma += valor;
                                    }
                                }
                            }

                            break;  
                        }
                    }

                    fclose(file);
                }
            }
        }
        closedir(dir);
    } else {
        perror("Error opening directory");
        exit(1);
    }

    return soma;
#endif
}
