

NAME := unit-test
NAME_COVERAGE := $(NAME)_coverage
NAME_PROFILING := $(NAME)_profiling

CC := gcc

CFLAGS := -Wall -Wextra -Werror -g3

# CMocka require this
CFLAGS += -DHAVE_SIGNAL_H
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-return-type

FANALYZER_FLAG =					\
-O2									\
-fanalyzer							\
-fdump-analyzer-callgraph			\
-fdump-analyzer-supergraph			\
-fstack-usage						\

#-fcallgraph-info=su,da				\

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

SRC := 						\
	src/list_test.c			\
	src/simple_test.c		\
	src/tree_test.c			\
	src/main_test.c			\
	cmocka/src/cmocka.c		\

#	TODO LMA
#	./src/node_test.c \

INCDIR := inc/				\
		  	libft/			\
			cmocka/include	\

IFLAGS 	= $(foreach D,$(INCDIR), -I$(D))

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
OBJ_COVERAGE = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.cov.o, $(SRC))
OBJ_PROFILING = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.profiling.o, $(SRC))
ASM = $(patsubst $(SRC_DIR)/%.c, $(ASM_DIR)/%.S, $(SRC))
PRE = $(patsubst $(SRC_DIR)/%.c, $(PRE_DIR)/%.i, $(SRC))

CALLGRAPHS_DOTFILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.c.callgraph.dot, $(SRC))
#CALLGRAPHS_CI = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.ci, $(SRC))
SUPERGRAPHS_DOTFILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.c.supergraph.dot, $(SRC))
SUPERGRAPHS_EG_DOTFILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.c.supergraph-eg.dot, $(SRC))
#CALLGRAPHS = $(patsubst $(OBJ_DIR)/%.c.callgraph.dot, $(CALLGRAPH_DIR)/%.callgraph.svg, $(CALLGRAPHS_DOTFILES))
CALLGRAPHS = $(patsubst $(OBJ_DIR)/%.c.callgraph.dot, $(CALLGRAPH_DIR)/%.callgraph.svg, $(CALLGRAPHS_DOTFILES))
SUPERGRAPHS = $(patsubst $(OBJ_DIR)/%.c.supergraph.dot, $(CALLGRAPH_DIR)/%.supergraph.svg, $(SUPERGRAPHS_DOTFILES))
SUPERGRAPHS_EG = $(patsubst $(OBJ_DIR)/%.c.supergraph-eg.dot, $(CALLGRAPH_DIR)/%.supergraph-eg.svg, $(SUPERGRAPHS_EG_DOTFILES))
GCDAFILES = $(patsubst $(OBJ_DIR)/%.cov.o, $(OBJ_DIR)/%.cov.gcda, $(OBJ))

MISRA_REPORT_FILE = $(BUILD_DIR)/misra_report.txt

all: prep asm obj callgraphs misra bin

asm: $(ASM)
prep: $(PRE)
obj: $(OBJ) $(OBJ_COVERAGE) $(OBJ_PROFILING)
#callgraphs: $(CALLGRAPHS) $(SUPERGRAPHS) $(SUPERGRAPHS_EG)
callgraphs: $(CALLGRAPHS)
misra: $(MISRA_REPORT_FILE)
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

#$(CALLGRAPHS_CI): $(OBJ)
$(SUPERGRAPHS_DOTFILES): $(OBJ)
$(SUPERGRAPHS_EG_DOTFILES): $(OBJ)
$(CALLGRAPHS_DOTFILES): $(OBJ)

#$(OBJ_DIR)/%.c.callgraph.dot: $(OBJ_DIR)/%.ci
#	@mkdir -p $(dir $@)
#	egypt $< > $@

$(CALLGRAPH_DIR)/%.callgraph.svg: $(OBJ_DIR)/%.c.callgraph.dot
	@mkdir -p $(dir $@)
	#dot -Tsvg -o $@ $<
	cd gcc_graph_tool && uv sync && . .venv/bin/activate && cd - && \
	./gcc_graph_tool/gcc_graph_tool.py -y ./gcc_graph_tool/yml/callgraph.yml -i $< -o $@

$(CALLGRAPH_DIR)/%.supergraph.svg: $(OBJ_DIR)/%.c.supergraph.dot
	@mkdir -p $(dir $@)
	dot -Tsvg -o $@ $<

$(CALLGRAPH_DIR)/%.supergraph-eg.svg: $(OBJ_DIR)/%.c.supergraph-eg.dot
	@mkdir -p $(dir $@)
	dot -Tsvg -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(NAME) $(NAME_COVERAGE) $(NAME_PROFILING)

$(MISRA_REPORT_FILE): $(SRC) $(wildcard $(INC_DIR)/*.h)
	cppcheck --template="{file}:{line}:{column}: {id}:{severity}:{message}" --output-file=$@ --addon=misra/misra.json $(SRC) $(wildcard $(INC_DIR)/*.h)

re: clean all
