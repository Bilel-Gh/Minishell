/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 04:32:12 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/07 20:00:06 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>

// #include "../src/builtins/builtins.c"
// #include "../src/builtins/b_export.c"
// #include "../src/parsing/lexeur.c"
// #include "../src/utils/split.c"
//
// #include "../src/utils/split.c"

extern char **environ;

enum e_character_type {
    ALPHANUM = 0,
    REDIRECT = 1,
	RED_IN = 2,
	RED_OUT = 3,
    PIPE = 4,
	ESPACE = 5,
	RED_D_IN = 6,
	RED_D_OUT = 7,
	QUOTE_S = 8,
	QUOTE_D = 9,
	LAST = 10
};

enum e_token_type {
    COMMANDE = 1,
    T_PIPE = 2,
    ARG = 3,
    INFILE = 4,
    OUTFILE = 5,
    REDIRECT_OUT = 6,
    REDIRECT_IN = 7,
    REDIRECT_D_OUT = 8,
    REDIRECT_D_IN = 9,
    LIMITEUR = 10,
    OPTION = 11,
};
// test de ce a quoi vont les structures

// infos sur tout le token (a completer)
typedef struct		token_info
{
	enum e_token_type	type;
}					t_token_info;

// chaque charactere est un character
typedef struct		s_character
{
	char				value;
	enum e_character_type	type;
	struct s_character	*next;
}					t_character;

// chaque token est un ensemble de charactere du meme type
typedef struct		token
{
    int token_index;
	char			*value;
	t_token_info		*info;
	struct token		*next;
    struct token		*prev;
}
					t_token;
typedef struct commande
{
    char**cmd;
	struct commande		*next;
}					t_commande;

char **ft_lexeur(char *line);

// Dossier BUILTIN
void builtin_echo(char **args);
void builtin_export(char **args);
void builtin_unset(char **args);
void builtin_env();
void builtin_pwd();
void builtin_cd(char **args);
void builtin_exit();

// Dossier PARSING
char    **ft_parsing(int *type_args, int *nb_args, char **args, int *error);
int *ft_get_info_args(char **line_split , int *give_nb_token);
char **join_inter_space(char **args, int *type_args, int *nb_args);
char **kick_quote(int *type_args, int nb_args, char **args);
t_token* ft_get_tokens_with_infos(char **args, int *info_args, int nb_args);
int	ft_isprint(int chara);
int ft_is_outfile(t_token *token);
int ft_is_infile(t_token *token);
t_token* addtoken(t_token* head, char* value);
int ft_get_infos_by_pos(t_token *token);

// Dossier UTILS
void	ft_complete( char const *s, char *str, int *i);
char	*ft_strjoin(char *s1, char *s2);
int	ft_strlen(const char *str);
char	**ft_split(char *str, char *charset);
char	*ft_strdup(char *src);
int		ft_strncmp(char *s1, char *s2, unsigned int n);
char	*ft_strcpy(char *dest, char *src);
char	*ft_strndup(const char *s1, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
int ft_is_upper(char c);
char	*ft_strchr(const char *s, int c);

// Dossier LEXEUR
t_token* merge_characters(t_character* head);
t_token	*ft_handle_quotes(t_token *tokenHead, t_character **current_charPtr);
char	*append_value(char *str, char c);

// Dossier error_args
bool search_error_args(int *type_args, int *nb_args, char **args);
bool	error_no_only_type(char *str);
bool	error_size(char *args, int size_max);
void	give_sp_args_redi(char *args, int *type_args, int pos);
bool	quote_is_open(char *args);

// Dossier EXPAND
void	expande(int *type_args, int nb_args, char **args);
char	*join_and_rp_args(char *args_con, char *expande, int size_extract);
char	*give_env_expand(char *expande_search, int size);
char	*is_sp_expand(char *expand_search);
char	*get_expende_detect(int size_of_expende, char *expande);
int	ft_size_of_expende(char *expande);

// Dossier INFOS_TOKENS
int ft_is_command(t_token *token);
int ft_is_argument(char* value);
int ft_is_option(char* value);
int is_env(t_token* token);
t_commande *cmd_complete(t_token *token);
char *ft_join_cmd(char *s1, char *s2);
#endif
