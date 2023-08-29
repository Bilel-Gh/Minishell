# export LDFLAGS="-L$(brew --prefix readline)/lib" #                                                                                               ─╯
# export CPPFLAGS="-I$(brew --prefix readline)/include" #

# /* ~~~~~~~ COMPILING INFO ~~~~~~~ */
CC = cc
CFLAGS = -Wall -Werror -Wextra -g3
# CFLAGS = -Wall -Werror -Wextra -fsanitize=address

IFLAGS:= -I ./includes

# /* ~~~~~~~ LINKING INFO ~~~~~~~ */
LDFLAGS = -L /Users/bilelgh/homebrew/Cellar/readline/8.2.1/lib #A supprimer
LDLIBS = -lreadline

# /* ~~~~~~ SOURCES ~~~~~~ */
SRCS_DIR = ./src/
SRCS =	main.c \
		main_utils.c \
		main_utils2.c \
		main_utils3.c \
		main_utils4.c \
		main_utils5.c \

OBJS = ${addprefix ${SRCS_DIR}, ${SRCS:.c=.o}}

# /* ~~~~~~~ UTILS ~~~~~~~ */
UTILS_DIR = ./src/utils/
UTILS =	split.c \
		ft_strjoin.c \
		utils.c \
		utils2.c \
		utils3.c \
		utils_debug.c \
		ft_fprintf.c \

OBJS_UTILS = ${addprefix ${UTILS_DIR}, ${UTILS:.c=.o}}

# /* ~~~~~~~ LEXEUR ~~~~~~~ */
LEXEUR_DIR = ./src/lexeur/
LEXEUR =	lexeur.c \
			lexeur2.c \
			merge_characters.c \
			merge_characters2.c \

OBJS_LEXEUR = ${addprefix ${LEXEUR_DIR}, ${LEXEUR:.c=.o}}

# /* ~~~~~~~ PARSING ~~~~~~~ */
PARSING_DIR = ./src/parsing/
PARSING =	get_info_args.c \
			parsing.c \
			parsing2.c \
			parsing3.c \
			join_inter_space.c \
			kick_quotes.c \
			/infos_token/get_token_infos.c \
			/infos_token/infos_utils.c \
			/infos_token/verif_cmd.c \
			/infos_token/verif_cmd2.c \
			/infos_token/recipe_for_exec.c \
			/infos_token/recipe_for_exec2.c \
			/infos_token/recipe_for_exec3.c \
			/infos_token/recipe_for_exec4.c \
			/infos_token/recipe_for_exec5.c \
			/infos_token/recipe_for_exec6.c \
			/error_args/search_error_args.c \
			/error_args/search_error_args2.c \
			/error_args/search_error_args3.c \
			/error_args/utils_error_args.c \
			/expand/expand.c \
			/expand/expand2.c \
			/expand/expand3.c \
			/expand/expand4.c \
			/expand/expand5.c \
			/expand/expand6.c \

OBJS_PARSING = ${addprefix ${PARSING_DIR}, ${PARSING:.c=.o}}

# /* ~~~~~~~ EXEC ~~~~~~~~~~ */
EXEC_DIR = ./src/exec/
EXEC =	exec_3.c  \
		exec_cmd.c \
		exec_multi.c \
		free_for_exec.c \
		infile.c \
		exec_2.c \
		exec.c \
		exec_multi_2.c \
		exec_solo.c \
		heredoc.c \
		outfile.c

OBJS_EXEC = ${addprefix ${EXEC_DIR}, ${EXEC:.c=.o}}

# /* ~~~~~~~ BUILTINS ~~~~~~~ */
BUILTINS_DIR = ./src/builtins/
BUILTINS =	builtins.c \
			export.c \
			export2.c \
			export3.c \
			export4.c \
			export5.c \
			export6.c \
			export7.c \
			echo.c \
			cd.c \
			cd2.c \
			cd3.c \
			env.c \
			pwd.c \
			unset.c \
			exit.c \
			exit2.c 

OBJS_BUILTINS = ${addprefix ${BUILTINS_DIR}, ${BUILTINS:.c=.o}}


# /* ~~~~~~~ TARGET ~~~~~~~ */
NAME = minishell
RM = rm -f

# /* ~~~~~~~ Colors ~~~~~~~ */
BLACK:="\033[1;30m"
RED:="\033[1;31m"
GREEN:="\033[1;32m"
PURPLE:="\033[1;35m"
CYAN:="\033[1;36m"
WHITE:="\033[1;37m"
EOC:="\033[0;0m"

all:	${NAME}

$(NAME): $(OBJS) $(OBJS_UTILS) $(OBJS_EXEC) $(OBJS_PARSING) $(OBJS_LEXEUR) $(OBJS_BUILTINS)
	@echo $(CYAN) " - Compiling $@" $(RED)
	@$(CC) $(CFLAGS) $(OBJS) $(OBJS_UTILS) $(OBJS_EXEC) $(OBJS_PARSING) $(OBJS_LEXEUR) $(OBJS_BUILTINS) $(IFLAGS) $(LDFLAGS) $(LDLIBS) -o $(NAME)
	@echo $(GREEN) "[OK COMPILED]" $(EOC)
	@echo $(GREEN) "[LAUNCH PROGRAMM]" $(EOC)
	@mkdir -p obj

clean:
	@echo $(PURPLE) "[🧹Cleaning...🧹]" $(EOC)
	@${RM} ${OBJS} $(OBJS_UTILS) $(OBJS_PARSING) $(OBJS_EXEC) $(OBJS_LEXEUR) $(OBJS_BUILTINS)
	@${RM} -r obj

fclean: clean
	@echo $(PURPLE) "[🧹FCleaning...🧹]" $(EOC)
	@${RM} ${NAME}


re: 	fclean all

.PHONY: all clean fclean re
