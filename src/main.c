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
#include "cenas.h"
#include "menu.h"
#include "pong_memoria.h"
#include "corrida_ponteiros.h"
#include "volei.h"
#include "tournament.h"

int main(void) {

    // Janela (aberta UMA vez para todo o jogo)
    InitWindow(1280, 720, "Malloc Sports");
    SetTargetFPS(144);

    // Estado inicial
    Cena cena_atual = CENA_MENU;
    ResultadoPartida resultado = {0};
    EstadoTorneio torneio = {0};
    bool em_torneio = false;

    // Loop de cenas 
    while (!WindowShouldClose() && cena_atual != CENA_SAIR) {

        switch (cena_atual) {

            case CENA_MENU:
                cena_atual = tela_menu_principal();
                break;

            case CENA_SELECAO_MODO:
                cena_atual = tela_selecao_modo(&torneio);
                if (cena_atual != CENA_MENU && cena_atual != CENA_SELECAO_MINIGAME && cena_atual != CENA_SAIR) {
                    em_torneio = true; // Caiu direto numa cena de jogo, então começou o torneio
                } else {
                    em_torneio = false;
                }
                break;

            case CENA_SELECAO_MINIGAME:
                cena_atual = tela_selecao_minigame();
                em_torneio = false;
                break;

            case CENA_CORRIDA: {
                int pts1 = 0, pts2 = 0;
                int vencedor = jogar_corrida_ponteiros(&pts1, &pts2);

                resultado.vencedor  = vencedor;
                resultado.pontos_p1 = pts1;
                resultado.pontos_p2 = pts2;
                resultado.em_torneio = em_torneio;
                resultado.torneio_final = false;
                resultado.ultimo_jogo = CENA_CORRIDA;

                if (em_torneio) {
                    registrar_resultado(&torneio, vencedor);
                    if (torneio_acabou(&torneio)) {
                        resultado.torneio_final = true;
                        resultado.vencedor = vencedor_torneio(&torneio);
                        resultado.pontos_p1 = torneio.placares[0];
                        resultado.pontos_p2 = torneio.placares[1];
                    }
                }
                
                cena_atual = WindowShouldClose() ? CENA_SAIR : CENA_RESULTADO;
                break;
            }

            case CENA_PONG: {
                int pts1 = 0, pts2 = 0;
                int vencedor = jogar_pong_memoria(&pts1, &pts2);

                resultado.vencedor  = vencedor;
                resultado.pontos_p1 = pts1;
                resultado.pontos_p2 = pts2;
                resultado.em_torneio = em_torneio;
                resultado.torneio_final = false;
                resultado.ultimo_jogo = CENA_PONG;

                if (em_torneio) {
                    registrar_resultado(&torneio, vencedor);
                    if (torneio_acabou(&torneio)) {
                        resultado.torneio_final = true;
                        resultado.vencedor = vencedor_torneio(&torneio);
                        resultado.pontos_p1 = torneio.placares[0];
                        resultado.pontos_p2 = torneio.placares[1];
                    }
                }

                // Se o jogador fechou a janela durante o jogo, sai 
                cena_atual = WindowShouldClose() ? CENA_SAIR : CENA_RESULTADO;
                break;
            }

            case CENA_VOLEI: {
                int pts1 = 0, pts2 = 0;
                int vencedor = jogar_volei(&pts1, &pts2);

                resultado.vencedor  = vencedor;
                resultado.pontos_p1 = pts1;
                resultado.pontos_p2 = pts2;
                resultado.em_torneio = em_torneio;
                resultado.torneio_final = false;
                resultado.ultimo_jogo = CENA_VOLEI;

                if (em_torneio) {
                    registrar_resultado(&torneio, vencedor);
                    if (torneio_acabou(&torneio)) {
                        resultado.torneio_final = true;
                        resultado.vencedor = vencedor_torneio(&torneio);
                        resultado.pontos_p1 = torneio.placares[0];
                        resultado.pontos_p2 = torneio.placares[1];
                    }
                }

                cena_atual = WindowShouldClose() ? CENA_SAIR : CENA_RESULTADO;
                break;
            }

            case CENA_RESULTADO:
                cena_atual = tela_resultado(resultado);
                if (cena_atual == CENA_MENU) {
                    em_torneio = false;
                } else if (cena_atual != CENA_SAIR && em_torneio && !resultado.torneio_final) {
                    // Quando clica em "Próximo", `tela_resultado` pode retornar algo genérico e caímos no próximo jogo
                    cena_atual = proxima_cena_torneio(&torneio);
                }
                break;

            case CENA_SAIR:
                break;
        }
    }

    CloseWindow();

    return 0;
}