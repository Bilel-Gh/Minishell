/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 04:32:12 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/03 16:40:00 by ncharii          ###   ########.fr       */
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
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

// #include "../src/builtins/builtins.c"
// #include "../src/builtins/b_export.c"
// #include "../src/parsing/lexeur.c"
// #include "../src/utils/split.c"
//
// #include "../src/utils/split.c"

extern char					**environ;

# define SIZE_PATH 4096
# define FIRST 0
# define DERNIER 1
# define INTER 2

extern int g_code_exit;

enum						e_character_type
{
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

enum						e_token_type
{
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

enum
{
	SUCCESS = 0,
	ERROR = 1,
	MISUSE = 2,
	CANTEXEC = 126,
	NOTFOUND = 127,
	CSIGINT = 130,
    CMD_FOUND = 200,
    ERROR_PIPE = 201,
    ERROR_REDIRECT = 202,
    ERROR_PIPE2 = 203,
    ERROR_QUOTE_S = 204,
    ERROR_QUOTE_D = 205,
    ERROR_BACKSLASH = 206,
};

// test de ce a quoi vont les structures

// infos sur tout le token (a completer)
typedef struct token_info
{
	enum e_token_type		type;
}							t_token_info;

// chaque charactere est un character
typedef struct s_character
{
	char					value;
	enum e_character_type	type;
	struct s_character		*next;
}							t_character;

// chaque token est un ensemble de charactere du meme type
typedef struct token
{
	int						token_index;
	char					*value;
	t_token_info			*info;
	struct token			*next;
	struct token			*prev;
}							t_token;
typedef struct commande
{
	char					**cmd;
	struct commande			*next;
}							t_commande;

typedef struct s_global_exec
{
	char					**export;
}							t_global_exec;

typedef struct s_global_parsing
{
	int						*info_args;
	char					**args;
	t_token					*tokens;
	t_commande				*commande;
	char					*line;
	t_global_exec			*exec;
}							t_global_parsing;

//debug
void						verifyString(const char *str);

// DOSSIER EXEC
char						**ft_lexeur(char *line);
char						**ft_db_array_dup(char **db_array);
char						*get_name(char *arg);

// Dossier BUILTIN
void						builtin_echo(char **args);
void						builtin_export(char **args, char ***env,
								t_global_exec **g_exec);
void						builtin_unset(char **args, char ***env,
								t_global_exec **g_exec);
void						builtin_env(char **args, char **env);
void						builtin_pwd(char **args);
void						builtin_cd(char **args, char ***env, char **export);
void						builtin_exit(char **args,
								t_global_parsing **g_pars);
void						ft_exec_bultins(char **args, char ***env,
								t_global_parsing **g_pars,
								t_global_exec **g_exec);
char						*ft_getenv(char *name, char **env);
int							ft_setenv(char *name, char *value, char ***env);
int							ft_check_solo_invalid_arg(char *args);
bool						ft_check_name(char *name);

// Dossier PARSING
char						**ft_parsing(int *nb_args,
								t_global_parsing **g_pars,
								char ***env);
int							*ft_get_info_args(char **line_split,
								int *give_nb_token);
char						**join_inter_space(char **args, int *type_args,
								int *nb_args);
char						**kick_quote(int *type_args, int nb_args,
								char **args);
t_token						*ft_get_tokens_with_infos(char **args, int nb_args);
int							ft_isprint(int chara);
int							ft_is_outfile(t_token *token);
int							ft_is_infile(t_token *token);
t_token						*addtoken(t_token *head, char *value);
int							ft_get_infos_by_pos(t_token *token);
char						*copy_cont(char *str);
char						*copy_sans_quote(char *args);
char	*copy_sans_quote(char *args);

// Dossier UTILS
void						ft_complete(char const *s, char *str, int *i);
char						*ft_strjoin(char *s1, char *s2);
int							ft_strlen(char *str);
char						**ft_split(char *str, char charset);
char						*ft_strdup(char *src);
int							ft_strncmp(char *s1, char *s2, unsigned int n);
char						*ft_strcpy(char *dest, char *src);
char						*ft_strndup(const char *s1, size_t n);
void						*ft_memcpy(void *dst, const void *src, size_t n);
int							ft_is_upper(char c);
char						*ft_strchr(const char *s, int c);
int							ft_strcmp(char *s1, char *s2);
size_t						ft_strlcpy(char *dst, const char *src, size_t size);
char						*ft_strtrim(char *s1, char *set);
void						free_db_array(char **array);
void						free_list_tokens(t_token *tokens);
void						free_list_commande(t_commande *commande);
int							ft_db_tablen(char **tab);
void						ft_free_g_parsing(t_global_parsing *g_parsing);
char						*ft_itoa(int num);
char	*ft_strcat(char *s1, const char *s2);
int ft_is_digit(char c);
char *ft_substr(char *s, unsigned int start, size_t len);
char *ft_strstr(char *str, char *to_find);
int is_only_space(char *str);
int ft_db_arr_len(char **arr);
void	ft_bzero(void *s, size_t n);

// Dossier LEXEUR
t_token						*merge_characters(t_character *head);
t_token						*ft_handle_quotes(t_token *tokenHead,
								t_character **current_charPtr);
char						*append_value(char *str, char c);
t_character					*ft_parse_string(const char *input);
void						get_current_charptr(t_character **current_charPtr,
								t_character *next_char);

// Dossier error_args
bool						search_error_args(int *type_args, int *nb_args,
								char **args);
bool						error_no_only_type(char *str);
bool						error_size(char *args, int size_max);
void						give_sp_args_redi(char *args, int *type_args,
								int pos);
bool						quote_is_open(char *args);

// Dossier EXPAND
void						expande(int **type_args, int *nb_args,
								t_global_parsing **g_pars, char **env);
char						*join_and_rp_args(char *args_con, char *expande,
								int size_extract);
char						*give_env_expand(char *expande_search, int size,
								char **env);
char						*give_env_expand(char *expande_search, int size,
								char **env);
char						*is_sp_expand(char *expand_search);
char						*get_expende_detect(int size_of_expende,
								char *expande);
int							ft_size_of_expende(char *expande);

// Dossier INFOS_TOKENS
int							ft_is_command(t_token *token);
int							ft_is_argument(char *value);
int							ft_is_option(char *value);
int							is_env(t_token *token);
t_commande					*cmd_complete(t_token *token);
char						*ft_join_cmd(char *s1, char *s2);
t_token						*ft_verif_cmd(t_token **tokenHead);
t_token						*create_token(t_token *head, char *value,
								int index);
t_token_info				*add_infos_to_token(char *value, t_token *token,
								int *is_commande_in_pipe);
t_token						*ft_save_next_tokens(t_token *current);
void						ft_free_value_splited(char **value_splited);
void						ft_free_paths(char **path_splited, char *pathCopy);
void						ft_free_paths2(char **path_splited, char *pathCopy,
								char *fullPath);
int							nb_pipe(t_token *info);

// Dossier exec
void	exec(t_token *tokens, t_commande *cmd, char ***env, t_global_parsing **g_pars);
void ft_check_error_exec(char **cmd);

#endif
