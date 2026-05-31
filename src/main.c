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
                    
                    // Somamos ao torneio os pontos já calculados pela fórmula
                    acumular_pontuacao(&torneio, resultado.pontos_p1, resultado.pontos_p2);
                    
                    // Salva o estado do placar geral acumulado para o menu exibir
                    resultado.pontos_torneio_p1 = torneio.pontuacoes[0];
                    resultado.pontos_torneio_p2 = torneio.pontuacoes[1];
                } else {
                    resultado.pontos_torneio_p1 = 0;
                    resultado.pontos_torneio_p2 = 0;
                }

                cena_atual = WindowShouldClose() ? CENA_SAIR : CENA_RESULTADO;
                break;
            }

            case CENA_PONG: {
                int pts1_cru = 0, pts2_cru = 0;
                int vencedor = jogar_pong_memoria(&pts1_cru, &pts2_cru);

                // 1. Calcula a pontuação com base na fórmula original (PRESERVADA)
                int pts1_calc = (pts1_cru * 100) + (vencedor == 1 ? 200 : 0);
                int pts2_calc = (pts2_cru * 100) + (vencedor == 2 ? 200 : 0);

                // 2. Preenche a estrutura de resultado para a tela intermediária
                resultado.vencedor = vencedor;
                resultado.pontos_p1 = pts1_calc; 
                resultado.pontos_p2 = pts2_calc; 
                resultado.em_torneio = em_torneio;
                resultado.torneio_final = false;
                resultado.ultimo_jogo = CENA_PONG;

                if (em_torneio) {
                    registrar_resultado(&torneio, vencedor);
                    acumular_pontuacao(&torneio, pts1_calc, pts2_calc);
                    
                    // NOVO: Salva os pontos totais acumulados até esta partida do Pong
                    resultado.pontos_torneio_p1 = torneio.pontuacoes[0];
                    resultado.pontos_torneio_p2 = torneio.pontuacoes[1];
                } else {
                    // Modo avulso deixa zerado
                    resultado.pontos_torneio_p1 = 0;
                    resultado.pontos_torneio_p2 = 0;
                }
                cena_atual = WindowShouldClose() ? CENA_SAIR : CENA_RESULTADO;
                break;
            }

            case CENA_VOLEI: {
                int pts1_cru = 0, pts2_cru = 0;
                int vencedor = jogar_volei(&pts1_cru, &pts2_cru);

                // Cálculo: Pontos * 100 + Bônus de 200 para o ganhador
                int pts1_calc = (pts1_cru * 100) + (vencedor == 1 ? 200 : 0);
                int pts2_calc = (pts2_cru * 100) + (vencedor == 2 ? 200 : 0);

                resultado.pontos_p1 = pts1_calc;
                resultado.pontos_p2 = pts2_calc;
                resultado.ultimo_jogo = CENA_VOLEI;
                resultado.em_torneio = em_torneio;

                if (em_torneio) {
                    // 1. Acumula os pontos do vôlei nas pontuações globais do torneio
                    acumular_pontuacao(&torneio, pts1_calc, pts2_calc);
                    registrar_resultado(&torneio, vencedor);

                    // 2. Como o vôlei é o último, ativa direto a tela final do torneio
                    resultado.torneio_final = true;
                    resultado.vencedor = vencedor_torneio(&torneio);
                    
                    // 3. Salva os pontos totais acumulados de todo o torneio
                    resultado.pontos_torneio_p1 = torneio.pontuacoes[0];
                    resultado.pontos_torneio_p2 = torneio.pontuacoes[1];

                    // ==== LÓGICA DO TOP 10 NO FINAL DO TORNEIO ====
                    Score *lista = scores_carregar(SCORES_ARQUIVO);
                    lista = scores_inserir(lista, torneio.nomes[0], torneio.pontuacoes[0]);
                    lista = scores_inserir(lista, torneio.nomes[1], torneio.pontuacoes[1]);
                    scores_salvar(SCORES_ARQUIVO, lista);
                    scores_liberar(lista);
                } else {
                    // Se for modo avulso normal, segue o padrão sem tela de torneio
                    resultado.torneio_final = false;
                    resultado.vencedor = vencedor;
                }

                cena_atual = WindowShouldClose() ? CENA_SAIR : CENA_RESULTADO;
                break;
            }

            case CENA_RESULTADO:
                cena_atual = tela_resultado(resultado);
                if (cena_atual == CENA_MENU) {
                    em_torneio = false;
                } else if (cena_atual != CENA_SAIR && em_torneio && !resultado.torneio_final) {
                    
                    // SE O TORNEIO ACABOU APÓS ESSA PARTIDA:
                    if (torneio_acabou(&torneio)) {
                        // Prepara os dados para exibir o grande vencedor do TORNEIO
                        resultado.torneio_final = true;
                        resultado.vencedor = vencedor_torneio(&torneio);
                        resultado.pontos_p1 = torneio.pontuacoes[0];
                        resultado.pontos_p2 = torneio.pontuacoes[1];

                        // ==== SALVA NO TOP 10 APENAS AQUI NO FIM DE TUDO ====
                        Score *lista = scores_carregar(SCORES_ARQUIVO);
                        lista = scores_inserir(lista, torneio.nomes[0], torneio.pontuacoes[0]);
                        lista = scores_inserir(lista, torneio.nomes[1], torneio.pontuacoes[1]);
                        scores_salvar(SCORES_ARQUIVO, lista);
                        scores_liberar(lista);

                        // Força o jogo a continuar na CENA_RESULTADO, mas agora com os dados do torneio!
                        cena_atual = CENA_RESULTADO;
                    } else {
                        // Se não acabou, segue normal para o próximo minigame do torneio
                        cena_atual = proxima_cena_torneio(&torneio);
                    }
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