#include "raylib.h"
#include "corrida_ponteiros.h"
#include <stdbool.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/*  Constantes de layout                                               */
/* ------------------------------------------------------------------ */
#define LARGURA           1280
#define ALTURA             720

/* Comprimento lógico da pista em pixels (maior que a tela) */
#define PISTA_COMPRIMENTO  2400.0f

/* Altura de cada pista na tela */
#define PISTA_ALTURA        220
/* Posição Y do centro da pista do P1 e P2 */
#define PISTA_Y_P1          200
#define PISTA_Y_P2          460

/* Área reservada para o HUD lateral esquerdo */
#define HUD_LARGURA         60

/* Corredor: tamanho visual */
#define CORREDOR_W           28
#define CORREDOR_H           40

/* Obstáculo: tamanho visual */
#define OBST_W               30
#define OBST_H               40

/* Linha de chegada: margem do fim da pista */
#define META_X             (PISTA_COMPRIMENTO - 60.0f)

/* ------------------------------------------------------------------ */
/*  Constantes de jogabilidade                                         */
/* ------------------------------------------------------------------ */
#define VEL_BASE           1.0f   /* px/s sem nenhum combo           */
#define FATOR_COMBO          8.0f   /* px/s adicionais por combo       */
#define PENALIDADE_COLISAO  80.0f   /* queda de velocidade ao colidir  */

#define GRAVIDADE          1800.0f  /* px/s² — aceleração de queda     */
#define FORCA_PULO        -620.0f   /* vel_y inicial do pulo (negativa = sobe) */
#define CHAO_Y               0.0f   /* y=0 significa "no chão" (relativo à pista) */

/* ------------------------------------------------------------------ */
/*  Structs                                                            */
/* ------------------------------------------------------------------ */

/* Posição lógica na pista (x = progresso, y = altura do pulo) */
typedef struct {
    float  x;           /* progresso na pista, 0 → PISTA_COMPRIMENTO  */
    float  y_pulo;      /* deslocamento vertical do pulo (0 = chão)    */
    float  vy;          /* velocidade vertical atual                   */
    float  vel;         /* velocidade horizontal atual (px/s)          */
    int    combo;       /* acertos consecutivos de alternância         */
    bool   no_chao;     /* true quando não está pulando                */
    int    tecla_esq;   /* tecla de movimento esquerda                 */
    int    tecla_dir;   /* tecla de movimento direita                  */
    int    tecla_pulo;  /* tecla de pulo                               */
    bool   esperando_dir; /* true = próxima tecla esperada é DIR       */
    bool   terminou;    /* cruzou a meta?                              */
} Corredor;

typedef struct {
    float x;            /* posição lógica na pista                     */
} Obstaculo;

/* ------------------------------------------------------------------ */
/*  Configurações de obstáculos                                        */
/* ------------------------------------------------------------------ */
#define MAX_OBSTACULOS  4

typedef struct {
    int   quantidade;
    float posicoes[MAX_OBSTACULOS]; /* frações da pista: 0.0 a 1.0     */
} ConfigObstaculos;

static const ConfigObstaculos CONFIGS[3] = {
    { 2, { 0.333f, 0.666f, 0.0f, 0.0f } },   /* Config 0 */
    { 3, { 0.25f,  0.50f,  0.75f, 0.0f } },  /* Config 1 */
    { 2, { 0.25f,  0.75f,  0.0f, 0.0f } },   /* Config 2 */
};

/* ------------------------------------------------------------------ */
/*  Funções auxiliares                                                 */
/* ------------------------------------------------------------------ */

static float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* Colisão AABB entre corredor e obstáculo (coordenadas de tela). */
static bool colide_aabb(float ax, float ay, float aw, float ah,
                        float bx, float by, float bw, float bh) {
    return ax < bx + bw && ax + aw > bx &&
        ay < by + bh && ay + ah > by;
}

/* Converte progresso lógico na pista para posição X na tela.
 * A pista é comprimida horizontalmente no espaço disponível. */
static float pista_para_tela_x(float pista_x) {
    float area = (float)(LARGURA - HUD_LARGURA);
    return HUD_LARGURA + (pista_x / PISTA_COMPRIMENTO) * area;
}

