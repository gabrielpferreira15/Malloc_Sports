/* ============================================================
 * pong_memoria.h — MINIGAME 2: Pong da Memória
 * ------------------------------------------------------------
 * Dono: DEV 2 — Matheus Assis
 *
 * IDEIA DO MINIGAME (do GDD seção 3.2)
 *   Pong clássico com temática de C. A bola é um "ponteiro
 *   alocado", as raquetes são verticais nos lados esquerdo e
 *   direito. Quando alguém não rebate, sofre um "vazamento de memória"
 *   (faixa colorida na borda da tela).
 *
 *   Powerup free() aleatório: ao tocar, dobra a velocidade da
 *   bola por 3 segundos.
 *
 *   Vitória: primeiro a 7 pontos.
 *
 * O QUE ESTE ARQUIVO PRECISA TER (passo a passo)
 *   PASSO 1: Header guard.
 *
 *   PASSO 2: Declare:
 *     int jogar_pong_memoria(int *pontos_p1, int *pontos_p2);
 *         // devolve 1 se P1 venceu, 2 se P2 venceu
 *         // preenche os pontos via ponteiro
 *
 *   PASSO 3: #endif
 * ============================================================ */

/* TODO: header guard + protótipo de jogar_pong_memoria */
