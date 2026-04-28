/* ============================================================
 * main.c — Ponto de entrada do Malloc Sports
 * ------------------------------------------------------------
 * Dono: DEV 1 — Gabriel Peixoto
 *
 * Responsabilidades:
 *   1. Abre a janela (uma única vez).
 *   2. Gerencia qual cena está ativa via switch.
 *   3. Fecha a janela ao sair.
 *
 * Fluxo atual:
 *   CENA_MENU  →  CENA_PONG  →  CENA_RESULTADO
 *                     ↑               |
 *                     └── revanche ───┘
 *                     └── menu ──→ CENA_MENU
 * ============================================================ */

#include "raylib.h"
//As cenas no momento estão limitadas pois temos apenas um minigame implementado e só um dos modos de jogo
#include "cenas.h"
#include "menu.h"
#include "pong_memoria.h"

int main(void) {

    // Janela (aberta UMA vez para todo o jogo)
    InitWindow(1280, 720, "Malloc Sports");
    SetTargetFPS(144);

    // Estado inicial
    Cena cena_atual = CENA_MENU;
    ResultadoPartida resultado = {0};

    // Loop de cenas 
    while (!WindowShouldClose() && cena_atual != CENA_SAIR) {

        switch (cena_atual) {

            case CENA_MENU:
                cena_atual = tela_menu_principal();
                break;

            case CENA_PONG: {
                int pts1 = 0, pts2 = 0;
                int vencedor = jogar_pong_memoria(&pts1, &pts2);

                /* Guarda resultado para a tela seguinte */
                resultado.vencedor  = vencedor;
                resultado.pontos_p1 = pts1;
                resultado.pontos_p2 = pts2;

                // Se o jogador fechou a janela durante o jogo, sai 
                cena_atual = WindowShouldClose() ? CENA_SAIR : CENA_RESULTADO;
                break;
            }

            case CENA_RESULTADO:
                cena_atual = tela_resultado(resultado);
                break;

            case CENA_SAIR:
                break;
        }
    }

    CloseWindow();

    return 0;
}