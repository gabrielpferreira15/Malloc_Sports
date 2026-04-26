/* ============================================================
 * menu.c — IMPLEMENTAÇÃO de todas as telas de UI
 * ------------------------------------------------------------
 * Dono: DEV 3 — Gabriel Mendes
 *
 * COMO ESTRUTURAR CADA TELA (padrão geral)
 *
 *   Cena tela_X(void) {
 *       while (!WindowShouldClose()) {
 *           // ----- INPUT -----
 *           if (IsKeyPressed(KEY_ENTER))  return CENA_PROXIMA;
 *           if (IsKeyPressed(KEY_ESCAPE)) return CENA_VOLTAR;
 *
 *           // ----- DRAW -----
 *           BeginDrawing();
 *           ClearBackground(BLACK);
 *           // DrawText, DrawRectangle, etc.
 *           EndDrawing();
 *       }
 *       return CENA_SAIR;  // janela fechada
 *   }
 *
 * SUGESTOES TELA POR TELA
 *
 *   --- tela_splash ---
 *   Mostra "MALLOC SPORTS" digitando letra por letra (efeito
 *   terminal). Use GetTime() ou um contador de frames pra
 *   revelar uma letra a cada N ms.
 *   Apos 2 segundos, retorna CENA_MENU.
 *
 *   Variaveis sugeridas:
 *     float tempo_inicio = GetTime();
 *     int letras_visiveis = 0;
 *     const char *título = "MALLOC SPORTS";
 *
 *   --- tela_menu_principal ---
 *   4 opções: Torneio, Minigame Único, Ranking, Sair.
 *
 *   Variaveis sugeridas:
 *     int opcao_selecionada = 0;  // 0..3
 *     const char *opções[4] = {"Torneio", "Minigame Único",
 *                              "Ranking", "Sair"};
 *
 *   KEY_UP / KEY_DOWN -> mexe opcao_selecionada (com wrap-around).
 *   KEY_ENTER -> retorna a cena correspondente.
 *
 *   No DRAW: itera sobre as opções; pinta a opcao_selecionada
 *   com cor diferente.
 *
 *   --- tela_selecao_minigame ---
 *   3 opções lado a lado (Corrida de Ponteiros, Pong da Memória, Estouro de Pilha).
 *   KEY_LEFT / KEY_RIGHT muda card; ENTER inicia.
 *
 *   --- tela_resultado ---
 *   Mostra "VENCEDOR: JOGADOR 1" gigante, placar parcial,
 *   "Aperte ENTER para continuar".
 *
 *   --- tela_campeonato ---
 *   Animação de confete (várias partículas pulando) usando
 *   um array de 50-100 estruturas Particula simples — pode
 *   declarar a struct localmente neste arquivo:
 *
 *     typedef struct {
 *         float x, y;
 *         float vel_x, vel_y;
 *         Color cor;
 *     } Particula;
 *
 *     Particula confete[100];
 *     // inicialize cada uma com posição e velocidades aleatórias
 *
 *   Input do nome:
 *     char nome[16] = {0};
 *     int  pos = 0;
 *     while (loop) {
 *         int c = GetCharPressed();   // função da raylib
 *         while (c > 0) {
 *             if (pos < 15 && c >= 32 && c <= 125) {
 *                 nome[pos++] = (char)c;
 *                 nome[pos]   = '\0';
 *             }
 *             c = GetCharPressed();
 *         }
 *         if (IsKeyPressed(KEY_BACKSPACE) && pos > 0)
 *             nome[--pos] = '\0';
 *         if (IsKeyPressed(KEY_ENTER) && pos > 0)
 *             break;  // confirmou
 *     }
 *     strcpy(nome_saida, nome);
 *
 *   --- tela_ranking ---
 *   Recebe a cabeça da lista de Score. Percorre com um while
 *   e desenha cada um com DrawText. ESC volta pro menu.
 *
 *     Score *atual = cabeça;
 *     int y = 100;
 *     while (atual != NULL) {
 *         DrawText(atual->nome, 100, y, 30, WHITE);
 *         DrawText(TextFormat("%d", atual->pontos),
 *                  500, y, 30, YELLOW);
 *         y += 40;
 *         atual = atual->próximo;
 *     }
 *
 * COMO TESTAR
 *   1. Faça CADA tela como uma simples DrawText("nome da tela")
 *      e ENTER avançando para a próxima.
 *   2. Quando a navegação toda funcionar, melhore o visual.
 *
 * ATENÇÃO
 *   - Cada tela deve responder a KEY_ESCAPE pra evitar que o
 *     jogador fique preso.
 *   - Use Color do raylib pras cores. Sugestao de paleta no
 *     GDD: ciano (SKYBLUE) e amarelo (YELLOW) sobre BLACK.
 *   - GetCharPressed devolve 0 quando não há tecla — sempre
 *     teste num while pra capturar várias teclas no mesmo frame.
 * ============================================================ */

/* TODO: includes + implementação das 6 telas */
