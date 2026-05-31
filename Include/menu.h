#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include "cenas.h"
#include "tournament.h"

/* Contexto do resultado da última partida para tela_resultado. */
typedef struct {
    int  vencedor;
    int  pontos_p1;
    int  pontos_p2;
    int  pontos_torneio_p1;  /* Pontuação acumulada do P1 no torneio */
    int  pontos_torneio_p2;  /* Pontuação acumulada do P2 no torneio */
    bool em_torneio;
    bool torneio_final;   /* true → exibe vencedor do torneio completo */
    Cena ultimo_jogo;
} ResultadoPartida;

/* Menu principal (Jogar / Highscores - Torneio / Sair). */
Cena tela_menu_principal(void);

/* Seleção de modo: Avulso ou Torneio.
 * Preenche *torneio se Torneio for escolhido. */
Cena tela_selecao_modo(EstadoTorneio *torneio);

/* Seleção de minigame para o modo Avulso. */
Cena tela_selecao_minigame(void);

/* Placar final com opções de Revanche / Próximo Jogo / Menu. */
Cena tela_resultado(ResultadoPartida r);

/* Pede o nome (máx 4 caracteres) de cada jogador antes do torneio.
 * nome_p1 e nome_p2 devem ser buffers de NOME_JOGADOR_TAM bytes. */
void tela_inserir_nomes(char *nome_p1, char *nome_p2);

/* Exibe o ranking das 10 melhores pontuações em grade 2×5. */
Cena tela_highscores(void);

#endif