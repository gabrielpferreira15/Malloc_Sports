#include "raylib.h"
#include <math.h>
#include <stdbool.h>

// ======================================================
// CONFIG
// ======================================================

#define LARGURA 1280
#define ALTURA 720

#define CHAO_Y 620

#define GRAVIDADE 1200.0f

#define VELOCIDADE_JOGADOR 420.0f
#define FORCA_PULO 650.0f

#define JOGADOR_W 60
#define JOGADOR_H 120

#define CABECA_RAIO 35

#define BOLA_RAIO 18

#define PONTOS_PARA_VENCER 7

// ======================================================
// STRUCTS
// ======================================================

typedef struct {

    float x, y;

    float vel_x;
    float vel_y;

    float largura;
    float altura;

    bool no_chao;

    Color cor;

} Jogador;

typedef struct {

    float x, y;

    float vel_x;
    float vel_y;

    float raio;

} Bola;

// ======================================================
// FUNÇÕES
// ======================================================

float clampf(float valor, float min, float max)
{
    if (valor < min) return min;
    if (valor > max) return max;

    return valor;
}

// ------------------------------------------------------

void resetar_bola(Bola *bola)
{
    bola->x = LARGURA / 2.0f;
    bola->y = 200;

    bola->vel_x =
        (GetRandomValue(0, 1) == 0)
        ? 300
        : -300;

    bola->vel_y = -200;

    bola->raio = BOLA_RAIO;
}

// ------------------------------------------------------

void aplicar_gravidade_bola(Bola *bola, float dt)
{
    bola->vel_y += GRAVIDADE * dt;

    bola->x += bola->vel_x * dt;
    bola->y += bola->vel_y * dt;
}

// ------------------------------------------------------

void aplicar_gravidade_jogador(Jogador *j, float dt)
{
    j->vel_y += GRAVIDADE * dt;

    j->y += j->vel_y * dt;

    if (j->y + j->altura >= CHAO_Y)
    {
        j->y = CHAO_Y - j->altura;

        j->vel_y = 0;

        j->no_chao = true;
    }
}

// ------------------------------------------------------

void mover_jogador(
    Jogador *j,
    int tecla_esquerda,
    int tecla_direita,
    int tecla_pulo,
    float dt
)
{
    j->vel_x = 0;

    if (IsKeyDown(tecla_esquerda))
        j->vel_x = -VELOCIDADE_JOGADOR;

    if (IsKeyDown(tecla_direita))
        j->vel_x = VELOCIDADE_JOGADOR;

    j->x += j->vel_x * dt;

    if (IsKeyPressed(tecla_pulo) && j->no_chao)
    {
        j->vel_y = -FORCA_PULO;

        j->no_chao = false;
    }
}

// ------------------------------------------------------

void limitar_jogador(Jogador *j, float min_x, float max_x)
{
    j->x = clampf(
        j->x,
        min_x,
        max_x - j->largura
    );
}

// ------------------------------------------------------

void colisao_bola_cabeca(
    Bola *bola,
    Jogador *j
)
{
    float cabeca_x =
        j->x + j->largura / 2.0f;

    float cabeca_y =
        j->y + CABECA_RAIO;

    float dx = bola->x - cabeca_x;
    float dy = bola->y - cabeca_y;

    float distancia =
        sqrtf(dx * dx + dy * dy);

    if (distancia < bola->raio + CABECA_RAIO)
    {
        float normal_x = dx / distancia;
        float normal_y = dy / distancia;

        float forca = 700.0f;

        bola->vel_x = normal_x * forca;

        bola->vel_y = normal_y * forca;

        // IMPULSO EXTRA PRA CIMA
        bola->vel_y -= 300;
    }
}

// ------------------------------------------------------

void colisao_bola_parede(Bola *bola)
{
    // Parede esquerda
    if (bola->x - bola->raio <= 0)
    {
        bola->x = bola->raio;

        bola->vel_x *= -1;
    }

    // Parede direita
    if (bola->x + bola->raio >= LARGURA)
    {
        bola->x = LARGURA - bola->raio;

        bola->vel_x *= -1;
    }

    // Teto
    if (bola->y - bola->raio <= 0)
    {
        bola->y = bola->raio;

        bola->vel_y *= -1;
    }
}

// ------------------------------------------------------

void colisao_rede(Bola *bola)
{
    Rectangle rede = {

        LARGURA / 2.0f - 5,
        CHAO_Y - 220,

        10,
        220
    };

    float ponto_x =
        clampf(
            bola->x,
            rede.x,
            rede.x + rede.width
        );

    float ponto_y =
        clampf(
            bola->y,
            rede.y,
            rede.y + rede.height
        );

    float dx = bola->x - ponto_x;
    float dy = bola->y - ponto_y;

    if ((dx * dx + dy * dy) <= bola->raio * bola->raio)
    {
        if (fabsf(dx) > fabsf(dy))
        {
            bola->vel_x *= -1;

            if (dx > 0)
                bola->x =
                    rede.x + rede.width + bola->raio;
            else
                bola->x =
                    rede.x - bola->raio;
        }
        else
        {
            bola->vel_y *= -1;

            if (dy > 0)
                bola->y =
                    rede.y + rede.height + bola->raio;
            else
                bola->y =
                    rede.y - bola->raio;
        }
    }
}

