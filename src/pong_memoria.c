/* ============================================================
 * pong_memoria.c — IMPLEMENTAÇÃO do Minigame 2 (Pong da Memória)
 * ------------------------------------------------------------
 * Dono: DEV 2 — Matheus Assis
 *
 * DEPENDÊNCIAS
 *   raylib.h  — janela, input, desenho, tempo
 *   pong_memoria.h — protótipo de jogar_pong_memoria
 *
 * CONTROLES
 *   Jogador 1 (esquerda): W / S
 *   Jogador 2 (direita) : SETA CIMA / SETA BAIXO
 *
 * FÍSICA
 *   - A bola rebate nas paredes superior e inferior.
 *   - Ao bater em uma raquete, a velocidade horizontal inverte
 *     e a vertical é recalculada pelo ponto de impacto
 *     (topo/base da raquete = ângulo máximo, centro = ângulo 0).
 *   - A cada ponto marcado a bola reinicia no centro, acelerando
 *     levemente (multiplicador de 1.05 por ponto, até um teto).
 *
 * POWERUP free()
 *   - Aparece em posição aleatória a cada POWERUP_INTERVALO s.
 *   - Se a bola colidir, a velocidade dobra por BOOST_DURACAO s.
 *   - Após o boost expirar, a velocidade volta ao valor anterior.
 * ============================================================ */

#include "raylib.h"
#include "pong_memoria.h"
#include <stdlib.h>   
#include <math.h>     

/* -----------------------------------
Constantes de layout e gameplay                                    
----------------------------------- */
#define LARGURA             1280
#define ALTURA              720

#define RAQUETE_W            16
#define RAQUETE_H           100
#define VEL_RAQUETE        450.0f   //px / s                          

#define BOLA_RAIO            10
#define VEL_BOLA_X_INICIAL 350.0f   // px / s                          
#define VEL_BOLA_Y_INICIAL 220.0f
#define VEL_MAX_X          800.0f   // teto de velocidade horizontal   
#define VEL_MAX_Y_ANGULO   500.0f   // vel_y máxima ao bater na ponta  
#define ACELERACAO_POR_PT  1.05f    // multiplicador a cada ponto      

#define PONTOS_PRA_VENCER    7

#define POWERUP_INTERVALO   8.0f    // segundos entre spawns          
#define BOOST_DURACAO        3.0f   // deuração do powerup             
#define BOOST_FATOR          2.0f   // multiplicador de velocidade do powerup    

/* Margem das raquetes em relação às bordas laterais/paredes */
#define MARGEM_RAQUETE      40

typedef struct {
    float x, y;          
    float vel_x, vel_y;  
    int   raio;
} Bola;

typedef struct {
    float  x, y;         // canto superior esquerdo da raquete
    int    tecla_cima;
    int    tecla_baixo;
} Raquete;

/* Garante que v fique em [lo, hi]. */
static float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* Retorna 1 se o círculo (cx,cy,r) sobrepõe o retângulo (rx,ry,rw,rh). */
static int circulo_colidiu_retangulo(float cx, float cy, float r,
                                    float rx, float ry, float rw, float rh) {
    float prox_x = clampf(cx, rx, rx + rw);
    float prox_y = clampf(cy, ry, ry + rh);
    float dx = cx - prox_x;
    float dy = cy - prox_y;
    return (dx * dx + dy * dy) <= (r * r);
}

/* Inicializa a bola no centro com direção aleatória (esq. ou dir.). */
static void resetar_bola(Bola *b, float vel_x_base, float vel_y_base) {
    b->x     = LARGURA / 2.0f;
    b->y     = ALTURA  / 2.0f;
    b->raio  = BOLA_RAIO;
    /* Direção horizontal aleatória */
    b->vel_x = (GetRandomValue(0, 1) == 0) ? vel_x_base : -vel_x_base;
    /* Direção vertical aleatória */
    b->vel_y = (GetRandomValue(0, 1) == 0) ? vel_y_base : -vel_y_base;
}

