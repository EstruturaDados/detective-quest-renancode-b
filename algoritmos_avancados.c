#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10
#define MAX_SUSPEITOS 20
#define TAM_NOME 50

typedef struct Sala {
    char nome[TAM_NOME];
    char pista[TAM_NOME];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

typedef struct NoPista {
    char pista[TAM_NOME];
    struct NoPista *esq;
    struct NoPista *dir;
} NoPista;

typedef struct NoHash {
    char pista[TAM_NOME];
    char suspeito[TAM_NOME];
    struct NoHash *prox;
} NoHash;

typedef struct {
    NoHash *tabela[TAM_HASH];
} Hash;

typedef struct {
    char nome[TAM_NOME];
    int quantidade;
} ContadorSuspeito;

/* =========================
   FUNCOES AUXILIARES
   ========================= */

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* =========================
   MAPA DA MANSAO
   ========================= */

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

void liberarMapa(Sala *raiz) {
    if (raiz != NULL) {
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        free(raiz);
    }
}

/* =========================
   BST DE PISTAS
   ========================= */

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

void emOrdemPistas(NoPista *raiz) {
    if (raiz != NULL) {
        emOrdemPistas(raiz->esq);
        printf("- %s\n", raiz->pista);
        emOrdemPistas(raiz->dir);
    }
}

void liberarPistas(NoPista *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esq);
        liberarPistas(raiz->dir);
        free(raiz);
    }
}

/* =========================
   TABELA HASH
   ========================= */

void inicializarHash(Hash *hash) {
    for (int i = 0; i < TAM_HASH; i++) {
        hash->tabela[i] = NULL;
    }
}

int funcaoHash(const char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += (int) chave[i];
    }
    return soma % TAM_HASH;
}

void inserirNaHash(Hash *hash, const char *pista, const char *suspeito) {
    int indice = funcaoHash(pista);

    NoHash *novo = (NoHash*) malloc(sizeof(NoHash));
    if (novo == NULL) {
        printf("Erro ao alocar memoria para hash.\n");
        exit(1);
    }

    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = hash->tabela[indice];
    hash->tabela[indice] = novo;
}

char* buscarSuspeitoNaHash(Hash *hash, const char *pista) {
    int indice = funcaoHash(pista);
    NoHash *atual = hash->tabela[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->prox;
    }

    return NULL;
}

void exibirHash(Hash *hash) {
    printf("\n=== ASSOCIACOES PISTA -> SUSPEITO ===\n");

    for (int i = 0; i < TAM_HASH; i++) {
        NoHash *atual = hash->tabela[i];

        if (atual != NULL) {
            printf("Indice %d:\n", i);
            while (atual != NULL) {
                printf("  %s -> %s\n", atual->pista, atual->suspeito);
                atual = atual->prox;
            }
        }
    }
}

