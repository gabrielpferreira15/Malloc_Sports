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

#define DURACAO_PARTIDA      60.0f

#define VEL_BOLA_X_INICIAL 300.0f
#define VEL_BOLA_Y_INICIAL -200.0f

#define TEMPO_SAQUE 1.0f

#define MAX_TOQUES 3

#define LADO_NENHUM 0
#define LADO_P1 1
#define LADO_P2 2

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

float clampf(float valor, float min, float max){
    if (valor < min) return min;
    if (valor > max) return max;

    return valor;
}

// ------------------------------------------------------

static int lado_bola_por_x(float x){
    return (x < LARGURA / 2.0f) ? LADO_P1 : LADO_P2;
}

// Mantem apenas o lado atual da bola (posicao), sem resetar toques aqui.
// O reset passa a acontecer no primeiro toque do time, evitando carry over.
static void atualizar_lado_bola(const Bola *bola, int *lado_bola){
    *lado_bola = lado_bola_por_x(bola->x);
}

// Registra um toque e garante reset correto quando a posse muda de time.
// Retorna false quando o time estoura o limite de toques.
static bool registrar_toque(int lado_jogador,int *lado_toque,int *toques_p1,int *toques_p2)
{
    int *toques = (lado_jogador == LADO_P1) ? toques_p1 : toques_p2;

    // Reset no primeiro toque do time (posse mudou), evitando carry over.
    if (*lado_toque != lado_jogador)
    {
        *lado_toque = lado_jogador;
        *toques = MAX_TOQUES;
    }

    if (*toques <= 0)
        return false;

    (*toques)--;
    return true;
}

// ------------------------------------------------------

