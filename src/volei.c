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
// FUNÇÕES AUXILIARES
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

        bola->vel_y -= 300;
    }
}

// ------------------------------------------------------

void colisao_bola_parede(Bola *bola)
{
    if (bola->x - bola->raio <= 0)
    {
        bola->x = bola->raio;

        bola->vel_x *= -1;
    }

    if (bola->x + bola->raio >= LARGURA)
    {
        bola->x = LARGURA - bola->raio;

        bola->vel_x *= -1;
    }

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
        }
        else
        {
            bola->vel_y *= -1;
        }
    }
}

// ======================================================
// FUNÇÃO PRINCIPAL DO JOGO
// ======================================================

int jogar_volei(int *pontos_p1, int *pontos_p2)
{
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

    Bola bola;

    resetar_bola(&bola);

    int pts1 = 0;
    int pts2 = 0;

    while (
        !WindowShouldClose() &&
        pts1 < PONTOS_PARA_VENCER &&
        pts2 < PONTOS_PARA_VENCER
    )
    {
        float dt = GetFrameTime();

        // PLAYER 1
        mover_jogador(
            &p1,
            KEY_A,
            KEY_D,
            KEY_W,
            dt
        );

        // PLAYER 2
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

        // BOLA
        aplicar_gravidade_bola(&bola, dt);

        colisao_bola_parede(&bola);

        colisao_rede(&bola);

        colisao_bola_cabeca(&bola, &p1);

        colisao_bola_cabeca(&bola, &p2);

        // PONTO
        if (bola.y + bola.raio >= CHAO_Y)
        {
            if (bola.x < LARGURA / 2.0f)
                pts2++;
            else
                pts1++;

            resetar_bola(&bola);
        }

        // DRAW
        BeginDrawing();

        ClearBackground((Color){135, 206, 235, 255});

        // Sol
        DrawCircle(
            1100,
            100,
            50,
            YELLOW
        );

        // Areia
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

        // PLAYER 1
        DrawRectangle(
            p1.x,
            p1.y + 30,
            p1.largura,
            p1.altura - 30,
            p1.cor
        );

        DrawCircle(
            p1.x + p1.largura / 2,
            p1.y + CABECA_RAIO,
            CABECA_RAIO,
            SKYBLUE
        );

        // PLAYER 2
        DrawRectangle(
            p2.x,
            p2.y + 30,
            p2.largura,
            p2.altura - 30,
            p2.cor
        );

        DrawCircle(
            p2.x + p2.largura / 2,
            p2.y + CABECA_RAIO,
            CABECA_RAIO,
            PINK
        );

        // BOLA
        DrawCircle(
            bola.x,
            bola.y,
            bola.raio,
            WHITE
        );

        // PLACAR
        DrawText(
            TextFormat("%d", pts1),
            500,
            40,
            60,
            BLACK
        );

        DrawText(
            TextFormat("%d", pts2),
            730,
            40,
            60,
            BLACK
        );

        EndDrawing();
    }

    *pontos_p1 = pts1;
    *pontos_p2 = pts2;

    return
        (pts1 >= PONTOS_PARA_VENCER)
        ? 1
        : 2;
}