void liberarHash(Hash *hash) {
    for (int i = 0; i < TAM_HASH; i++) {
        NoHash *atual = hash->tabela[i];

        while (atual != NULL) {
            NoHash *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
}

/* =========================
   ANALISE DOS SUSPEITOS
   ========================= */

int encontrarIndiceSuspeito(ContadorSuspeito contadores[], int total, const char *nome) {
    for (int i = 0; i < total; i++) {
        if (strcmp(contadores[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

void contarSuspeito(ContadorSuspeito contadores[], int *total, const char *nome) {
    int indice = encontrarIndiceSuspeito(contadores, *total, nome);

    if (indice != -1) {
        contadores[indice].quantidade++;
    } else {
        strcpy(contadores[*total].nome, nome);
        contadores[*total].quantidade = 1;
        (*total)++;
    }
}

void coletarContagemSuspeitos(NoPista *raiz, Hash *hash, ContadorSuspeito contadores[], int *total) {
    if (raiz != NULL) {
        coletarContagemSuspeitos(raiz->esq, hash, contadores, total);

        char *suspeito = buscarSuspeitoNaHash(hash, raiz->pista);
        if (suspeito != NULL) {
            contarSuspeito(contadores, total, suspeito);
        }

        coletarContagemSuspeitos(raiz->dir, hash, contadores, total);
    }
}

void exibirAnaliseFinal(NoPista *arvorePistas, Hash *hash) {
    if (arvorePistas == NULL) {
        printf("\nNenhuma pista foi encontrada. Nao ha como analisar suspeitos.\n");
        return;
    }

    ContadorSuspeito contadores[MAX_SUSPEITOS];
    int total = 0;

    coletarContagemSuspeitos(arvorePistas, hash, contadores, &total);

    if (total == 0) {
        printf("\nNenhum suspeito foi associado as pistas encontradas.\n");
        return;
    }

    printf("\n=== CONTAGEM DE SUSPEITOS ===\n");
    for (int i = 0; i < total; i++) {
        printf("%s -> %d pista(s)\n", contadores[i].nome, contadores[i].quantidade);
    }

    int maior = 0;
    for (int i = 1; i < total; i++) {
        if (contadores[i].quantidade > contadores[maior].quantidade) {
            maior = i;
        }
    }

    printf("\n=== SUSPEITO MAIS CITADO ===\n");
    printf("Suspeito: %s\n", contadores[maior].nome);
    printf("Quantidade de pistas associadas: %d\n", contadores[maior].quantidade);
}

/* =========================
   EXIBICAO DE PISTAS COM SUSPEITOS
   ========================= */

void exibirPistasComSuspeitos(NoPista *raiz, Hash *hash) {
    if (raiz != NULL) {
        exibirPistasComSuspeitos(raiz->esq, hash);

        char *suspeito = buscarSuspeitoNaHash(hash, raiz->pista);
        if (suspeito != NULL) {
            printf("- %s -> %s\n", raiz->pista, suspeito);
        } else {
            printf("- %s -> sem suspeito associado\n", raiz->pista);
        }

        exibirPistasComSuspeitos(raiz->dir, hash);
    }
}

/* =========================
   EXPLORACAO DO JOGO
   ========================= */

void explorarSalas(Sala *atual, NoPista **arvorePistas, Hash *hash) {
    char opcao;

    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            if (!buscarPista(*arvorePistas, atual->pista)) {
                printf("Voce encontrou uma pista: %s\n", atual->pista);
                *arvorePistas = inserirPista(*arvorePistas, atual->pista);

                char *suspeito = buscarSuspeitoNaHash(hash, atual->pista);
                if (suspeito != NULL) {
                    printf("Essa pista esta associada ao suspeito: %s\n", suspeito);
                }
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
        printf("a -> ver associacoes pista -> suspeito das pistas encontradas\n");
        printf("s -> sair da exploracao\n");
        printf("Opcao: ");

        scanf(" %c", &opcao);

        if (opcao == 's') {
            printf("Exploracao encerrada.\n");
            return;
        } else if (opcao == 'p') {
            printf("\n=== PISTAS ENCONTRADAS EM ORDEM ALFABETICA ===\n");
            if (*arvorePistas == NULL) {
                printf("Nenhuma pista encontrada ainda.\n");
            } else {
                emOrdemPistas(*arvorePistas);
            }
        } else if (opcao == 'a') {
            printf("\n=== PISTAS ENCONTRADAS E SEUS SUSPEITOS ===\n");
            if (*arvorePistas == NULL) {
                printf("Nenhuma pista encontrada ainda.\n");
            } else {
                exibirPistasComSuspeitos(*arvorePistas, hash);
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

/* =========================
   MAIN
   ========================= */

int main() {
    Sala *hall = criarSala("Hall de Entrada", "");
    Sala *biblioteca = criarSala("Biblioteca", "livro");
    Sala *salaJantar = criarSala("Sala de Jantar", "");
    Sala *escritorio = criarSala("Escritorio Secreto", "chave");
    Sala *quarto = criarSala("Quarto Antigo", "retrato");
    Sala *cozinha = criarSala("Cozinha Abandonada", "faca");
    Sala *porao = criarSala("Porao Escuro", "mapa");

    NoPista *arvorePistas = NULL;
    Hash tabelaHash;

    inicializarHash(&tabelaHash);

    /* Relacoes pista -> suspeito */
    inserirNaHash(&tabelaHash, "livro", "Mordomo");
    inserirNaHash(&tabelaHash, "chave", "Governanta");
    inserirNaHash(&tabelaHash, "retrato", "Herdeiro");
    inserirNaHash(&tabelaHash, "faca", "Cozinheiro");
    inserirNaHash(&tabelaHash, "mapa", "Mordomo");

    hall->esquerda = biblioteca;
    hall->direita = salaJantar;

    biblioteca->esquerda = escritorio;
    biblioteca->direita = quarto;

    salaJantar->esquerda = cozinha;
    salaJantar->direita = porao;

    printf("=== MISTERIO NA MANSAO - NIVEL MESTRE ===\n");
    printf("Comecando pelo Hall de Entrada...\n");

    explorarSalas(hall, &arvorePistas, &tabelaHash);

    printf("\n=== TODAS AS ASSOCIACOES DA HASH ===\n");
    exibirHash(&tabelaHash);

    printf("\n=== PISTAS FINAIS ENCONTRADAS ===\n");
    if (arvorePistas == NULL) {
        printf("Nenhuma pista foi encontrada.\n");
    } else {
        exibirPistasComSuspeitos(arvorePistas, &tabelaHash);
    }

    exibirAnaliseFinal(arvorePistas, &tabelaHash);

    liberarMapa(hall);
    liberarPistas(arvorePistas);
    liberarHash(&tabelaHash);

    return 0;
}