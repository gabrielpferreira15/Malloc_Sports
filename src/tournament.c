#include "tournament.h"

void iniciar_torneio(EstadoTorneio *t) {
    t->placares[0]    = 0;
    t->placares[1]    = 0;
    t->jogo_atual     = 0;
    t->jogos_ordem[0] = CENA_CORRIDA;
    t->jogos_ordem[1] = CENA_PONG;
    t->jogos_ordem[2] = CENA_VOLEI;
    t->pontuacoes[0]  = 0;
    t->pontuacoes[1]  = 0;
    t->nomes[0][0]    = '\0';
    t->nomes[1][0]    = '\0';
}

void registrar_resultado(EstadoTorneio *t, int vencedor) {
    if (vencedor == 1)      t->placares[0]++;
    else if (vencedor == 2) t->placares[1]++;
    /* empate: ninguém pontua no placar de vitórias */
    t->jogo_atual++;
}

void acumular_pontuacao(EstadoTorneio *t, int pts_p1, int pts_p2) {
    t->pontuacoes[0] += pts_p1;
    t->pontuacoes[1] += pts_p2;
}

/* O vencedor do torneio é determinado pela PONTUAÇÃO ACUMULADA,
 * não pelo número de vitórias. */
int vencedor_torneio(const EstadoTorneio *t) {
    if (t->pontuacoes[0] > t->pontuacoes[1]) return 1;
    if (t->pontuacoes[1] > t->pontuacoes[0]) return 2;
    return 0; /* empate */
}

bool torneio_acabou(const EstadoTorneio *t) {
    return t->jogo_atual >= TORNEIO_TOTAL_JOGOS;
}

Cena proxima_cena_torneio(const EstadoTorneio *t) {
    return t->jogos_ordem[t->jogo_atual];
}