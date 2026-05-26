#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Configurações da Tela
#define LARGURA       800
#define ALTURA        450

// Configurações da Matriz do Cenário
#define LINHAS        9
#define COLUNAS       16
#define TILE_SIZE     50 // Cada bloco tem 50x50 pixels (16x50 = 800px de largura)

// Tipos de blocos na Matriz
#define AR            0
#define AREIA         1
#define REDE          2

// Física
#define GRAVIDADE     980.0f
#define FORCA_PULO    -500.0f
#define VEL_JOGADOR   300.0f

/* ------------------------------------------------------------------ */
/* 1. STRUCTS (Estruturas)                                           */
/* ------------------------------------------------------------------ */

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float raio;
} Bola;

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float largura, altura;
    int pontos;
    int tecla_esq, tecla_dir, tecla_pulo;
    Color cor;
} Jogador;

// Estrutura para as Partículas (Lista Encadeada)
typedef struct Particula {
    Vector2 pos;
    Vector2 vel;
    float vida;       // Tempo de vida restante em segundos
    Color cor;
} Particula;

// Nó da Lista Encadeada
typedef struct Node {
    Particula p;
    struct Node* proximo;
} Node;

/* ------------------------------------------------------------------ */
/* 2. ALOCAÇÃO DINÂMICA E LISTAS ENCADEADAS                          */
/* ------------------------------------------------------------------ */

// Função para adicionar uma nova partícula na lista (MALLOC)
void adicionar_particula(Node** topo, Vector2 pos, Vector2 vel, Color cor) {
    Node* novo_nodo = (Node*)malloc(sizeof(Node));
    if (novo_nodo == NULL) return; // Proteção caso falte memória

    novo_nodo->p.pos = pos;
    novo_nodo->p.vel = vel;
    novo_nodo->p.vida = 0.5f; // Meio segundo de vida
    novo_nodo->p.cor = cor;
    
    novo_nodo->proximo = *topo;
    *topo = novo_nodo;
}

// Função para atualizar e limpar partículas mortas (FREE)
void atualizar_e_desenhar_particulas(Node** topo, float dt) {
    Node* atual = *topo;
    Node* anterior = NULL;

    while (atual != NULL) {
        // Atualiza física da partícula
        atual->p.pos.x += atual->p.vel.x * dt;
        atual->p.pos.y += atual->p.vel.y * dt;
        atual->p.vida -= dt;

        // Desenha a partícula
        DrawCircleV(atual->p.pos, 3, atual->p.cor);

        // Se a partícula morreu, remove da lista encadeada e libera memória
        if (atual->p.vida <= 0) {
            Node* deletar = atual;
            if (anterior == NULL) {
                *topo = atual->proximo;
                atual = *topo;
            } else {
                anterior->proximo = atual->proximo;
                atual = atual->proximo;
            }
            free(deletar); // Libera a memória dinamicamente alocada
        } else {
            anterior = atual;
            atual = atual->proximo;
        }
    }
}

// Limpa toda a lista antes de fechar o jogo para não dar Memory Leak
void limpar_todas_particulas(Node* topo) {
    while (topo != NULL) {
        Node* aux = topo;
        topo = topo->proximo;
        free(aux);
    }
}

/* ------------------------------------------------------------------ */
/* 3. FUNÇÃO PRINCIPAL DO JOGO                                       */
/* ------------------------------------------------------------------ */

