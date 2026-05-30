/* =================================================================
    cenas.h — Enum único de cenas, compartilhado por todo o projeto
  =============================================================== */

#ifndef CENAS_H
#define CENAS_H
typedef enum {
    CENA_MENU,
    CENA_SELECAO_MODO,
    CENA_SELECAO_MINIGAME,
    CENA_PONG,
    CENA_CORRIDA,
    CENA_VOLEI,
    CENA_RESULTADO,
    CENA_HIGHSCORES,
    CENA_SAIR
} Cena;

#endif