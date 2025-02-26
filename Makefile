# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/25 22:54:23 by rbardet-          #+#    #+#              #
#    Updated: 2025/02/26 06:26:27 by throbert         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g3

SRC_DIR     = src
LIBFT_DIR   = libft
INC_DIR     = includes
OBJ_DIR     = objs

BUILTINS_DIR  = $(SRC_DIR)/builtins
EXEC_DIR      = $(SRC_DIR)/exec
SUB_EXEC_DIR  = $(EXEC_DIR)/exec
REDIRECT_DIR  = $(EXEC_DIR)/redirect
PARSE_DIR     = $(SRC_DIR)/parsing
SYNTAX_DIR    = $(PARSE_DIR)/syntax
QUOTE_DIR     = $(PARSE_DIR)/quotes
TOKENIZE_DIR  = $(PARSE_DIR)/tokenize
WILDCARD_DIR  = $(PARSE_DIR)/wildcards

SRCS = $(SRC_DIR)/main.c \
	$(SRC_DIR)/get_readline.c \
	$(SRC_DIR)/shell_loop.c \
	$(SRC_DIR)/signal.c \
	$(SRC_DIR)/signal2.c \
	$(BUILTINS_DIR)/ft_env.c \
	$(BUILTINS_DIR)/ft_pwd.c \
	$(BUILTINS_DIR)/ft_echo.c \
	$(BUILTINS_DIR)/ft_export.c \
	$(BUILTINS_DIR)/ft_unset.c \
	$(BUILTINS_DIR)/ft_exit.c \
	$(BUILTINS_DIR)/ft_cd.c \
	$(BUILTINS_DIR)/atol.c \
	$(SUB_EXEC_DIR)/builtin_exec.c \
	$(SUB_EXEC_DIR)/cmd_validation.c \
	$(SUB_EXEC_DIR)/exec_cmd.c \
	$(SUB_EXEC_DIR)/exec_local.c \
	$(REDIRECT_DIR)/parse_cmd.c \
	$(REDIRECT_DIR)/parse_cmd2.c \
	$(REDIRECT_DIR)/parse_cmd3.c \
	$(REDIRECT_DIR)/here_doc.c \
	$(REDIRECT_DIR)/find_env_path.c \
	$(REDIRECT_DIR)/redirect.c \
	$(REDIRECT_DIR)/redirect_open.c \
	$(REDIRECT_DIR)/redirect_exec.c \
	$(REDIRECT_DIR)/redirect_utils.c \
	$(REDIRECT_DIR)/get_flag.c \
	$(REDIRECT_DIR)/redirect_loop.c \
	$(REDIRECT_DIR)/find_next_pipe.c \
	$(EXEC_SUBDIR_SRCS) \
	$(PARSING_SUBDIR_SRCS) \
	$(QUOTE_SUBDIR_SRCS) \
    $(QUOTE_DIR)/quote_remover.c \
    $(QUOTE_DIR)/quote_state_utils.c \
    $(QUOTE_DIR)/all_closed.c \
    $(QUOTE_DIR)/all_closed_utils.c \
    $(QUOTE_DIR)/backslash_utils.c \
    $(QUOTE_DIR)/input_preprocess.c \
    $(QUOTE_DIR)/input_preprocess2.c \
    $(QUOTE_DIR)/input_validation.c \
    $(QUOTE_DIR)/input_validation2.c \
    $(QUOTE_DIR)/preprocess.c \
    $(SYNTAX_DIR)/syntax.c \
    $(SYNTAX_DIR)/syntax_error.c \
    $(SYNTAX_DIR)/syntax_error2.c \
	$(SYNTAX_DIR)/error_utils.c \
    $(TOKENIZE_DIR)/tokenizer.c \
    $(TOKENIZE_DIR)/word_parser.c \
    $(TOKENIZE_DIR)/append_utils.c \
    $(TOKENIZE_DIR)/dollar_utils.c \
    $(TOKENIZE_DIR)/dollar_utils2.c \
    $(TOKENIZE_DIR)/input_parser.c \
    $(TOKENIZE_DIR)/input_parser_utils.c \
	$(TOKENIZE_DIR)/cmd_check.c \
	$(TOKENIZE_DIR)/env.c \
	$(TOKENIZE_DIR)/parse_utils.c \
	$(TOKENIZE_DIR)/token_utils.c \
	$(WILDCARD_DIR)/handle_wildcard.c \
	$(WILDCARD_DIR)/match_pattern.c

OBJS    = $(SRCS:%.c=$(OBJ_DIR)/%.o)
INCLUDES = -I $(INC_DIR) -I $(LIBFT_DIR)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_DIR)/libft.a
	@echo "\033[32mLinking objects and creating $(NAME)...\033[0m"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_DIR)/libft.a -o $(NAME) -lreadline
	@echo "\033[32m$(NAME) successfully created!\033[0m"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT_DIR)/libft.a:
	@echo "\033[34mBuilding libft...\033[0m"
	@make -C $(LIBFT_DIR) > /dev/null 2>&1

clean:
	@echo "\033[33mCleaning object files...\033[0m"
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean > /dev/null 2>&1
	@echo "\033[32mClean completed successfully!\033[0m"

fclean: clean
	@echo "\033[33mRemoving $(NAME)...\033[0m"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean > /dev/null 2>&1
	@echo "\033[32mFull clean completed successfully!\033[0m"

re: fclean all

.PHONY: all clean fclean re