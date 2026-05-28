# ============================================================
# Makefile — Malloc Sports (multiplataforma Windows + Linux)
# ------------------------------------------------------------
# Dono: DEV 1 — Gabriel Peixoto
#
# COMO USAR
#   make / mingw32-make         -> compila e gera o executável
#   make clean / mingw32-make clean -> remove os .o e o executável
#   ./malloc_sports             (Linux/macOS)
#   .\malloc_sports.exe         (Windows / PowerShell)
#
# DEPENDÊNCIA
#   Raylib 5.x. Veja o README.md para instalação por SO.
#
# DETECÇÃO DE OS
#   A variável OS=Windows_NT existe no cmd/PowerShell do Windows.
#   No Linux/macOS ela não existe, então caímos no else.
# ============================================================
CC      = gcc
SRC     = $(wildcard src/*.c)
TARGET  = malloc_sports

ifeq ($(OS),Windows_NT)

# ---------- WINDOWS (MSYS2 UCRT64 + Raylib) ----------

CFLAGS  = -Wall -Wextra -std=c11 -IInclude
LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm -lm

EXE_EXT = .exe
RM      = del /Q
OBJ     = $(SRC:.c=.o)

else

# ---------- LINUX / macOS ----------

CFLAGS  = -Wall -Wextra -std=c11 -IInclude `pkg-config --cflags raylib`
LDFLAGS = `pkg-config --libs raylib` -lm

EXE_EXT =
RM      = rm -f
OBJ     = $(SRC:.c=.o)

endif

OUT = $(TARGET)$(EXE_EXT)

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compilar .c -> .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	-$(RM) src\*.o $(OUT) 2> nul
else
	$(RM) $(OBJ) $(OUT)
endif

.PHONY: all clean