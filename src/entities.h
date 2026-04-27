/* ============================================================
 * entities.h — DECLARAÇÃO das structs centrais do jogo
 * ------------------------------------------------------------
 * Dono: DEV 1 — Gabriel Peixoto
 *
 * IDEIA GERAL
 *   Este header reune todas as STRUCTS que representam os
 *   "personagens" e "objetos" do jogo. Os 3 minigames vão
 *   incluir este arquivo para ter acesso a essas definições.
 *
 *   ANALOGIA: uma struct é uma "ficha de cadastro" que junta
 *   várias informações de um mesmo objeto. Por exemplo, um
 *   Jogador tem posição, pontuação, vidas e teclas.
 *
 * O QUE ESTE ARQUIVO PRECISA TER (passo a passo)
 *
 *   PASSO 1: Header guard (sempre o primeiro)
 *     POR QUE? Se um .c incluir este arquivo 2 vezes (direto
 *     ou indireto), o compilador reclama de "redefinição". O
 *     header guard impede isso.
 *
 *       #ifndef ENTITIES_H
 *       #define ENTITIES_H
 *       ... todo o conteúdo do header ...
 *       #endif
 *
 *   PASSO 2: Struct Jogador — usada nos 3 minigames
 *     Sugestao de campos em português:
 *       int x, y;          // posição na tela em pixels
 *       int pontos;        // quantos pontos esse jogador tem
 *       int vidas;         // quantas vidas ele tem
 *       int tecla_cima;    // ex.: KEY_W ou KEY_UP
 *       int tecla_baixo;   // ex.: KEY_S ou KEY_DOWN
 *       int tecla_esq;     // ex.: KEY_A ou KEY_LEFT
 *       int tecla_dir;     // ex.: KEY_D ou KEY_RIGHT
 *
 *     POR QUE guardar a tecla DENTRO da struct? Porque assim
 *     a mesma função mover_jogador(j) serve pros dois jogadores
 *     — cada Jogador sabe quais são SUAS teclas.
 *
 *   PASSO 3: Struct Bola — usada no Pong
 *       float x, y;        // posição (float pra movimento suave)
 *       float vel_x, vel_y;// velocidade em cada eixo (px por seg)
 *       int   raio;        // raio da bola em pixels
 *
 *   PASSO 4: Struct Bloco — usada no Estouro de Pilha
 *       int formato[4][4]; // matriz 4x4 com 1=tem-quadrado, 0=vazio
 *       int cor;           // id da cor do tetromino (1..7)
 *       int x, y;          // posição no GRID (não em pixels)
 *
 *   PASSO 5: Struct Obstáculo — usada na Corrida de Ponteiros
 *     ATENÇÃO: tem ponteiro pra ela mesma porque vai virar
 *     no de uma LISTA ENCADEADA.
 *       float x, y;            // posição
 *       float largura, altura; // tamanho da hitbox
 *       float velocidade;      // px por segundo
 *       struct Obstáculo *próximo;  // ponteiro pro próximo da lista
 *
 *     POR QUE struct Obstáculo *próximo, e não Obstáculo *próximo?
 *     Porque dentro da DEFINICAO da struct, o nome dela ainda
 *     não foi "registrado". Por isso usamos a forma longa.
 *     Apos o typedef (logo depois) podemos usar o nome curto.
 *
 *   PASSO 6: PROTOTIPOS das funções de criação/destruição
 *     (a implementação fica em entities.c)
 *
 *       Jogador *criar_jogador(int x_inicial, int y_inicial,
 *                              int t_cima, int t_baixo,
 *                              int t_esq, int t_dir);
 *       void destruir_jogador(Jogador *j);
 *
 *       Bola *criar_bola(float x, float y, float vx, float vy);
 *       void  destruir_bola(Bola *b);
 *
 *       Bloco *criar_bloco(int tipo, int x, int y);
 *       void   destruir_bloco(Bloco *b);
 *
 *       Obstáculo *criar_obstaculo(float x, float y,
 *                                  float largura, float altura,
 *                                  float velocidade);
 *       void destruir_obstaculo(Obstáculo *o);
 *
 *   PASSO 7: Feche o header guard com #endif
 *
 * NOMENCLATURA (Portugues x Ingles)
 *   O GDD pode usar outros nomes para esses tipos.
 *   Aqui sugerimos nomes em português. Use UM padrão
 *   e seja consistente. Se preferir manter os do GDD,
 *   tudo bem — só não misture.
 *
 * REQUISITOS DA DISCIPLINA QUE ESTE ARQUIVO COBRE
 *   - Structs (todos os 4 tipos)
 *   - Ponteiros (Obstáculo *próximo)
 *
 * COMO TESTAR
 *   No main, crie um Jogador e faça printf de cada campo.
 *   Se compila e mostra os valores certos, está OK.
 *
 * ATENÇÃO
 *   - NÃO ponha código de implementação aqui — só declaração.
 *     Implementação vai em entities.c
 *   - O typedef (typedef struct Jogador Jogador;) deixa você
 *     escrever "Jogador" em vez de "struct Jogador". Use.
 * ============================================================ */

/* TODO: header guard (#ifndef ENTITIES_H ...) */


/* TODO: typedef struct Jogador { ... } Jogador; */


/* TODO: typedef struct Bola { ... } Bola; */


/* TODO: typedef struct Bloco { ... } Bloco; */


/* TODO: typedef struct Obstáculo { ... } Obstáculo; */


/* TODO: protótipos das funções criar_ destruir_ */


/* TODO: feche o header guard com #endif */
