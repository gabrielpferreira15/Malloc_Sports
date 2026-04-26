/* ============================================================
 * entities.c — IMPLEMENTAÇÃO das funções de criar/destruir
 * ------------------------------------------------------------
 * Dono: DEV 1 — Gabriel Peixoto
 *
 * IDEIA GERAL
 *   Aqui você implementa as funções que ALOCAM e LIBERAM
 *   memória das structs declaradas em entities.h.
 *
 *   Para cada struct, você vai escrever um par de funções:
 *     - criar_X():   recebe os parâmetros iniciais, faz malloc()
 *                    do tamanho da struct, preenche os campos
 *                    e devolve um PONTEIRO pra ela.
 *     - destruir_X(): recebe o ponteiro, da free() e pronto.
 *
 *   POR QUE usar malloc em vez de variável local?
 *     Variável local vive na PILHA (stack). Some quando a função
 *     que a criou retorna. Como queremos que o Jogador exista
 *     enquanto o jogo todo roda, precisamos coloca-lo na HEAP
 *     com malloc — assim ele só morre quando dermos free().
 *
 *   ANALOGIA: stack é um post-it na sua mesa (some quando você
 *   sai). Heap é uma gaveta (só some quando você abre e tira).
 *
 * O QUE ESTE ARQUIVO PRECISA TER (passo a passo)
 *
 *   PASSO 1: Inclua os headers
 *     #include "entities.h"
 *     #include <stdlib.h>     // malloc, free, NULL
 *
 *   PASSO 2: Implemente criar_jogador()
 *     a) Jogador *j = malloc(sizeof(Jogador));
 *        // sizeof(Jogador) = quantos bytes a struct ocupa
 *     b) Se j == NULL, retorne NULL (memória acabou).
 *     c) Preencha cada campo com os parâmetros recebidos:
 *          j->x = x_inicial;
 *          j->y = y_inicial;
 *          j->pontos = 0;
 *          j->vidas  = 3;
 *          j->tecla_cima  = t_cima;
 *          j->tecla_baixo = t_baixo;
 *          j->tecla_esq   = t_esq;
 *          j->tecla_dir   = t_dir;
 *     d) return j;
 *
 *   PASSO 3: Implemente destruir_jogador(Jogador *j)
 *     a) Se j == NULL, retorne (não da free em ponteiro nulo).
 *     b) free(j);
 *
 *     POR QUE checar NULL? Porque alguém pode chamar
 *     destruir_jogador(NULL) por engano. free(NULL) é seguro
 *     no padrão C, mas é bom hábito conferir.
 *
 *   PASSO 4: Repita o padrão para Bola, Bloco e Obstáculo.
 *     ATENÇÃO no Obstáculo: ele tem ->próximo. Decida se
 *     destruir_obstaculo libera SÓ ele ou TODA a lista a
 *     partir dele.
 *     Recomendado: destruir_obstaculo libera SÓ um no.
 *     Quem libera a lista inteira é outra função
 *     (lista_liberar, em linked_list.c).
 *
 *   EXEMPLO COMPLETO (Bola):
 *     Bola *criar_bola(float x, float y, float vx, float vy) {
 *         Bola *b = malloc(sizeof(Bola));
 *         if (b == NULL) return NULL;
 *         b->x = x;
 *         b->y = y;
 *         b->vel_x = vx;
 *         b->vel_y = vy;
 *         b->raio  = 8;
 *         return b;
 *     }
 *
 *     void destruir_bola(Bola *b) {
 *         if (b == NULL) return;
 *         free(b);
 *     }
 *
 * REQUISITOS DA DISCIPLINA QUE ESTE ARQUIVO COBRE
 *   - Alocação dinâmica (malloc/free)
 *   - Ponteiros
 *   - Structs
 *
 * COMO TESTAR
 *   No main, crie um Jogador, imprima os campos, depois libere.
 *   Compile com: gcc ... -fsanitize=address -g
 *   Se houver vazamento, esse compilador avisa.
 *
 * ATENÇÃO (erros comuns)
 *   - SEMPRE cheque se malloc retornou NULL.
 *   - Para CADA malloc deve existir UM free correspondente.
 *   - Depois do free, o ponteiro vira "pendurado" (dangling).
 *     Se você usar o ponteiro depois do free, o programa pode
 *     crashar OU pior — funcionar errado intermitentemente.
 *     Boa prática: ponteiro = NULL logo após o free.
 *   - NÃO dê free 2 vezes no mesmo ponteiro (double free).
 *     Isso quebra a heap e crasha tudo.
 * ============================================================ */

/* TODO: inclua "entities.h" e <stdlib.h> */


/* TODO: implemente criar_jogador e destruir_jogador */


/* TODO: implemente criar_bola e destruir_bola */


/* TODO: implemente criar_bloco e destruir_bloco */


/* TODO: implemente criar_obstaculo e destruir_obstaculo */
