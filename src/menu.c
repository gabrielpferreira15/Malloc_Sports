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

#include "raylib.h"
#include "menu.h"
#include "cenas.h"

/* ------------------------------------------------------------------ */
/*  Helpers de UI                                                      */
/* ------------------------------------------------------------------ */

// Retorna true se o mouse está sobre o retângulo e o botão foi solto. 
static bool botao_clicado(Rectangle r) {
    return CheckCollisionPointRec(GetMousePosition(), r)
        && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}

// Desenha um botão arredondado.
// highlighted == true → cor de destaque (amarelo). 
static void desenhar_botao(Rectangle r, const char *label, bool highlighted) {
    bool hover = CheckCollisionPointRec(GetMousePosition(), r);

    Color fundo;
    if (highlighted)
        fundo = hover ? (Color){220, 180, 0, 255} : (Color){255, 210, 0, 255};
    else
        fundo = hover ? (Color){70, 70, 70, 255} : (Color){45, 45, 45, 255};

    DrawRectangleRounded(r, 0.25f, 12, fundo);
    DrawRectangleRoundedLines(r, 0.25f, 12, RAYWHITE);

    int fs = 28;
    int tw = MeasureText(label, fs);
    Color cor_texto = highlighted ? BLACK : RAYWHITE;
    DrawText(label,
            (int)(r.x + r.width  / 2 - tw / 2),
            (int)(r.y + r.height / 2 - fs / 2),
            fs, cor_texto);
}

/* ------------------------------------------------------------------ */
/*  tela_menu_principal                                               */
/* ------------------------------------------------------------------ */

Cena tela_menu_principal(void) {
    const int W = GetScreenWidth();
    const int H = GetScreenHeight();

    // Botões centralizados verticalmente 
    float bx  = W / 2.0f - 180;
    Rectangle btn_jogar = { bx, H / 2.0f - 60, 360, 60 };
    Rectangle btn_sair  = { bx, H / 2.0f + 20,  360, 60 };

    while (!WindowShouldClose()) {
        // ----- INPUT ----- 
        if (botao_clicado(btn_jogar)) return CENA_PONG;
        if (botao_clicado(btn_sair))  return CENA_SAIR;
        if (IsKeyPressed(KEY_ESCAPE)) return CENA_SAIR;

        // ----- DRAW ----- 
        BeginDrawing();
            ClearBackground((Color){18, 22, 35, 255});

            // Título 
            const char *titulo = "MALLOC SPORTS";
            int ts = 64;
            DrawText(titulo,
                    W / 2 - MeasureText(titulo, ts) / 2,
                    H / 2 - 200, ts, RAYWHITE);

            // Subtítulo
            const char *sub = "Minigames esportivos multiplayer";
            DrawText(sub,
                    W / 2 - MeasureText(sub, 22) / 2,
                    H / 2 - 120, 22, LIGHTGRAY);

            // Linha decorativa
            DrawLine(W / 2 - 200, H / 2 - 90, W / 2 + 200, H / 2 - 90,
                    (Color){80, 80, 80, 200});

            desenhar_botao(btn_jogar, "Jogar",  true);
            desenhar_botao(btn_sair,  "Sair",   false);

            // Dica de controles
            DrawText("Controles:  P1 = W/S     P2 = Setas",
                    W / 2 - MeasureText("Controles:  P1 = W/S     P2 = Setas", 18) / 2,
                    H - 40, 18, DARKGRAY);

        EndDrawing();
    }

    return CENA_SAIR;
}

/* ------------------------------------------------------------------ */
/*  tela_resultado                                                    */
/* ------------------------------------------------------------------ */
Cena tela_resultado(ResultadoPartida r) {
    const int W = GetScreenWidth();
    const int H = GetScreenHeight();

    float bx = W / 2.0f - 180;
    Rectangle btn_revanche = { bx, H / 2.0f + 40,  360, 60 };
    Rectangle btn_menu     = { bx, H / 2.0f + 120, 360, 60 };

    // Mensagem do vencedor 
    const char *msg_venc = (r.vencedor == 1) ? "Jogador 1 venceu!" : "Jogador 2 venceu!";
    
    // Placar formatado
    char placar[32];
    TextFormat("%d  x  %d", r.pontos_p1, r.pontos_p2);
    // TextFormat usa buffer interno — copiamos manualmente 
    int n = 0;
    const char *fmt = TextFormat("%d  x  %d", r.pontos_p1, r.pontos_p2);
    while (fmt[n] && n < 31) { placar[n] = fmt[n]; n++; }
    placar[n] = '\0';

    while (!WindowShouldClose()) {
        // ----- INPUT ----- 
        if (botao_clicado(btn_revanche) || IsKeyPressed(KEY_ENTER))
            return CENA_PONG;
        if (botao_clicado(btn_menu) || IsKeyPressed(KEY_ESCAPE))
            return CENA_MENU;

        // ----- DRAW ----- 
        BeginDrawing();
            ClearBackground(BLACK);

            // Faixa superior colorida (amarelo para o vencedor) 
            DrawRectangle(0, 0, W, 8,
                        (Color){255, 210, 0, 255});

            // Nome do vencedor */
            int mv = 52;
            DrawText(msg_venc,
                    W / 2 - MeasureText(msg_venc, mv) / 2,
                    H / 2 - 140, mv, YELLOW);

            // Placar */
            const char *label_p1 = "P1";
            const char *label_p2 = "P2";
            int pts_fs = 80;
            int label_fs = 24;

            // P1
            DrawText(label_p1,
                    W / 2 - 140 - MeasureText(label_p1, label_fs) / 2,
                    H / 2 - 60, label_fs, LIGHTGRAY);
            DrawText(TextFormat("%d", r.pontos_p1),
                    W / 2 - 140 - MeasureText("0", pts_fs) / 2,
                    H / 2 - 30, pts_fs,
                    r.vencedor == 1 ? YELLOW : GRAY);

            // Separador
            DrawText("x",
                    W / 2 - MeasureText("x", pts_fs) / 2,
                    H / 2 - 30, pts_fs, DARKGRAY);

            // P2
            DrawText(label_p2,
                    W / 2 + 140 - MeasureText(label_p2, label_fs) / 2,
                    H / 2 - 60, label_fs, LIGHTGRAY);

            DrawText(TextFormat("%d", r.pontos_p2),
                    W / 2 + 140 - MeasureText("0", pts_fs) / 2,
                    H / 2 - 30, pts_fs,
                    r.vencedor == 2 ? YELLOW : GRAY);

            // Botões
            desenhar_botao(btn_revanche, "Revanche",      true);
            desenhar_botao(btn_menu,     "Voltar ao Menu", false);

            // Dica de teclado
            DrawText("ENTER = Revanche     ESC = Menu",
                    W / 2 - MeasureText("ENTER = Revanche     ESC = Menu", 18) / 2,
                    H - 40, 18, DARKGRAY);

        EndDrawing();
    }

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
 * ATENÇÃO
 *   - Cada tela deve responder a KEY_ESCAPE pra evitar que o
 *     jogador fique preso.
 *   - GetCharPressed devolve 0 quando não há tecla — sempre
 *     teste num while pra capturar várias teclas no mesmo frame.
 * ============================================================ */
