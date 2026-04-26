/* ============================================================
 * linked_list.h — LISTA ENCADEADA GENÉRICA
 * ------------------------------------------------------------
 * Dono: DEV 1 — Gabriel Peixoto
 *
 * IDEIA GERAL
 *   Uma LISTA ENCADEADA é uma sequência de "nós" onde cada
 *   nó aponta pro próximo. Diferente de um array, ela cresce
 *   sem precisar saber o tamanho de antemão.
 *
 *     [no1| dado | próximo] -> [no2| dado | próximo] -> NULL
 *
 *   "Generica" significa que ela pode guardar QUALQUER tipo
 *   de dado, porque cada nó tem um campo void* (ponteiro pra
 *   tipo desconhecido). Quem usa a lista faz o cast pro tipo
 *   correto.
 *
 *   ANALOGIA: pense numa "caça ao tesouro" — cada papelzinho
 *   tem uma pista (o dado) e um endereço do próximo papelzinho
 *   (o ponteiro). O último aponta pra "fim" (NULL).
 *
 *   Onde a gente usa?
 *     - Corrida de Ponteiros: lista de Obstáculo (gera novos sob demanda,
 *       remove os que saíram da tela);
 *     - Scores: lista de Score, ordenada por pontos.
 *
 * NOMENCLATURA
 *   - cabeça  : ponteiro pro PRIMEIRO nó da lista (ou NULL se vazia)
 *   - nó      : um elemento da lista (struct No)
 *   - dado    : o conteúdo guardado pelo nó (void *, tipo genérico)
 *   - próximo : ponteiro pro próximo nó (ou NULL se é o último)
 *
 * O QUE ESTE ARQUIVO PRECISA TER (passo a passo)
 *
 *   PASSO 1: Header guard
 *     #ifndef LINKED_LIST_H
 *     #define LINKED_LIST_H
 *
 *   PASSO 2: Inclua <stdbool.h> (vai precisar de bool nos comparadores)
 *
 *   PASSO 3: Defina a struct No
 *
 *     typedef struct No {
 *         void *dado;          // ponteiro pro conteúdo (tipo genérico)
 *         struct No *próximo;  // ponteiro pro próximo (ou NULL)
 *     } No;
 *
 *     POR QUE void*? Porque assim a mesma lista serve pra guardar
 *     Obstáculo, Score, ou qualquer outro tipo. Quem usa a lista
 *     faz o cast: Obstáculo *o = (Obstáculo *)no->dado;
 *
 *   PASSO 4: Declare os protótipos das operações
 *
 *     No *lista_inserir(No *cabeça, void *dado);
 *         // adiciona no começo da lista; devolve a NOVA cabeça
 *         // EXEMPLO de uso:
 *         //   No *lista = NULL;
 *         //   lista = lista_inserir(lista, criar_obstaculo(...));
 *
 *     No *lista_remover(No *cabeça, void **dado_saida);
 *         // remove o primeiro nó
 *         // se dado_saida não for NULL, coloca o dado removido lá
 *         // devolve a nova cabeça
 *
 *     No *lista_buscar(No *cabeça,
 *                      bool (*comparador)(void *));
 *         // percorre a lista e devolve o nó que satisfaz o
 *         // comparador (ex.: "achar obstáculo com x < 0")
 *         // POR QUE ponteiro pra função? Para deixar a busca
 *         // genérica — quem chama decide o critério.
 *
 *     int lista_tamanho(No *cabeça);
 *         // conta quantos nós há na lista
 *
 *     void lista_liberar(No *cabeça,
 *                        void (*destrutor)(void *));
 *         // libera TODA a lista
 *         // chama destrutor em cada dado antes do free do nó
 *         // se destrutor for NULL, não libera o dado (só o nó)
 *
 *   PASSO 5: Feche com #endif
 *
 * REQUISITOS DA DISCIPLINA QUE ESTE ARQUIVO COBRE
 *   - Listas encadeadas (REQUISITO CHAVE da disciplina)
 *   - Ponteiros (incluindo void* e ponteiros pra função)
 *   - Alocação dinâmica
 *
 * ATENÇÃO
 *   - void* é poderoso mas perigoso: o compilador não confere
 *     o tipo. Quem usa a lista precisa lembrar o tipo certo.
 *   - lista_liberar recebe um PONTEIRO PARA FUNÇÃO porque cada
 *     tipo de dado pode ter sua própria forma de ser liberado.
 *     Para Obstáculo, basta passar destruir_obstaculo. Para
 *     tipos sem nada complicado dentro, basta passar free.
 * ============================================================ */

/* TODO: header guard, struct No, protótipos, #endif */
