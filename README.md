# Malloc Sports

> "If it compiles, it ships."

Projeto da disciplina **Programação Imperativa e Funcional (PIF) — 2026.1 — CESAR School**.

Coletânea de minigames competitivos de 2 jogadores no mesmo teclado, em **C + Raylib**, com temática do universo do C (malloc, free, segfault, ponteiros).

## Equipe

- Gabriel Peixoto e Silva Ferreira — Núcleo do motor + Corrida de Ponteiros
- Matheus Assis de Souza Jacome — Sistemas de Jogo + Pong da Memória
- Gabriel Mendes Cavalcanti — Interface + Estouro de Pilha

---

## Instalação no WINDOWS (PowerShell, sem WSL)

A ideia é ter 2 coisas no PC:
1. **mingw-w64** — compilador `gcc` e `mingw32-make` para Windows.
2. **Raylib 5.x** — biblioteca gráfica.

A forma mais simples é usar o pacote oficial da Raylib para Windows, que **já vem com o mingw-w64 incluso**.

### Passo 1: baixar a Raylib (com mingw incluso)

Abra o **PowerShell** (NÃO precisa ser admin) e rode:

```powershell
# 1. Baixa o zip da Raylib (com mingw-w64) na pasta de Downloads
$url = "https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_win64_mingw-w64.zip"
$zip = "$env:USERPROFILE\Downloads\raylib.zip"
Invoke-WebRequest -Uri $url -OutFile $zip

# 2. Cria a pasta C:\raylib e extrai o conteúdo nela
New-Item -ItemType Directory -Force -Path "C:\raylib" | Out-Null
Expand-Archive -Path $zip -DestinationPath "C:\raylib" -Force

# 3. Renomeia a pasta interna para "raylib" (o Makefile espera C:\raylib\raylib)
Rename-Item "C:\raylib\raylib-5.5_win64_mingw-w64" "raylib"
```

> **Se a versão 5.5 não existir mais**, vá em https://github.com/raysan5/raylib/releases, copie o link do `raylib-X.X_win64_mingw-w64.zip` mais recente e troque a `$url` acima.

### Passo 2: adicionar o mingw-w64 ao PATH

Também no PowerShell:

```powershell
# Adiciona PERMANENTEMENTE C:\raylib\raylib\mingw\bin ao PATH do usuário
$mingw = "C:\raylib\raylib\mingw\bin"
$path  = [Environment]::GetEnvironmentVariable("PATH", "User")
if ($path -notlike "*$mingw*") {
    [Environment]::SetEnvironmentVariable("PATH", "$path;$mingw", "User")
}
# Atualiza o PATH da sessão atual também
$env:PATH += ";$mingw"
```

**Feche e reabra o PowerShell** depois disso. Para testar:

```powershell
gcc --version
mingw32-make --version
```

Se aparecer a versão em ambos, deu certo.

### Passo 3: compilar e rodar

Navegue até a pasta do projeto e rode:

```powershell
cd "D:\Malloc Sports"
mingw32-make
.\malloc_sports.exe
```

Para limpar os arquivos compilados:

```powershell
mingw32-make clean
```

> **Por que `mingw32-make` e não `make`?** Porque o `make` puro não vem no Windows. O mingw-w64 instala o comando como `mingw32-make`. Se você preferir digitar só `make`, pode criar um alias no perfil do PowerShell:
>
> ```powershell
> Add-Content $PROFILE "`nSet-Alias make mingw32-make"
> ```

---

## Instalação no LINUX

```bash
sudo apt update
sudo apt install build-essential libraylib-dev pkg-config
```

Depois:

```bash
cd "Malloc Sports"
make
./malloc_sports
```

## Instalação no macOS

```bash
brew install raylib pkg-config
```

Depois:

```bash
cd "Malloc Sports"
make
./malloc_sports
```

---

## Alternativa: WSL no Windows (caso o caminho nativo dê problema)

Se a instalação nativa não funcionar, dá para usar o **WSL** (Subsistema Linux para Windows) e seguir as instruções do Linux:

```powershell
# No PowerShell como ADMIN
wsl --install -d Ubuntu
```

Reinicie o PC. Quando o Ubuntu abrir pela primeira vez, crie usuário e senha. Depois, dentro do Ubuntu:

```bash
sudo apt update
sudo apt install build-essential libraylib-dev pkg-config

