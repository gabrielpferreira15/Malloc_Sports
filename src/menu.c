#include <string.h>
#include "raylib.h"
#include "menu.h"
#include "cenas.h"
#include "scores.h"

/* ------------------------------------------------------------------ */
/*  Helpers de UI                                                       */
/* ------------------------------------------------------------------ */

static bool botao_clicado(Rectangle r) {
    return CheckCollisionPointRec(GetMousePosition(), r)
        && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}

static void desenhar_botao(Rectangle r, const char *label, bool highlighted) {
    bool hover = CheckCollisionPointRec(GetMousePosition(), r);

    Color fundo;
    if (highlighted)
        fundo = hover ? (Color){220, 180,   0, 255} : (Color){255, 210, 0, 255};
    else
        fundo = hover ? (Color){ 70,  70,  70, 255} : (Color){ 45,  45, 45, 255};

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
/*  tela_menu_principal                                                 */
/* ------------------------------------------------------------------ */
Cena tela_menu_principal(void) {
    const int W = GetScreenWidth();
    const int H = GetScreenHeight();

    float bx = W / 2.0f - 180;
    /* Três botões com 80 px de espaçamento entre si. */
    Rectangle btn_jogar       = { bx, H / 2.0f - 100, 360, 60 };
    Rectangle btn_highscores  = { bx, H / 2.0f -  20, 360, 60 };
    Rectangle btn_sair        = { bx, H / 2.0f +  60, 360, 60 };

    Cena cena_atual = CENA_MENU;

    while (!WindowShouldClose()) {
        Cena prox = cena_atual;

        if (botao_clicado(btn_jogar))      prox = CENA_SELECAO_MODO;
        else if (botao_clicado(btn_highscores)) prox = CENA_HIGHSCORES;
        else if (botao_clicado(btn_sair))  prox = CENA_SAIR;

        BeginDrawing();
            ClearBackground((Color){18, 22, 35, 255});

            /* Título */
            const char *titulo = "MALLOC SPORTS";
            int ts = 64;
            DrawText(titulo,
                W / 2 - MeasureText(titulo, ts) / 2,
                H / 2 - 210, ts, RAYWHITE);

            /* Subtítulo */
            const char *sub = "Minigames esportivos multiplayer";
            DrawText(sub,
                W / 2 - MeasureText(sub, 22) / 2,
                H / 2 - 130, 22, LIGHTGRAY);

            /* Linha decorativa */
            DrawLine(W / 2 - 200, H / 2 - 108,
                    W / 2 + 200, H / 2 - 108,
                    (Color){80, 80, 80, 200});

            desenhar_botao(btn_jogar,      "Jogar",                true);
            desenhar_botao(btn_highscores, "Highscores - Torneio", false);
            desenhar_botao(btn_sair,       "Sair",                 false);

            DrawText("Controles:  P1 = W/S/A/D     P2 = Setas",
                W / 2 - MeasureText("Controles:  P1 = W/S/A/D     P2 = Setas", 18) / 2,
                H - 40, 18, DARKGRAY);
        EndDrawing();

        if (prox != cena_atual) return prox;
    }
    return CENA_SAIR;
}

/* ------------------------------------------------------------------ */
/*  tela_selecao_modo                                                   */
/* ------------------------------------------------------------------ */
Cena tela_selecao_modo(EstadoTorneio *t) {
    const int W = GetScreenWidth();
    const int H = GetScreenHeight();

    float bx = W / 2.0f - 180;
    Rectangle btn_avulso  = { bx, H / 2.0f - 60, 360, 60 };
    Rectangle btn_torneio = { bx, H / 2.0f + 20, 360, 60 };

    Cena cena_atual = CENA_SELECAO_MODO;

    while (!WindowShouldClose()) {
        Cena prox = cena_atual;

        if (botao_clicado(btn_avulso)) {
            prox = CENA_SELECAO_MINIGAME;
        } else if (botao_clicado(btn_torneio)) {
            iniciar_torneio(t); 
            Cena prox = tela_inserir_nomes(t->nomes[0], t->nomes[1]);
            if(prox == CENA_SELECAO_MODO) {
                prox = CENA_SELECAO_MODO;
            } else {
                prox = proxima_cena_torneio(t);
            }
        } else if (IsKeyPressed(KEY_ESCAPE)) {
            prox = CENA_MENU;
        }

        BeginDrawing();
            ClearBackground((Color){18, 22, 35, 255});
            DrawText("SELECIONE O MODO",
                W / 2 - MeasureText("SELECIONE O MODO", 40) / 2,
                H / 2 - 150, 40, RAYWHITE);
            desenhar_botao(btn_avulso,  "Minigame Único", false);
            desenhar_botao(btn_torneio, "Torneio",        false);
            DrawText("ESC = Voltar",
                W / 2 - MeasureText("ESC = Voltar", 18) / 2,
                H - 40, 18, DARKGRAY);
        EndDrawing();

        if (prox != cena_atual) return prox;
    }
    return CENA_SAIR;
}

/* ------------------------------------------------------------------ */
/*  tela_selecao_minigame                                               */
/* ------------------------------------------------------------------ */
Cena tela_selecao_minigame(void) {
    const int W = GetScreenWidth();
    const int H = GetScreenHeight();

    int opcao = 0;
    Cena cena_atual = CENA_SELECAO_MINIGAME;

    while (!WindowShouldClose()) {
        Cena prox = cena_atual;

        if (IsKeyPressed(KEY_RIGHT)) opcao = (opcao + 1) % 3;
        if (IsKeyPressed(KEY_LEFT))  opcao = (opcao - 1 + 3) % 3;
        if (IsKeyPressed(KEY_ENTER)) {
            if (opcao == 0) prox = CENA_CORRIDA;
            if (opcao == 1) prox = CENA_PONG;
            if (opcao == 2) prox = CENA_VOLEI;
        }
        if (IsKeyPressed(KEY_ESCAPE)) prox = CENA_SELECAO_MODO;

        Rectangle card_corrida = { W / 2.0f - 340, H / 2.0f - 100, 200, 200 };
        Rectangle card_pong    = { W / 2.0f - 100, H / 2.0f - 100, 200, 200 };
        Rectangle card_volei   = { W / 2.0f + 140, H / 2.0f - 100, 200, 200 };

        if (botao_clicado(card_corrida)) prox = CENA_CORRIDA;
        else if (botao_clicado(card_pong))   prox = CENA_PONG;
        else if (botao_clicado(card_volei))  prox = CENA_VOLEI;

        BeginDrawing();
            ClearBackground((Color){18, 22, 35, 255});
            DrawText("SELECIONE O JOGO",
                W / 2 - MeasureText("SELECIONE O JOGO", 40) / 2,
                H / 2 - 200, 40, RAYWHITE);
            desenhar_botao(card_corrida, "Corrida", opcao == 0);
            desenhar_botao(card_pong,    "Pong",    opcao == 1);
            desenhar_botao(card_volei,   "Vôlei",   opcao == 2);
            DrawText("USE AS SETAS E ENTER",
                W / 2 - MeasureText("USE AS SETAS E ENTER", 20) / 2,
                H / 2 + 150, 20, LIGHTGRAY);
            DrawText("ESC = Voltar",
                W / 2 - MeasureText("ESC = Voltar", 18) / 2,
                H - 40, 18, DARKGRAY);
        EndDrawing();

        if (prox != cena_atual) return prox;
    }
    return CENA_SAIR;
}

/* ------------------------------------------------------------------ */
/* tela_resultado                                                      */
/* ------------------------------------------------------------------ */
Cena tela_resultado(ResultadoPartida r) {
    const int W = GetScreenWidth();
    const int H = GetScreenHeight();

    // Botões movidos estrategicamente para baixo para evitar qualquer sobreposição
    float bx = W / 2.0f - 180;
    Rectangle btn_principal = { bx, (float)H - 190, 360, 55 };
    Rectangle btn_menu      = { bx, (float)H - 120, 360, 55 };

    /* Mensagem de vencedor */
    const char *msg_venc;
    if (r.torneio_final) {
        if      (r.vencedor == 1) msg_venc = "Jogador 1 venceu o Torneio!";
        else if (r.vencedor == 2) msg_venc = "Jogador 2 venceu o Torneio!";
        else                      msg_venc = "Torneio Empatado!";
    } else {
        if      (r.vencedor == 1) msg_venc = "Jogador 1 venceu!";
        else if (r.vencedor == 2) msg_venc = "Jogador 2 venceu!";
        else                      msg_venc = "Empate!";
    }

    /* Texto do botão principal */
    const char *texto_btn;
    if (r.torneio_final)   texto_btn = "Voltar ao Menu";
    else if (r.em_torneio) texto_btn = "Próximo Jogo";
    else                   texto_btn = "Revanche";

    Cena cena_atual = CENA_RESULTADO;

    while (!WindowShouldClose()) {
        Cena prox = cena_atual;

        if (botao_clicado(btn_principal) || IsKeyPressed(KEY_ENTER)) {
            if (r.torneio_final)   prox = CENA_MENU;
            else if (r.em_torneio) prox = CENA_PONG;   /* sinal "Próximo" para o main */
            else                   prox = r.ultimo_jogo;
        }
        if (!r.torneio_final) {
            if (botao_clicado(btn_menu) || IsKeyPressed(KEY_ESCAPE))
                prox = CENA_MENU;
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawRectangle(0, 0, W, 8, (Color){255, 210, 0, 255});

            if (r.torneio_final) {
                DrawText("RESULTADO DO TORNEIO",
                    W / 2 - MeasureText("RESULTADO DO TORNEIO", 30) / 2,
                    20, 30, GOLD);
            } else {
                DrawText("RESULTADO DA PARTIDA",
                    W / 2 - MeasureText("RESULTADO DA PARTIDA", 30) / 2,
                    20, 30, RAYWHITE);
            }

            /* Mensagem do Vencedor */
            int mv = 44;
            DrawText(msg_venc,
                W / 2 - MeasureText(msg_venc, mv) / 2,
                100, mv, YELLOW);

            /* ============================================================
            PLACAR CONDICIONAL REORGANIZADO
            ============================================================ */
            
            if (r.torneio_final) {
                // FIM DO TORNEIO: Exibe o placar direto do Vôlei E o placar acumulado total
                
                // 1. Placar Direto do Vôlei (Último jogo)
                const char *volei_p1 = TextFormat("Vôlei: %d pts", r.pontos_p1);
                const char *volei_p2 = TextFormat("Vôlei: %d pts", r.pontos_p2);

                DrawText("ÚLTIMO JOGO", W / 2 - MeasureText("ÚLTIMO JOGO", 18) / 2, H / 2 - 130, 18, GRAY);
                DrawText(volei_p1, W / 2 - 180 - MeasureText(volei_p1, 22) / 2, H / 2 - 105, 22, LIGHTGRAY);
                DrawText("x", W / 2 - MeasureText("x", 22) / 2, H / 2 - 105, 22, DARKGRAY);
                DrawText(volei_p2, W / 2 + 180 - MeasureText(volei_p2, 22) / 2, H / 2 - 105, 22, LIGHTGRAY);

                // Linha divisória fina para separar as informações
                DrawLine(W / 2 - 200, H / 2 - 65, W / 2 + 200, H / 2 - 65, (Color){60, 60, 60, 255});

                // 2. Pontuação Geral Acumulada do Torneio Completo
                const char *total_p1 = TextFormat("%d", r.pontos_torneio_p1);
                const char *total_p2 = TextFormat("%d", r.pontos_torneio_p2);

                DrawText("PONTUAÇÃO TOTAL DO TORNEIO", W / 2 - MeasureText("PONTUAÇÃO TOTAL DO TORNEIO", 18) / 2, H / 2 - 45, 18, GOLD);
                
                /* P1 */
                DrawText("P1", W / 2 - 180 - MeasureText("P1", 24) / 2, H / 2 - 15, 24, LIGHTGRAY);
                DrawText(total_p1, W / 2 - 180 - MeasureText(total_p1, 54) / 2, H / 2 + 15, 54, r.vencedor == 1 ? YELLOW : RAYWHITE);

                /* Separador */
                DrawText("x", W / 2 - MeasureText("x", 40) / 2, H / 2 + 20, 40, DARKGRAY);

                /* P2 */
                DrawText("P2", W / 2 + 180 - MeasureText("P2", 24) / 2, H / 2 - 15, 24, LIGHTGRAY);
                DrawText(total_p2, W / 2 + 180 - MeasureText(total_p2, 54) / 2, H / 2 + 15, 54, r.vencedor == 2 ? YELLOW : RAYWHITE);
            }
            else if (r.em_torneio) {
                // DURANTE O TORNEIO (Após Pong ou Corrida)
                const char *jogo_p1  = TextFormat("Jogo: %d pts", r.pontos_p1);
                const char *total_p1 = TextFormat("Total: %d", r.pontos_torneio_p1);
                const char *jogo_p2  = TextFormat("Jogo: %d pts", r.pontos_p2);
                const char *total_p2 = TextFormat("Total: %d", r.pontos_torneio_p2);

                /* P1 (Esquerda) */
                DrawText("P1", W / 2 - 160 - MeasureText("P1", 24) / 2, H / 2 - 65, 24, LIGHTGRAY);
                DrawText(jogo_p1, W / 2 - 160 - MeasureText(jogo_p1, 20) / 2, H / 2 - 38, 20, GRAY);
                DrawText(total_p1, W / 2 - 160 - MeasureText(total_p1, 32) / 2, H / 2 - 12, 32, r.vencedor == 1 ? YELLOW : RAYWHITE);

                /* Separador Central */
                DrawText("x", W / 2 - MeasureText("x", 32) / 2, H / 2 - 12, 32, DARKGRAY);

                /* P2 (Direita) */
                DrawText("P2", W / 2 + 160 - MeasureText("P2", 24) / 2, H / 2 - 65, 24, LIGHTGRAY);
                DrawText(jogo_p2, W / 2 + 160 - MeasureText(jogo_p2, 20) / 2, H / 2 - 38, 20, GRAY);
                DrawText(total_p2, W / 2 + 160 - MeasureText(total_p2, 32) / 2, H / 2 - 12, 32, r.vencedor == 2 ? YELLOW : RAYWHITE);
            } 
            else {
                // JOGO AVULSO NORMAL
                if (r.ultimo_jogo == CENA_CORRIDA) {
                    // Oculta completamente a pontuação na corrida avulsa, exibe apenas que terminou.
                    // (A mensagem de "Jogador X Venceu" já está sendo desenhada na parte de cima da tela)
                    const char *msg_fim = "FIM DA CORRIDA";
                    DrawText(msg_fim, W / 2 - MeasureText(msg_fim, 24) / 2, H / 2 - 20, 24, LIGHTGRAY);
                } else {
                    // Exibe puramente e direto a pontuação da partida grande e limpa para Pong ou Vôlei
                    const char *str_p1 = TextFormat("%d", r.pontos_p1);
                    const char *str_p2 = TextFormat("%d", r.pontos_p2);

                    /* P1 */
                    DrawText("P1", W / 2 - 160 - MeasureText("P1", 24) / 2, H / 2 - 60, 24, LIGHTGRAY);
                    DrawText(str_p1, W / 2 - 160 - MeasureText(str_p1, 80) / 2, H / 2 - 30, 80, r.vencedor == 1 ? YELLOW : GRAY);

                    /* Separador */
                    DrawText("x", W / 2 - MeasureText("x", 80) / 2, H / 2 - 30, 80, DARKGRAY);

                    /* P2 */
                    DrawText("P2", W / 2 + 160 - MeasureText("P2", 24) / 2, H / 2 - 60, 24, LIGHTGRAY);
                    DrawText(str_p2, W / 2 + 160 - MeasureText(str_p2, 80) / 2, H / 2 - 30, 80, r.vencedor == 2 ? YELLOW : GRAY);
                }
            }
            /* ============================================================ */

            /* Renderização dos Botões */
            desenhar_botao(btn_principal, texto_btn, true);
            
            if (!r.torneio_final) {
                desenhar_botao(btn_menu, "Voltar ao Menu", false);
                DrawText("ENTER = Confirmar     ESC = Menu",
                    W / 2 - MeasureText("ENTER = Confirmar     ESC = Menu", 18) / 2,
                    H - 45, 18, DARKGRAY);
            } else {
                DrawText("ENTER = Voltar ao Menu",
                    W / 2 - MeasureText("ENTER = Voltar ao Menu", 18) / 2,
                    H - 45, 18, DARKGRAY);
            }
        EndDrawing();

        if (prox != cena_atual) return prox;
    }
    return CENA_SAIR;
}

/* ------------------------------------------------------------------ */
/*  tela_inserir_nomes                                                  */
/* ------------------------------------------------------------------ */
Cena tela_inserir_nomes(char *nome_p1, char *nome_p2) {
    const int W = GetScreenWidth();
    const int H = GetScreenHeight();

    char       *alvos[2]   = { nome_p1, nome_p2 };
    const char *legendas[2] = {
        "Jogador 1 — digite seu nome (max 4 letras):",
        "Jogador 2 — digite seu nome (max 4 letras):"
    };
    const char *padroes[2] = { "P1", "P2" };

    for (int fase = 0; fase < 2 && !WindowShouldClose(); fase++) {
        char *alvo = alvos[fase];
        alvo[0] = '\0';
        int  len       = 0;
        bool confirmado = false;

        while (!WindowShouldClose() && !confirmado) {

            /* --- INPUT --- */
            int c;
            while ((c = GetCharPressed()) > 0) {
                /* Aceita somente letras e dígitos (evita quebrar formato do .txt). */
                bool letra   = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
                bool digito  = (c >= '0' && c <= '9');
                if (len < 4 && (letra || digito)) {
                    alvo[len++] = (char)c;
                    alvo[len]   = '\0';
                }
            }
            if (IsKeyPressed(KEY_BACKSPACE) && len > 0) {
                alvo[--len] = '\0';
            }
            if (IsKeyPressed(KEY_ENTER)) {
                if (len == 0)
                    /* Nome em branco → usa padrão */
                    strncpy(alvo, padroes[fase], NOME_JOGADOR_TAM);
                confirmado = true;
            }

            /* --- DRAW --- */
            BeginDrawing();
                ClearBackground((Color){18, 22, 35, 255});

                /* Voltar tela */
                DrawText("ESC = Voltar",
                W / 2 - MeasureText("ESC = Voltar", 18) / 2,
                H - 40, 18, DARKGRAY);

                if (IsKeyPressed(KEY_ESCAPE)) {
                return CENA_SELECAO_MODO;
                }

                /* Cabeçalho */
                const char *tit = "TORNEIO — Insira os nomes";
                DrawText(tit,
                    W / 2 - MeasureText(tit, 36) / 2,
                    H / 2 - 160, 36, GOLD);

                /* Linha decorativa */
                DrawLine(W / 2 - 250, H / 2 - 110,
                        W / 2 + 250, H / 2 - 110,
                        (Color){80, 80, 80, 200});

                /* Legenda do jogador atual */
                int lg = 26;
                DrawText(legendas[fase],
                    W / 2 - MeasureText(legendas[fase], lg) / 2,
                    H / 2 - 80, lg, RAYWHITE);

                /* Caixa de entrada */
                Rectangle caixa = {
                    (float)(W / 2 - 130),
                    (float)(H / 2 - 10),
                    260.0f, 62.0f
                };
                DrawRectangleRec(caixa, (Color){30, 30, 55, 255});
                DrawRectangleLinesEx(caixa, 2,
                    fase == 0 ? SKYBLUE : ORANGE);

                /* Texto digitado */
                if (len > 0)
                    DrawText(alvo,
                        (int)(caixa.x + 16),
                        (int)(caixa.y + 16),
                        32,
                        fase == 0 ? SKYBLUE : ORANGE);

                /* Cursor piscante */
                if ((int)(GetTime() * 2) % 2 == 0 && len < 4) {
                    int cx = (int)(caixa.x + 16) +
                        (len > 0 ? MeasureText(alvo, 32) : 0);
                    Color cor_cur = fase == 0 ? SKYBLUE : ORANGE;
                    DrawRectangle(cx + 2, (int)(caixa.y + 10),
                        3, 42, cor_cur);
                }

                /* Contador de caracteres */
                DrawText(TextFormat("%d/4", len),
                    (int)(caixa.x + caixa.width - 36),
                    (int)(caixa.y + caixa.height - 22),
                    16, DARKGRAY);

                /* Instruções */
                DrawText("ENTER para confirmar",
                    W / 2 - MeasureText("ENTER para confirmar", 18) / 2,
                    H / 2 + 68, 18, LIGHTGRAY);
                DrawText("(deixe em branco para usar o nome padrão)",
                    W / 2 - MeasureText("(deixe em branco para usar o nome padrão)", 16) / 2,
                    H / 2 + 94, 16, DARKGRAY);

                /* Indicador de progresso (P1 ✓ ou a fazer) */
                const char *ind0 = (fase == 0) ? "► P1" : "✓ P1";
                const char *ind1 = (fase == 1) ? "► P2" : "  P2";
                Color c0 = (fase == 0) ? SKYBLUE  : GREEN;
                Color c1 = (fase == 1) ? ORANGE   : DARKGRAY;
                DrawText(ind0, W / 2 - 80, H / 2 + 130, 22, c0);
                DrawText(ind1, W / 2 + 20, H / 2 + 130, 22, c1);

            EndDrawing();
        }

        /* Garante que nome não ficou vazio caso a janela feche */
        if (!confirmado || alvo[0] == '\0')
            strncpy(alvo, padroes[fase], NOME_JOGADOR_TAM);
    }
}

/* ------------------------------------------------------------------ */
/*  tela_highscores                                                     */
/* ------------------------------------------------------------------ */
Cena tela_highscores(void) {
    const int W = GetScreenWidth();
    const int H = GetScreenHeight();

    Rectangle btn_voltar = { W / 2.0f - 180, H - 100, 360, 60 };
    Cena cena_atual = CENA_HIGHSCORES;

    // Carrega do arquivo texto e joga pra lista encadeada
    Score *lista = scores_carregar(SCORES_ARQUIVO);

    while (!WindowShouldClose()) {
        Cena prox = cena_atual;

        if (botao_clicado(btn_voltar) || IsKeyPressed(KEY_ESCAPE)) {
            prox = CENA_MENU;
        }

        BeginDrawing();
            ClearBackground((Color){18, 22, 35, 255});

            DrawText("HIGHSCORES - TORNEIO",
                W / 2 - MeasureText("HIGHSCORES - TORNEIO", 40) / 2,
                50, 40, GOLD);

            // Renderiza 2 colunas e 5 linhas
            int inicio_x_col1 = W / 2 - 400; // Coluna 1
            int inicio_x_col2 = W / 2 + 50;  // Coluna 2
            int inicio_y = 150;
            int espaco_y = 70;

            Score *atual = lista;
            for (int i = 0; i < 10; i++) {
                int col_x = (i % 2 == 0) ? inicio_x_col1 : inicio_x_col2;
                int row_y = inicio_y + (i / 2) * espaco_y;

                if (atual != NULL) {
                    // Tem pontuação pra mostrar, formata como "Pos: Nome - Pontos"
                    const char *texto = TextFormat("%d: %s - %02d", i + 1, atual->nome, atual->pontos);
                    DrawText(texto, col_x, row_y, 30, RAYWHITE);
                    atual = atual->proximo;
                } else {
                    // Posição vazia
                    const char *texto = TextFormat("%d: ---- - 00", i + 1);
                    DrawText(texto, col_x, row_y, 30, DARKGRAY);
                }
            }

            desenhar_botao(btn_voltar, "Voltar", false);
        EndDrawing();

        if (prox != cena_atual) {
            scores_liberar(lista);
            return prox;
        }
    }

    scores_liberar(lista);
    return CENA_SAIR;
}