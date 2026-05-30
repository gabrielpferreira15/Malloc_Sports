#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <stdbool.h>
#include "cenas.h"

#define TORNEIO_TOTAL_JOGOS  3
#define NOME_JOGADOR_TAM     5   /* 4 chars + '\0' */

typedef struct {
    int  placares[2];                        /* vitórias: [0]=P1  [1]=P2 */
    int  jogo_atual;                         /* índice em jogos_ordem[]  */
    Cena jogos_ordem[TORNEIO_TOTAL_JOGOS];   /* sequência de cenas       */
    int  pontuacoes[2];                      /* pontuação acumulada do torneio */
    char nomes[2][NOME_JOGADOR_TAM];         /* nomes dos jogadores      */
} EstadoTorneio;

/* Zera o estado e define a ordem dos jogos. */
void iniciar_torneio(EstadoTorneio *t);

/* Registra o resultado de uma rodada e avança jogo_atual.
 * vencedor: 1=P1, 2=P2, 0=empate. */
void registrar_resultado(EstadoTorneio *t, int vencedor);

/* Soma pontuações calculadas pelo main ao acumulado do torneio. */
void acumular_pontuacao(EstadoTorneio *t, int pts_p1, int pts_p2);

/* Vencedor do torneio determinado pela PONTUAÇÃO acumulada.
 * Retorna 1, 2 ou 0 (empate). */
int vencedor_torneio(const EstadoTorneio *t);

/* True se todos os jogos já foram disputados. */
bool torneio_acabou(const EstadoTorneio *t);

/* Retorna a cena do próximo jogo do torneio. */
Cena proxima_cena_torneio(const EstadoTorneio *t);

#endif