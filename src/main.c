/* ============================================================
 * main.c — PONTO DE ENTRADA do jogo + loop principal
 * ------------------------------------------------------------
 * Dono: DEV 1 — Gabriel Peixoto (Nucleo do motor)
 *
 * IDEIA GERAL
 *   Este arquivo é o "coração" do jogo. Ele:
 *     1) Abre a janela com a Raylib;
 *     2) Mantém um loop que roda 60 vezes por segundo;
 *     3) A cada frame, decide QUAL TELA mostrar (splash, menu,
 *        Corrida de Ponteiros, Pong da Memória, Estouro de Pilha, resultado, ranking...);
 *     4) Fecha a janela e libera memória quando o jogo termina.
 *
 *   ANALOGIA: pense num controle remoto de TV. Você não
 *   constrói as cenas; você só escolhe qual canal exibir.
 *   A função main faz exatamente isso com as cenas do jogo.
 *
 * NOMENCLATURA (português)
 *   - cena_atual   : qual tela está sendo exibida agora
 *   - estado_torn  : estado do torneio (placar acumulado)
 *   - lista_scores : cabeça da lista encadeada de ranking
 *
 *   OBS.: o GDD escreveu alguns campos em inglês (score, lives,
 *   key_up...). Aqui usamos português nos exemplos. Use o que
 *   fizer mais sentido pra vocês, MAS seja CONSISTENTE em todo
 *   o projeto. Misturar inglês e português no mesmo arquivo
 *   confunde quem lê.
 *
 * O QUE ESTE ARQUIVO PRECISA TER (passo a passo)
 *
 *   PASSO 1: Inclua os headers
 *     #include <raylib.h>     (janela, desenho, input)
 *     #include "menu.h"       (telas de UI)
 *     #include "corrida_ponteiros.h"     (minigame 1)
 *     #include "pong_memoria.h"       (minigame 2)
 *     #include "estouro_pilha.h" (minigame 3)
 *     #include "tournament.h" (estado do torneio)
 *     #include "scores.h"     (ranking)
 *
 *   PASSO 2: Defina o enum Cena
 *     POR QUE? Em vez de usar números mágicos (0=splash,
 *     1=menu...) que confundem, usamos nomes que se LEEM
 *     como português. O compilador atribui números sozinho.
 *
 *     Exemplo de declaração:
 *       typedef enum {
 *           CENA_SPLASH,
 *           CENA_MENU,
 *           CENA_SELECAO,
 *           CENA_CORRIDA_PONTEIROS,
 *           CENA_PONG,
 *           CENA_ESTOURO_PILHA,
 *           CENA_RESULTADO,
 *           CENA_CAMPEONATO,
 *           CENA_RANKING,
 *           CENA_SAIR
 *       } Cena;
 *
 *   PASSO 3: A função main() — preparação
 *     a) const int LARGURA = 1280;  // largura da janela em pixels
 *        const int ALTURA  = 720;   // altura da janela em pixels
 *     b) InitWindow(LARGURA, ALTURA, "Malloc Sports");
 *     c) SetTargetFPS(60);          // limita o jogo a 60fps
 *     d) Cena cena_atual = CENA_SPLASH;
 *     e) EstadoTorneio estado_torn;
 *        iniciar_torneio(&estado_torn);
 *     f) Score *lista_scores = scores_carregar("scores.dat");
 *        // pode retornar NULL se o arquivo ainda não existe
 *
 *   PASSO 4: O LOOP PRINCIPAL
 *     while (!WindowShouldClose() && cena_atual != CENA_SAIR) {
 *         BeginDrawing();
 *         ClearBackground(BLACK);
 *
 *         switch (cena_atual) {
 *             case CENA_SPLASH:
 *                 cena_atual = tela_splash();
 *                 break;
 *             case CENA_MENU:
 *                 cena_atual = tela_menu_principal();
 *                 break;
 *             case CENA_CORRIDA_PONTEIROS:
 *                 cena_atual = jogar_corrida_ponteiros(...);
 *                 break;
 *             // ... e assim para todas as cenas ...
 *         }
 *
 *         EndDrawing();
 *     }
 *
 *     OBS.: WindowShouldClose() retorna true quando o usuário
 *     fecha a janela (X) ou aperta ESC. Isso garante que a gente
 *     sempre saia do loop limpamente.
 *
 *   PASSO 5: Limpeza (em ordem inversa da criação)
 *     a) scores_salvar("scores.dat", lista_scores);
 *     b) scores_liberar(lista_scores);   // free na lista toda
 *     c) CloseWindow();
 *     d) return 0;
 *
 * REQUISITOS DA DISCIPLINA QUE ESTE ARQUIVO COBRE
 *   - Ponteiros (passamos &estado_torn pras funções)
 *   - Structs globais
 *   - Loop principal (animação + movimento)
 *   - Arquivo E/S (delegado a scores.c, mas chamado daqui)
 *
 * COMO TESTAR (em ordem, sem pular etapa)
 *   1. Faça um main MÍNIMO: só abre janela, escreve o título
 *      e fecha com ESC. Quando isso compilar e rodar, vá para o
 *      passo 2.
 *   2. Adicione UMA cena (ex.: CENA_SPLASH) com transição
 *      automática pra CENA_MENU após 2 segundos.
 *   3. Adicione o menu, depois a seleção, depois cada minigame.
 *   4. Por último, integre o torneio e o ranking.
 *
 *   Adicionando uma cena de cada vez você não quebra TUDO de
 *   uma vez. Se quebrar, você sabe que foi a cena que adicionou.
 *
 * ATENÇÃO (erros comuns)
 *   - NUNCA chame BeginDrawing() dentro de outro BeginDrawing().
 *     Só pode ter UM por frame.
 *   - NÃO chame InitWindow várias vezes. UMA só.
 *   - Lembre-se de liberar TUDO o que você alocou com malloc.
 *     No Linux, teste com `valgrind ./malloc_sports`.
 *   - O enum Cena facilita muito a leitura. NÃO use números
 *     mágicos (0, 1, 2...) para identificar cenas.
 * ============================================================ */

/* TODO: inclua os headers aqui */


/* TODO: defina o enum Cena aqui */


/* TODO: implemente a função main aqui, seguindo os passos acima.
 * Escreva você mesmo cada linha — você precisa explicar tudo
 * isso na apresentação oral. */