int main(void) {
    InitWindow(LARGURA, ALTURA, "Vôlei de Ponteiros - Estrutura de Dados");

    // Ponteiro para o topo da lista encadeada de partículas
    Node* lista_particulas = NULL;

    /* -------------------------------------------------------------- */
    /* MATRIZ DO CENÁRIO (Tilemap)                                   */
    /* 0 = Ar, 1 = Areia (Chão), 2 = Rede                            */
    /* -------------------------------------------------------------- */
    int cenario[LINHAS][COLUNAS] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0}, // Linha da Rede
        {0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0}, // Linha da Rede
        {0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0}, // Linha da Rede
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}  // Chão de Areia
    };

    // Inicialização dos Jogadores (P1 na Esquerda, P2 na Direita)
    Jogador p1 = {
        .pos = { 150, 300 }, .vel = { 0, 0 }, .largura = 30, .altura = 60,
        .tecla_esq = KEY_A, .tecla_dir = KEY_D, .tecla_pulo = KEY_W, .pontos = 0, .cor = SKYBLUE
    };

    Jogador p2 = {
        .pos = { 620, 300 }, .vel = { 0, 0 }, .largura = 30, .altura = 60,
        .tecla_esq = KEY_LEFT, .tecla_dir = KEY_RIGHT, .tecla_pulo = KEY_UP, .pontos = 0, .cor = ORANGE
    };

    // Inicialização da Bola
    Bola bola = { .pos = { 200, 100 }, .vel = { 180, 0 }, .raio = 12 };

    // Limite do chão baseado na matriz (Linha 8 * TILE_SIZE)
    float chao_y = 8 * TILE_SIZE;
    float rede_x = 8 * TILE_SIZE; // Coluna 8 é onde fica a rede

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        /* ---------------------------------------------------------- */
        /* CONTROLES E MOVIMENTAÇÃO (Uso de Ponteiros implícito)     */
        /* ---------------------------------------------------------- */
        
        // Jogador 1
        if (IsKeyDown(p1.tecla_esq)) p1.vel.x = -VEL_JOGADOR;
        else if (IsKeyDown(p1.tecla_dir)) p1.vel.x = VEL_JOGADOR;
        else p1.vel.x = 0;

        if (IsKeyPressed(p1.tecla_pulo) && p1.pos.y + p1.altura >= chao_y) p1.vel.y = FORCA_PULO;

        // Jogador 2
        if (IsKeyDown(p2.tecla_esq)) p2.vel.x = -VEL_JOGADOR;
        else if (IsKeyDown(p2.tecla_dir)) p2.vel.x = VEL_JOGADOR;
        else p2.vel.x = 0;

        if (IsKeyPressed(p2.tecla_pulo) && p2.pos.y + p2.altura >= chao_y) p2.vel.y = FORCA_PULO;

        /* ---------------------------------------------------------- */
        /* FÍSICA E GRAVIDADE                                        */
        /* ---------------------------------------------------------- */
        
        // Aplica gravidade nos jogadores e atualiza posições
        p1.vel.y += GRAVIDADE * dt;
        p2.vel.y += GRAVIDADE * dt;
        p1.pos.x += p1.vel.x * dt;
        p1.pos.y += p1.vel.y * dt;
        p2.pos.x += p2.vel.x * dt;
        p2.pos.y += p2.vel.y * dt;

        // Aplica gravidade na Bola
        bola.vel.y += (GRAVIDADE * 0.6f) * dt; // Bola cai um pouco mais leve
        bola.pos.x += bola.vel.x * dt;
        bola.pos.y += bola.vel.y * dt;

        /* ---------------------------------------------------------- */
        /* COLISÕES E LIMITES                                        */
        /* ---------------------------------------------------------- */

        // Bloqueio dos jogadores para não passarem da rede nem saírem da tela
        if (p1.pos.x < 0) p1.pos.x = 0;
        if (p1.pos.x + p1.largura > rede_x) p1.pos.x = rede_x - p1.largura;

        if (p2.pos.x < rede_x + 10) p2.pos.x = rede_x + 10;
        if (p2.pos.x + p2.largura > LARGURA) p2.pos.x = LARGURA - p2.largura;

        // Trata colisão dos jogadores com o chão
        if (p1.pos.y + p1.altura >= chao_y) {
            p1.pos.y = chao_y - p1.altura;
            p1.vel.y = 0;
        }
        if (p2.pos.y + p2.altura >= chao_y) {
            p2.pos.y = chao_y - p2.altura;
            p2.vel.y = 0;
        }

        // Colisão da Bola com as Paredes Laterais
        if (bola.pos.x - bola.raio < 0 || bola.pos.x + bola.raio > LARGURA) {
            bola.vel.x *= -1;
        }

        // Colisão da Bola com a Rede (Coluna central)
        if (bola.pos.y > 5 * TILE_SIZE && bola.pos.x + bola.raio >= rede_x && bola.pos.x - bola.raio <= rede_x + 10) {
            bola.vel.x *= -1;
            // Efeito de faíscas na rede
            for(int i=0; i<5; i++) 
                adicionar_particula(&lista_particulas, bola.pos, (Vector2){GetRandomValue(-50,50), GetRandomValue(-50,50)}, WHITE);
        }

        // Colisão da Bola com o Jogador 1 (AABB vs Círculo simplificado)
        if (CheckCollisionCircleRec(bola.pos, bola.raio, (Rectangle){p1.pos.x, p1.pos.y, p1.largura, p1.altura})) {
            bola.vel.y = -350.0f; // Joga a bola para cima
            bola.vel.x = (bola.pos.x - (p1.pos.x + p1.largura/2)) * 10; // Direção baseada em onde bateu no corpo
            
            // Cria partículas de impacto (Suor/Esforço)
            for(int i=0; i<8; i++)
                adicionar_particula(&lista_particulas, bola.pos, (Vector2){GetRandomValue(-100,100), GetRandomValue(-200,50)}, SKYBLUE);
        }

        // Colisão da Bola com o Jogador 2
        if (CheckCollisionCircleRec(bola.pos, bola.raio, (Rectangle){p2.pos.x, p2.pos.y, p2.largura, p2.altura})) {
            bola.vel.y = -350.0f;
            bola.vel.x = (bola.pos.x - (p2.pos.x + p2.largura/2)) * 10;

            for(int i=0; i<8; i++)
                adicionar_particula(&lista_particulas, bola.pos, (Vector2){GetRandomValue(-100,100), GetRandomValue(-200,50)}, ORANGE);
        }

        // Bola tocou no Chão de Areia (Ponto!)
        if (bola.pos.y + bola.raio >= chao_y) {
            // Cria explosão de areia na posição do impacto
            for(int i=0; i<20; i++) {
                adicionar_particula(&lista_particulas, bola.pos, 
                    (Vector2){(float)GetRandomValue(-150, 150), (float)GetRandomValue(-300, -50)}, YELLOW);
            }

            // Verifica de qual lado caiu para dar o ponto
            if (bola.pos.x < rede_x) p2.pontos++;
            else p1.pontos++;

            // Reseta a bola no meio do lado de quem pontuou
            bola.pos = (bola.pos.x < rede_x) ? (Vector2){600, 100} : (Vector2){200, 100};
            bola.vel = (bola.pos.x < rede_x) ? (Vector2){-180, 0} : (Vector2){180, 0};
        }

        /* ---------------------------------------------------------- */
        /* RENDERIZAÇÃO / DESENHO                                    */
        /* ---------------------------------------------------------- */
        BeginDrawing();
        ClearBackground(EXERALD); // Um fundo azul-esverdeado para simular o céu/mar

        // Desenha o cenário lendo a MATRIZ
        for (int i = 0; i < LINHAS; i++) {
            for (int j = 0; j < COLUNAS; j++) {
                if (cenario[i][j] == AREIA) {
                    DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, KHAKI);
                } else if (cenario[i][j] == REDE) {
                    DrawRectangle(j * TILE_SIZE, i * TILE_SIZE + 5, 10, TILE_SIZE, LIGHTGRAY);
                    // Detalhe da rede quadriculada
                    DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE + 5, 10, TILE_SIZE, DARKGRAY);
                }
            }
        }

        // Desenha as Partículas Ativas e gerencia a memória delas
        atualizar_e_desenhar_particulas(&lista_particulas, dt);

        // Desenha os Jogadores
        DrawRectangleRec((Rectangle){p1.pos.x, p1.pos.y, p1.largura, p1.altura}, p1.cor);
        DrawRectangleRec((Rectangle){p2.pos.x, p2.pos.y, p2.largura, p2.altura}, p2.cor);

        // Desenha a Bola
        DrawCircleV(bola.pos, bola.raio, WHITE);
        DrawCircleLines((int)bola.pos.x, (int)bola.pos.y, bola.raio, BLACK); // Contorno da bola de vôlei

        // Placar
        DrawText(TextFormat("%d", p1.pontos), LARGURA/2 - 100, 30, 40, SKYBLUE);
        DrawText("x", LARGURA/2 - 10, 35, 30, WHITE);
        DrawText(TextFormat("%d", p2.pontos), LARGURA/2 + 80, 30, 40, ORANGE);

        EndDrawing();
    }

    // Libera a memória de qualquer partícula que tenha sobrado antes de fechar
    limpar_todas_particulas(lista_particulas);

    CloseWindow();
    return 0;
}