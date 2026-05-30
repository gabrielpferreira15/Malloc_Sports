#include "scores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  scores_carregar                                                     */
/* ------------------------------------------------------------------ */
Score *scores_carregar(const char *arquivo) {
    FILE *f = fopen(arquivo, "r");
    if (!f) return NULL;   /* Arquivo ainda não existe (primeira execução) — Retorna lista vazia */

    Score *head = NULL;
    Score *tail  = NULL;

    char nome[NOME_TAM];
    int  pontos;

    /* * O formato "\"%4[^\"]\" - \"%d\"\n" funciona assim:
     * \"         -> Consome a aspa inicial
     * %4[^\"]    -> Lê até 4 caracteres que NÃO sejam outra aspa (captura o nome)
     * \" - \"    -> Consome a aspa de fechamento, o espaço, o hífen, o espaço e a aspa de abertura do placar
     * %d         -> Lê o inteiro da pontuação
     * \"\n       -> Consome a última aspa e a quebra de linha
     */
    while (fscanf(f, "\"%4[^\"]\" - \"%d\"\n", nome, &pontos) == 2) {
        Score *novo = malloc(sizeof(Score));
        if (!novo) break;

        strncpy(novo->nome, nome, NOME_TAM - 1);
        novo->nome[NOME_TAM - 1] = '\0';
        novo->pontos  = pontos;
        novo->proximo = NULL;

        /* Insere no final para preservar a ordem decrescente vinda do arquivo */
        if (!head) {
            head = novo;
            tail  = novo;
        } else {
            tail->proximo = novo;
            tail = novo;
        }
    }

    fclose(f);
    return head;
}

/* ------------------------------------------------------------------ */
/*  scores_salvar                                                       */
/* ------------------------------------------------------------------ */
void scores_salvar(const char *arquivo, Score *head) {
    FILE *f = fopen(arquivo, "w");   // Sempre abre em formato re-escrita
    if (!f) return;

    Score *atual = head;
    while (atual) {
        // Formato pedido: "nome_do_jogador" - "pontuação_do_jogador"
        fprintf(f, "\"%s\" - \"%d\"\n", atual->nome, atual->pontos);
        atual = atual->proximo;
    }
    fclose(f);
}

/* ------------------------------------------------------------------ */
/*  scores_inserir                                                      */
/* ------------------------------------------------------------------ */
Score *scores_inserir(Score *head, const char *nome, int pontos) {
    Score *novo = malloc(sizeof(Score));
    if (!novo) return head;

    strncpy(novo->nome, nome, NOME_TAM - 1);
    novo->nome[NOME_TAM - 1] = '\0';
    novo->pontos = pontos;
    novo->proximo = NULL;

    /* Verifica se deve ir para a primeira posição. 
    Se for um empate exato com o 1º (ex: ambos 100), pela sua regra o mais antigo 
    (que já é a head) mantém prioridade. Portanto, checa apenas se o NOVO for MAIOR (>). */
    if (!head || pontos > head->pontos) {
        novo->proximo = head;
        head = novo;
    } else {
        Score *atual = head;
        
        /* Avança a lista encadeada até encontrar onde pontuação NOVA é MAIOR que o PROXIMO.
        O sinal de >= na condição garante que ele pula por cima de todo mundo que 
        tenha a mesma pontuação ou mais, forçando o mais novo no empate ir pro final dele. */
        while (atual->proximo && atual->proximo->pontos >= pontos) {
            atual = atual->proximo;
        }
        novo->proximo = atual->proximo;
        atual->proximo = novo;
    }

    /* Limita o TOP 10 apagando quem sobrar e liberando a memória (exatamente 10º maior) */
    Score *cursor = head;
    for (int i = 1; i < SCORES_MAX && cursor; i++) {
        cursor = cursor->proximo;
    }
    
    if (cursor && cursor->proximo) {
        Score *remover = cursor->proximo;
        while (remover) {
            Score *temp = remover->proximo;
            free(remover);
            remover = temp;
        }
        cursor->proximo = NULL; // Trunca a lista encadeada após o 10º nó.
    }

    return head;
}

/* ------------------------------------------------------------------ */
/*  scores_liberar                                                      */
/* ------------------------------------------------------------------ */
void scores_liberar(Score *head) {
    while (head) {
        Score *prox = head->proximo;
        free(head);
        head = prox;
    }
}

/* ------------------------------------------------------------------ */
/*  scores_tamanho                                                      */
/* ------------------------------------------------------------------ */
int scores_tamanho(Score *head) {
    int n = 0;
    while (head) { n++; head = head->proximo; }
    return n;
}