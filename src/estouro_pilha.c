/* ============================================================
 * estouro_pilha.c — IMPLEMENTAÇÃO do Minigame 3 (Estouro de Pilha)
 * ------------------------------------------------------------
 * Dono: DEV 3 — Gabriel Mendes
 *
 * IDEIA DA REPRESENTACAO
 *   Cada jogador tem uma matriz int grid[20][10].
 *     0 = célula vazia
 *     1..7 = id do bloco que ocupa aquela célula (cor)
 *
 *   O bloco que está CAINDO NÃO é gravado no grid ainda —
 *   só a posição + formato do bloco atual são mantidos em
 *   variaveis a parte. Quando ele "trava" (toca o chão ou
 *   outro bloco), copiamos as células dele pro grid.
 *
 *   ANALOGIA: o grid é como uma planilha do Excel onde você
 *   colore as células conforme as peças vão caindo.
 *
 * NOMENCLATURA
 *   - grid_p1[20][10], grid_p2[20][10] : matrizes do estado fixo
 *   - bloco_atual_p1, bloco_atual_p2  : Bloco caindo agora
 *   - proximo_bloco_p1, proximo_bloco_p2: pra mostrar no HUD
 *   - timer_queda                      : contador da queda automática
 *   - intervalo_queda                  : a cada quantos seg cai 1 linha
 *   - delta_t                          : segundos desde o último frame
 *
 * O QUE FAZER (passo a passo)
 *
 *   PASSO 1: SETUP
 *
 *   1. int grid_p1[GRID_ALTURA][GRID_LARGURA] = {0};
 *      int grid_p2[GRID_ALTURA][GRID_LARGURA] = {0};
 *      // {0} zera tudo automaticamente
 *
 *   2. Defina os 7 formatos de Tetris numa matriz constante
 *      (procure os 7 tetrominoes I, O, T, L, J, S, Z na internet
 *      e represente cada um como uma matriz 4x4 de 1s e 0s).
 *
 *      const int FORMATOS[7][4][4] = { ... };
 *
 *   3. Bloco *bloco_atual_p1 = sortear_bloco();
 *      Bloco *bloco_atual_p2 = sortear_bloco();
 *      // sortear_bloco é uma função auxiliar que você escreve:
 *      // escolhe um formato aleatório com GetRandomValue(0, 6),
 *      // copia o formato pra um Bloco novo, e devolve.
 *
 *   4. int pontos_p1 = 0, pontos_p2 = 0;
 *      float timer_queda_p1 = 0, timer_queda_p2 = 0;
 *      float intervalo_queda = 0.5f;  // 1 linha a cada 0.5s
 *
 *   PASSO 2: LOOP (até alguém sofrer stack overflow)
 *
 *   while (!perdeu_p1 && !perdeu_p2) {
 *
 *       float delta_t = GetFrameTime();
 *
 *       // ----- INPUT P1: A/D mover, W rotacionar, S queda rápida -----
 *       if (IsKeyPressed(KEY_A)) tentar_mover(bloco_atual_p1, grid_p1, -1, 0);
 *       if (IsKeyPressed(KEY_D)) tentar_mover(bloco_atual_p1, grid_p1, +1, 0);
 *       if (IsKeyPressed(KEY_W)) tentar_rotacionar(bloco_atual_p1, grid_p1);
 *       if (IsKeyDown(KEY_S))    intervalo_queda_p1 = 0.05f;
 *
 *       // ----- INPUT P2: SETAS -----
 *       Idem mas com KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN.
 *
 *       // ----- UPDATE -----
 *
 *       timer_queda_p1 += delta_t;
 *       if (timer_queda_p1 >= intervalo_queda) {
 *           // Tenta descer 1 linha
 *           bloco_atual_p1->y += 1;
 *
 *           if (colide_com_grid(bloco_atual_p1, grid_p1)) {
 *               // VOLTOU 1 linha porque colidiu
 *               bloco_atual_p1->y -= 1;
 *
 *               // Copia o bloco pro grid (TRAVOU)
 *               for (int i = 0; i < 4; i++)
 *                 for (int j = 0; j < 4; j++)
 *                   if (bloco_atual_p1->formato[i][j])
 *                     grid_p1[bloco_atual_p1->y + i]
 *                            [bloco_atual_p1->x + j] =
 *                            bloco_atual_p1->cor;
 *
 *               // Cheque linhas completas e remova
 *               int linhas_limpas = limpar_linhas(grid_p1);
 *               pontos_p1 += linhas_limpas * 100;
 *
 *               // Se 2+, manda lixo pro adversário
 *               if (linhas_limpas >= 2)
 *                   enviar_linha_lixo(grid_p2);
 *
 *               // Sorteia novo bloco
 *               destruir_bloco(bloco_atual_p1);
 *               bloco_atual_p1 = sortear_bloco();
 *
 *               // Se já nasce sobre algo no grid -> game over
 *               if (colide_com_grid(bloco_atual_p1, grid_p1))
 *                   perdeu_p1 = true;
 *           }
 *           timer_queda_p1 = 0;
 *       }
 *       Idem para o jogador 2.
 *
 *       // ----- DRAW -----
 *       BeginDrawing();
 *       ClearBackground(BLACK);
 *       desenhar_grid(grid_p1, 100, 100);
 *       desenhar_grid(grid_p2, 600, 100);
 *       desenhar_bloco(bloco_atual_p1, 100, 100);
 *       desenhar_bloco(bloco_atual_p2, 600, 100);
 *       // HUD com pontos
 *       EndDrawing();
 *   }
 *
 * FUNCOES AUXILIARES sugeridas (escreva você mesmo)
 *
 *   bool colide_com_grid(Bloco *b, int grid[GRID_ALTURA][GRID_LARGURA])
 *       Para cada célula (i,j) do formato 4x4 que vale 1:
 *         linha_grid = b->y + i;
 *         coluna_grid = b->x + j;
 *         se linha_grid >= GRID_ALTURA -> colide (chão)
 *         se coluna_grid < 0 ou >= GRID_LARGURA -> colide (parede)
 *         se grid[linha_grid][coluna_grid] != 0 -> colide (outro bloco)
 *       Devolve true se algum colide.
 *
 *   void tentar_mover(Bloco *b, int grid[][], int dx, int dy)
 *       1. Salva pos antiga.
 *       2. b->x += dx; b->y += dy;
 *       3. Se colide_com_grid, restaura pos antiga.
 *
 *   void tentar_rotacionar(Bloco *b, int grid[][])
 *       Rotação 90 graus de matriz 4x4:
 *         novo[i][j] = velho[3-j][i]
 *       1. Faz a rotação numa matriz temporaria.
 *       2. Troca o formato do bloco pela rotacionada.
 *       3. Se colide, desfaz (volta pra matriz original).
 *
 *   int limpar_linhas(int grid[][])
 *       1. Para cada linha de baixo pra cima:
 *            Se TODAS as 10 células != 0:
 *              Move tudo acima dela 1 linha pra baixo.
 *              Zera a linha 0.
 *              contador++;
 *       2. return contador.
 *
 * REQUISITOS COBERTOS
 *   - Matriz 2D (REQUISITO CHAVE — grid e formato)
 *   - Structs (Bloco)
 *   - Animação + movimento (queda)
 *   - Colisão
 *
 * COMO TESTAR (em ordem)
 *   1. Desenhe um grid vazio (só as bordas).
 *   2. Mova um bloco quadrado com setas, sem física.
 *   3. Adicione queda automática.
 *   4. Adicione travamento (copiar bloco pro grid).
 *   5. Adicione detecção de linha completa.
 *   6. Adicione os 7 formatos + rotação.
 *   7. Adicione o segundo jogador.
 *   8. Adicione "linha de lixo" como ataque.
 *
 * ATENÇÃO
 *   - SEMPRE cheque os índices da matriz pra não acessar fora
 *     dos limites. Em C, isso não da erro de compilação —
 *     corrompe memória e o jogo crasha de formas estranhas.
 *   - O bloco em ROTACAO pode "invadir" a parede. Se invadir,
 *     desfaça a rotação (testa antes de aplicar).
 *   - Em jogos de 2 players no mesmo teclado, IsKeyPressed()
 *     funciona pros 2 ao mesmo tempo (teclado moderno suporta
 *     "n-key rollover" — várias teclas simultâneas).
 * ============================================================ */

/* TODO: includes + implementação */
