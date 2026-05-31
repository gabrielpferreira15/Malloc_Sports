#include "raylib.h"
#include "pong_memoria.h"
#include <math.h>

#define LARGURA             1280
#define ALTURA               720
#define RAQUETE_W             63
#define RAQUETE_H             93
#define VEL_RAQUETE         450.0f
#define BOLA_RAIO             10
#define VEL_BOLA_X_INICIAL  350.0f
#define VEL_BOLA_Y_INICIAL  220.0f
#define VEL_MAX_X           800.0f
#define VEL_MAX_Y_ANGULO    500.0f
#define ACELERACAO_POR_PT     1.1f
#define DURACAO_PARTIDA      90.0f
#define TEMPO_SAQUE           2.0f
#define POWERUP_INTERVALO     8.0f
#define BOOST_DURACAO         3.0f
#define BOOST_FATOR           2.0f
#define MARGEM_RAQUETE        40
#define NUM_FRAMES 5

#define LADO_P1 1
#define LADO_P2 2

typedef struct {
    float x, y; // posição atual da bola
    float vel_x, vel_y;
    int   raio;
} Bola;

typedef struct {
    float x, y; // posição atual da raquete
    int   tecla_cima;
    int   tecla_baixo;
} Raquete;

static float clampf(float v, float lo, float hi) {
    if (v < lo) return lo; // retorna o limite minimo
    if (v > hi) return hi; // retorna o limite maximo
    return v;
}

// Mantem o boost sem perder a velocidade base quando ela muda.
static float aplicar_boost(float v, bool boost_ativo) {
    return boost_ativo ? v * BOOST_FATOR : v;
}

static int circulo_colidiu_retangulo(float cx, float cy, float r,
        float rx, float ry, float rw, float rh) {
    float px = clampf(cx, rx, rx + rw); //ponto X da raquete mais proximo a bola
    float py = clampf(cy, ry, ry + rh); //ponto Y da raquete mais proximo a bola
    float dx = cx - px; //distancia entre os pontos X
    float dy = cy - py; //distancia entre os pontos Y
    return (dx * dx + dy * dy) <= (r * r); // retorna verdadeiro caso tenha tido colisao
}

static void resetar_bola(Bola *b, int *direcao_saque, float *tempo_saque) {
    b->x     = LARGURA / 2.0f; // posição de retorno da bola
    b->y     = ALTURA  / 2.0f;
    b->raio  = BOLA_RAIO; // define o tamanho da bola
    b->vel_x = 0.0f;
    b->vel_y = 0.0f;

    *direcao_saque =
        (GetRandomValue(0, 1) == 0)
        ? LADO_P1
        : LADO_P2;

    *tempo_saque = TEMPO_SAQUE;
}