// ======================================================
// MAIN
// ======================================================

int main(void)
{
    InitWindow(
        LARGURA,
        ALTURA,
        "Head Volleyball"
    );

    SetTargetFPS(60);

    // ==================================================
    // PLAYERS
    // ==================================================

    Jogador p1 = {

        200,
        CHAO_Y - JOGADOR_H,

        0,
        0,

        JOGADOR_W,
        JOGADOR_H,

        true,

        BLUE
    };

    Jogador p2 = {

        1000,
        CHAO_Y - JOGADOR_H,

        0,
        0,

        JOGADOR_W,
        JOGADOR_H,

        true,

        RED
    };

    // ==================================================
    // BALL
    // ==================================================

    Bola bola;

    resetar_bola(&bola);

    // ==================================================
    // SCORE
    // ==================================================

    int pontos_p1 = 0;
    int pontos_p2 = 0;

    // ==================================================
    // LOOP
    // ==================================================

    while (
        !WindowShouldClose() &&
        pontos_p1 < PONTOS_PARA_VENCER &&
        pontos_p2 < PONTOS_PARA_VENCER
    )
    {
        float dt = GetFrameTime();

        // ==============================================
        // MOVIMENTO
        // ==============================================

        mover_jogador(
            &p1,
            KEY_A,
            KEY_D,
            KEY_W,
            dt
        );

        mover_jogador(
            &p2,
            KEY_LEFT,
            KEY_RIGHT,
            KEY_UP,
            dt
        );

        aplicar_gravidade_jogador(&p1, dt);
        aplicar_gravidade_jogador(&p2, dt);

        limitar_jogador(
            &p1,
            0,
            LARGURA / 2.0f - 20
        );

        limitar_jogador(
            &p2,
            LARGURA / 2.0f + 20,
            LARGURA
        );

        // ==============================================
        // BOLA
        // ==============================================

        aplicar_gravidade_bola(&bola, dt);

        colisao_bola_parede(&bola);

        colisao_rede(&bola);

        colisao_bola_cabeca(&bola, &p1);

        colisao_bola_cabeca(&bola, &p2);

        // ==============================================
        // PONTO
        // ==============================================

        if (bola.y + bola.raio >= CHAO_Y)
        {
            if (bola.x < LARGURA / 2.0f)
                pontos_p2++;
            else
                pontos_p1++;

            resetar_bola(&bola);
        }

        // ==============================================
        // DRAW
        // ==============================================

        BeginDrawing();

        ClearBackground((Color){135, 206, 235, 255});

        // Sol
        DrawCircle(
            1100,
            100,
            50,
            YELLOW
        );

        // Chão areia
        DrawRectangle(
            0,
            CHAO_Y,
            LARGURA,
            ALTURA - CHAO_Y,
            (Color){230, 200, 120, 255}
        );

        // Rede
        DrawRectangle(
            LARGURA / 2 - 5,
            CHAO_Y - 220,
            10,
            220,
            WHITE
        );

        // Jogador 1 corpo
        DrawRectangle(
            p1.x,
            p1.y + 30,
            p1.largura,
            p1.altura - 30,
            p1.cor
        );

        // Jogador 1 cabeça
        DrawCircle(
            p1.x + p1.largura / 2,
            p1.y + CABECA_RAIO,
            CABECA_RAIO,
            SKYBLUE
        );

        // Jogador 2 corpo
        DrawRectangle(
            p2.x,
            p2.y + 30,
            p2.largura,
            p2.altura - 30,
            p2.cor
        );

        // Jogador 2 cabeça
        DrawCircle(
            p2.x + p2.largura / 2,
            p2.y + CABECA_RAIO,
            CABECA_RAIO,
            PINK
        );

        // Bola
        DrawCircle(
            bola.x,
            bola.y,
            bola.raio,
            WHITE
        );

        // Placar
        DrawText(
            TextFormat("%d", pontos_p1),
            500,
            40,
            60,
            BLACK
        );

        DrawText(
            TextFormat("%d", pontos_p2),
            730,
            40,
            60,
            BLACK
        );

        // Controles
        DrawText(
            "P1: A D W",
            40,
            40,
            24,
            BLACK
        );

        DrawText(
            "P2: <- -> ^",
            980,
            40,
            24,
            BLACK
        );

        EndDrawing();
    }

    // ==================================================
    // TELA FINAL
    // ==================================================

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        if (pontos_p1 > pontos_p2)
        {
            DrawText(
                "PLAYER 1 VENCEU!",
                380,
                300,
                50,
                BLUE
            );
        }
        else
        {
            DrawText(
                "PLAYER 2 VENCEU!",
                380,
                300,
                50,
                RED
            );
        }

        DrawText(
            "ESC para sair",
            500,
            400,
            30,
            WHITE
        );

        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE))
            break;
    }

    CloseWindow();

    return 0;
}