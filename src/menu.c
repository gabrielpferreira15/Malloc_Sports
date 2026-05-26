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

    Cena cena_atual = CENA_MENU;

    while (!WindowShouldClose()) {
        Cena prox_cena = cena_atual;

        // ----- INPUT ----- 
        if (botao_clicado(btn_jogar)) prox_cena = CENA_SELECAO_MODO;
        else if (botao_clicado(btn_sair))  prox_cena = CENA_SAIR;
        else if (IsKeyPressed(KEY_ESCAPE)) prox_cena = CENA_SAIR;

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
        
        if (prox_cena != cena_atual) return prox_cena;
    }

    return CENA_SAIR;
}

/* ------------------------------------------------------------------ */
/*  tela_selecao_modo                                                 */
/* ------------------------------------------------------------------ */
Cena tela_selecao_modo(EstadoTorneio *t) {
    const int W = GetScreenWidth();
    const int H = GetScreenHeight();

    float bx = W / 2.0f - 180;
    Rectangle btn_avulso  = { bx, H / 2.0f - 60, 360, 60 };
    Rectangle btn_torneio = { bx, H / 2.0f + 20, 360, 60 };

    Cena cena_atual = CENA_SELECAO_MODO;

    while (!WindowShouldClose()) {
        Cena prox_cena = cena_atual;

        if (botao_clicado(btn_avulso)) {
            prox_cena = CENA_SELECAO_MINIGAME;
        }
        else if (botao_clicado(btn_torneio)) {
            iniciar_torneio(t);
            prox_cena = proxima_cena_torneio(t);
        }
        else if (IsKeyPressed(KEY_ESCAPE)) {
            prox_cena = CENA_MENU;
        }

        BeginDrawing();
        ClearBackground((Color){18, 22, 35, 255});
        
        DrawText("SELECIONE O MODO", W/2 - MeasureText("SELECIONE O MODO", 40)/2, H/2 - 150, 40, RAYWHITE);
        
        desenhar_botao(btn_avulso, "Minigame Único", true);
        desenhar_botao(btn_torneio, "Torneio", false);

        DrawText("ESC = Voltar", W/2 - MeasureText("ESC = Voltar", 18)/2, H - 40, 18, DARKGRAY);

        EndDrawing();
        
        if (prox_cena != cena_atual) return prox_cena;
    }
    return CENA_SAIR;
}

