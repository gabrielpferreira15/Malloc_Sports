/* ============================================================
 * pong_memoria.h — MINIGAME 2: Pong da Memória
 * ------------------------------------------------------------
 * Dono: DEV 2 — Matheus Assis
 *
 * IDEIA DO MINIGAME
 *   Pong clássico com temática de C. A bola é um "ponteiro
 *   alocado", as raquetes são verticais nos lados esquerdo e
 *   direito. Quando alguém não rebate, sofre um "vazamento de
 *   memória" (faixa colorida na borda da tela).
 *
 *   Powerup free() aleatório: ao tocar, dobra a velocidade da
 *   bola por 3 segundos.
 *
 *   Vitória: primeiro a 7 pontos.
 * ============================================================ */

#ifndef PONG_MEMORIA_H
#define PONG_MEMORIA_H

/* Inicia e executa o loop completo do minigame.
 * Preenche os pontos finais via ponteiro.
 * Retorna 1 se o Jogador 1 venceu, 2 se o Jogador 2 venceu. */
int jogar_pong_memoria(int *pontos_p1, int *pontos_p2);

#endif /* PONG_MEMORIA_H */