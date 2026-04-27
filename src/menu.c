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
 *   3 opções: Jogar (Que abre uma tela com as opções de "Minigame Único" e "Torneio"), Configurações, Sair.
 *
 *   Variaveis sugeridas:
 *     int opcao_selecionada = 0;  // 0..3
 *     const char *opções[3] = {"Jogar", "Configurações", "Sair"};
 *
 *   KEY_UP / KEY_DOWN -> mexe opcao_selecionada (com wrap-around).
 *   KEY_ENTER -> retorna a cena correspondente.
 *
 *   No DRAW: itera sobre as opções; pinta a opcao_selecionada
 *   com cor diferente.
 */ 
typedef enum {
    CENA_MENU,
    CENA_PONG,
    CENA_RESULTADO,
    CENA_SAIR
} Cena;

#include "raylib.h"

static bool ButtonClicked(Rectangle rect)
{
    return CheckCollisionPointRec(GetMousePosition(), rect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static void DrawMenuButton(Rectangle rect, const char *label, bool active)
{
    bool hover = CheckCollisionPointRec(GetMousePosition(), rect);

    Color fill = active ? (hover ? DARKBLUE : BLUE)
                        : (hover ? DARKGRAY : GRAY);

    DrawRectangleRounded(rect, 0.2f, 12, fill);
    DrawRectangleRoundedLines(rect, 0.2f, 12, RAYWHITE);

    int fontSize = 28;
    int textWidth = MeasureText(label, fontSize);

    DrawText(label,
            (int)(rect.x + rect.width/2 - textWidth/2),
            (int)(rect.y + rect.height/2 - fontSize/2 - 4),
            fontSize,
            RAYWHITE);
}

Cena menu_principal(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Malloc Sports");
    SetTargetFPS(60);

    bool running = true;

    Rectangle btnPlay     = { screenWidth/2.0f - 180, 280, 360, 60 };
    Rectangle btnSettings = { screenWidth/2.0f - 180, 360, 360, 60 };
    Rectangle btnExit     = { screenWidth/2.0f - 180, 440, 360, 60 };

    while (running && !WindowShouldClose())
    {
        if (ButtonClicked(btnExit) || IsKeyPressed(KEY_ESCAPE))
            running = false;

        BeginDrawing();
        ClearBackground((Color){ 18, 22, 35, 255 });

        DrawText("MALLOC SPORTS",
                screenWidth/2 - MeasureText("MALLOC SPORTS", 56)/2,
                90, 56, RAYWHITE);

        DrawText("Mini games esportivos multiplayer",
                screenWidth/2 - MeasureText("Mini games esportivos multiplayer", 24)/2,
                150, 24, LIGHTGRAY);

        DrawMenuButton(btnPlay, "Jogar", false);
        DrawMenuButton(btnSettings, "Configuracoes", false);
        DrawMenuButton(btnExit, "Sair", true);

        EndDrawing();
    }

    CloseWindow();
    return CENA_SAIR;
}

    /*
 *  --- tela_seleção_modo ---
 *  Opção entre "Minigame Único" e "Torneio".
 *  Ao clicar em "Minigame Único", vai pra tela_selecao_minigame.
 *  Ao clicar em "Torneio", incia o torneio pegando um dos minigames aleatoriamente e vai 
 *  escolhendo os próximos até passar por todos os jogos.
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