/* ------------------------------------------------------------------ */
/*  tela_selecao_minigame                                             */
/* ------------------------------------------------------------------ */
Cena tela_selecao_minigame(void) {
    const int W = GetScreenWidth();
    const int H = GetScreenHeight();

    int opcao_selecionada = 0; // 0=Corrida, 1=Pong
    Cena cena_atual = CENA_SELECAO_MINIGAME;
    
    while (!WindowShouldClose()) {
        Cena prox_cena = cena_atual;

        if (IsKeyPressed(KEY_RIGHT)) {
            opcao_selecionada = (opcao_selecionada + 1) % 2;
        }
        if (IsKeyPressed(KEY_LEFT)) {
            opcao_selecionada = (opcao_selecionada - 1 + 2) % 2; 
        }
        if (IsKeyPressed(KEY_ENTER)) {
            if (opcao_selecionada == 0) prox_cena = CENA_CORRIDA;
            if (opcao_selecionada == 1) prox_cena = CENA_PONG;
        }
        if (IsKeyPressed(KEY_ESCAPE)) prox_cena = CENA_SELECAO_MODO;

        Rectangle card_corrida = { W / 2.0f - 240, H / 2.0f - 100, 200, 200 };
        Rectangle card_pong    = { W / 2.0f + 40,  H / 2.0f - 100, 200, 200 };

        if (botao_clicado(card_corrida)) prox_cena = CENA_CORRIDA;
        else if (botao_clicado(card_pong)) prox_cena = CENA_PONG;

        BeginDrawing();
        ClearBackground((Color){18, 22, 35, 255});

        DrawText("SELECIONE O JOGO", W/2 - MeasureText("SELECIONE O JOGO", 40)/2, H/2 - 200, 40, RAYWHITE);

        desenhar_botao(card_corrida, "Corrida", opcao_selecionada == 0);
        desenhar_botao(card_pong, "Pong", opcao_selecionada == 1);

        DrawText("USE AS SETAS E ENTER", W/2 - MeasureText("USE AS SETAS E ENTER", 20)/2, H/2 + 150, 20, LIGHTGRAY);
        DrawText("ESC = Voltar", W/2 - MeasureText("ESC = Voltar", 18)/2, H - 40, 18, DARKGRAY);

        EndDrawing();

        if (prox_cena != cena_atual) return prox_cena;
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
    const char *msg_venc;
    if (r.vencedor == 1) msg_venc = "Jogador 1 venceu!";
    else if (r.vencedor == 2) msg_venc = "Jogador 2 venceu!";
    else msg_venc = "Empate!";

    if (r.torneio_final) {
        if (r.vencedor == 1) msg_venc = "Jogador 1 venceu o Torneio!";
        else if (r.vencedor == 2) msg_venc = "Jogador 2 venceu o Torneio!";
        else msg_venc = "Torneio Empatado!";
    }

    // Texto do botão principal (destaque)
    const char *texto_btn_principal = "Revanche";
    if (r.torneio_final) texto_btn_principal = "Voltar ao Menu";
    else if (r.em_torneio) texto_btn_principal = "Próximo Jogo";
    
    // Placar formatado
    char placar[32];
    int n = 0;
    const char *fmt = TextFormat("%d  x  %d", r.pontos_p1, r.pontos_p2);
    while (fmt[n] && n < 31) { placar[n] = fmt[n]; n++; }
    placar[n] = '\0';

    Cena cena_atual = CENA_RESULTADO;

    while (!WindowShouldClose()) {
        Cena prox_cena = cena_atual;

        // ----- INPUT ----- 
        if (botao_clicado(btn_revanche) || IsKeyPressed(KEY_ENTER)) {
            if (r.torneio_final) prox_cena = CENA_MENU;
            else if (r.em_torneio) prox_cena = CENA_PONG; // Retorna PONG genérico para sinalizar 'Próximo', o main decide
            else prox_cena = r.ultimo_jogo; // Retorna o jogo que acabamos jogar
        }
        
        if (!r.torneio_final) {
            if (botao_clicado(btn_menu) || IsKeyPressed(KEY_ESCAPE))
                prox_cena = CENA_MENU;
        }

        // ----- DRAW ----- 
        BeginDrawing();
            ClearBackground(BLACK);

            // Faixa superior colorida (amarelo para o vencedor) 
            DrawRectangle(0, 0, W, 8, (Color){255, 210, 0, 255});

            if (r.torneio_final) {
                DrawText("RESULTADO DO TORNEIO", W / 2 - MeasureText("RESULTADO DO TORNEIO", 30) / 2, 40, 30, GOLD);
            }

            // Nome do vencedor 
            int mv = 48;
            DrawText(msg_venc,
                    W / 2 - MeasureText(msg_venc, mv) / 2,
                    H / 2 - 140, mv, YELLOW);

            // Placar 
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
            desenhar_botao(btn_revanche, texto_btn_principal, true);
            if (!r.torneio_final) {
                desenhar_botao(btn_menu, "Voltar ao Menu", false);
                DrawText("ENTER = Confirmar     ESC = Menu",
                        W / 2 - MeasureText("ENTER = Confirmar     ESC = Menu", 18) / 2,
                        H - 40, 18, DARKGRAY);
            } else {
                DrawText("ENTER = Confirmar", W / 2 - MeasureText("ENTER = Confirmar", 18) / 2, H - 40, 18, DARKGRAY);
            }

        EndDrawing();
        
        if (prox_cena != cena_atual) return prox_cena;
    }

    return CENA_SAIR;
}

/*                                  
 * ATENÇÃO
 *   - Cada tela deve responder a KEY_ESCAPE pra evitar que o
 *     jogador fique preso.
 *   - GetCharPressed devolve 0 quando não há tecla — sempre
 *     teste num while pra capturar várias teclas no mesmo frame.
 * ============================================================ */
