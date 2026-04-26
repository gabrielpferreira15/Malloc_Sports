/* ============================================================
 * linked_list.c — IMPLEMENTAÇÃO da lista encadeada genérica
 * ------------------------------------------------------------
 * Dono: DEV 1 — Gabriel Peixoto
 *
 * IDEIA DAS OPERACOES (passo a passo, em português estruturado)
 *
 *   --- INSERIR (no começo da lista) ---
 *   POR QUE no começo e não no fim? Inserir no começo é O(1)
 *   (instantâneo). Inserir no fim seria O(N) (precisa percorrer
 *   tudo). Como a ordem dos obstáculos não importa, começo é
 *   melhor.
 *
 *   1. Aloque um No novo: No *novo = malloc(sizeof(No));
 *   2. Se novo == NULL, retorne cabeça (falha graciosa).
 *   3. novo->dado = dado_recebido;
 *   4. novo->próximo = cabeca_atual;
 *   5. return novo;  // novo é a NOVA cabeça
 *
 *   --- REMOVER (o primeiro nó) ---
 *   1. Se cabeça == NULL, retorne NULL (lista vazia).
 *   2. No *segundo = cabeça->próximo;
 *   3. Se dado_saida != NULL: *dado_saida = cabeça->dado;
 *      (assim quem chama recebe o dado removido)
 *   4. free(cabeça);
 *   5. return segundo;
 *
 *   --- BUSCAR (achar um nó que satisfaz um critério) ---
 *   1. No *atual = cabeça;
 *   2. while (atual != NULL) {
 *          if (comparador(atual->dado)) return atual;
 *          atual = atual->próximo;
 *      }
 *   3. return NULL;  // não achou
 *
 *   --- TAMANHO (contar quantos nós) ---
 *   1. int n = 0;
 *   2. No *atual = cabeça;
 *   3. while (atual != NULL) {
 *          n++;
 *          atual = atual->próximo;
 *      }
 *   4. return n;
 *
 *   --- LIBERAR TUDO ---
 *   1. while (cabeça != NULL) {
 *        No *próximo = cabeça->próximo;     // guarda o próximo
 *        if (destrutor != NULL)             // libera o dado se há destrutor
 *            destrutor(cabeça->dado);
 *        free(cabeça);                       // libera o nó
 *        cabeça = próximo;                   // avança
 *      }
 *
 *   POR QUE guardar 'próximo' antes do free? Porque depois do
 *   free(cabeça), não podemos mais acessar cabeça->próximo
 *   (memória liberada é "lixo" — pode ter qualquer coisa).
 *
 * O QUE ESTE ARQUIVO PRECISA TER (passo a passo)
 *   PASSO 1: #include "linked_list.h"
 *   PASSO 2: #include <stdlib.h>   (malloc, free)
 *   PASSO 3: Implemente as 5 funções seguindo a lógica acima.
 *
 * COMO TESTAR
 *   No main, faça este teste:
 *     int *a = malloc(sizeof(int)); *a = 10;
 *     int *b = malloc(sizeof(int)); *b = 20;
 *     int *c = malloc(sizeof(int)); *c = 30;
 *     No *lista = NULL;
 *     lista = lista_inserir(lista, a);
 *     lista = lista_inserir(lista, b);
 *     lista = lista_inserir(lista, c);
 *     printf("tamanho = %d\n", lista_tamanho(lista));  // 3
 *     lista_liberar(lista, free);
 *
 *   Compile com -fsanitize=address e veja que não há vazamento.
 *
 * ATENÇÃO
 *   - O dado em si NÃO é copiado. A lista guarda apenas
 *     um PONTEIRO. Se você passar &x e depois x sair de
 *     escopo, vira lixo.
 *   - Recomendado: sempre alocar o dado com malloc antes
 *     de fazer inserir. Assim ele vive enquanto a lista viver.
 *   - Lembre-se: o ponteiro PARA FUNÇÃO recebido em buscar e
 *     liberar é chamado com (). Ex.: comparador(x), não
 *     &comparador(x).
 * ============================================================ */

/* TODO: includes (linked_list.h e stdlib.h) */


/* TODO: implemente lista_inserir */


/* TODO: implemente lista_remover */


/* TODO: implemente lista_buscar */


/* TODO: implemente lista_tamanho */


/* TODO: implemente lista_liberar */
