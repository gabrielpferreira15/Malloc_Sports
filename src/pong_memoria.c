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
#include <math.h>

#define LARGURA             1280
#define ALTURA               720
#define RAQUETE_W             16
#define RAQUETE_H            100
#define VEL_RAQUETE         450.0f
#define BOLA_RAIO             10
#define VEL_BOLA_X_INICIAL  350.0f
#define VEL_BOLA_Y_INICIAL  220.0f
#define VEL_MAX_X           800.0f
#define VEL_MAX_Y_ANGULO    500.0f
#define ACELERACAO_POR_PT     1.05f
#define PONTOS_PRA_VENCER     7
#define POWERUP_INTERVALO     8.0f
#define BOOST_DURACAO         3.0f
#define BOOST_FATOR           2.0f
#define MARGEM_RAQUETE        40

typedef struct {
    float x, y;
    float vel_x, vel_y;
    int   raio;
} Bola;

typedef struct {
    float x, y;
    int   tecla_cima;
    int   tecla_baixo;
} Raquete;

static float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static int circulo_colidiu_retangulo(float cx, float cy, float r,
        float rx, float ry, float rw, float rh) {
    float px = clampf(cx, rx, rx + rw);
    float py = clampf(cy, ry, ry + rh);
    float dx = cx - px, dy = cy - py;
    return (dx * dx + dy * dy) <= (r * r);
}

static void resetar_bola(Bola *b, float vx, float vy) {
    b->x     = LARGURA / 2.0f;
    b->y     = ALTURA  / 2.0f;
    b->raio  = BOLA_RAIO;
    b->vel_x = (GetRandomValue(0, 1) == 0) ?  vx : -vx;
    b->vel_y = (GetRandomValue(0, 1) == 0) ?  vy : -vy;
}