// Prepara o saque: bola parada no centro e direcao sorteada.
void resetar_bola(Bola *bola, int *direcao_saque, float *tempo_saque)
{
    bola->x = LARGURA / 2.0f;
    bola->y = 200;

    bola->vel_x = 0.0f;
    bola->vel_y = 0.0f;

    bola->raio = BOLA_RAIO;

    *direcao_saque =
        (GetRandomValue(0, 1) == 0)
        ? LADO_P1
        : LADO_P2;

    *tempo_saque = TEMPO_SAQUE;
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

bool colisao_bola_cabeca(Bola *bola, Jogador *j){
    float cabeca_x =
        j->x + j->largura / 2.0f;

    float cabeca_y =
        j->y + CABECA_RAIO;

    float dx = bola->x - cabeca_x;
    float dy = bola->y - cabeca_y;

    float distancia =
        sqrtf(dx * dx + dy * dy);

    if (distancia <= 0.0001f)
        return false;

    if (distancia < bola->raio + CABECA_RAIO)
    {
        float normal_x = dx / distancia;
        float normal_y = dy / distancia;

        // Afasta a bola para fora da cabeca, evitando overlap em varios frames.
        float ajuste = (bola->raio + CABECA_RAIO) - distancia + 0.5f;
        if (ajuste > 0.0f)
        {
            bola->x += normal_x * ajuste;
            bola->y += normal_y * ajuste;
        }

        float forca = 700.0f;

        bola->vel_x = normal_x * forca;

        bola->vel_y = normal_y * forca;

        bola->vel_y -= 300;

        return true;
    }

    return false;
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

// Aplica ponto e reinicia o rally quando nao for ponto de ouro.
static void aplicar_ponto(
    int jogador_ponto,
    int *pts1,
    int *pts2,
    bool ponto_de_ouro,
    int *vencedor,
    Bola *bola,
    int *lado_toque,
    int *toques_p1,
    int *toques_p2,
    int *direcao_saque,
    float *tempo_saque
)
{
    if (jogador_ponto == 1)
        (*pts1)++;
    else
        (*pts2)++;

    // Sempre reseta toques e posse para evitar carry over no proximo rally.
    *lado_toque = LADO_NENHUM;
    *toques_p1 = MAX_TOQUES;
    *toques_p2 = MAX_TOQUES;

    if (ponto_de_ouro)
    {
        *vencedor = jogador_ponto;
        return;
    }

    resetar_bola(bola, direcao_saque, tempo_saque);
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

    int pts1 = 0;
    int pts2 = 0;

    float tempo_restante = DURACAO_PARTIDA;
    bool  ponto_de_ouro = false;
    int   vencedor = 0;

    int toques_p1 = MAX_TOQUES;
    int toques_p2 = MAX_TOQUES;
    int lado_bola = LADO_NENHUM;  // Lado da bola por posicao (x) para filtrar colisao.
    int lado_toque = LADO_NENHUM; // Ultimo time que tocou (posse) para reset correto.

    bool contato_p1 = false;
    bool contato_p2 = false;

    float tempo_saque = 0.0f;
    int direcao_saque = LADO_P1;

    resetar_bola(&bola, &direcao_saque, &tempo_saque);
    
    while (!WindowShouldClose() && vencedor == 0)
    {
        float dt = GetFrameTime();

        // Atualização do tempo da partida e controle de "ponto de ouro"
        if (!ponto_de_ouro) {
            tempo_restante -= dt;
            if (tempo_restante <= 0.0f) {
                tempo_restante = 0.0f;
                if (pts1 == pts2)
                    ponto_de_ouro = true; // empate -> morte subita
                else
                    vencedor = (pts1 > pts2) ? 1 : 2;
            }
        }

        if (vencedor == 0)
        {
            bool ponto_aplicado = false;
            bool bola_parada = (tempo_saque > 0.0f);

            if (bola_parada)
            {
                contato_p1 = false;
                contato_p2 = false;
            }

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

            // Contagem regressiva do saque; quando zera, libera a bola.
            if (bola_parada)
            {
                tempo_saque -= dt;

                if (tempo_saque <= 0.0f)
                {
                    tempo_saque = 0.0f;

                    bola.vel_x =
                        (direcao_saque == LADO_P1)
                        ? -VEL_BOLA_X_INICIAL
                        : VEL_BOLA_X_INICIAL;

                    bola.vel_y = VEL_BOLA_Y_INICIAL;

                    bola_parada = false;
                }
            }

            if (!bola_parada)
            {
                bool colidiu_p1 = false;
                bool colidiu_p2 = false;

                // BOLA
                aplicar_gravidade_bola(&bola, dt);

                colisao_bola_parede(&bola);

                colisao_rede(&bola);

                atualizar_lado_bola(&bola, &lado_bola);

                if (!ponto_aplicado && lado_bola == LADO_P1)
                {
                    colidiu_p1 = colisao_bola_cabeca(&bola, &p1);
                    if (colidiu_p1 && !contato_p1)
                    {
                        // Usa posse para resetar toques no primeiro contato do time.
                        if (!registrar_toque(LADO_P1, &lado_toque, &toques_p1, &toques_p2))
                        {
                            // Quarto toque sem reset: ponto do adversario.
                            aplicar_ponto(
                                2,
                                &pts1,
                                &pts2,
                                ponto_de_ouro,
                                &vencedor,
                                &bola,
                                &lado_toque,
                                &toques_p1,
                                &toques_p2,
                                &direcao_saque,
                                &tempo_saque
                            );
                            ponto_aplicado = true;
                        }
                    }
                }

                if (!ponto_aplicado && lado_bola == LADO_P2)
                {
                    colidiu_p2 = colisao_bola_cabeca(&bola, &p2);
                    if (colidiu_p2 && !contato_p2)
                    {
                        // Usa posse para resetar toques no primeiro contato do time.
                        if (!registrar_toque(LADO_P2, &lado_toque, &toques_p1, &toques_p2))
                        {
                            // Quarto toque sem reset: ponto do adversario.
                            aplicar_ponto(
                                1,
                                &pts1,
                                &pts2,
                                ponto_de_ouro,
                                &vencedor,
                                &bola,
                                &lado_toque,
                                &toques_p1,
                                &toques_p2,
                                &direcao_saque,
                                &tempo_saque
                            );
                            ponto_aplicado = true;
                        }
                    }
                }

                contato_p1 = colidiu_p1;
                contato_p2 = colidiu_p2;

                // PONTO
                if (!ponto_aplicado && bola.y + bola.raio >= CHAO_Y)
                {
                    int jogador_ponto =
                        (bola.x < LARGURA / 2.0f)
                        ? 2
                        : 1;

                    aplicar_ponto(
                        jogador_ponto,
                        &pts1,
                        &pts2,
                        ponto_de_ouro,
                        &vencedor,
                        &bola,
                        &lado_toque,
                        &toques_p1,
                        &toques_p2,
                        &direcao_saque,
                        &tempo_saque
                    );
                    ponto_aplicado = true;
                }
            }
        }

        // DRAW
        BeginDrawing();

        ClearBackground((Color){135, 206, 235, 255});

        // Timer central (ou "Ponto de ouro!")
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
                BLACK
            );
        }

        // Seta indica a direcao do saque enquanto a bola esta parada.
        if (tempo_saque > 0.0f)
        {
            float seta_x = LARGURA / 2.0f;
            float seta_y = 80.0f;
            float seta_tam = 26.0f;

            Vector2 ponta = {
                seta_x + ((direcao_saque == LADO_P1) ? -seta_tam : seta_tam),
                seta_y
            };

            Vector2 base1 = { seta_x, seta_y - seta_tam / 2.0f };
            Vector2 base2 = { seta_x, seta_y + seta_tam / 2.0f };

            // Mantem a mesma orientacao do triangulo para evitar culling em um dos lados.
            if (direcao_saque == LADO_P1)
            {
                Vector2 tmp = base1;
                base1 = base2;
                base2 = tmp;
            }

            DrawTriangle(ponta, base1, base2, BLACK);
        }

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

    if (vencedor == 0){
        if (pts1 > pts2)
            vencedor = 1;
        else if (pts2 > pts1)
            vencedor = 2;
        else
            vencedor = 0;
    }

    return vencedor;
}