/* Inicializa um corredor. */
static void init_corredor(Corredor *c, int tecla_esq, int tecla_dir, int tecla_pulo) {
    c->x             = 0.0f;
    c->y_pulo        = CHAO_Y;
    c->vy            = 0.0f;
    c->vel           = VEL_BASE;
    c->combo         = 0;
    c->no_chao       = true;
    c->tecla_esq     = tecla_esq;
    c->tecla_dir     = tecla_dir;
    c->tecla_pulo    = tecla_pulo;
    c->esperando_dir = true;   /* começa esperando DIR para alternar   */
    c->terminou      = false;
}

/* Recalcula a velocidade com base no combo atual. */
static void atualizar_velocidade(Corredor *c) {
    c->vel = clampf(VEL_BASE + c->combo * FATOR_COMBO,
                    VEL_BASE, VEL_BASE + 40 * FATOR_COMBO);
}

/* ------------------------------------------------------------------ */
/*  Funções de desenho                                                 */
/* ------------------------------------------------------------------ */

static void desenhar_pista(int pista_y, Color cor_pista) {
    /* Fundo da pista */
    DrawRectangle(HUD_LARGURA, pista_y - PISTA_ALTURA / 2,
        LARGURA - HUD_LARGURA, PISTA_ALTURA, cor_pista);
    /* Bordas */
    DrawRectangle(HUD_LARGURA, pista_y - PISTA_ALTURA / 2,
        LARGURA - HUD_LARGURA, 3, DARKGRAY);
    DrawRectangle(HUD_LARGURA, pista_y + PISTA_ALTURA / 2 - 3,
        LARGURA - HUD_LARGURA, 3, DARKGRAY);
    /* Linha de largada */
    DrawRectangle(HUD_LARGURA, pista_y - PISTA_ALTURA / 2,
        4, PISTA_ALTURA, YELLOW);
    /* Linha de chegada */
    int meta_px = (int)pista_para_tela_x(META_X);
    for (int i = 0; i < PISTA_ALTURA; i += 16)
        DrawRectangle(meta_px, pista_y - PISTA_ALTURA / 2 + i,
        8, 8, (i / 8 % 2 == 0) ? WHITE : BLACK);
}

static void desenhar_corredor(const Corredor *c, int pista_y, Color cor) {
    float tx = pista_para_tela_x(c->x);
    float ty = (float)(pista_y) - (float)(CORREDOR_H) - c->y_pulo;
    DrawRectangle((int)(tx - CORREDOR_W / 2), (int)ty,
        CORREDOR_W, CORREDOR_H, cor);
}

static void desenhar_obstaculos(const Obstaculo *obs, int qtd, int pista_y, Color cor) {
    for (int i = 0; i < qtd; i++) {
        float tx = pista_para_tela_x(obs[i].x);
        float ty = (float)(pista_y) - (float)(OBST_H);
        DrawRectangle((int)(tx - OBST_W / 2), (int)ty,
            OBST_W, OBST_H, cor);
    }
}

static void desenhar_hud_lateral(const Corredor *c1, const Corredor *c2) {
    /* Barra de progresso P1 */
    float prog1 = c1->x / PISTA_COMPRIMENTO;
    float prog2 = c2->x / PISTA_COMPRIMENTO;

    DrawText("P1", 4, PISTA_Y_P1 - 30, 18, SKYBLUE);
    DrawRectangle(4, PISTA_Y_P1,      46, 10, DARKGRAY);
    DrawRectangle(4, PISTA_Y_P1,      (int)(46 * prog1), 10, SKYBLUE);

    DrawText("P2", 4, PISTA_Y_P2 - 30, 18, ORANGE);
    DrawRectangle(4, PISTA_Y_P2,      46, 10, DARKGRAY);
    DrawRectangle(4, PISTA_Y_P2,      (int)(46 * prog2), 10, ORANGE);

    /* Combos */
    DrawText(TextFormat("x%d", c1->combo), 4, PISTA_Y_P1 + 20, 16, SKYBLUE);
    DrawText(TextFormat("x%d", c2->combo), 4, PISTA_Y_P2 + 20, 16, ORANGE);
}

