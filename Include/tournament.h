#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <stdbool.h>
#include "cenas.h"
#include "tournament.h"

#define TORNEIO_TOTAL_JOGOS 2 //No final do jogo o número total será 3, mas, no momento só temos o pong e corrida

typedef struct {
    int  placares[2];                       // [0]=P1  [1]=P2              
    int  jogo_atual;                        // índice em jogos_ordem[]     
    Cena jogos_ordem[TORNEIO_TOTAL_JOGOS];  // sequência de cenas         
} EstadoTorneio;

// Zera o estado e define a ordem dos jogos 
void iniciar_torneio(EstadoTorneio *t);

// Registra o resultado de uma rodada e avança jogo_atual
// Vencedor: 1=P1, 2=P2, 0=empate 
void registrar_resultado(EstadoTorneio *t, int vencedor);

// Retorna 1, 2 ou 0 (empate) com base nos placares acumulados.
int vencedor_torneio(const EstadoTorneio *t);

// True se todos os jogos já foram disputados 
bool torneio_acabou(const EstadoTorneio *t);

// Retorna a cena do jogo atual do torneio 
Cena proxima_cena_torneio(const EstadoTorneio *t);

#endif 