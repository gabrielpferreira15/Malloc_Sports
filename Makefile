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
# ---------- WINDOWS (PowerShell / cmd, com mingw-w64) ----------
# Caminho onde a Raylib oficial pra Windows costuma ser instalada.
# Se você instalou em outro lugar, ajuste RAYLIB_PATH abaixo.
RAYLIB_PATH ?= C:/raylib/raylib

# Busca automática pelo libraylib.a em locais comuns
RAYLIB_LIB_FILE := $(firstword \
	$(wildcard C:/raylib/raylib/src/libraylib.a) \
	$(wildcard C:/raylib/w64devkit/lib/libraylib.a) \
	$(wildcard $(RAYLIB_PATH)/src/libraylib.a) \
)

# Busca automática pelo gcc do bundle do Raylib 
RAYLIB_GCC := $(firstword \
	$(wildcard $(RAYLIB_PATH)/mingw/bin/gcc.exe) \
	$(wildcard $(RAYLIB_PATH)/w64devkit/bin/gcc.exe) \
	$(wildcard C:/raylib/w64devkit/bin/gcc.exe) \
)

ifeq ($(RAYLIB_GCC),)
$(error Não foi possível encontrar o gcc do bundle da Raylib. Por favor, ajuste RAYLIB_PATH manualmente ou defina RAYLIB_GCC=C:/.../gcc.exe)
endif

CC := $(RAYLIB_GCC)
TOOLCHAIN_BIN := $(dir $(RAYLIB_GCC))

ifeq ($(RAYLIB_LIB_FILE),)
$(error Não foi possível encontrar o libraylib.a. Por favor, ajuste RAYLIB_PATH manualmente ou instale o Raylib.)
endif

RAYLIB_LIB_DIR := $(dir $(RAYLIB_LIB_FILE))

CFLAGS  = -Wall -Wextra -std=c11 -IInclude -I"$(RAYLIB_PATH)/src" -B"$(TOOLCHAIN_BIN)"
LDFLAGS = -L"$(RAYLIB_LIB_DIR)" -B"$(TOOLCHAIN_BIN)" -lraylib -lopengl32 -lgdi32 -lwinmm -lm

EXE_EXT = .exe
RM      = del /Q
OBJ     = $(SRC:.c=.o)

else
# ---------- LINUX / macOS ----------
# pkg-config descobre as flags certas automaticamente.
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

# Regra padrão do make: como compilar .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	-$(RM) src\*.o $(OUT) 2> nul
else
	$(RM) $(OBJ) $(OUT)
endif

.PHONY: all clean
