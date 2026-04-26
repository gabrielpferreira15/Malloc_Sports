/* ============================================================
 * scores.c — IMPLEMENTAÇÃO leitura/escrita do scores.dat
 * ------------------------------------------------------------
 * Dono: DEV 2 — Matheus Assis
 *
 * IDEIA: arquivo BINÁRIO simples
 *   Você vai gravar só os campos que importam: nome (16 bytes)
 *   e pontos (int). Sem o ponteiro 'próximo' — ele é recriado
 *   na leitura.
 *
 *   FORMATO sugerido do scores.dat:
 *     [int  N]                       // quantos scores há
 *     [Score 1: nome (16) + int pontos]
 *     [Score 2: ...]
 *     ...
 *
 *   POR QUE binário e não texto? Por 2 motivos:
 *     1) É menor (int é 4 bytes; em texto pode ocupar 10).
 *     2) Cobre o requisito "fwrite/fread" que o GDD pediu.
 *
 * NOMENCLATURA
 *   - arquivo : caminho do scores.dat
 *   - cabeça  : ponteiro pro primeiro Score da lista
 *   - n       : quantos scores (lidos do arquivo)
 *   - novo    : Score recém criado pra inserir
 *
 * O QUE FAZER (passo a passo)
 *
 *   --- scores_carregar(arquivo) ---
 *
 *   1. FILE *f = fopen(arquivo, "rb");
 *      // "rb" = read binary. Sem o b, em Windows a leitura
 *      // converte \r\n e quebra dados binários.
 *
 *   2. if (f == NULL) return NULL;
 *      // arquivo ainda não existe — é a primeira vez que
 *      // rodam o jogo. Comportamento esperado.
 *
 *   3. int n;
 *      fread(&n, sizeof(int), 1, f);
 *      // lê quantos scores estão no arquivo
 *
 *   4. Score *cabeça = NULL;
 *      for (int i = 0; i < n; i++) {
 *          Score *novo = malloc(sizeof(Score));
 *          fread(novo->nome, 16, 1, f);
 *          fread(&novo->pontos, sizeof(int), 1, f);
 *          cabeça = scores_inserir_no(cabeça, novo);
 *          // inserir ordenado, ou se você gravou já em ordem
 *          // basta inserir no começo
 *      }
 *
 *   5. fclose(f);
 *
 *   6. return cabeça;
 *
 *   --- scores_salvar(arquivo, cabeça) ---
 *
 *   1. FILE *f = fopen(arquivo, "wb");
 *   2. if (f == NULL) return;
 *
 *   3. int n = 0;
 *      Score *atual = cabeça;
 *      while (atual != NULL) { n++; atual = atual->próximo; }
 *      // contou quantos scores tem na lista
 *
 *   4. fwrite(&n, sizeof(int), 1, f);
 *
 *   5. atual = cabeça;
 *      while (atual != NULL) {
 *          fwrite(atual->nome, 16, 1, f);
 *          fwrite(&atual->pontos, sizeof(int), 1, f);
 *          atual = atual->próximo;
 *      }
 *
 *   6. fclose(f);
 *
 *   --- scores_inserir(cabeça, nome, pontos) ---
 *
 *   1. Score *novo = malloc(sizeof(Score));
 *      strncpy(novo->nome, nome, 15);
 *      novo->nome[15] = '\0';   // garante o terminador
 *      novo->pontos = pontos;
 *      novo->próximo = NULL;
 *
 *   2. // Caso 1: lista vazia OU novo bate o primeiro
 *      if (cabeça == NULL || pontos > cabeça->pontos) {
 *          novo->próximo = cabeça;
 *          cabeça = novo;
 *      } else {
 *          // Caso 2: andar pela lista achando o ponto certo
 *          Score *atual = cabeça;
 *          while (atual->próximo != NULL &&
 *                 atual->próximo->pontos >= pontos) {
 *              atual = atual->próximo;
 *          }
 *          novo->próximo = atual->próximo;
 *          atual->próximo = novo;
 *      }
 *
 *   3. // Cortar pra ficar com no máximo 10 scores
 *      Score *atual = cabeça;
 *      for (int i = 0; i < 9 && atual != NULL; i++)
 *          atual = atual->próximo;
 *      if (atual != NULL && atual->próximo != NULL) {
 *          // libera tudo a partir de atual->próximo
 *          Score *resto = atual->próximo;
 *          atual->próximo = NULL;
 *          scores_liberar(resto);
 *      }
 *
 *   4. return cabeça;
 *
 *   --- scores_liberar(cabeça) ---
 *
 *   1. while (cabeça != NULL) {
 *          Score *prox = cabeça->próximo;
 *          free(cabeça);
 *          cabeça = prox;
 *      }
 *
 * COMO TESTAR
 *   1. scores_inserir 3 nomes fictícios, scores_salvar.
 *   2. Reinicie o programa, scores_carregar, percorra e
 *      imprima. Se aparecer os 3 na ordem certa, OK.
 *   3. Inspecione scores.dat com `xxd scores.dat | head` —
 *      você deve ver os nomes legíveis no meio dos bytes.
 *
 * ATENÇÃO (erros comuns)
 *   - "rb" e "wb" (binário), não "r"/"w" — em Windows o "r"
 *     converte \r\n em \n e quebra dados binários.
 *   - strncpy não garante terminador nulo se o nome tem 16+
 *     caracteres. Sempre force novo->nome[15] = '\0'.
 *   - fopen pode retornar NULL — sempre cheque.
 *   - fread/fwrite devolve quantos elementos foram lidos/escritos.
 *     Em código de produção, conferir esse retorno seria boa
 *     prática. Para esse projeto, basta ter os checks de fopen.
 * ============================================================ */

/* TODO: includes (scores.h, stdio.h, stdlib.h, string.h) */


/* TODO: implemente scores_carregar */


/* TODO: implemente scores_salvar */


/* TODO: implemente scores_inserir */


/* TODO: implemente scores_liberar */
