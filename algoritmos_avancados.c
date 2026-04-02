#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[50];
    char pista[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

typedef struct NoPista {
    char pista[50];
    struct NoPista *esq;
    struct NoPista *dir;
} NoPista;

Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memoria para sala.\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

NoPista* criarNoPista(const char *pista) {
    NoPista *novo = (NoPista*) malloc(sizeof(NoPista));
    if (novo == NULL) {
        printf("Erro ao alocar memoria para pista.\n");
        exit(1);
    }

    strcpy(novo->pista, pista);
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

NoPista* inserirPista(NoPista *raiz, const char *pista) {
    if (raiz == NULL) {
        return criarNoPista(pista);
    }

    int comp = strcmp(pista, raiz->pista);

    if (comp < 0) {
        raiz->esq = inserirPista(raiz->esq, pista);
    } else if (comp > 0) {
        raiz->dir = inserirPista(raiz->dir, pista);
    }

    return raiz;
}

int buscarPista(NoPista *raiz, const char *pista) {
    if (raiz == NULL) {
        return 0;
    }

    int comp = strcmp(pista, raiz->pista);

    if (comp == 0) {
        return 1;
    } else if (comp < 0) {
        return buscarPista(raiz->esq, pista);
    } else {
        return buscarPista(raiz->dir, pista);
    }
}

void emOrdem(NoPista *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq);
        printf("- %s\n", raiz->pista);
        emOrdem(raiz->dir);
    }
}

void explorarSalas(Sala *atual, NoPista **arvorePistas) {
    char opcao;

    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            if (!buscarPista(*arvorePistas, atual->pista)) {
                printf("Voce encontrou uma pista: %s\n", atual->pista);
                *arvorePistas = inserirPista(*arvorePistas, atual->pista);
            } else {
                printf("Esta pista ja foi encontrada antes: %s\n", atual->pista);
            }
        }

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho! Voce chegou a uma sala final.\n");
            return;
        }

        printf("\nEscolha uma opcao:\n");
        if (atual->esquerda != NULL) {
            printf("e -> ir para a esquerda\n");
        }
        if (atual->direita != NULL) {
            printf("d -> ir para a direita\n");
        }
        printf("p -> ver pistas em ordem alfabetica\n");
        printf("s -> sair da exploracao\n");
        printf("Opcao: ");
        scanf(" %c", &opcao);

        if (opcao == 's') {
            printf("Exploracao encerrada.\n");
            return;
        } else if (opcao == 'p') {
            printf("\n=== PISTAS ENCONTRADAS ===\n");
            if (*arvorePistas == NULL) {
                printf("Nenhuma pista encontrada ainda.\n");
            } else {
                emOrdem(*arvorePistas);
            }
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

void liberarMapa(Sala *raiz) {
    if (raiz != NULL) {
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        free(raiz);
    }
}

void liberarPistas(NoPista *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esq);
        liberarPistas(raiz->dir);
        free(raiz);
    }
}

int main() {
    Sala *hall = criarSala("Hall de Entrada", "");
    Sala *biblioteca = criarSala("Biblioteca", "livro");
    Sala *salaJantar = criarSala("Sala de Jantar", "");
    Sala *escritorio = criarSala("Escritorio Secreto", "chave");
    Sala *quarto = criarSala("Quarto Antigo", "retrato");
    Sala *cozinha = criarSala("Cozinha Abandonada", "faca");
    Sala *porao = criarSala("Porao Escuro", "mapa");

    NoPista *arvorePistas = NULL;

    hall->esquerda = biblioteca;
    hall->direita = salaJantar;

    biblioteca->esquerda = escritorio;
    biblioteca->direita = quarto;

    salaJantar->esquerda = cozinha;
    salaJantar->direita = porao;

    printf("=== MISTERIO NA MANSAO - NIVEL AVENTUREIRO ===\n");
    printf("Comecando pelo Hall de Entrada...\n");

    explorarSalas(hall, &arvorePistas);

    printf("\n=== PISTAS FINAIS EM ORDEM ALFABETICA ===\n");
    if (arvorePistas == NULL) {
        printf("Nenhuma pista foi encontrada.\n");
    } else {
        emOrdem(arvorePistas);
    }

    liberarMapa(hall);
    liberarPistas(arvorePistas);

    return 0;
}