int jogar_pong_memoria(int *pontos_p1, int *pontos_p2) {

    Raquete r1 = {
        .x           = MARGEM_RAQUETE,
        .y           = ALTURA / 2.0f - RAQUETE_H / 2.0f,
        .tecla_cima  = KEY_W,
        .tecla_baixo = KEY_S
    };
    Raquete r2 = {
        .x           = LARGURA - MARGEM_RAQUETE - RAQUETE_W,
        .y           = ALTURA  / 2.0f - RAQUETE_H / 2.0f,
        .tecla_cima  = KEY_UP,
        .tecla_baixo = KEY_DOWN
    };

    Bola  bola;
    float vel_x_atual = VEL_BOLA_X_INICIAL;
    float vel_y_atual = VEL_BOLA_Y_INICIAL;
    resetar_bola(&bola, vel_x_atual, vel_y_atual);

    int pts1 = 0, pts2 = 0;

    float flash_p1 = 0.0f, flash_p2 = 0.0f;
    const float FLASH_DUR = 0.4f;

    bool  powerup_ativo  = false;
    float powerup_x      = 0.0f, powerup_y = 0.0f;
    float timer_powerup  = POWERUP_INTERVALO;
    float boost_restante = 0.0f;
    bool  boost_ativo    = false;

    while (!WindowShouldClose() &&
        pts1 < PONTOS_PRA_VENCER &&
        pts2 < PONTOS_PRA_VENCER) {

        float dt = GetFrameTime();

        // Input
        if (IsKeyDown(r1.tecla_cima))   r1.y -= VEL_RAQUETE * dt;
        if (IsKeyDown(r1.tecla_baixo))  r1.y += VEL_RAQUETE * dt;
        if (IsKeyDown(r2.tecla_cima))   r2.y -= VEL_RAQUETE * dt;
        if (IsKeyDown(r2.tecla_baixo))  r2.y += VEL_RAQUETE * dt;
        r1.y = clampf(r1.y, 0.0f, ALTURA - RAQUETE_H);
        r2.y = clampf(r2.y, 0.0f, ALTURA - RAQUETE_H);

        // Update da bola
        bola.x += bola.vel_x * dt;
        bola.y += bola.vel_y * dt;

        if (bola.y - bola.raio < 0) {
            bola.y     = (float)bola.raio;
            bola.vel_y = fabsf(bola.vel_y);
        }
        if (bola.y + bola.raio > ALTURA) {
            bola.y     = ALTURA - (float)bola.raio;
            bola.vel_y = -fabsf(bola.vel_y);
        }

        if (bola.vel_x < 0 &&
            circulo_colidiu_retangulo(bola.x, bola.y, (float)bola.raio,
                                r1.x, r1.y, RAQUETE_W, RAQUETE_H)) {
            bola.x     = r1.x + RAQUETE_W + (float)bola.raio;
            bola.vel_x = fabsf(bola.vel_x);
            float offset = (bola.y - (r1.y + RAQUETE_H / 2.0f)) / (RAQUETE_H / 2.0f);
            bola.vel_y   = VEL_MAX_Y_ANGULO * offset;
        }

        if (bola.vel_x > 0 &&
            circulo_colidiu_retangulo(bola.x, bola.y, (float)bola.raio,
                r2.x, r2.y, RAQUETE_W, RAQUETE_H)) {
            bola.x     = r2.x - (float)bola.raio;
            bola.vel_x = -fabsf(bola.vel_x);
            float offset = (bola.y - (r2.y + RAQUETE_H / 2.0f)) / (RAQUETE_H / 2.0f);
            bola.vel_y   = VEL_MAX_Y_ANGULO * offset;
        }

        if (fabsf(bola.vel_x) > VEL_MAX_X)
            bola.vel_x = (bola.vel_x > 0) ? VEL_MAX_X : -VEL_MAX_X;

        // Ponto para P2
        if (bola.x + bola.raio < 0) {
            pts2++;
            flash_p1 = FLASH_DUR;
            vel_x_atual = clampf(vel_x_atual * ACELERACAO_POR_PT, VEL_BOLA_X_INICIAL, VEL_MAX_X * 0.8f);
            vel_y_atual = clampf(vel_y_atual * ACELERACAO_POR_PT, VEL_BOLA_Y_INICIAL, VEL_MAX_Y_ANGULO * 0.6f);
            boost_ativo = false; boost_restante = 0.0f;
            powerup_ativo = false; timer_powerup = POWERUP_INTERVALO;
            resetar_bola(&bola, vel_x_atual, vel_y_atual);
        }

        // Ponto para P1
        if (bola.x - bola.raio > LARGURA) {
            pts1++;
            flash_p2 = FLASH_DUR;
            vel_x_atual = clampf(vel_x_atual * ACELERACAO_POR_PT, VEL_BOLA_X_INICIAL, VEL_MAX_X * 0.8f);
            vel_y_atual = clampf(vel_y_atual * ACELERACAO_POR_PT, VEL_BOLA_Y_INICIAL, VEL_MAX_Y_ANGULO * 0.6f);
            boost_ativo = false; boost_restante = 0.0f;
            powerup_ativo = false; timer_powerup = POWERUP_INTERVALO;
            resetar_bola(&bola, vel_x_atual, vel_y_atual);
        }

        // Powerup
        timer_powerup -= dt;
        if (!powerup_ativo && timer_powerup <= 0.0f) {
            powerup_x     = (float)GetRandomValue(150, LARGURA - 150);
            powerup_y     = (float)GetRandomValue(60,  ALTURA  - 60);
            powerup_ativo = true;
            timer_powerup = POWERUP_INTERVALO;
        }

        if (powerup_ativo) {
            float dx = bola.x - powerup_x, dy = bola.y - powerup_y;
            float rs = (float)(bola.raio + 14);
            if (dx * dx + dy * dy <= rs * rs) {
                if (!boost_ativo) { bola.vel_x *= BOOST_FATOR; bola.vel_y *= BOOST_FATOR; boost_ativo = true; }
                boost_restante = BOOST_DURACAO;
                powerup_ativo  = false;
                timer_powerup  = POWERUP_INTERVALO;
            }
        }

        if (boost_ativo) {
            boost_restante -= dt;
            if (boost_restante <= 0.0f) {
                bola.vel_x /= BOOST_FATOR;
                bola.vel_y /= BOOST_FATOR;
                boost_ativo = false;
            }
        }

        if (flash_p1 > 0.0f) flash_p1 -= dt;
        if (flash_p2 > 0.0f) flash_p2 -= dt;

        // DRAW
        BeginDrawing();
            ClearBackground(BLACK);

            if (flash_p1 > 0.0f) {
                unsigned char a = (unsigned char)(255 * (flash_p1 / FLASH_DUR));
                DrawRectangle(0, 0, 12, ALTURA, (Color){220, 50, 50, a});
            }
            if (flash_p2 > 0.0f) {
                unsigned char a = (unsigned char)(255 * (flash_p2 / FLASH_DUR));
                DrawRectangle(LARGURA - 12, 0, 12, ALTURA, (Color){220, 50, 50, a});
            }

            for (int i = 0; i < ALTURA; i += 30)
                DrawRectangle(LARGURA / 2 - 2, i, 4, 15, (Color){80, 80, 80, 200});

            DrawRectangle((int)r1.x, (int)r1.y, RAQUETE_W, RAQUETE_H, WHITE);
            DrawRectangle((int)r2.x, (int)r2.y, RAQUETE_W, RAQUETE_H, WHITE);

            Color cor_bola = boost_ativo ? YELLOW : RAYWHITE;
            DrawCircle((int)bola.x, (int)bola.y, (float)bola.raio, cor_bola);

            if (powerup_ativo) {
                DrawCircle((int)powerup_x, (int)powerup_y, 14, (Color){0, 200, 80, 200});
                DrawText("free()", (int)powerup_x - 22, (int)powerup_y - 8, 14, BLACK);
            }

            DrawText(TextFormat("%d", pts1), LARGURA / 2 - 80, 20, 60, WHITE);
            DrawText(TextFormat("%d", pts2), LARGURA / 2 + 40, 20, 60, WHITE);

            DrawText("P1 [W/S]",  (int)r1.x,      ALTURA - 30, 18, GRAY);
            DrawText("P2 [U/D]",  (int)r2.x - 30, ALTURA - 30, 18, GRAY);

            if (boost_ativo)
                DrawText(TextFormat("BOOST! %ds", (int)boost_restante + 1),
                LARGURA / 2 - 55, ALTURA - 40, 22, YELLOW);

        EndDrawing();
    }

    *pontos_p1 = pts1;
    *pontos_p2 = pts2;
    return (pts1 >= PONTOS_PRA_VENCER) ? 1 : 2;
}