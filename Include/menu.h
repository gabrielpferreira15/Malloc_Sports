#ifndef MENU_H
#define MENU_H

#include "cenas.h"
#include "tournament.h"

/* Guarda o contexto do resultado da última partida 
para que tela_resultado saiba o que exibir. */
typedef struct {
    int vencedor;   
    int pontos_p1;
    int pontos_p2;
    bool em_torneio;
    bool torneio_final; // se verdadeiro, exibe quem ganhou o torneio final
    Cena ultimo_jogo;
} ResultadoPartida;

// Exibe o menu principal
/* A opcao de "Jogar" leva para a selecao de minigames. */
Cena tela_menu_principal(void);

/* Seleção de modo: retorna CENA_SELECAO_MINIGAME ou CENA_CORRIDA
 * (início de torneio). Preenche *torneio se o modo Torneio
 * for escolhido. */
Cena tela_selecao_modo(EstadoTorneio *torneio);

/* Selecao de minigame (modo Avulso): retorna CENA_CORRIDA, CENA_PONG ou CENA_VOLEI. */
Cena tela_selecao_minigame(void);

// Exibe o placar final com opções de Revanche e Menu.
/* Retorna para a cena do jogo atual, que atualmente é apenas o pong mas iremos 
implementar mais, (revanche) ou CENA_MENU (voltar) */
Cena tela_resultado(ResultadoPartida r);

#endif 