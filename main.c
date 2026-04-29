#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura
typedef struct {
    int numero;
    char titular[50];
    float saldo;
} Conta;

const char* ARQUIVO = "banco.dat";

// Protótipos das funções
void cadastrarConta();
void consultarSaldo();
void realizarDeposito();
void realizarSaque();
void limparBuffer();

int main() {
    int opcao;

    do {
        printf("\n--- SAFEBANK C - SISTEMA OPERACIONAL ---\n");
        printf("1. Abrir Nova Conta\n");
        printf("2. Consultar Saldo\n");
        printf("3. Deposito\n");
        printf("4. Saque\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            continue;
        }
        limparBuffer();

        switch(opcao) {
            case 1: cadastrarConta(); break;
            case 2: consultarSaldo(); break;
            case 3: realizarDeposito(); break;
            case 4: realizarSaque(); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while(opcao != 0);

    return 0;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cadastrarConta() {
    FILE *file = fopen(ARQUIVO, "ab");
    if (file == NULL) return;

    Conta c;
    printf("Numero da conta: ");
    scanf("%d", &c.numero);
    limparBuffer();

    printf("Nome do titular: ");
    fgets(c.titular, 50, stdin);
    c.titular[strcspn(c.titular, "\n")] = 0;

    c.saldo = 0.0;

    fwrite(&c, sizeof(Conta), 1, file);
    fclose(file);
    printf("Conta cadastrada com sucesso!\n");
}

void realizarDeposito() {
    FILE *file = fopen(ARQUIVO, "rb+");
    if (file == NULL) return;

    int num;
    float valor;
    Conta c;
    int achou = 0;

    printf("Numero da conta: ");
    scanf("%d", &num);

    while(fread(&c, sizeof(Conta), 1, file)) {
        if(c.numero == num) {
            printf("Titular: %s | Saldo: %.2f\n", c.titular, c.saldo);
            printf("Valor do deposito: ");
            scanf("%f", &valor);

            c.saldo += valor;

            // CORREÇÃO DO OVERFLOW: Usando cast para (long)
            fseek(file, -(long)sizeof(Conta), SEEK_CUR);
            fwrite(&c, sizeof(Conta), 1, file);

            achou = 1;
            printf("Deposito realizado!\n");
            break;
        }
    }
    if(!achou) printf("Conta nao encontrada.\n");
    fclose(file);
}

void realizarSaque() {
    FILE *file = fopen(ARQUIVO, "rb+");
    if (file == NULL) return;

    int num;
    float valor;
    Conta c;
    int achou = 0;

    printf("Numero da conta: ");
    scanf("%d", &num);

    while(fread(&c, sizeof(Conta), 1, file)) {
        if(c.numero == num) {
            printf("Valor do saque: ");
            scanf("%f", &valor);

            if(c.saldo >= valor) {
                c.saldo -= valor;
                // CORREÇÃO DO OVERFLOW: Usando cast para (long)
                fseek(file, -(long)sizeof(Conta), SEEK_CUR);
                fwrite(&c, sizeof(Conta), 1, file);
                printf("Saque realizado!\n");
            } else {
                printf("Saldo insuficiente!\n");
            }
            achou = 1;
            break;
        }
    }
    if(!achou) printf("Conta nao encontrada.\n");
    fclose(file);
}

void consultarSaldo() {
    FILE *file = fopen(ARQUIVO, "rb");
    if (file == NULL) return;

    int num;
    Conta c;
    printf("Numero da conta: ");
    scanf("%d", &num);

    while(fread(&c, sizeof(Conta), 1, file)) {
        if(c.numero == num) {
            printf("\n--- EXTRATO ---");
            printf("\nTitular: %s", c.titular);
            printf("\nSaldo: R$ %.2f\n", c.saldo);
            fclose(file);
            return;
        }
    }
    printf("Conta nao encontrada.\n");
    fclose(file);
}