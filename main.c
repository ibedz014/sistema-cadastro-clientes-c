#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM 100
#define RESET "\033[0m"
#define VERDE "\033[1;32m"
#define VERMELHO "\033[1;31m"
#define AZUL "\033[1;34m"
#define AMARELO "\033[1;33m"

// =========================
// ESTRUTURA
// =========================

struct Cliente{
    char nome[TAM];
    int idade;
    char email[TAM];
};

// =========================
// FUNÇÕES AUXILIARES
// =========================

// Linha decorativa
void linha(){
    printf(AZUL "==================================================\n" RESET);
}

// Limpa terminal
void limparTela(){

    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Pausa sistema
void pausarSistema(){

    printf(AMARELO "\nPressione ENTER para continuar..." RESET);
    getchar();
}

// Converte string para minúsculo
void converterMinusculo(char str[]){

    for(int i = 0; str[i]; i++){

        str[i] = tolower(str[i]);
    }
}

// Validação mais profunda de email
int validarEmail(char email[]){

    int arroba = 0;
    int ponto = 0;

    for(int i = 0; email[i] != '\0'; i++){

        if(email[i] == '@'){
            arroba++;
        }

        if(email[i] == '.'){
            ponto = 1;
        }
    }

    if(arroba == 1 && ponto){
        return 1;
    }

    return 0;
}

// =========================
// CADASTRAR CLIENTE
// =========================

void cadastrarCliente(struct Cliente **clientes, int *quantidade){

    *clientes = realloc(*clientes, (*quantidade + 1) * sizeof(struct Cliente));

    if(*clientes == NULL){

        printf(VERMELHO "Erro de memoria!\n" RESET);
        exit(1);
    }

    limparTela();

    linha();
    printf(VERDE "             CADASTRO DE CLIENTE\n" RESET);
    linha();

    // Nome
    printf("Nome  : ");
    fgets((*clientes)[*quantidade].nome, TAM, stdin);
    (*clientes)[*quantidade].nome[strcspn((*clientes)[*quantidade].nome, "\n")] = '\0';

    // Idade
    do{

        printf("Idade : ");
        scanf("%d", &(*clientes)[*quantidade].idade);
        getchar();

        if((*clientes)[*quantidade].idade <= 0){

            printf(VERMELHO "Idade invalida!\n" RESET);
        }

    } while((*clientes)[*quantidade].idade <= 0);

    // Email
    do{

        printf("Email : ");
        fgets((*clientes)[*quantidade].email, TAM, stdin);
        (*clientes)[*quantidade].email[strcspn((*clientes)[*quantidade].email, "\n")] = '\0';

        if(!validarEmail((*clientes)[*quantidade].email)){

            printf(VERMELHO "Email invalido!\n" RESET);
        }

    } while(!validarEmail((*clientes)[*quantidade].email));

    (*quantidade)++;

    linha();
    printf(VERDE "Cliente cadastrado com sucesso!\n" RESET);
    linha();

    pausarSistema();
}

// =========================
// LISTAR CLIENTES
// =========================

void listarClientes(struct Cliente *clientes, int quantidade){

    limparTela();

    linha();
    printf(VERDE "             CLIENTES CADASTRADOS\n" RESET);
    linha();

    if(quantidade == 0){

        printf(VERMELHO "Nenhum cliente cadastrado.\n" RESET);

    } else {

        int pagina = 0;
        int porPagina = 3;

        while(pagina * porPagina < quantidade){

            limparTela();

            linha();
            printf(VERDE "             LISTA DE CLIENTES\n" RESET);
            linha();

            for(int i = pagina * porPagina;
                i < quantidade && i < (pagina + 1) * porPagina;
                i++){

                printf("\nCliente %d\n", i + 1);
                printf("------------------------------------------\n");
                printf("Nome  : %s\n", clientes[i].nome);
                printf("Idade : %d\n", clientes[i].idade);
                printf("Email : %s\n", clientes[i].email);
            }

            pagina++;

            if(pagina * porPagina < quantidade){

                pausarSistema();
            }
        }
    }

    linha();
    pausarSistema();
}

// =========================
// BUSCAR CLIENTE
// =========================

void buscarCliente(struct Cliente *clientes, int quantidade){

    char busca[TAM];
    char nomeTemp[TAM];

    int encontrado = 0;

    limparTela();

    linha();
    printf(VERDE "               BUSCAR CLIENTE\n" RESET);
    linha();

    printf("Digite o nome: ");
    fgets(busca, TAM, stdin);
    busca[strcspn(busca, "\n")] = '\0';

    converterMinusculo(busca);

    for(int i = 0; i < quantidade; i++){

        strcpy(nomeTemp, clientes[i].nome);

        converterMinusculo(nomeTemp);

        if(strcmp(nomeTemp, busca) == 0){

            printf("\nCliente encontrado!\n");
            printf("------------------------------------------\n");
            printf("Nome  : %s\n", clientes[i].nome);
            printf("Idade : %d\n", clientes[i].idade);
            printf("Email : %s\n", clientes[i].email);

            encontrado = 1;
        }
    }

    if(!encontrado){

        printf(VERMELHO "\nCliente nao encontrado.\n" RESET);
    }

    linha();
    pausarSistema();
}

// =========================
// EDITAR CLIENTE
// =========================

void editarCliente(struct Cliente *clientes, int quantidade){

    int indice;

    limparTela();

    linha();
    printf(VERDE "               EDITAR CLIENTE\n" RESET);
    linha();

    listarClientes(clientes, quantidade);

    printf("\nDigite o numero do cliente: ");
    scanf("%d", &indice);
    getchar();

    indice--;

    if(indice < 0 || indice >= quantidade){

        printf(VERMELHO "Cliente invalido!\n" RESET);
        pausarSistema();
        return;
    }

    printf("\nNovo nome: ");
    fgets(clientes[indice].nome, TAM, stdin);
    clientes[indice].nome[strcspn(clientes[indice].nome, "\n")] = '\0';

    do{

        printf("Nova idade: ");
        scanf("%d", &clientes[indice].idade);
        getchar();

    } while(clientes[indice].idade <= 0);

    do{

        printf("Novo email: ");
        fgets(clientes[indice].email, TAM, stdin);
        clientes[indice].email[strcspn(clientes[indice].email, "\n")] = '\0';

    } while(!validarEmail(clientes[indice].email));

    printf(VERDE "\nCliente atualizado!\n" RESET);

    pausarSistema();
}

// =========================
// REMOVER CLIENTE
// =========================

void removerCliente(struct Cliente **clientes, int *quantidade){

    int indice;

    limparTela();

    linha();
    printf(VERDE "               REMOVER CLIENTE\n" RESET);
    linha();

    listarClientes(*clientes, *quantidade);

    printf("\nDigite o numero do cliente: ");
    scanf("%d", &indice);
    getchar();

    indice--;

    if(indice < 0 || indice >= *quantidade){

        printf(VERMELHO "Cliente invalido!\n" RESET);
        pausarSistema();
        return;
    }

    for(int i = indice; i < *quantidade - 1; i++){

        (*clientes)[i] = (*clientes)[i + 1];
    }

    (*quantidade)--;

    *clientes = realloc(*clientes, (*quantidade) * sizeof(struct Cliente));

    printf(VERDE "\nCliente removido com sucesso!\n" RESET);

    pausarSistema();
}

// =========================
// MAIN
// =========================

int main(){

    struct Cliente *clientes = NULL;

    int quantidade = 0;
    int opcao;

    do{

        limparTela();

        linha();
        printf(VERDE "         SISTEMA DE CADASTRO DE CLIENTES\n" RESET);
        linha();

        printf("1 - Cadastrar cliente\n");
        printf("2 - Listar clientes\n");
        printf("3 - Buscar cliente\n");
        printf("4 - Editar cliente\n");
        printf("5 - Remover cliente\n");
        printf("0 - Sair\n");

        linha();

        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao){

            case 1:

                cadastrarCliente(&clientes, &quantidade);

                break;

            case 2:

                listarClientes(clientes, quantidade);

                break;

            case 3:

                buscarCliente(clientes, quantidade);

                break;

            case 4:

                editarCliente(clientes, quantidade);

                break;

            case 5:

                removerCliente(&clientes, &quantidade);

                break;

            case 0:

                limparTela();

                linha();
                printf(VERDE "Encerrando sistema...\n" RESET);
                linha();

                break;

            default:

                printf(VERMELHO "\nOpcao invalida!\n" RESET);

                pausarSistema();
        }

    } while(opcao != 0);

    // Libera memória
    free(clientes);

    return 0;
}