int jogar_pong_memoria(int *pontos_p1, int *pontos_p2) {
    Texture2D fundo = LoadTexture("assets/sprites/background_pong.png");

    Texture2D raquete_p1 = LoadTexture("assets/sprites/raquete1.png");
    Texture2D raquete_p2 = LoadTexture("assets/sprites/raquete2.png");

    Texture2D bola_boost = LoadTexture("assets/sprites/bola_boost.png");

    Texture2D texturas_bola[NUM_FRAMES];
    bool texturas_validas = true;

    for (int i = 0; i < NUM_FRAMES; i++) {
        // Vai procurar os ficheiros frame0.png, frame1.png, etc...
        const char* caminho_ficheiro = TextFormat("assets/sprites/ping%d.png", i);
        texturas_bola[i] = LoadTexture(caminho_ficheiro);

        if (texturas_bola[i].id <= 0 || texturas_bola[i].width <= 0) {
            texturas_validas = false; // Se uma falhar, cancelamos a animação visual por segurança
        }
    }
    
    int frame_atual = 0;

    float tempo_animacao = 0.0f;
    float velocidade_frame = 0.08f;

    // Controle do frame de impacto
    float tempo_impacto = 0.0f;

    bool mostrando_impacto = false;

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

    Bola bola;

    float vel_x_atual = VEL_BOLA_X_INICIAL;
    float vel_y_atual = VEL_BOLA_Y_INICIAL;

    float tempo_saque = 0.0f;
    int direcao_saque = LADO_P1;

    resetar_bola(&bola, &direcao_saque, &tempo_saque);

    int pts1 = 0, pts2 = 0;

    float flash_p1 = 0.0f, flash_p2 = 0.0f;
    const float FLASH_DUR = 0.4f;

    // Controle de tempo da partida e "ponto de ouro".
    float tempo_restante = DURACAO_PARTIDA;
    bool  ponto_de_ouro = false;
    int   vencedor = 0;

    bool  powerup_ativo  = false;
    float powerup_x      = 0.0f, powerup_y = 0.0f;
    float timer_powerup  = POWERUP_INTERVALO;
    float boost_restante = 0.0f;
    bool  boost_ativo    = false;

    // =========================
    // NOVO SISTEMA DE REBATES
    // =========================
    int contador_rebates = 0; //*temporário* (apenas para visualizar o aumento da velocidade com as rebatidas)

    float velocidade_raquete = VEL_RAQUETE;

    const float ACELERACAO_REBATE_BOLA = 100.0f;
    const float ACELERACAO_REBATE_RAQUETE = 50.0f;

    const int LIMITE_REBATES = 10;

    bool mostrar_anim_boost = false;
    float tempo_anim_boost = 0.0f;

    const float DURACAO_ANIM_BOOST = 0.4f;

    while (!WindowShouldClose() && vencedor == 0) {

        float dt = GetFrameTime();
// ==========================================
// 2. SISTEMA NOVO DE ANIMAÇÃO
// ==========================================

        if (mostrando_impacto) {

            tempo_impacto -= dt;

            // frame4 = impacto
            frame_atual = 4;

            // terminou impacto
            if (tempo_impacto <= 0.0f) {

                mostrando_impacto = false;

                // reinicia animação
                frame_atual = 0;
                tempo_animacao = 0.0f;
            }

        } else {

            // anima apenas até frame3
            if (frame_atual < 3) {

                tempo_animacao += dt;

                if (tempo_animacao >= velocidade_frame) {

                    tempo_animacao = 0.0f;
                    frame_atual++;
                }
            }

            // quando chega no frame3:
            // permanece parado até rebater
        }

        // Atualiza o cronômetro (só roda enquanto não entrou no ponto de ouro).
        if (!ponto_de_ouro) {
            tempo_restante -= dt;
            if (tempo_restante <= 0.0f) {
                tempo_restante = 0.0f;
                if (pts1 == pts2)
                    ponto_de_ouro = true; // empate no fim do tempo → morte súbita
                else
                    vencedor = (pts1 > pts2) ? 1 : 2;
            }
        }

        // Input
        if (IsKeyDown(r1.tecla_cima))
            r1.y -= velocidade_raquete * dt;

        if (IsKeyDown(r1.tecla_baixo))
            r1.y += velocidade_raquete * dt;

        if (IsKeyDown(r2.tecla_cima))
            r2.y -= velocidade_raquete * dt;

        if (IsKeyDown(r2.tecla_baixo))
            r2.y += velocidade_raquete * dt;

        r1.y = clampf(r1.y, 0.0f, ALTURA - RAQUETE_H);
        r2.y = clampf(r2.y, 0.0f, ALTURA - RAQUETE_H);

        bool bola_parada = (tempo_saque > 0.0f);

        if (bola_parada) {
            tempo_saque -= dt;

            if (tempo_saque <= 0.0f) {
                tempo_saque = 0.0f;

                bola.vel_x =
                    (direcao_saque == LADO_P1)
                    ? -vel_x_atual
                    :  vel_x_atual;

                bola.vel_y =
                    (GetRandomValue(0, 1) == 0)
                    ?  vel_y_atual
                    : -vel_y_atual;

                bola_parada = false;
            }
        }

        if (!bola_parada) {
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

            // Colisão com raquete 1
            if (bola.vel_x < 0 &&
                circulo_colidiu_retangulo(
                    bola.x, bola.y,
                    (float)bola.raio,
                    r1.x, r1.y,
                    RAQUETE_W, RAQUETE_H)) {

                // ATIVA O EFEITO DE IMPACTO
                mostrando_impacto = true;
                tempo_impacto = 0.12f;

                bola.x     = r1.x + RAQUETE_W + (float)bola.raio;
                bola.vel_x = fabsf(bola.vel_x);

                float offset =
                    (bola.y - (r1.y + RAQUETE_H / 2.0f))
                    / (RAQUETE_H / 2.0f);

                bola.vel_y = aplicar_boost(
                    VEL_MAX_Y_ANGULO * offset,
                    boost_ativo
                );

                // =========================
                // AUMENTO DE VELOCIDADE
                // =========================
                if (contador_rebates < LIMITE_REBATES) {

                    contador_rebates++;

                    float sinal_x =
                        (bola.vel_x > 0) ? 1.0f : -1.0f;

                    bola.vel_x = aplicar_boost(
                        sinal_x *
                        (VEL_BOLA_X_INICIAL +
                        contador_rebates *
                        ACELERACAO_REBATE_BOLA),
                        boost_ativo
                    );

                    velocidade_raquete =
                        VEL_RAQUETE +
                        contador_rebates *
                        ACELERACAO_REBATE_RAQUETE;
                }
            }

            // Colisão com raquete 2
            if (bola.vel_x > 0 &&
                circulo_colidiu_retangulo(
                    bola.x, bola.y,
                    (float)bola.raio,
                    r2.x, r2.y,
                    RAQUETE_W, RAQUETE_H)) {

                // ATIVA O EFEITO DE IMPACTO
                mostrando_impacto = true;
                tempo_impacto = 0.12f;

                bola.x     = r2.x - (float)bola.raio;
                bola.vel_x = -fabsf(bola.vel_x);

                float offset =
                    (bola.y - (r2.y + RAQUETE_H / 2.0f))
                    / (RAQUETE_H / 2.0f);

                bola.vel_y = aplicar_boost(
                    VEL_MAX_Y_ANGULO * offset,
                    boost_ativo
                );

                // =========================
                // AUMENTO DE VELOCIDADE
                // =========================
                if (contador_rebates < LIMITE_REBATES) {

                    contador_rebates++;

                    float sinal_x =
                        (bola.vel_x > 0) ? 1.0f : -1.0f;

                    bola.vel_x = aplicar_boost(
                        sinal_x *
                        (VEL_BOLA_X_INICIAL +
                        contador_rebates *
                        ACELERACAO_REBATE_BOLA),
                        boost_ativo
                    );

                    velocidade_raquete =
                        VEL_RAQUETE +
                        contador_rebates *
                        ACELERACAO_REBATE_RAQUETE;
                }
            }

            float vel_max_x =
                boost_ativo
                ? VEL_MAX_X * BOOST_FATOR
                : VEL_MAX_X;

            if (fabsf(bola.vel_x) > vel_max_x)
                bola.vel_x =
                    (bola.vel_x > 0)
                    ? vel_max_x
                    : -vel_max_x;

            // Ponto para P2
            if (bola.x + bola.raio < 0) {

                pts2++;

                flash_p1 = FLASH_DUR;

                vel_x_atual = clampf(
                    vel_x_atual * ACELERACAO_POR_PT,
                    VEL_BOLA_X_INICIAL,
                    VEL_MAX_X * 0.8f
                );

                vel_y_atual = clampf(
                    vel_y_atual * ACELERACAO_POR_PT,
                    VEL_BOLA_Y_INICIAL,
                    VEL_MAX_Y_ANGULO * 0.6f
                );

                boost_ativo = false;
                boost_restante = 0.0f;

                powerup_ativo = false;
                timer_powerup = POWERUP_INTERVALO;

                // RESET SISTEMA DE REBATES
                contador_rebates = 0;
                velocidade_raquete = VEL_RAQUETE;

                frame_atual = 0;
                tempo_animacao = 0.0f;

                mostrando_impacto = false;

                mostrar_anim_boost = false;
                tempo_anim_boost = 0.0f;

                resetar_bola(&bola, &direcao_saque, &tempo_saque);

                // Em ponto de ouro, o primeiro a marcar vence imediatamente.
                if (ponto_de_ouro)
                    vencedor = 2;
            }

            // Ponto para P1
            if (bola.x - bola.raio > LARGURA) {

                pts1++;

                flash_p2 = FLASH_DUR;

                vel_x_atual = clampf(
                    vel_x_atual * ACELERACAO_POR_PT,
                    VEL_BOLA_X_INICIAL,
                    VEL_MAX_X * 0.8f
                );

                vel_y_atual = clampf(
                    vel_y_atual * ACELERACAO_POR_PT,
                    VEL_BOLA_Y_INICIAL,
                    VEL_MAX_Y_ANGULO * 0.6f
                );

                boost_ativo = false;
                boost_restante = 0.0f;

                powerup_ativo = false;
                timer_powerup = POWERUP_INTERVALO;

                // RESET SISTEMA DE REBATES
                contador_rebates = 0;
                velocidade_raquete = VEL_RAQUETE;

                frame_atual = 0;
                tempo_animacao = 0.0f;

                mostrando_impacto = false;

                mostrar_anim_boost = false;
                tempo_anim_boost = 0.0f;
                
                resetar_bola(&bola, &direcao_saque, &tempo_saque);

                // Em ponto de ouro, o primeiro a marcar vence imediatamente.
                if (ponto_de_ouro)
                    vencedor = 1;
            }
        }

        // Powerup
        timer_powerup -= dt;

        if (!powerup_ativo &&
            timer_powerup <= 0.0f) {

            powerup_x =
                (float)GetRandomValue(
                    150,
                    LARGURA - 150
                );

            powerup_y =
                (float)GetRandomValue(
                    60,
                    ALTURA - 60
                );

            powerup_ativo = true;
            timer_powerup = POWERUP_INTERVALO;
        }

        if (powerup_ativo && tempo_saque <= 0.0f) {

            float dx = bola.x - powerup_x;
            float dy = bola.y - powerup_y;

            float rs =
                (float)(bola.raio + 14);

            if (dx * dx + dy * dy <= rs * rs) {
                mostrar_anim_boost = true;
                tempo_anim_boost = DURACAO_ANIM_BOOST;

                if (!boost_ativo) {

                    bola.vel_x *= BOOST_FATOR;
                    bola.vel_y *= BOOST_FATOR;

                    boost_ativo = true;
                }

                boost_restante = BOOST_DURACAO;

                powerup_ativo = false;
                timer_powerup = POWERUP_INTERVALO;
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

        if (flash_p1 > 0.0f)
            flash_p1 -= dt;

        if (flash_p2 > 0.0f)
            flash_p2 -= dt;

        // DRAW
        BeginDrawing();

            
            DrawTexturePro(
                fundo,
                (Rectangle){0, 0, fundo.width, fundo.height},
                (Rectangle){0, 0, LARGURA, ALTURA},
                (Vector2){0, 0},
                0.0f,
                WHITE
            );

            if (flash_p1 > 0.0f) {

                unsigned char a =
                    (unsigned char)(
                        255 *
                        (flash_p1 / FLASH_DUR)
                    );

                DrawRectangle(
                    0, 0,
                    12,
                    ALTURA,
                    (Color){220, 50, 50, a}
                );
            }

            if (flash_p2 > 0.0f) {

                unsigned char a =
                    (unsigned char)(
                        255 *
                        (flash_p2 / FLASH_DUR)
                    );

                DrawRectangle(
                    LARGURA - 12,
                    0,
                    12,
                    ALTURA,
                    (Color){220, 50, 50, a}
                );
            }

            for (int i = 0; i < ALTURA; i += 30)
                DrawRectangle(
                    LARGURA / 2 - 2,
                    i,
                    4,
                    15,
                    (Color){80, 80, 80, 200}
                );

            DrawTexturePro(
                raquete_p1,
                (Rectangle){
                    0,
                    0,
                    raquete_p1.width,
                    raquete_p1.height
                },
                (Rectangle){
                    r1.x,
                    r1.y,
                    RAQUETE_W,
                    RAQUETE_H
                },
                (Vector2){0,0},
                0.0f,
                WHITE
            );

            DrawTexturePro(
                raquete_p2,
                (Rectangle){
                    0,
                    0,
                    raquete_p2.width,
                    raquete_p2.height
                },
                (Rectangle){
                    r2.x,
                    r2.y,
                    RAQUETE_W,
                    RAQUETE_H
                },
                (Vector2){0,0},
                0.0f,
                WHITE
            );

            Color cor_bola =
                boost_ativo
                ? YELLOW
                : RAYWHITE;

            // ==========================================
            // 3. DESENHAR O FRAME DA LISTA
            // ==========================================
            if (texturas_validas) {
                Texture2D tex_atual;

                if (mostrar_anim_boost)
                    tex_atual = bola_boost;
                else
                    tex_atual = texturas_bola[frame_atual];

                float largura_tex = (float)tex_atual.width;
                float altura_tex = (float)tex_atual.height;

                // Inverte a imagem dependendo da direção da bola (passando uma largura negativa)
                float direcao_sprite = (bola.vel_x < 0) ? -largura_tex : largura_tex;

                Rectangle source = {
                    0.0f,
                    0.0f,
                    direcao_sprite,
                    altura_tex
                };

                Rectangle dest = {
                    bola.x,
                    bola.y,
                    bola.raio * 4, // Multiplica por 4 para o tamanho se ajustar corretamente no ecrã
                    bola.raio * 4
                };
                
                Vector2 origin = { (dest.width / 2), (dest.height / 2) };

                DrawTexturePro(tex_atual, source, dest, origin, 0.0f, cor_bola);
            } else {
                DrawCircle(
                    (int)bola.x,
                    (int)bola.y,
                    bola.raio,
                    cor_bola
                );
            }

            if (tempo_saque > 0.0f) {
                float dir = (direcao_saque == LADO_P1) ? -1.0f : 1.0f;
                float seta_tam = 18.0f;
                float base_offset = bola.raio + 12.0f;
                float ponta_offset = base_offset + seta_tam;
                float base_x = bola.x + dir * base_offset;
                float seta_y = bola.y;

                Vector2 ponta = {
                    bola.x + dir * ponta_offset,
                    seta_y
                };

                Vector2 base1 = { base_x, seta_y - seta_tam / 2.0f };
                Vector2 base2 = { base_x, seta_y + seta_tam / 2.0f };

                if (dir < 0.0f) {
                    DrawTriangle(ponta, base2, base1, RAYWHITE);
                } else {
                    DrawTriangle(ponta, base1, base2, RAYWHITE);
                }
            }

            if (powerup_ativo) {

                DrawCircle(
                    (int)powerup_x,
                    (int)powerup_y,
                    14,
                    (Color){0, 200, 80, 200}
                );

                DrawText(
                    "boost()",
                    (int)powerup_x - 22,
                    (int)powerup_y - 8,
                    14,
                    BLACK
                );
            }

            DrawText(
                TextFormat("%d", pts1),
                LARGURA / 2 - 70,
                52,
                36,
                YELLOW
            );

            DrawText(
                TextFormat("%d", pts2),
                LARGURA / 2 + 40,
                52,
                36,
                YELLOW
            );

            // Timer central (ou "Ponto de ouro!" quando necessário).
            if (ponto_de_ouro) {
                const char *msg_ouro = "Ponto de ouro!";
                int fs_ouro = 28;
                DrawText(
                    msg_ouro,
                    LARGURA / 2 - MeasureText(msg_ouro, fs_ouro) / 2,
                    20,
                    fs_ouro,
                    GOLD
                );
            } else {
                int tempo_int = (int)ceilf(tempo_restante);
                if (tempo_int < 0) tempo_int = 0;

                int min = tempo_int / 60;
                int seg = tempo_int % 60;

                const char *timer_txt = TextFormat("%d:%02d", min, seg);
                int fs_timer = 28;
                DrawText(
                    timer_txt,
                    LARGURA / 2 - MeasureText(timer_txt, fs_timer) / 2,
                    20,
                    fs_timer,
                    YELLOW
                );
            }

            DrawText(
                "P1 [W/S]",
                (int)r1.x,
                ALTURA - 30,
                18,
                GRAY
            );

            DrawText(
                "P2 [U/D]",
                (int)r2.x - 30,
                ALTURA - 30,
                18,
                GRAY
            );

            // MOSTRA REBATES, apenas para testar, na versao final a gente comenta essa parte
            DrawText(
                TextFormat("REBATES: %d", contador_rebates),
                LARGURA / 2 - 85,
                90,
                24,
                LIGHTGRAY
            );

            if (boost_ativo)
                DrawText(
                    TextFormat(
                        "BOOST! %ds",
                        (int)boost_restante + 1
                    ),
                    LARGURA / 2 - 55,
                    ALTURA - 40,
                    22,
                    YELLOW
                );

        EndDrawing();
    }

    *pontos_p1 = pts1;
    *pontos_p2 = pts2;

    // ==========================================
    // 4. LIMPAR A MEMÓRIA DA LISTA
    // ==========================================
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (texturas_bola[i].id > 0) {
            UnloadTexture(texturas_bola[i]);
        }
    }
    UnloadTexture(fundo);
    UnloadTexture(bola_boost);

    UnloadTexture(raquete_p1);
    UnloadTexture(raquete_p2);

    if (vencedor == 0)
        vencedor = (pts1 >= pts2) ? 1 : 2;

    return vencedor;
}