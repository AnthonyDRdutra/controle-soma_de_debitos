# controle/soma_de_debitos(WIP)
Projeto criado para listagem, soma e revisão de débitos de clientes.

Utilizando C, o código gera um arquivo.txt que irá armazenar nome do cliente, data, valor e frentista, o usuário pode solicitar o cadastro de um novo cliente, listagem de clientes ou a soma do débito de um cliente específico. 

O código utiliza de diretivas de pré-processador para incluir diferentes conjuntos de cabeçalhos, dependendo do sistema operacional em que o código está sendo compilado.
```
#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif
```
Usando de `sfprintf` salvaremos no buffer seus caracteres e valores e com `fopen` em modo `'append'`, escrevermos os inputs sem sobrescrever os dados ja contidos no .txt:
```
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
}
```
Para a listagem dos .txt já gerados, utilizaremos de códigos dentro do ambiente do Windows utilizando de sua API, com `WIN32_FIND_DATA`, `Handle` e `FindFirstFile()` e iremos imprimir uma mensagem de erro caso não seja possivel concluir a operação:
````
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
````


Quando cadastrado o .txt será formatado desta maneira:
```
~////////////////////////~ 

Cliente: exemplo
Data: 12/02/2023
Valor: R$ 340.00
Frentista: frentista

~////////////////////////~ 
~////////////////////////~ 

Cliente: exemplo
Data: 13/02/2023
Valor: R$ 800.00
Frentista: frentista

~////////////////////////~ 
```
Este será o output no terminal ou cmd: 
```
1. Nova transacao
2. Listar arquivos
3. Somar valores para cliente
0. Sair
Escolha a opcao: 3
2.txt
exemplo.txt
Enter cliente name: 2

/////////////////////////////////////////////
Soma dos valores para o cliente 2: R$ 800.00
```
