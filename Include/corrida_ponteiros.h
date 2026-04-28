/* ============================================================
 * corrida_ponteiros.h — MINIGAME 1: Corrida de Ponteiros (corrida)
 * ------------------------------------------------------------
 * Dono: DEV 1 — Gabriel Peixoto
 *
 * IDEIA DO MINIGAME (do GDD seção 3.1)
 *   Corrida lateral 2D. Os 2 atletas (PTR e FREE) correm em
 *   pistas paralelas. Obstáculos aparecem e sobem de velocidade.
 *   Cada um pula (W para P1, SETA CIMA para P2) pra desviar.
 *
 *   Vence: quem chegar ao fim primeiro OU tiver menos colisões
 *   no tempo limite (60 segundos).
 *
 * O QUE ESTE ARQUIVO PRECISA TER (passo a passo)
 *
 *   PASSO 1: Header guard
 *     #ifndef CORRIDA_PONTEIROS_H
 *     #define CORRIDA_PONTEIROS_H
 *
 *   PASSO 2: Declare a função principal
 *
 *     int jogar_corrida_ponteiros(int *pontos_p1, int *pontos_p2);
 *
 *     - Roda o minigame até alguém vencer ou o tempo acabar.
 *     - Devolve: 1 se P1 venceu, 2 se P2 venceu, 0 se empate.
 *     - Preenche os ponteiros recebidos com a pontuação final
 *       (pra integrar com o Torneio depois).
 *
 *     POR QUE int* nos parâmetros? Porque assim quem chamou a
 *     função recebe os pontos finais sem precisar de variável
 *     global.
 *
 *   PASSO 3: #endif
 *
 * ATENÇÃO
 *   Só protótipo aqui. Implementação em corrida_ponteiros.c.
 * ============================================================ */

/* TODO: header guard + protótipo de jogar_corrida_ponteiros */
