# 🏆 Malloc Sports

**Malloc Sports** é um título composto por múltiplos minigames de competição esportiva, desenvolvido com foco na experiência multiplayer local e inspirado no clássico "Wii Sports". 

---

## 🔧 **Ferramentas Utilizadas**

<details>
<summary><strong> Ver ferramentas </strong></summary>

**Linguagem:**

* C 

**Biblioteca Gráfica:**

* Raylib

**IDE:**

* VS Code

**Ambiente:**

* Linux (WSL)
* macOS

**Controle de Versão:**

* GitHub

</details>



## 👥 Equipe

<details>
<summary><strong> Ver Equipe </strong></summary>

- **Gabriel Peixoto e Silva Ferreira**
- **Matheus Assis de Souza Jácome**
- **Gabriel Mendes Cavalcanti**

</details>

## 🎮 Sobre o Jogo

<details>
<summary><strong> Ver Detalhes do Jogo  </strong></summary>

### Modos de jogo:
- **Modo Torneio:** Os jogadores participam de todos os 3 esportes em sequência onde o desempenho em cada minigame rende pontos acumulativos. Ao final, o grande campeão é coroado e as pontuações dos jogadores podem entrar para a lista de *Highscores* caso consigam ficar entre as 10 melhores!

- **Minigame Único:** Os jogadores escolhem apenas um dos jogos para um desafio rápido e direto, com suporte a revanches imediatas.

### Minigames e Controles

O teclado é dividido para que dois jogadores compitam simultaneamente no mesmo computador.

#### 1. Corrida com obstáculos
Uma disputa de velocidade e tempo de reação para saltar obstáculos e ser o primeiro a chegar ao final.
- **Jogador 1:** `A` e `D` alternados (Correr), `W` (Pular)
- **Jogador 2:** `<` e `>` alternados (Correr), `^` Seta Cima (Pular)

#### 2. Ping-Pong
O clássico refeito! Conta com mecânicas de aceleração da bola, "boosts" e limite de duração de 60 segundos. 
- **Jogador 1:** `W` / `S` (Mover raquete)
- **Jogador 2:** `Seta Cima` / `Seta Baixo` (Mover raquete)

#### 3. Vôlei de Praia
Uma partida intensa de 60 segundos aplicando física de gravidade, pulo e mecânicas de toque na bola.
- **Jogador 1 & 2:** Movimentação clássica direcional e botões de pulo aplicados às mecânicas de colisão 2D com a bola e a rede.

### Funcionalidades e Estrutura Técnica

O código do projeto reflete um design modular e a aplicação prática de conceitos de programação estruturada:

- **Arquitetura de Cenas:** Um gerenciador central no `main.c` utiliza uma *State Machine* (Máquina de Estados) definida em `cenas.h` para transitar suavemente entre Menus, Seleção de Modos, Minigames e Telas de Resultado.

- **Sistema de Torneio (`tournament.h/c`):** Uma estrutura robusta que gerencia a fila de minigames, mantém o estado dos placares, acumula as pontuações em background e declara o vencedor final.

- **Persistência e Listas Encadeadas (`scores.h/c`):** O sistema de *Highscores* utiliza **Listas Encadeadas** dinâmicas para ordenar as pontuações do torneio. O TOP 10 é salvo de forma persistente no arquivo `highscores.txt`.

- **Física Simples e Colisão:** Implementação de gravidade, detecção de colisão AABB (Axis-Aligned Bounding Box) e vetores de velocidade.

</details>

## 🛠️ Guia de Compilação e Execução 

<details>
<summary><strong> Ver Guia </strong></summary>

Este documento explica como configurar o ambiente, compilar e executar o **Malloc Sports** em diferentes sistemas operacionais (**Windows, Linux e macOS**), utilizando o `Makefile` incluso no projeto.

### Estrutura de Pastas Esperada

Para que a compilação funcione corretamente, certifique-se de que a estrutura do seu projeto esteja organizada da seguinte forma:
```text
📂 Malloc Sports/
├── 📂 Include/          # Todos os arquivos de cabeçalho (.h)
├── 📂 src/              # Todos os arquivos de código-fonte (.c)
├── 📄 Makefile          # Arquivo de automação da compilação
└── 📄 highscores.txt    # Arquivo de persistência (gerado automaticamente)
```

---

### 1. Compilação no Windows (Via PowerShell)

O ambiente recomendado para Windows é o **MSYS2 (UCRT64)** combinado com a biblioteca **Raylib**.

#### Passo 1.1: Instalar Dependências (MSYS2 e Raylib)
1. Baixe e instale o [MSYS2](https://www.msys2.org/).
2. Abra o terminal **MSYS2 UCRT64** e instale o compilador `gcc`, a ferramenta `make` e a biblioteca `raylib` rodando o comando:
   ```bash
   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make mingw-w64-ucrt-x86_64-raylib
   ```

#### Passo 1.2: Compilar o Jogo
1. Abra o **PowerShell** ou **Prompt de Comando (CMD)**.
2. Navegue até a pasta raiz do projeto:
   ```powershell
   cd "caminho\para\o\seu\projeto"
   ```
3. Execute o comando do Make para Windows:
   ```powershell
   mingw32-make
   ```

#### Passo 1.3: Executar
Após o término, um arquivo executável será gerado. Para jogar, basta rodar:
```powershell
.\malloc_sports.exe
```

---

### 2. Compilação no Linux (Ubuntu, Debian, Fedora, etc.)

No Linux, usamos o gerenciador de pacotes nativo para instalar o compilador e a Raylib.

#### Passo 2.1: Instalar Dependências
Abra o terminal e instale as ferramentas essenciais e a Raylib. 

**No Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential libraylib-dev pkg-config
```

**No Fedora:**
```bash
sudo dnf install @development-tools raylib-devel pkg-config
```

#### Passo 2.2: Compilar o Jogo
1. Navegue até a pasta do projeto:
   ```bash
   cd caminho/para/o/seu/projeto
   ```
2. Compile o código rodando apenas:
   ```bash
   make
   ```

#### Passo 2.3: Executar
Rode o binário gerado:
```bash
./malloc_sports
```

---

### 3. Compilação no macOS

No Mac, usamos o gerenciador de pacotes **Homebrew** para obter as ferramentas necessárias.

#### Passo 3.1: Instalar Dependências
1. Certifique-se de ter as ferramentas de linha de comando do Xcode instaladas (`xcode-select --install`).

2. Instale a Raylib e o `pkg-config` via [Homebrew](https://brew.sh/):
   ```bash
   brew install raylib pkg-config
   ```

#### Passo 3.2: Compilar o Jogo
1. Navegue até a pasta do projeto pelo Terminal:
   ```bash
   cd caminho/para/o/seu/projeto
   ```
2. Compile o código:
   ```bash
   make
   ```

#### Passo 3.3: Executar
Rode o binário gerado:
```bash
./malloc_sports
```

---

### Limpando Arquivos Temporários (Todos os Sistemas)

Se você modificar a estrutura do código ou quiser recompilar o projeto do zero, é recomendável limpar os arquivos de objetos (`.o`) antigos criados durante a compilação anterior.

- **No Windows (PowerShell/CMD):**
  ```powershell
  mingw32-make clean
  ```

- **No Linux / macOS (Terminal):**
  ```bash
  make clean
  ```

  </details>