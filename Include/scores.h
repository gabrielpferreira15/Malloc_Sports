#ifndef SCORES_H
#define SCORES_H

/* ================================================================
 * scores.h — Lista encadeada de highscores para o modo Torneio
 * ----------------------------------------------------------------
 * Formato do arquivo .txt (uma linha por entrada):
 *   NOME - PONTUACAO
 *   Ex: ALEX - 576000
 *
 * Regras de inserção:
 *   - Ordem decrescente de pontuação.
 *   - Empate: o jogador mais antigo permanece à frente
 *     (novo com mesmo score vai DEPOIS do existente).
 *   - Mantém no máximo SCORES_MAX entradas.
 * ================================================================ */

#define SCORES_ARQUIVO  "highscores.txt"
#define SCORES_MAX      10
#define NOME_TAM         5   /* 4 chars + terminador nulo '\0' */

typedef struct Score {
    char          nome[NOME_TAM];
    int           pontos;
    struct Score *proximo;
} Score;

/* Carrega a lista do arquivo .txt.
 * Devolve NULL se o arquivo não existir (primeira execução). */
Score *scores_carregar(const char *arquivo);

/* Grava a lista no arquivo, sobrescrevendo o conteúdo anterior.
 * Formato de cada linha: "NOME - PONTOS\n" */
void scores_salvar(const char *arquivo, Score *cabeca);

/* Insere um novo score em ordem decrescente.
 * Empate → novo entra DEPOIS dos existentes de mesmo valor.
 * Aplica SCORES_MAX: entradas excedentes são liberadas.
 * Devolve a nova cabeça da lista. */
Score *scores_inserir(Score *cabeca, const char *nome, int pontos);

/* Libera todos os nós da lista. */
void scores_liberar(Score *cabeca);

/* Conta quantos nós há na lista. */
int scores_tamanho(Score *cabeca);

#endif