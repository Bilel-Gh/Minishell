# /* ~~~~~~ SOURCES ~~~~~~ */
SRCS_DIR = ./src/
SRCS =	main.c \

OBJS = ${addprefix ${SRCS_DIR}, ${SRCS:.c=.o}}

# /* ~~~~~~~ UTILS ~~~~~~~ */
UTILS_DIR = ./src/utils/
UTILS =	split.c \
		ft_strjoin.c \

OBJS_UTILS = ${addprefix ${UTILS_DIR}, ${UTILS:.c=.o}}

# /* ~~~~~~~ PARSING ~~~~~~~ */
PARSING_DIR = ./src/parsing/
PARSING =	lexeur.c \
			ft_info_token.c \
			parsing.c \

OBJS_PARSING = ${addprefix ${PARSING_DIR}, ${PARSING:.c=.o}}

# /* ~~~~~~~ BUILTINS ~~~~~~~ */
BUILTINS_DIR = ./src/builtins/
BUILTINS =	builtins.c \
			b_export.c \

OBJS_BUILTINS = ${addprefix ${BUILTINS_DIR}, ${BUILTINS:.c=.o}}



# /* ~~~~~~~ COMPILING INFO ~~~~~~~ */
CC = cc
CFLAGS = -Wall -Werror -Wextra -g3
IFLAGS:= -I ./includes

# /* ~~~~~~~ OTHER ~~~~~~~ */
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

$(NAME): $(OBJS) $(OBJS_UTILS) $(OBJS_PARSING) $(OBJS_BUILTINS)
	@echo $(CYAN) " - Compiling $@" $(RED)
	@$(CC) $(CFLAGS) $(OBJS) $(OBJS_UTILS) $(OBJS_PARSING) $(OBJS_BUILTINS) $(IFLAGS) -o $(NAME) -l readline
	@echo $(GREEN) "[OK COMPILED]" $(EOC)
	@echo $(GREEN) "[LAUNCH PROGRAMM]" $(EOC)

clean:
		@echo $(PURPLE) "[🧹Cleaning...🧹]" $(EOC)
		@${RM} ${OBJS} $(OBJS_UTILS) $(OBJS_PARSING) $(OBJS_BUILTINS)

fclean: clean
		@echo $(PURPLE) "[🧹FCleaning...🧹]" $(EOC)
		@${RM} ${OBJS} $(OBJS_UTILS) $(OBJS_PARSING) $(OBJS_BUILTINS) ${NAME}

re: 	fclean all

.PHONY: all clean fclean re