# Acessar a pasta do projeto que está em D:\Malloc Sports
cd /mnt/d/Malloc\ Sports
make
./malloc_sports
```

> **Atenção no WSL:** abrir janelas gráficas funciona automaticamente no Windows 11 (WSLg). No Windows 10, é preciso instalar um servidor X (ex.: VcXsrv) — recomendado usar Windows 11 ou seguir o caminho nativo.

---

## Controles (teclado dividido)

| Ação             | Jogador 1     | Jogador 2          |
|------------------|---------------|--------------------|
| Mover esq/dir    | A / D         | SETA ESQ / DIR     |
| Mover cima/baixo | W / S         | SETA CIMA / BAIXO  |
| Ação principal   | W / ESPAÇO    | SETA CIMA / ENTER  |
| Pausar           | ESC           | ESC                |
| Confirmar (UI)   | ENTER         | ENTER              |

## Modos de jogo

- **Torneio:** os 2 jogadores disputam os 3 minigames em sequência. Maior pontuação no fim vence.
- **Minigame Único:** escolha livre de qualquer minigame para uma partida avulsa.

## Minigames

1. **Corrida de Ponteiros** — corrida lateral com obstáculos. Pula com W / SETA CIMA.
2. **Pong da Memória** — pong clássico. Quem perde a bola sofre "vazamento de memória" visual.
3. **Estouro de Pilha** — Tetris competitivo. Limpar linhas envia lixo para o adversário.

## Estrutura do projeto

```
Malloc Sports/
├── src/             arquivos .c e .h (1 módulo por arquivo)
├── assets/          sprites e sons
├── Makefile         compilação (Windows + Linux)
├── README.md        este arquivo
├── .gitignore
└── scores.dat       gerado em runtime (ranking)
```

## Como cada arquivo deve ser preenchido

Cada `.c`/`.h` já tem um cabeçalho de comentários com a IDEIA, os PASSOS e os REQUISITOS DA DISCIPLINA que ele cobre.

**Cada dev escreve o código do módulo dele**, lendo os comentários como roteiro. Os comentários NÃO são código pronto — eles guiam o raciocínio.

## Requisitos da disciplina cobertos

- Código em C puro (apenas .c e .h)
- Raylib como biblioteca gráfica
- Structs (Jogador, Bola, Bloco, Obstáculo, Score, EstadoTorneio)
- Ponteiros em todo lado
- Alocação dinâmica (malloc/free) sem vazamentos (testar com `valgrind` no Linux)
- Lista encadeada (obstáculos da Corrida de Ponteiros + ranking)
- Matriz 2D (grid do Estouro de Pilha)
- Arquivo E/S (`scores.dat`)
- Animação + movimento
- Colisão (AABB na Corrida de Ponteiros, bola no Pong, grid no Estouro de Pilha)

## ATENÇÃO — Política de IA

> O enunciado da disciplina diz textualmente: **"o uso de ferramentas de geração de código (ex. GitHub Copilot, ChatGPT para código) zera a nota integralmente. Cada membro deve entender e saber explicar qualquer linha do projeto."**

Os comentários deste esqueleto descrevem **a ideia** e **o passo a passo**, mas o código você escreve você mesmo. Use os comentários como roteiro de estudo, não como cola.

## Cronograma resumido

| Semana | Datas         | Foco                              |
|--------|---------------|-----------------------------------|
| 1      | 26/04 - 02/05 | entities + physics + linked_list + tournament + scores + menu |
| 2      | 03/05 - 09/05 | Corrida de Ponteiros + Pong + Estouro de Pilha jogáveis |
| 3      | 10/05 - 16/05 | Integração com tournament + UI final |
| 4      | 17/05 - 23/05 | Testes (valgrind), correção de bugs, vídeo de demonstração, slides |
| 5      | 24/05 - 31/05 | Reserva / congelamento do repositório |

**Entrega final: 31/05/2026 — 23:59.**

## Problemas comuns no Windows

- **`gcc: command not found`** — você não reabriu o PowerShell após editar o PATH, ou o caminho `C:\raylib\raylib\mingw\bin` está diferente do que ficou na sua máquina. Verifique com `Get-ChildItem C:\raylib\raylib\mingw\bin\gcc.exe`.
- **`raylib.h: No such file`** — sua Raylib foi extraída em outro caminho. Edite a variável `RAYLIB_PATH` no Makefile, ou rode `mingw32-make RAYLIB_PATH=C:/seu/caminho/raylib`.
- **`undefined reference to WinMain`** — você tentou compilar com `-mwindows` em algum lugar. O Makefile deste projeto NÃO usa essa flag, então a janela do jogo abre junto de um console (útil para debug). Para esconder o console na entrega final, adicione `-mwindows` ao `LDFLAGS` do bloco Windows.
