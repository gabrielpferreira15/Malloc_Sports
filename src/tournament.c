#include "tournament.h"

void iniciar_torneio(EstadoTorneio *t) {
    t->placares[0]    = 0;
    t->placares[1]    = 0;
    t->jogo_atual     = 0;
    t->jogos_ordem[0] = CENA_CORRIDA;
    t->jogos_ordem[1] = CENA_PONG;
}

void registrar_resultado(EstadoTorneio *t, int vencedor) {
    if (vencedor == 1)      t->placares[0]++;
    else if (vencedor == 2) t->placares[1]++;
    /* empate: ninguém pontua */
    t->jogo_atual++;
}

int vencedor_torneio(const EstadoTorneio *t) {
    if (t->placares[0] > t->placares[1]) return 1;
    if (t->placares[1] > t->placares[0]) return 2;
    return 0;
}

bool torneio_acabou(const EstadoTorneio *t) {
    return t->jogo_atual >= TORNEIO_TOTAL_JOGOS;
}

Cena proxima_cena_torneio(const EstadoTorneio *t) {
    return t->jogos_ordem[t->jogo_atual];
}