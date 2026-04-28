/* ============================================================
 * menu.h — TELAS DE NAVEGACAO (UI)
 * ------------------------------------------------------------
 * Dono: DEV 3 — Gabriel Mendes */

#ifndef MENU_H
#define MENU_H

#include "cenas.h"

/* Guarda o contexto do resultado da última partida 
para que tela_resultado saiba o que exibir. */
typedef struct {
    int vencedor;   
    int pontos_p1;
    int pontos_p2;
} ResultadoPartida;

// Exibe o menu principal
/* No momento a opção de "Jogar" só vai levar para o Pong, 
já que é o único jogo implpementado */
Cena tela_menu_principal(void);

// Exibe o placar final com opções de Revanche e Menu.
/* Retorna para a cena do jogo atual, que atualmente é apenas o pong mas iremos 
implementar mais, (revanche) ou CENA_MENU (voltar) */
Cena tela_resultado(ResultadoPartida r);

#endif 