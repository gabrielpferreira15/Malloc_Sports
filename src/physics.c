/* ============================================================
 * physics.c — IMPLEMENTAÇÃO da colisão AABB
 * ------------------------------------------------------------
 * Dono: DEV 1 — Gabriel Peixoto
 *
 * IDEIA DA COLISAO AABB (passo a passo)
 *
 *   Imagine 2 retângulos A e B no plano. Cada um com canto
 *   superior-esquerdo em (x, y), largura w e altura h.
 *
 *   Eles NÃO colidem se uma destas 4 afirmações for verdadeira:
 *
 *     1) A está totalmente a esquerda de B:
 *          ax + a_largura < bx
 *
 *     2) A está totalmente a direita de B:
 *          ax > bx + b_largura
 *
 *     3) A está totalmente acima de B:
 *          ay + a_altura < by
 *
 *     4) A está totalmente abaixo de B:
 *          ay > by + b_altura
 *
 *   Logo, eles COLIDEM se NENHUMA dessas é verdade.
 *   Em código, fica assim (escreva você mesmo):
 *
 *     return (ax < bx + b_largura) &&
 *            (ax + a_largura > bx) &&
 *            (ay < by + b_altura) &&
 *            (ay + a_altura > by);
 *
 *   POR QUE 4 condições e não mais? Porque qualquer outra forma
 *   de "não se sobrepor" é impossível — se eles não estão 100%
 *   separados em pelo menos um eixo, eles se sobrepoem.
 *
 * O QUE ESTE ARQUIVO PRECISA TER (passo a passo)
 *   PASSO 1: #include "physics.h"
 *   PASSO 2: Implemente colide_aabb seguindo a lógica acima.
 *   PASSO 3 (opcional): implemente distância() e clamp().
 *
 * IMPLEMENTAÇÃO sugerida de clamp():
 *     float clamp(float valor, float mínimo, float máximo) {
 *         if (valor < mínimo) return mínimo;
 *         if (valor > máximo) return máximo;
 *         return valor;
 *     }
 *
 * COMO TESTAR
 *   Coloque uns prints no main:
 *     printf("%d\n", colide_aabb(0,0,10,10,  5,5,10,10));  // 1
 *     printf("%d\n", colide_aabb(0,0,10,10, 20,0,10,10));  // 0
 *
 * ATENÇÃO
 *   - Use < e > (NÃO <= >=) se você quer que retângulos só
 *     "tocando" (compartilhando a borda) NÃO sejam contados como colisão.
 *     Ou use <= se contam. Decida e mantenha consistente nos
 *     3 minigames.
 *   - Cuidado com números negativos: se algum retângulo tiver
 *     largura ou altura negativa, a lógica quebra. Garanta que
 *     todos são >= 0 no código que chama.
 * ============================================================ */

/* TODO: inclua "physics.h" */


/* TODO: implemente colide_aabb seguindo a lógica documentada acima */


/* TODO: (opcional) implemente clamp e distância */