/* Tela de contagem regressiva 3-2-1-VÁ antes da corrida. */
static void contagem_regressiva(void) {
    const char *msgs[] = { "3", "2", "1", "VAI!" };
    for (int i = 0; i < 4; i++) {
        double inicio = GetTime();
        while (GetTime() - inicio < 0.8) {
            BeginDrawing();
                ClearBackground(BLACK);
                int fs = (i == 3) ? 90 : 120;
                Color cor = (i == 3) ? GREEN : YELLOW;
                DrawText(msgs[i],
                    LARGURA / 2 - MeasureText(msgs[i], fs) / 2,
                    ALTURA  / 2 - fs / 2, fs, cor);
            EndDrawing();
            if (WindowShouldClose()) return;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Função principal do minigame                                       */
/* ------------------------------------------------------------------ */
int jogar_corrida_ponteiros(int *pontos_p1, int *pontos_p2) {

    /* -------- Sorteia configuração de obstáculos -------- */
    int cfg_idx = GetRandomValue(0, 2);
    const ConfigObstaculos *cfg = &CONFIGS[cfg_idx];

    /* Monta array de obstáculos (posição lógica na pista) */
    Obstaculo obstaculos[MAX_OBSTACULOS];
    for (int i = 0; i < cfg->quantidade; i++)
        obstaculos[i].x = cfg->posicoes[i] * PISTA_COMPRIMENTO;

    /* -------- Inicializa corredores -------- */
    Corredor p1, p2;
    init_corredor(&p1, KEY_A, KEY_D, KEY_W);
    init_corredor(&p2, KEY_LEFT, KEY_RIGHT, KEY_UP);

    /* -------- Contagem regressiva -------- */
    contagem_regressiva();
    if (WindowShouldClose()) { *pontos_p1 = 0; *pontos_p2 = 0; return 1; }

    /* ================================================================
     * LOOP PRINCIPAL
     * ================================================================ */
    while (!WindowShouldClose() && !p1.terminou && !p2.terminou) {

        float dt = GetFrameTime();

        /* ---- INPUT + MOVIMENTO: helper macro para cada jogador ---- */

        /* Processa um corredor: alternância de teclas e pulo. */
        #define PROCESSAR_CORREDOR(c)                                       \
        do {                                                                \
            /* Alternância de teclas de corrida */                          \
            bool pressionou_esq = IsKeyPressed((c).tecla_esq);              \
            bool pressionou_dir = IsKeyPressed((c).tecla_dir);              \
            if (pressionou_esq || pressionou_dir) {                         \
                bool acertou = (c).esperando_dir ? pressionou_dir           \
                    : pressionou_esq;                                       \
                if (acertou) {                                              \
                    (c).combo++;                                            \
                    (c).esperando_dir = !(c).esperando_dir;                 \
                    atualizar_velocidade(&(c));                             \
                } else {                                                    \
                    (c).combo = 0;                                          \
                    atualizar_velocidade(&(c));                             \
                }                                                           \
            }                                                               \
            /* Pulo */                                                      \
            if (IsKeyPressed((c).tecla_pulo) && (c).no_chao) {              \
                (c).vy       = FORCA_PULO;                                  \
                (c).no_chao  = false;                                       \
            }                                                               \
        } while (0)

        PROCESSAR_CORREDOR(p1);
        PROCESSAR_CORREDOR(p2);
        #undef PROCESSAR_CORREDOR

        /* ---- UPDATE: posição horizontal ---- */
        p1.x += p1.vel * dt;
        p2.x += p2.vel * dt;

        /* ---- UPDATE: física do pulo ---- */
        #define ATUALIZAR_PULO(c)                                           \
        do {                                                                \
            if (!(c).no_chao) {                                             \
                (c).vy      += GRAVIDADE * dt;                              \
                (c).y_pulo  -= (c).vy * dt;                                 \
                if ((c).y_pulo <= CHAO_Y) {                                 \
                    (c).y_pulo  = CHAO_Y;                                   \
                    (c).vy      = 0.0f;                                     \
                    (c).no_chao = true;                                     \
                }                                                           \
            }                                                               \
        } while (0)

        ATUALIZAR_PULO(p1);
        ATUALIZAR_PULO(p2);
        #undef ATUALIZAR_PULO

        /* ---- COLISÃO com obstáculos ---- */
        for (int i = 0; i < cfg->quantidade; i++) {

            /* Posição de tela do corredor P1 */
            float p1_tx = pista_para_tela_x(p1.x) - CORREDOR_W / 2.0f;
            float p1_ty = (float)(PISTA_Y_P1) - (float)(CORREDOR_H) - p1.y_pulo;
            float o_tx  = pista_para_tela_x(obstaculos[i].x) - OBST_W / 2.0f;
            float o_ty  = (float)(PISTA_Y_P1) - (float)(OBST_H);

            if (p1.no_chao &&  /* só colide se estiver no chão */
                colide_aabb(p1_tx, p1_ty, CORREDOR_W, CORREDOR_H,
                    o_tx,  o_ty,  OBST_W,    OBST_H)) {
                p1.vel  = clampf(p1.vel - PENALIDADE_COLISAO, VEL_BASE, 9999.0f);
                p1.combo = 0;
                /* Empurra o corredor para além do obstáculo para não ficar "preso" */
                p1.x = obstaculos[i].x + (PISTA_COMPRIMENTO / (float)LARGURA) * (OBST_W + 4);
            }

            /* Posição de tela do corredor P2 */
            float p2_tx = pista_para_tela_x(p2.x) - CORREDOR_W / 2.0f;
            float p2_ty = (float)(PISTA_Y_P2) - (float)(CORREDOR_H) - p2.y_pulo;
            float o2_ty = (float)(PISTA_Y_P2) - (float)(OBST_H);

            if (p2.no_chao &&
                colide_aabb(p2_tx, p2_ty, CORREDOR_W, CORREDOR_H,
                    o_tx,  o2_ty, OBST_W,    OBST_H)) {
                p2.vel  = clampf(p2.vel - PENALIDADE_COLISAO, VEL_BASE, 9999.0f);
                p2.combo = 0;
                p2.x = obstaculos[i].x + (PISTA_COMPRIMENTO / (float)LARGURA) * (OBST_W + 4);
            }
        }

        /* ---- Verifica chegada à meta ---- */
        if (p1.x >= META_X) p1.terminou = true;
        if (p2.x >= META_X) p2.terminou = true;

        /* ---- DRAW ---- */
        BeginDrawing();
            ClearBackground((Color){18, 22, 35, 255});

            /* Pistas */
            desenhar_pista(PISTA_Y_P1, (Color){30, 30, 50, 255});
            desenhar_pista(PISTA_Y_P2, (Color){30, 30, 50, 255});

            /* Obstáculos */
            desenhar_obstaculos(obstaculos, cfg->quantidade,
                PISTA_Y_P1, RED);
            desenhar_obstaculos(obstaculos, cfg->quantidade,
                PISTA_Y_P2, RED);

            /* Corredores */
            desenhar_corredor(&p1, PISTA_Y_P1, SKYBLUE);
            desenhar_corredor(&p2, PISTA_Y_P2, ORANGE);

            /* HUD lateral */
            desenhar_hud_lateral(&p1, &p2);

            /* Instrução de controles */
            DrawText("P1: A/D correr  W pular",
                HUD_LARGURA + 10, PISTA_Y_P1 - PISTA_ALTURA / 2 + 6,
                16, SKYBLUE);
            DrawText("P2: </> correr  ^ pular",
                HUD_LARGURA + 10, PISTA_Y_P2 - PISTA_ALTURA / 2 + 6,
                16, ORANGE);

            /* Velocidades actuais */
            DrawText(TextFormat("%.0f px/s", p1.vel),
                LARGURA - 120, PISTA_Y_P1 - PISTA_ALTURA / 2 + 6, 16, SKYBLUE);
            DrawText(TextFormat("%.0f px/s", p2.vel),
                LARGURA - 120, PISTA_Y_P2 - PISTA_ALTURA / 2 + 6, 16, ORANGE);

        EndDrawing();
    }

    /* -------- Determina vencedor -------- */
    int vencedor;
    if (p1.terminou && !p2.terminou)       vencedor = 1;
    else if (p2.terminou && !p1.terminou)  vencedor = 2;
    else                                   vencedor = 1; /* empate: P1 leva */

    *pontos_p1 = (vencedor == 1) ? 1 : 0;
    *pontos_p2 = (vencedor == 2) ? 1 : 0;
    return vencedor;
}