/* ------------------------------------------------------------------ */
/*  Função principal do minigame                                       */
/* ------------------------------------------------------------------ */
int jogar_pong_memoria(int *pontos_p1, int *pontos_p2) {

    /* -------- Inicialização da janela -------- */
    InitWindow(LARGURA, ALTURA, "Malloc Sports — Pong da Memória");
    SetTargetFPS(144);

    /* -------- Raquetes -------- */
    Raquete r1 = {
        .x           = MARGEM_RAQUETE,
        .y           = ALTURA / 2.0f - RAQUETE_H / 2.0f,
        .tecla_cima  = KEY_W,
        .tecla_baixo = KEY_S
    };
    Raquete r2 = {
        .x           = LARGURA - MARGEM_RAQUETE - RAQUETE_W,
        .y           = ALTURA / 2.0f - RAQUETE_H / 2.0f,
        .tecla_cima  = KEY_UP,
        .tecla_baixo = KEY_DOWN
    };

    /* -------- Bola -------- */
    Bola bola;
    /* Velocidades atuais (crescem a cada ponto) */
    float vel_x_atual = VEL_BOLA_X_INICIAL;
    float vel_y_atual = VEL_BOLA_Y_INICIAL;
    resetar_bola(&bola, vel_x_atual, vel_y_atual);

    /* -------- Placar -------- */
    int pts1 = 0, pts2 = 0;

    /* -------- Efeito de "leak" (flash na borda) -------- */
    float flash_p1  = 0.0f;   /* tempo restante de flash vermelho P1  */
    float flash_p2  = 0.0f;
    const float FLASH_DUR = 0.4f;

    /* -------- Powerup free() -------- */
    bool  powerup_ativo    = false;
    float powerup_x        = 0.0f;
    float powerup_y        = 0.0f;
    float timer_powerup    = POWERUP_INTERVALO;  /* contagem para o próximo spawn */
    float boost_restante   = 0.0f;
    bool  boost_ativo      = false;

    /* -------- Loop principal -------- */
    while (!WindowShouldClose() && pts1 < PONTOS_PRA_VENCER && pts2 < PONTOS_PRA_VENCER) {
        float dt = GetFrameTime();

        /* ===== INPUT — movimento das raquetes ===== */
        if (IsKeyDown(r1.tecla_cima))   r1.y -= VEL_RAQUETE * dt;
        if (IsKeyDown(r1.tecla_baixo))  r1.y += VEL_RAQUETE * dt;
        if (IsKeyDown(r2.tecla_cima))   r2.y -= VEL_RAQUETE * dt;
        if (IsKeyDown(r2.tecla_baixo))  r2.y += VEL_RAQUETE * dt;

        /* Limita raquetes dentro da tela */
        r1.y = clampf(r1.y, 0.0f, ALTURA - RAQUETE_H);
        r2.y = clampf(r2.y, 0.0f, ALTURA - RAQUETE_H);

        /* ===== UPDATE — bola ===== */
        bola.x += bola.vel_x * dt;
        bola.y += bola.vel_y * dt;

        /* Colisão com paredes superior e inferior */
        if (bola.y - bola.raio < 0) {
            bola.y     = (float)bola.raio;
            bola.vel_y = fabsf(bola.vel_y);   /* garante que vai para baixo */
        }
        if (bola.y + bola.raio > ALTURA) {
            bola.y     = ALTURA - (float)bola.raio;
            bola.vel_y = -fabsf(bola.vel_y);  /* garante que vai para cima  */
        }

        /* ---- Colisão com raquete 1 (esquerda) ---- */
        if (bola.vel_x < 0 &&
            circulo_colidiu_retangulo(bola.x, bola.y, (float)bola.raio,
                                    r1.x, r1.y, RAQUETE_W, RAQUETE_H)) {
            /* Reposiciona fora da raquete para evitar "tunneling" */
            bola.x    = r1.x + RAQUETE_W + (float)bola.raio;
            bola.vel_x = fabsf(bola.vel_x);

            /* Ângulo de saída proporcional ao ponto de impacto */
            float centro_r1 = r1.y + RAQUETE_H / 2.0f;
            float offset    = (bola.y - centro_r1) / (RAQUETE_H / 2.0f);
            bola.vel_y      = VEL_MAX_Y_ANGULO * offset;
        }

        /* ---- Colisão com raquete 2 (direita) ---- */
        if (bola.vel_x > 0 &&
            circulo_colidiu_retangulo(bola.x, bola.y, (float)bola.raio,
                                    r2.x, r2.y, RAQUETE_W, RAQUETE_H)) {
            bola.x    = r2.x - (float)bola.raio;
            bola.vel_x = -fabsf(bola.vel_x);

            float centro_r2 = r2.y + RAQUETE_H / 2.0f;
            float offset    = (bola.y - centro_r2) / (RAQUETE_H / 2.0f);
            bola.vel_y      = VEL_MAX_Y_ANGULO * offset;
        }

        /* ---- Limita velocidade máxima ---- */
        if (fabsf(bola.vel_x) > VEL_MAX_X)
            bola.vel_x = (bola.vel_x > 0) ? VEL_MAX_X : -VEL_MAX_X;

        /* ---- Ponto para P2 (bola saiu pela esquerda) ---- */
        if (bola.x + bola.raio < 0) {
            pts2++;
            flash_p1 = FLASH_DUR;

            /* Acelera levemente a cada ponto */
            vel_x_atual = clampf(vel_x_atual * ACELERACAO_POR_PT,
                                 VEL_BOLA_X_INICIAL, VEL_MAX_X * 0.8f);
            vel_y_atual = clampf(vel_y_atual * ACELERACAO_POR_PT,
                                 VEL_BOLA_Y_INICIAL, VEL_MAX_Y_ANGULO * 0.6f);

            /* Cancela boost se estava ativo */
            boost_ativo    = false;
            boost_restante = 0.0f;
            powerup_ativo  = false;
            timer_powerup  = POWERUP_INTERVALO;

            resetar_bola(&bola, vel_x_atual, vel_y_atual);
        }

        /* ---- Ponto para P1 (bola saiu pela direita) ---- */
        if (bola.x - bola.raio > LARGURA) {
            pts1++;
            flash_p2 = FLASH_DUR;

            vel_x_atual = clampf(vel_x_atual * ACELERACAO_POR_PT,
                                 VEL_BOLA_X_INICIAL, VEL_MAX_X * 0.8f);
            vel_y_atual = clampf(vel_y_atual * ACELERACAO_POR_PT,
                                 VEL_BOLA_Y_INICIAL, VEL_MAX_Y_ANGULO * 0.6f);

            boost_ativo    = false;
            boost_restante = 0.0f;
            powerup_ativo  = false;
            timer_powerup  = POWERUP_INTERVALO;

            resetar_bola(&bola, vel_x_atual, vel_y_atual);
        }

        /* ===== POWERUP free() ===== */

        /* Contagem regressiva para próximo spawn */
        timer_powerup -= dt;
        if (!powerup_ativo && timer_powerup <= 0.0f) {
            /* Spawna em posição aleatória, longe das bordas */
            powerup_x    = (float)GetRandomValue(150, LARGURA - 150);
            powerup_y    = (float)GetRandomValue(60,  ALTURA  - 60);
            powerup_ativo = true;
            timer_powerup = POWERUP_INTERVALO;
        }

        /* Colisão bola × powerup */
        if (powerup_ativo) {
            float dx = bola.x - powerup_x;
            float dy = bola.y - powerup_y;
            if (dx * dx + dy * dy <= (float)(bola.raio + 14) * (bola.raio + 14)) {
                /* Aplica boost apenas se não estava ativo */
                if (!boost_ativo) {
                    bola.vel_x   *= BOOST_FATOR;
                    bola.vel_y   *= BOOST_FATOR;
                    boost_ativo   = true;
                }
                boost_restante = BOOST_DURACAO;
                powerup_ativo  = false;
                timer_powerup  = POWERUP_INTERVALO;
            }
        }

        /* Contagem regressiva do boost */
        if (boost_ativo) {
            boost_restante -= dt;
            if (boost_restante <= 0.0f) {
                bola.vel_x  /= BOOST_FATOR;
                bola.vel_y  /= BOOST_FATOR;
                boost_ativo  = false;
            }
        }

        /* Atualiza temporizadores de flash */
        if (flash_p1 > 0.0f) flash_p1 -= dt;
        if (flash_p2 > 0.0f) flash_p2 -= dt;

        /* ===== DRAW ===== */
        BeginDrawing();
            ClearBackground(BLACK);

            /* --- Flash de "memory leak" nas bordas laterais --- */
            if (flash_p1 > 0.0f) {
                unsigned char alpha = (unsigned char)(255 * (flash_p1 / FLASH_DUR));
                DrawRectangle(0, 0, 12, ALTURA, (Color){220, 50, 50, alpha});
            }
            if (flash_p2 > 0.0f) {
                unsigned char alpha = (unsigned char)(255 * (flash_p2 / FLASH_DUR));
                DrawRectangle(LARGURA - 12, 0, 12, ALTURA, (Color){220, 50, 50, alpha});
            }

            /* --- Linha central tracejada --- */
            for (int i = 0; i < ALTURA; i += 30)
                DrawRectangle(LARGURA / 2 - 2, i, 4, 15, (Color){80, 80, 80, 200});

            /* --- Raquetes --- */
            DrawRectangle((int)r1.x, (int)r1.y, RAQUETE_W, RAQUETE_H, WHITE);
            DrawRectangle((int)r2.x, (int)r2.y, RAQUETE_W, RAQUETE_H, WHITE);

            /* --- Bola (cor muda quando em boost) --- */
            Color cor_bola = boost_ativo ? YELLOW : RAYWHITE;
            DrawCircle((int)bola.x, (int)bola.y, (float)bola.raio, cor_bola);

            /* --- Powerup free() --- */
            if (powerup_ativo) {
                DrawCircle((int)powerup_x, (int)powerup_y, 14, (Color){0, 200, 80, 200});
                DrawText("free()", (int)powerup_x - 22, (int)powerup_y - 8, 14, BLACK);
            }

            /* --- HUD — placar --- */
            DrawText(TextFormat("%d", pts1),
                    LARGURA / 2 - 80, 20, 60, WHITE);
            DrawText(TextFormat("%d", pts2),
                    LARGURA / 2 + 40, 20, 60, WHITE);

            /* --- Labels dos jogadores --- */
            DrawText("P1  [W/S]",
                    (int)r1.x, ALTURA - 30, 18, GRAY);
            DrawText("P2  [↑/↓]",
                    (int)r2.x - 30, ALTURA - 30, 18, GRAY);

            /* --- Indicador de boost ativo --- */
            if (boost_ativo) {
                int seg = (int)boost_restante + 1;
                DrawText(TextFormat("BOOST! %ds", seg),
                        LARGURA / 2 - 50, ALTURA - 40, 22, YELLOW);
            }

        EndDrawing();
    }   /* fim do loop principal */

    /* -------- Tela de fim de partida -------- */
    int vencedor = (pts1 > pts2) ? 1 : 2;
    float timer_fim = 3.0f;  /* exibe o resultado por 3 segundos */

    while (!WindowShouldClose() && timer_fim > 0.0f) {
        timer_fim -= GetFrameTime();
        BeginDrawing();
            ClearBackground(BLACK);
            const char *msg = (vencedor == 1) ? "Jogador 1 venceu!" : "Jogador 2 venceu!";
            DrawText(msg,
                    LARGURA / 2 - MeasureText(msg, 50) / 2,
                    ALTURA  / 2 - 60, 50, YELLOW);
            DrawText(TextFormat("Placar final:  %d  x  %d", pts1, pts2),
                    LARGURA / 2 - MeasureText("Placar final:  0  x  0", 30) / 2,
                    ALTURA  / 2 + 10, 30, WHITE);
        EndDrawing();
    }

    CloseWindow();

    /* Preenche os ponteiros de saída */
    *pontos_p1 = pts1;
    *pontos_p2 = pts2;
    return vencedor;
}