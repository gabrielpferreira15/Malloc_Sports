/* ============================================================
 * pong_memoria.c — IMPLEMENTAÇÃO do Minigame 2 (Pong da Memória)
 * ------------------------------------------------------------
 * Dono: DEV 2 — Matheus Assis
 *
 * NOMENCLATURA
 *   - raquete1, raquete2  : Jogador (usa o campo y como posição)
 *   - bola                : struct Bola (criada com criar_bola)
 *   - pontos_p1/pontos_p2 : placar do round atual
 *   - powerup_x, powerup_y: posição do free() flutuante
 *   - powerup_ativo       : bool, há um powerup na tela?
 *   - boost_restante      : segundos restantes do efeito do powerup
 *   - delta_t             : segundos desde o último frame
 *
 * ESTRUTURA DO LOOP (passo a passo)
 *
 *   --- SETUP (1 vez) ---
 *
 *   1. const int LARGURA = 1280;
 *      const int ALTURA  = 720;
 *      const int RAQUETE_W = 16;
 *      const int RAQUETE_H = 100;
 *      const float VELOCIDADE_RAQUETE = 400.0f;  // px/seg
 *      const int PONTOS_PRA_VENCER = 7;
 *
 *   2. Cria 2 Jogadores (raquetes):
 *        raquete1: x = 50,         y = ALTURA/2 - RAQUETE_H/2
 *                  tecla_cima = KEY_W, tecla_baixo = KEY_S
 *        raquete2: x = LARGURA-50, y = idem
 *                  tecla_cima = KEY_UP, tecla_baixo = KEY_DOWN
 *
 *   3. Cria 1 Bola no centro com velocidade inicial:
 *        bola = criar_bola(LARGURA/2, ALTURA/2, 300, 200);
 *
 *   4. int pontos_p1 = 0, pontos_p2 = 0;
 *      bool powerup_ativo = false;
 *      float boost_restante = 0;
 *
 *   --- LOOP (até alguém chegar a 7 pontos) ---
 *
 *   while (pontos_p1 < 7 && pontos_p2 < 7) {
 *
 *       float delta_t = GetFrameTime();
 *
 *       // ----- INPUT -----
 *       if (IsKeyDown(raquete1->tecla_cima))
 *           raquete1->y -= VELOCIDADE_RAQUETE * delta_t;
 *       if (IsKeyDown(raquete1->tecla_baixo))
 *           raquete1->y += VELOCIDADE_RAQUETE * delta_t;
 *       Idem para raquete2.
 *
 *       // Limita a raquete dentro da tela:
 *       raquete1->y = clamp(raquete1->y, 0, ALTURA - RAQUETE_H);
 *       raquete2->y = clamp(raquete2->y, 0, ALTURA - RAQUETE_H);
 *
 *       // ----- UPDATE -----
 *
 *       // 1) Mover a bola
 *       bola->x += bola->vel_x * delta_t;
 *       bola->y += bola->vel_y * delta_t;
 *
 *       // 2) Colisão com paredes superior/inferior
 *       if (bola->y - bola->raio < 0 ||
 *           bola->y + bola->raio > ALTURA)
 *           bola->vel_y *= -1;
 *
 *       // 3) Colisão com raquete (use colide_aabb)
 *       //    Se colide, inverte vel_x:
 *       //      bola->vel_x *= -1;
 *       //    E ajusta vel_y baseado em ONDE bateu na raquete:
 *       //      offset = (bola->y - centro_raquete) /
 *       //               (RAQUETE_H / 2);
 *       //      bola->vel_y = VELOCIDADE_MAX_Y * offset;
 *       //    Assim, acertar na ponta manda a bola "viajada".
 *
 *       // 4) Bola saiu pela direita ou esquerda?
 *       if (bola->x < 0) {
 *           pontos_p2++;
 *           // resetar bola pro centro:
 *           bola->x = LARGURA/2; bola->y = ALTURA/2;
 *           bola->vel_x = 300; bola->vel_y = 200;
 *           // disparar efeito visual de "leak" no P1
 *       }
 *       if (bola->x > LARGURA) { ... idem pra P2 ... }
 *
 *       // 5) Powerup free()
 *       //    A cada N segundos, com chance, spawna em pos
 *       //    aleatória: powerup_x = GetRandomValue(...);
 *       //    Se a bola colide com o powerup, dobra vel_x e
 *       //    vel_y, marca boost_restante = 3.0f
 *       //    Quando boost_restante zera, divide por 2 de volta.
 *
 *       // ----- DRAW -----
 *       BeginDrawing();
 *       ClearBackground(BLACK);
 *       DrawRectangle(raquete1->x, raquete1->y, RAQUETE_W, RAQUETE_H, WHITE);
 *       DrawRectangle(raquete2->x, raquete2->y, RAQUETE_W, RAQUETE_H, WHITE);
 *       DrawCircle(bola->x, bola->y, bola->raio, RAYWHITE);
 *       if (powerup_ativo)
 *           DrawText("free()", powerup_x, powerup_y, 20, GREEN);
 *       // HUD:
 *       DrawText(TextFormat("%d", pontos_p1), 100, 20, 40, WHITE);
 *       DrawText(TextFormat("%d", pontos_p2), LARGURA-150, 20, 40, WHITE);
 *       EndDrawing();
 *   }
 *
 *   --- FIM ---
 *   *pontos_saida_p1 = pontos_p1;
 *   *pontos_saida_p2 = pontos_p2;
 *   destruir_jogador(raquete1);
 *   destruir_jogador(raquete2);
 *   destruir_bola(bola);
 *   return (pontos_p1 > pontos_p2) ? 1 : 2;
 *
 * REQUISITOS COBERTOS
 *   - Structs (Bola, Jogador), ponteiros, animação, colisão.
 *
 * COMO TESTAR (em ordem)
 *   1. Só bola que se move e bate nas paredes (sem raquete).
 *   2. Adicione raquetes e movimento delas.
 *   3. Adicione colisão raquete-bola.
 *   4. Placar.
 *   5. Powerup.
 *
 * ATENÇÃO
 *   - Use GetFrameTime() pro delta_t. Não confie em "60 fps fixos".
 *   - Cuidado com "tunneling": se a bola estiver muito rápida e
 *     a raquete fina, ela pode pular por dentro sem detectar
 *     colisão. Solução simples: limitar a velocidade máxima.
 *   - Não se esqueça de DAR FREE em raquete1, raquete2 e bola
 *     no fim. Quem aloca, libera.
 * ============================================================ */

/* TODO: includes + implementação seguindo o roteiro */
