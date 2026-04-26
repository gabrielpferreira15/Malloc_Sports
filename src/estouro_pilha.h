/* ============================================================
 * estouro_pilha.h — MINIGAME 3: Estouro de Pilha (Tetris competitivo)
 * ------------------------------------------------------------
 * Dono: DEV 3 — Gabriel Mendes
 *
 * IDEIA DO MINIGAME (do GDD seção 3.3)
 *   Tetris para 2 jogadores lado a lado. Cada um tem seu
 *   campo (grid 10x20). Bloco mal posicionado = stack overflow.
 *   Limpar 2+ linhas envia "lixo" pro adversário.
 *
 *   Vence: quem sobreviver mais tempo OU quem tiver mais
 *   linhas limpas no fim.
 *
 *   ESTE MINIGAME EH O QUE MAIS USA MATRIZ 2D — é o
 *   requisito-chave da disciplina que ele cobre.
 *
 * O QUE ESTE ARQUIVO PRECISA TER (passo a passo)
 *
 *   PASSO 1: Header guard
 *     #ifndef ESTOURO_PILHA_H
 *     #define ESTOURO_PILHA_H
 *
 *   PASSO 2: Constantes
 *     #define GRID_LARGURA 10
 *     #define GRID_ALTURA  20
 *     #define TAM_BLOCO    24   // pixels por célula no desenho
 *
 *   PASSO 3: Declare a função principal
 *     int jogar_estouro_pilha(int *pontos_p1, int *pontos_p2);
 *
 *   PASSO 4: #endif
 * ============================================================ */

/* TODO: header guard, constantes, protótipo, #endif */
