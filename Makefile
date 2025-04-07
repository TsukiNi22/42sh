##
## EPITECH PROJECT, 2025
## Makefile
## File description:
## Radar makefile de la ta
##

CC := gcc

TARGET := 42sh
TEST_TARGET := unit_tests
SYS_CMD_TARGET = mysh
BUILD_DIR := .obj

BIN_DIR := $(wildcard /usr/local/bin/)

W := -W -Wall -Wextra -Wpedantic -Wunused-parameter -Wshadow
W += -Wuninitialized -Wmaybe-uninitialized

DEBUG := -g -ggdb3

CPPFLAGS := -I ./include/
LDFLAGS := -L ./lib/ -lmy
CFLAGS := $(W)

ifeq ($(d), t)
	CFLAGS := $(DEBUG)
else ifeq ($(d), o)
	CFLAGS += -O1
endif

INIT :=		init/init_data.c \
			init/init_global.c \
			init/init_prompt.c \
			init/init_env.c \
			init/init_bonus.c

SIGNAL :=	handle_signal/sigsegv.c \
			handle_signal/sigint.c \
			handle_signal/sigtstp.c \
			handle_signal/sigquit.c

GLOBAL :=	main.c \
			const.c \
			minishell.c \
			free_data.c

MAIN := 	main/prompt.c \
			main/get_input.c \
			main/history.c \
			main/check_syntax.c \
			main/get_input_type.c \
			main/exe_input.c \
			main/check_accesible_dir.c

UPDATE :=	update/sys_func_update.c

PARSER := 	main/parser/inputs_parser.c \
			main/parser/input_parser.c \
			main/parser/cmd_parser.c

SYNTAX := 	check_syntax/check_syntax_exit.c \
			check_syntax/check_syntax_cd.c \
			check_syntax/check_syntax_setenv.c \
			check_syntax/check_syntax_unsetenv.c \
			check_syntax/check_syntax_env.c \
			check_syntax/check_syntax_color.c \
			check_syntax/check_syntax_help.c \
			check_syntax/check_syntax_alias.c \
			check_syntax/check_syntax_unalias.c \
			check_syntax/check_syntax_history.c \
			check_syntax/check_syntax_source.c \
			check_syntax/check_syntax_rehash.c \
			check_syntax/check_syntax_silent.c

BUILTIN := 	builtin/exit.c \
			builtin/cd.c \
			builtin/setenv.c \
			builtin/unsetenv.c \
			builtin/env.c \
			builtin/color.c \
			builtin/help.c \
			builtin/alias.c \
			builtin/unalias.c \
			builtin/history.c \
			builtin/source.c \
			builtin/rehash.c \
			builtin/silent.c

REDIRECT := main/redirection/get_heredoc.c

FILES := $(INIT) $(SIGNAL) $(GLOBAL) $(MAIN) $(UPDATE)
FILES += $(PARSER) $(REDIRECT) $(SYNTAX) $(BUILTIN) #$(REDIRECT)
SRC := $(addprefix src/, $(FILES))
OBJ := $(SRC:%.c=$(BUILD_DIR)/%.o)

TEST_OBJ := $(filter-out $(BUILD_DIR)/src/main.o, $(OBJ))

all: $(TARGET)

$(TARGET): $(OBJ)
	@make -C lib/my --no-print-directory D=$(d)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -c -o $@ $^

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f tests/*.o
	@rm -f *.gc*
	@rm -f vgcore.*
	@make clean -C lib/my --no-print-directory

fclean: clean
	@rm -f $(TARGET)
	@rm -f $(TEST_TARGET)
	@make fclean -C lib/my --no-print-directory

.NOTPARALLEL:
re: fclean $(TARGET)

unit_tests: $(TARGET)
	$(CC) -o $(TEST_TARGET) $(TEST_OBJ) tests/*.c \
	$(CPPFLAGS) $(LDFLAGS) --coverage -lcriterion

tests_run:    unit_tests
	./$(TEST_TARGET)
	gcovr . --exclude tests/ --exclude lib/

sys_cmd: $(TARGET)
	@echo "Copying the target in \'/usr/local/bin\'..."
	@sudo cp $(TARGET) /usr/local/bin/$(TARGET)

get_unregistered_files:
	@find src/ -name "*.c" | while read file; do \
        echo "$(SRC)" | grep -q "$$file" || \
		echo "$$file" >> new_files_detected.txt; \
    done
	@if [ -f new_files_detected.txt ]; then \
        cat new_files_detected.txt | sed "s/src\///g"; \
        rm -f new_files_detected.txt; \
    fi

get_unknow_files:
	@for file in $(SRC); do \
        if [ ! -f "$$file" ]; then \
            echo "$$file" >> missing_files.txt; \
        fi; \
    done;
	@if [ -f missing_files.txt ]; then \
        cat missing_files.txt | sed "s/src\///g"; \
        rm -f missing_files.txt; \
    fi

.PHONY: all clean fclean re tests_run sys_cmd get_unregistered_files get_unknow_files
