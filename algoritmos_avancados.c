#include <stdio.h>
#include <stdlib.h>

typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));

    if (nova == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    int i = 0;
    while (nome[i] != '\0' && i < 49) {
        nova->nome[i] = nome[i];
        i++;
    }
    nova->nome[i] = '\0';

    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

void explorarSalas(Sala *atual) {
    char opcao;

    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho! Voce chegou a uma sala final.\n");
            return;
        }

        printf("Escolha um caminho:\n");

        if (atual->esquerda != NULL) {
            printf("e -> ir para a esquerda\n");
        }

        if (atual->direita != NULL) {
            printf("d -> ir para a direita\n");
        }

        printf("s -> sair da exploracao\n");
        printf("Opcao: ");
        scanf(" %c", &opcao);

        if (opcao == 's') {
            printf("Exploracao encerrada.\n");
            return;
        } else if (opcao == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Nao existe caminho para a esquerda.\n");
            }
        } else if (opcao == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Nao existe caminho para a direita.\n");
            }
        } else {
            printf("Opcao invalida.\n");
        }
    }
}

void liberarArvore(Sala *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

int main() {
    Sala *hall = criarSala("Hall de Entrada");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *salaJantar = criarSala("Sala de Jantar");
    Sala *escritorio = criarSala("Escritorio Secreto");
    Sala *quarto = criarSala("Quarto Antigo");
    Sala *cozinha = criarSala("Cozinha Abandonada");
    Sala *porao = criarSala("Porao Escuro");

    hall->esquerda = biblioteca;
    hall->direita = salaJantar;

    biblioteca->esquerda = escritorio;
    biblioteca->direita = quarto;

    salaJantar->esquerda = cozinha;
    salaJantar->direita = porao;

    printf("=== MAPA DA MANSAO ===\n");
    printf("Comecando pelo Hall de Entrada...\n");

    explorarSalas(hall);

    liberarArvore(hall);

    return 0;
}