/* ============================================================
 * corrida_ponteiros.c — IMPLEMENTAÇÃO do Minigame 1 (Corrida de Ponteiros)
 * ------------------------------------------------------------
 * Dono: DEV 1 — Gabriel Peixoto
 *
 * IDEIA GERAL
 *   Este é o seu minigame. Ele combina QUASE TODOS os
 *   conceitos da disciplina de uma só vez:
 *     - Structs (Jogador, Obstáculo)
 *     - Ponteiros (passados por todo lado)
 *     - Lista encadeada (pra obstáculos)
 *     - malloc/free (criar e destruir obstáculos)
 *     - Animação + movimento (todos os frames)
 *     - Colisão AABB (atleta x obstáculo)
 *
 * NOMENCLATURA (variaveis em português)
 *   - jogador1, jogador2     : ponteiros pra struct Jogador
 *   - obstáculos             : cabeça da lista encadeada
 *   - tempo_restante         : segundos até o fim
 *   - velocidade_base        : px por segundo (sobe com o tempo)
 *   - timer_proximo_obstaculo: contador pra spawnar obstáculo
 *   - delta_t                : segundos desde o último frame
 *
 * ESTRUTURA DO LOOP DO MINIGAME (passo a passo)
 *
 *   PASSO 1: SETUP (executa 1 vez)
 *     a) Crie 2 Jogadores com criar_jogador.
 *          jogador1 com tecla_cima = KEY_W
 *          jogador2 com tecla_cima = KEY_UP
 *     b) No *obstáculos = NULL;  (lista vazia)
 *     c) float tempo_restante = 60.0f;
 *        float velocidade_base = 200.0f;   // px/seg
 *        float timer_proximo_obstaculo = 0.0f;
 *
 *     EXEMPLO de criação:
 *       jogador1 = criar_jogador(100, 400, KEY_W, 0, 0, 0);
 *       jogador2 = criar_jogador(100, 500, KEY_UP, 0, 0, 0);
 *
 *   PASSO 2: LOOP (roda até o jogo acabar)
 *     while (tempo_restante > 0 && !ninguem_chegou_na_meta) {
 *
 *       float delta_t = GetFrameTime();
 *           // segundos desde o último frame, ex.: 0.0166 (60fps)
 *
 *       // ----- INPUT -----
 *       if (IsKeyPressed(jogador1->tecla_cima) && jogador1_no_chao)
 *           pular(jogador1);
 *       Idem para jogador2.
 *
 *       // ----- UPDATE -----
 *       a) Aplicar gravidade nos jogadores
 *            jogador->vy += GRAVIDADE * delta_t;
 *            jogador->y  += jogador->vy * delta_t;
 *            (precisa adicionar campo vy no Jogador, ou tratar
 *             local mesmo)
 *
 *       b) Mover obstáculos pra esquerda
 *            Para cada no da lista:
 *              Obstáculo *o = (Obstáculo *) no->dado;
 *              o->x -= o->velocidade * delta_t;
 *
 *       c) Spawnar novo obstáculo
 *            timer_proximo_obstaculo -= delta_t;
 *            if (timer_proximo_obstaculo <= 0) {
 *                Obstáculo *novo = criar_obstaculo(
 *                    LARGURA_TELA, ALTURA_PISTA,
 *                    32, 32, velocidade_base);
 *                obstáculos = lista_inserir(obstáculos, novo);
 *                timer_proximo_obstaculo = GetRandomValue(1, 3);
 *            }
 *
 *       d) Aumentar dificuldade (a cada 10 segundos)
 *            velocidade_base += 5.0f * delta_t;
 *
 *       e) Detectar colisões
 *            Percorra a lista. Para cada obstáculo, teste com
 *            colide_aabb(jogador1->x, jogador1->y, ..., o->x, ...).
 *            Se colidiu: jogador->pontos -= 1; animacao_tropeco;
 *
 *       f) Remover obstáculos que saíram da tela
 *            Use lista_buscar com um comparador que retorna true
 *            se o->x < -50. Remova-os e libere com destruir_obstaculo.
 *
 *       g) Atualizar timer
 *            tempo_restante -= delta_t;
 *
 *       // ----- DRAW -----
 *       Desenhe o cenário (DrawRectangle pro chão).
 *       Desenhe os 2 atletas (DrawRectangle ou DrawTexture).
 *       Percorra a lista e desenhe cada obstáculo.
 *       HUD: DrawText do tempo, do pontos_p1, do pontos_p2.
 *     }
 *
 *   PASSO 3: TEARDOWN (limpeza)
 *     a) lista_liberar(obstáculos, destruir_obstaculo);
 *     b) destruir_jogador(jogador1);
 *        destruir_jogador(jogador2);
 *     c) Decida vencedor:
 *          if (jogador1->pontos > jogador2->pontos) return 1;
 *          else if (jogador2->pontos > jogador1->pontos) return 2;
 *          else return 0;
 *        (mas atenção: você já deu free dos jogadores acima!
 *         calcule o vencedor ANTES de liberar.)
 *
 * REQUISITOS COBERTOS
 *   - Listas encadeadas (lista de obstáculos)
 *   - malloc/free
 *   - Colisão
 *   - Animação + movimento
 *   - Structs e ponteiros em todo lado
 *
 * COMO TESTAR (em ordem)
 *   1. Faça aparecer 2 retângulos coloridos representando
 *      os atletas. Mover só com W e SETA CIMA. Só pular.
 *   2. Faça 1 obstáculo aparecer e mover pra esquerda.
 *      Não precisa lista ainda.
 *   3. Troque pelo geração via lista_inserir em intervalos.
 *   4. Adicione colisão e pontos.
 *   5. Adicione timer e condição de vitória.
 *
 * ATENÇÃO
 *   - Use GetFrameTime() pra ter o delta_t. Não hardcode
 *     velocidades em "pixels por frame", senao mudando o FPS
 *     o jogo fica diferente.
 *   - Cuidado pra liberar TODOS os obstáculos no fim, mesmo
 *     os que ainda estão na tela quando o jogo acaba.
 *   - Use as funções do Raylib: DrawRectangle, DrawText,
 *     IsKeyDown, IsKeyPressed, GetFrameTime, GetRandomValue.
 * ============================================================ */

/* TODO: includes (raylib.h, corrida_ponteiros.h, entities.h, physics.h,
 *       linked_list.h, stdlib.h) */


/* TODO: implemente jogar_corrida_ponteiros seguindo o roteiro acima */
