/* ============================================================
 * tournament.c — IMPLEMENTAÇÃO do modo Torneio
 * ------------------------------------------------------------
 * Dono: DEV 2 — Matheus Assis
 *
 * REGRA DE PONTUACAO sugerida
 *   - Vitória = 3 pontos
 *   - Empate  = 1 ponto pra cada
 *   - Derrota = 0 pontos
 *   (decida com a equipe e mantenha consistente.)
 *
 * O QUE FAZER (passo a passo)
 *
 *   --- iniciar_torneio(t) ---
 *   1. t->placares[0] = 0;
 *   2. t->placares[1] = 0;
 *   3. t->jogo_atual  = 0;
 *   4. t->total_jogos = 3;
 *
 *   --- registrar_resultado(t, vencedor) ---
 *   1. const int PONTOS_VITORIA = 3;
 *      const int PONTOS_EMPATE  = 1;
 *
 *   2. if (vencedor == 1) {
 *          t->placares[0] += PONTOS_VITORIA;
 *      } else if (vencedor == 2) {
 *          t->placares[1] += PONTOS_VITORIA;
 *      } else { // empate
 *          t->placares[0] += PONTOS_EMPATE;
 *          t->placares[1] += PONTOS_EMPATE;
 *      }
 *
 *   3. t->jogo_atual++;  // avança pro próximo minigame
 *
 *   --- vencedor_torneio(t) ---
 *   1. if (t->placares[0] > t->placares[1]) return 1;
 *      else if (t->placares[1] > t->placares[0]) return 2;
 *      else return 0;
 *
 *   --- torneio_acabou(t) ---
 *   1. return t->jogo_atual >= t->total_jogos;
 *
 * COMO INTEGRAR COM main.c
 *
 *   No loop principal, o fluxo do Torneio fica assim:
 *
 *     EstadoTorneio t;
 *     iniciar_torneio(&t);
 *     while (!torneio_acabou(&t)) {
 *         int vencedor;
 *         switch (t.jogo_atual) {
 *             case 0: vencedor = jogar_corrida_ponteiros(&p1, &p2); break;
 *             case 1: vencedor = jogar_pong_memoria(&p1, &p2);   break;
 *             case 2: vencedor = jogar_estouro_pilha(&p1, &p2);  break;
 *         }
 *         registrar_resultado(&t, vencedor);
 *         tela_resultado(vencedor, t.placares[0], t.placares[1]);
 *     }
 *     tela_campeonato(t.placares[0], t.placares[1], nome);
 *
 * COMO TESTAR
 *   No main, simule um torneio sem rodar os minigames:
 *     EstadoTorneio t;
 *     iniciar_torneio(&t);
 *     registrar_resultado(&t, 1);    // P1 venceu
 *     registrar_resultado(&t, 2);    // P2 venceu
 *     registrar_resultado(&t, 1);    // P1 venceu
 *     printf("vencedor: %d\n", vencedor_torneio(&t));   // 1
 *     printf("acabou? %d\n", torneio_acabou(&t));       // 1 (true)
 *
 * ATENÇÃO
 *   - Os números mágicos PONTOS_VITORIA/EMPATE em const evitam
 *     ter "3" e "1" espalhados pelo código. Se um dia mudar a
 *     pontuação, troca em UM lugar só.
 * ============================================================ */

/* TODO: includes (tournament.h) */


/* TODO: implemente iniciar_torneio */


/* TODO: implemente registrar_resultado */


/* TODO: implemente vencedor_torneio */


/* TODO: implemente torneio_acabou */
