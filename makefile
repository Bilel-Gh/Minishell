# export LDFLAGS="-L$(brew --prefix readline)/lib" #                                                                                               ─╯
# export CPPFLAGS="-I$(brew --prefix readline)/include" #

# /* ~~~~~~~ COMPILING INFO ~~~~~~~ */
CC = gcc
CFLAGS = -Wall -Werror -Wextra -g3
IFLAGS:= -I ./includes

# /* ~~~~~~~ LINKING INFO ~~~~~~~ */
LDFLAGS = -L /Users/bilelgh/homebrew/Cellar/readline/8.2.1/lib #A supprimer
LDLIBS = -lreadline

# /* ~~~~~~ SOURCES ~~~~~~ */
SRCS_DIR = ./src/
SRCS =	main.c \

OBJS = ${addprefix ${SRCS_DIR}, ${SRCS:.c=.o}}

# /* ~~~~~~~ UTILS ~~~~~~~ */
UTILS_DIR = ./src/utils/
UTILS =	split.c \
		ft_strjoin.c \

OBJS_UTILS = ${addprefix ${UTILS_DIR}, ${UTILS:.c=.o}}

# /* ~~~~~~~ LEXEUR ~~~~~~~ */
LEXEUR_DIR = ./src/lexeur/
LEXEUR =	lexeur.c \
			merge_tokens.c \

OBJS_LEXEUR = ${addprefix ${LEXEUR_DIR}, ${LEXEUR:.c=.o}}

# /* ~~~~~~~ PARSING ~~~~~~~ */
PARSING_DIR = ./src/parsing/
PARSING =	get_info_args.c \
			parsing.c \
			/error_args/search_error_args.c \
			/error_args/utils_error_args.c \

OBJS_PARSING = ${addprefix ${PARSING_DIR}, ${PARSING:.c=.o}}

# /* ~~~~~~~ BUILTINS ~~~~~~~ */
BUILTINS_DIR = ./src/builtins/
BUILTINS =	builtins.c \
			b_export.c \

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

$(NAME): $(OBJS) $(OBJS_UTILS) $(OBJS_PARSING) $(OBJS_LEXEUR) $(OBJS_BUILTINS)
	@echo $(CYAN) " - Compiling $@" $(RED)
	@$(CC) $(CFLAGS) $(OBJS) $(OBJS_UTILS) $(OBJS_PARSING) $(OBJS_LEXEUR) $(OBJS_BUILTINS) $(IFLAGS) $(LDFLAGS) $(LDLIBS) -o $(NAME)
	@echo $(GREEN) "[OK COMPILED]" $(EOC)
	@echo $(GREEN) "[LAUNCH PROGRAMM]" $(EOC)
	@mkdir -p obj
	@mv $(OBJS) $(OBJS_UTILS) $(OBJS_PARSING) $(OBJS_LEXEUR) $(OBJS_BUILTINS) obj/

clean:
	@echo $(PURPLE) "[🧹Cleaning...🧹]" $(EOC)
	@${RM} ${OBJS} $(OBJS_UTILS) $(OBJS_PARSING) $(OBJS_LEXEUR) $(OBJS_BUILTINS)
	@${RM} -r obj

fclean: clean
	@echo $(PURPLE) "[🧹FCleaning...🧹]" $(EOC)
	@${RM} ${NAME}


re: 	fclean all

.PHONY: all clean fclean re
