

NAME := unit-test
NAME_COVERAGE := $(NAME)_coverage
NAME_PROFILING := $(NAME)_profiling

CC := gcc

CFLAGS := -Wall -Wextra -Werror -g3

FANALYZER_FLAG =					\
-fanalyzer							\
-fdump-analyzer-supergraph			\
-fdump-analyzer-callgraph			\

FPROFILE_FLAG = 					\
-pg									\

COVERAGE_FLAG = 					\
--coverage							\

SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
ASM_DIR = $(BUILD_DIR)/asm
PRE_DIR = $(BUILD_DIR)/preprocessing
CALLGRAPH_DIR = $(BUILD_DIR)/callgraph

SRC := \
	src/list_test.c \
	src/tree_test.c \
	src/main_test.c

#	TODO LMA
#	./src/node_test.c \

INCDIR := inc/ \
		  libft/

IFLAGS 	= $(foreach D,$(INCDIR), -I$(D))

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
OBJ_COVERAGE = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.cov.o, $(SRC))
OBJ_PROFILING = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.profiling.o, $(SRC))
ASM = $(patsubst $(SRC_DIR)/%.c, $(ASM_DIR)/%.S, $(SRC))
PRE = $(patsubst $(SRC_DIR)/%.c, $(PRE_DIR)/%.i, $(SRC))

DOTFILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.c.callgraph.dot, $(SRC))
CALLGRAPHS = $(patsubst $(OBJ_DIR)/%.c.callgraph.dot, $(CALLGRAPH_DIR)/%.png, $(DOTFILES))

GCDAFILES = $(patsubst $(OBJ_DIR)/%.cov.o, $(OBJ_DIR)/%.cov.gcda, $(OBJ))

all: prep asm obj callgraphs bin

asm: $(ASM)
prep: $(PRE)
obj: $(OBJ)
callgraphs: $(CALLGRAPHS)
bin: $(NAME) $(NAME_COVERAGE) $(NAME_PROFILING)

$(NAME): $(OBJ) 
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBFT) $(IFLAGS)

$(NAME_COVERAGE): $(OBJ_COVERAGE)
	$(CC) $(CFLAGS) -o $@ $(OBJ_COVERAGE) $(LIBFT) $(IFLAGS) -lgcov

$(NAME_PROFILING): $(OBJ_PROFILING)
	$(CC) $(CFLAGS) -o $@ $(OBJ_PROFILING) $(LIBFT) $(IFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(FANALYZER_FLAG) -o $@ -c $< $(IFLAGS)

$(OBJ_DIR)/%.cov.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(COVERAGE_FLAG) -o $@ -c $< $(IFLAGS)

$(OBJ_DIR)/%.profiling.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(FPROFILE_FLAG) -o $@ -c $< $(IFLAGS)

$(PRE_DIR)/%.i: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< $(IFLAGS)

$(ASM_DIR)/%.S: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< $(IFLAGS)

$(DOTFILES): $(OBJ)

$(CALLGRAPH_DIR)/%.png: $(OBJ_DIR)/%.c.callgraph.dot
	@mkdir -p $(dir $@)
	dot -Tpng -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(NAME) $(NAME_COVERAGE) $(NAME_PROFILING)

re: clean all
