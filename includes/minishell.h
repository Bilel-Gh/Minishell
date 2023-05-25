/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 04:32:12 by bghandri          #+#    #+#             */
/*   Updated: 2023/05/25 10:22:01 by bghandri         ###   ########.fr       */
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

// #include "../src/builtins/builtins.c"
// #include "../src/builtins/b_export.c"
// #include "../src/parsing/lexeur.c"
// #include "../src/utils/split.c"

// #include "../src/utils/split.c"

extern char **environ;

enum e_token_type {
    ALPHANUM,
    REDIRECT,
    PIPE,
	ESPACE
};
// test de ce a quoi vont les structures

// infos sur tout le node (a completer)
typedef struct		node_info
{
	int is_builtin;
	int is_option;
	int is_cmd;
	int is_output;
	int is_input;
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
char	**ft_split(char *str);

void builtin_echo(char **args);
void builtin_export(char **args);
void builtin_unset(char **args);
void builtin_env();
void builtin_pwd();
void builtin_cd(char **args);
void builtin_exit();

#endif
