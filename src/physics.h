/* ============================================================
 * physics.h — DECLARAÇÃO das funções de física/colisão
 * ------------------------------------------------------------
 * Dono: DEV 1 — Gabriel Peixoto
 *
 * IDEIA GERAL
 *   Este módulo guarda funções GENÉRICAS que servem para
 *   vários minigames. A principal é a colisão AABB:
 *
 *     AABB = Axis-Aligned Bounding Box
 *     = "caixa retangular alinhada aos eixos"
 *
 *   Cada objeto é tratado como um retângulo (x, y, largura,
 *   altura). Dois retângulos colidem se eles SE SOBREPOEM
 *   tanto no eixo X quanto no eixo Y.
 *
 *   Onde a gente usa?
 *     - Corrida de Ponteiros: atleta colide com obstáculo;
 *     - Pong:   bola colide com raquete;
 *     - Estouro de Pilha: bloco colide com chão/outro bloco.
 *
 * O QUE ESTE ARQUIVO PRECISA TER (passo a passo)
 *
 *   PASSO 1: Header guard
 *     #ifndef PHYSICS_H
 *     #define PHYSICS_H
 *
 *   PASSO 2: Inclua <stdbool.h> (pra usar bool/true/false).
 *     POR QUE? C original não tem tipo bool — vem dessa lib.
 *
 *   PASSO 3: Declare o protótipo da colisão AABB
 *
 *     bool colide_aabb(float ax, float ay,
 *                      float a_largura, float a_altura,
 *                      float bx, float by,
 *                      float b_largura, float b_altura);
 *
 *     - (ax, ay) é o canto superior-esquerdo do retângulo A.
 *     - (a_largura, a_altura) é o tamanho dele.
 *     - Mesma coisa para B.
 *     - Devolve true se eles se tocam.
 *
 *   PASSO 4 (opcional): outros utilitários genéricos
 *
 *     float distância(float x1, float y1, float x2, float y2);
 *         // distância euclidiana entre 2 pontos
 *         // útil pra spawn não colocar coisas muito perto
 *
 *     float clamp(float valor, float mínimo, float máximo);
 *         // garante que valor fique entre mín e máx
 *         // usado pra raquete não sair da tela no Pong
 *
 *   PASSO 5: Feche com #endif.
 *
 * REQUISITOS DA DISCIPLINA QUE ESTE ARQUIVO COBRE
 *   - Funções (com retorno e parâmetros múltiplos)
 *   - Reuso de código entre módulos
 *
 * ATENÇÃO
 *   Só protótipos aqui. Implementação em physics.c.
 * ============================================================ */

/* TODO: header guard, protótipos, #endif */
