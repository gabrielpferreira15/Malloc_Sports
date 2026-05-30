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
#include "scores.h"

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
                int pts1_cru = 0, pts2_cru = 0;
                int vencedor = jogar_corrida_ponteiros(&pts1_cru, &pts2_cru);

                // Nova regra: 1º colocado ganha 500 pontos, 2º colocado ganha 200 pontos
                int pts1_calc = 0;
                int pts2_calc = 0;

                if (vencedor == 1) {
                    pts1_calc = 500; // Jogador 1 venceu (1º lugar)
                    pts2_calc = 200; // Jogador 2 perdeu (2º lugar)
                } else if (vencedor == 2) {
                    pts1_calc = 200; // Jogador 1 perdeu (2º lugar)
                    pts2_calc = 500; // Jogador 2 venceu (1º lugar)
                } else {
                    // Tratamento seguro caso ocorra um empate exato na corrida:
                    // Ambos recebem a pontuação mínima de 2º lugar
                    pts1_calc = 200;
                    pts2_calc = 200;
                }

                resultado.vencedor = vencedor;
                resultado.pontos_p1 = pts1_calc;
                resultado.pontos_p2 = pts2_calc;
                resultado.em_torneio = em_torneio;
                resultado.torneio_final = false;
                resultado.ultimo_jogo = CENA_CORRIDA;

                if (em_torneio) {
                    registrar_resultado(&torneio, vencedor);
                    // Passa os valores fixos (500 e 200) para acumular no torneio
                    acumular_pontuacao(&torneio, pts1_calc, pts2_calc);
                }
                cena_atual = WindowShouldClose() ? CENA_SAIR : CENA_RESULTADO;
                break;
            }

            case CENA_PONG: {
                int pts1_cru = 0, pts2_cru = 0;
                int vencedor = jogar_pong_memoria(&pts1_cru, &pts2_cru);

                // 1. Calcula a pontuação com base na fórmula
                int pts1_calc = (pts1_cru * 100) + (vencedor == 1 ? 300 : 0);
                int pts2_calc = (pts2_cru * 100) + (vencedor == 2 ? 300 : 0);

                // 2. Preenche a estrutura de resultado para a tela intermediária
                resultado.vencedor = vencedor;
                resultado.pontos_p1 = pts1_calc;  // <-- CORREÇÃO: mude de pts1_cru para pts1_calc
                resultado.pontos_p2 = pts2_calc;  // <-- CORREÇÃO: mude de pts2_cru para pts2_calc
                resultado.em_torneio = em_torneio;
                resultado.torneio_final = false;
                resultado.ultimo_jogo = CENA_PONG;

                if (em_torneio) {
                    registrar_resultado(&torneio, vencedor);
                    acumular_pontuacao(&torneio, pts1_calc, pts2_calc);
                }
                cena_atual = WindowShouldClose() ? CENA_SAIR : CENA_RESULTADO;
                break;
            }

            case CENA_VOLEI: {
                int pts1_cru = 0, pts2_cru = 0;
                int vencedor = jogar_volei(&pts1_cru, &pts2_cru);

                // Cálculo: Pontos * 100 + Bônus de 300 para o ganhador
                int pts1_calc = (pts1_cru * 100) + (vencedor == 1 ? 300 : 0);
                int pts2_calc = (pts2_cru * 100) + (vencedor == 2 ? 300 : 0);

                resultado.vencedor = vencedor;
                resultado.pontos_p1 = pts1_calc;
                resultado.pontos_p2 = pts2_calc;
                resultado.em_torneio = em_torneio;
                resultado.torneio_final = false;
                resultado.ultimo_jogo = CENA_VOLEI;

                if (em_torneio) {
                    registrar_resultado(&torneio, vencedor);
                    acumular_pontuacao(&torneio, pts1_calc, pts2_calc);
                }
                cena_atual = WindowShouldClose() ? CENA_SAIR : CENA_RESULTADO;
                break;
            }

            case CENA_RESULTADO:
                // Se acabamos de vir de um jogo em torneio, vamos ver se era o final
                if (em_torneio && torneio_acabou(&torneio) && !resultado.torneio_final) {
                    resultado.torneio_final = true;
                    // Define o vencedor pela pontuação final acumulada
                    resultado.vencedor = (torneio.pontuacoes[0] > torneio.pontuacoes[1]) ? 1 : (torneio.pontuacoes[1] > torneio.pontuacoes[0] ? 2 : 0);
                    resultado.pontos_p1 = torneio.pontuacoes[0];
                    resultado.pontos_p2 = torneio.pontuacoes[1];

                    // ==== LÓGICA DO TOP 10 NO FINAL DO TORNEIO ====
                    Score *lista = scores_carregar(SCORES_ARQUIVO);
                    
                    // Insere jogador 1 e depois jogador 2 (se entrarem no top 10, a própria função organiza e corta os piores)
                    lista = scores_inserir(lista, torneio.nomes[0], torneio.pontuacoes[0]);
                    lista = scores_inserir(lista, torneio.nomes[1], torneio.pontuacoes[1]);
                    
                    scores_salvar(SCORES_ARQUIVO, lista); // Salva sobreescrevendo com "w" e formatação correta
                    scores_liberar(lista);
                }

                cena_atual = tela_resultado(resultado);
                if (cena_atual == CENA_MENU) {
                    em_torneio = false;
                } else if (cena_atual != CENA_SAIR && em_torneio && !resultado.torneio_final) {
                    cena_atual = proxima_cena_torneio(&torneio);
                }
                break;

            case CENA_HIGHSCORES:
                cena_atual = tela_highscores(); // Chama a função que criamos em menu.c
                break;

            case CENA_SAIR:
                break;
        }
    }

    CloseWindow();

    return 0;
}