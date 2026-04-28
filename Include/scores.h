/* ============================================================
 * scores.h — RANKING persistido em arquivo
 * ------------------------------------------------------------
 * Dono: DEV 2 — Matheus Assis
 *
 * IDEIA GERAL
 *   Quando alguém ganha um torneio, ele insere o nome dele
 *   no ranking. Os 10 melhores ficam guardados num arquivo
 *   chamado "scores.dat" — assim, dá para ver os recordes
 *   mesmo depois de fechar e reabrir o jogo.
 *
 *   A estrutura usa LISTA ENCADEADA (em vez de array) por
 *   2 motivos pedagogicos:
 *     1) Cobre o requisito "lista encadeada" da disciplina.
 *     2) Permite inserção ORDENADA elegante (insertion sort
 *        de 1 elemento por vez, sem precisar deslocar array).
 *
 * NOMENCLATURA
 *   - nome    : string com o nome do jogador (máx 15 chars + \0)
 *   - pontos  : pontuação final do torneio
 *   - próximo : ponteiro pro próximo Score da lista
 *
 * O QUE ESTE ARQUIVO PRECISA TER (passo a passo)
 *
 *   PASSO 1: Header guard
 *     #ifndef SCORES_H
 *     #define SCORES_H
 *
 *   PASSO 2: Defina a struct Score
 *
 *     typedef struct Score {
 *         char nome[16];          // 15 chars + terminador nulo
 *         int  pontos;            // pontuação
 *         struct Score *próximo;  // próximo nó da lista
 *     } Score;
 *
 *     POR QUE 16? Para caber nomes curtos tipo "GABRIEL_M\0" e
 *     ainda sobrar espaço. É um número "redondo" pra alinhar
 *     na memória.
 *
 *   PASSO 3: Declare os protótipos
 *
 *     Score *scores_carregar(const char *arquivo);
 *         // lê o arquivo binário e devolve a cabeça da lista
 *         // se o arquivo não existe (1a vez), devolve NULL
 *
 *     void scores_salvar(const char *arquivo, Score *cabeça);
 *         // grava a lista no arquivo binário
 *
 *     Score *scores_inserir(Score *cabeça,
 *                           const char *nome, int pontos);
 *         // insere ORDENADO por pontos (maior primeiro)
 *         // mantém só os 10 melhores (corta o resto)
 *
 *     void scores_liberar(Score *cabeça);
 *         // free em todos os nós
 *
 *   PASSO 4: #endif
 *
 * REQUISITOS DA DISCIPLINA QUE ESTE ARQUIVO COBRE
 *   - Arquivo E/S (fwrite/fread)
 *   - Lista encadeada
 *   - malloc/free
 *
 * ATENÇÃO
 *   - const char *arquivo é assinatura padrão em C pra
 *     "string que não vai ser modificada". Pode passar literais.
 * ============================================================ */

/* TODO: header guard, struct Score, protótipos, #endif */
