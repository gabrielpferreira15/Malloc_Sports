/* ============================================================
 * menu.h — TELAS DE NAVEGACAO (UI)
 * ------------------------------------------------------------
 * Dono: DEV 3 — Gabriel Mendes
 *
 * IDEIA GERAL
 *   Este módulo cuida de TUDO que NÃO é minigame:
 *     - Splash de boot (logo aparecendo);
 *     - Menu principal (Torneio | Minigame Único | Ranking | Sair);
 *     - Seleção de minigame (só no modo Minigame Único);
 *     - Tela de resultado de round;
 *     - Tela final de campeonato (placar + input do nome);
 *     - Tela de Ranking (lendo da lista do scores.c).
 *
 *   Cada tela vira uma função que:
 *     1) Roda seu proprio mini-loop até o usuário apertar algo;
 *     2) Devolve a CENA seguinte (que o main.c usa pra trocar).
 *
 *   ANALOGIA: cada tela é tipo uma "porta" — você entra, faz
 *   algumas coisas, e sai pela próxima porta.
 *
 * O QUE ESTE ARQUIVO PRECISA TER (passo a passo)
 *
 *   PASSO 1: Header guard
 *     #ifndef MENU_H
 *     #define MENU_H
 *
 *   PASSO 2: Inclua o header do scores (precisa do tipo Score)
 *     #include "scores.h"
 *
 *   PASSO 3: O enum Cena
 *     Como o main.c também usa Cena, sugestão: definir o enum
 *     num arquivo cenas.h dedicado, OU declarar aqui mesmo e
 *     o main.c só inclui menu.h.
 *
 *     enum Cena (igual ao definido em main.c — manter sincronizado).
 *
 *   PASSO 4: Declare os protótipos
 *
 *     Cena tela_splash(void);
 *         // animação de boot, retorna CENA_MENU após 2s
 *
 *     Cena tela_menu_principal(void);
 *         // 4 opções; retorna a cena da escolhida
 *
 *     Cena tela_selecao_minigame(void);
 *         // 3 cards; retorna CENA_CORRIDA_PONTEIROS, CENA_PONG ou CENA_ESTOURO_PILHA
 *
 *     Cena tela_resultado(int vencedor, int p1, int p2);
 *         // mostra vencedor e placar parcial, ENTER pra continuar
 *
 *     Cena tela_campeonato(int p1, int p2, char *nome_saida);
 *         // animação de confete; lê o nome do vencedor;
 *         // grava em nome_saida (máx 15 chars + \0)
 *
 *     Cena tela_ranking(Score *cabeça);
 *         // recebe a lista carregada e desenha
 *
 *   PASSO 5: #endif
 *
 * REQUISITOS COBERTOS
 *   - Structs de estado, callbacks (cada tela retorna a próxima)
 *   - Input (teclado), animação (transições, confete)
 * ============================================================ */

/* TODO: header guard, includes, enum Cena, protótipos das telas, #endif */
