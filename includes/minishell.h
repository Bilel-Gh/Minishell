/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 04:32:12 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/01 20:34:13 by bghandri         ###   ########.fr       */
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

enum e_token_type {
    ALPHANUM,
    REDIRECT,
	RED_IN,
	RED_OUT,
    PIPE,
	ESPACE,
	RED_D_IN,
	RED_D_OUT,
	QUOTE_S,
	QUOTE_D,
	LAST
};

enum e_node_type {
    COMMANDE,
    FICHIER,
    ARG,
    OPERATOR,
    OPTION,
    N_PIPE,
    ERROR,
    ENV,
    EXPAND,
    REDIRECT_OUT,
    REDIRECT_IN,
    REDIRECT_D_OUT,
    REDIRECT_D_IN,
};
// test de ce a quoi vont les structures

// infos sur tout le node (a completer)
typedef struct		node_info
{
	enum e_node_type	type;
}					t_node_info;

// chaque charactere est un token
typedef struct		s_token
{
	char				value;
	enum e_token_type	type;
	struct s_token	*next;
}					t_token;

// chaque node est un ensemble de charactere du meme type
typedef struct		node
{
	char			*value;
	t_node_info		*info;
	struct node		*next;
}					t_node;

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
int *ft_get_info_args(char **line_split , int *give_nb_node);
char **join_inter_space(char **args, int *type_args, int *nb_args);
char **kick_quote(int *type_args, int nb_args, char **args);
t_node* ft_get_nodes_with_infos(char **args, int *info_args, int nb_args);
int	ft_isprint(int chara);
t_node* addNode(t_node* head, char* value);

// Dossier UTILS
char	*ft_strjoin(char *s1, char *s2);
int	ft_strlen(const char *str);

// Dossier LEXEUR
t_node* mergeTokens(t_token* head);

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

#endif
