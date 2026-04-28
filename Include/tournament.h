/* ============================================================
 * tournament.h — ESTADO E LÓGICA do modo Torneio
 * ------------------------------------------------------------
 * Dono: DEV 2 — Matheus Assis
 *
 * IDEIA GERAL
 *   No modo Torneio, os 2 jogadores disputam os 3 minigames em
 *   sequência. Cada vitória vale pontos. No fim, quem tem mais
 *   pontos vence o torneio.
 *
 *   Este módulo guarda o ESTADO ("em qual jogo estamos? quantos
 *   pontos cada um tem?") e expõe funções para avançar a partida.
 *
 *   ANALOGIA: imagine uma planilha de campeonato — cada linha
 *   é um jogo, cada coluna é um jogador. Esta struct guarda
 *   essa planilha em memória.
 *
 * NOMENCLATURA
 *   - placares    : array com pontos do P1 e P2 acumulados
 *   - jogo_atual  : 0=Corrida de Ponteiros, 1=Pong, 2=Estouro de Pilha, 3=fim do torneio
 *   - total_jogos : quantos minigames tem no torneio (=3)
 *
 *   OBS.: a ideia do GDD aqui é o estado do torneio, com
 *   campos como placares, jogo_atual e total_jogos. Aqui
 *   sugerimos português. Use o que preferir, mas seja consistente.
 *
 * O QUE ESTE ARQUIVO PRECISA TER (passo a passo)
 *
 *   PASSO 1: Header guard
 *     #ifndef TOURNAMENT_H
 *     #define TOURNAMENT_H
 *
 *   PASSO 2: Inclua <stdbool.h>  (bool retornado por torneio_acabou)
 *
 *   PASSO 3: Defina a struct EstadoTorneio
 *
 *     typedef struct EstadoTorneio {
 *         int placares[2];   // [0]=P1, [1]=P2
 *         int jogo_atual;    // qual minigame da rodada atual
 *         int total_jogos;   // 3 no nosso caso
 *     } EstadoTorneio;
 *
 *   PASSO 4: Declare os protótipos
 *
 *     void iniciar_torneio(EstadoTorneio *t);
 *         // zera placares, jogo_atual=0, total_jogos=3
 *
 *     void registrar_resultado(EstadoTorneio *t, int vencedor);
 *         // vencedor: 1 = P1, 2 = P2, 0 = empate
 *         // soma os pontos da rodada e avança jogo_atual
 *
 *     int vencedor_torneio(const EstadoTorneio *t);
 *         // devolve 1, 2, ou 0 se empate
 *         // const porque só LÊ o estado, não modifica
 *
 *     bool torneio_acabou(const EstadoTorneio *t);
 *         // true se já jogamos os 3 minigames
 *
 *   PASSO 5: #endif
 *
 * REQUISITOS DA DISCIPLINA QUE ESTE ARQUIVO COBRE
 *   - Structs (EstadoTorneio)
 *   - Ponteiros (passamos *t para todas as funções)
 *
 * ATENÇÃO
 *   - POR QUE EstadoTorneio *t e não EstadoTorneio t?
 *     Em C, parâmetros são passados POR VALOR (cópia). Para
 *     MODIFICAR o original, precisamos do ponteiro. Sem o *,
 *     a função mexeria numa cópia e o original ficaria intacto.
 *   - POR QUE const EstadoTorneio *t nas funções que só leem?
 *     Para deixar CLARO que essas funções não alteram nada. O
 *     compilador vai reclamar se você tentar escrever em t->...
 *     dentro delas — é uma proteção contra bug bobo.
 * ============================================================ */

/* TODO: header guard, includes */


/* TODO: typedef struct EstadoTorneio { ... } EstadoTorneio; */


/* TODO: protótipos das 4 funções */


/* TODO: #endif */
