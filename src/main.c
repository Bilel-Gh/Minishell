/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:36:06 by ncharii           #+#    #+#             */
/*   Updated: 2023/07/22 23:37:25 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <readline/history.h>  // pour faire fonctionner readline sur mac
#include <readline/readline.h> // pour faire fonctionner readline sur mac
#include <string.h>            // a supprimer

#define MAX_ARGS 64
#define MAX_ARG_LENGTH 256

int		g_code_exit = 0;

void	ft_add_quote_exp(const char *str, int equal_found, char *new_str);

int		ft_is_error_parsing(t_global_parsing *g_parsing, int nb_args);

int		ft_is_error_par_part2(t_global_parsing *g_parsing);

void	ft_do_parsing(char ***env, t_global_parsing *g_parsing, int *nb_args);

void	ft_do_exec(char ***env, t_global_parsing *g_parsing, int nb_args);

int		ft_general_error(t_global_parsing *g_parsing);

void	ft_print_error_redirect(char **args);

int		ft_is_error_full_cmd(char *const *cmd, char *full_cmd);

void	ft_get_exit_code_lstat(char *const *cmd, struct stat *path_stat);

void	ft_init_global_parsing(t_global_parsing *g_parsing)
{
	if (!g_parsing)
		g_parsing = NULL;
	g_parsing->args = NULL;
	g_parsing->info_args = NULL;
	g_parsing->tokens = NULL;
	g_parsing->commande = NULL;
}

void	ft_free_g_parsing(t_global_parsing *g_parsing)
{
	if (g_parsing->line)
		free(g_parsing->line);
	if (g_parsing->args)
		free_db_array(g_parsing->args);
	if (g_parsing->info_args)
		free(g_parsing->info_args);
	if (g_parsing->tokens)
		free_list_tokens(g_parsing->tokens);
	if (g_parsing->commande)
		free_list_commande(g_parsing->commande);
}

void	ft_set_index_for_exec(t_token **tokens)
{
	t_token	*head;
	int		i;

	i = 0;
	head = *tokens;
	while (*tokens)
	{
		while ((*tokens)->info->type != 2)
		{
			(*tokens)->token_index = i;
			*tokens = (*tokens)->next;
			if (!*tokens)
				break ;
		}
		if (!*tokens)
			break ;
		(*tokens)->token_index = i;
		i++;
		*tokens = (*tokens)->next;
	}
	*tokens = head;
}

int	only_misuse(char *str, char *invalid_char)
{
	int	i;
	int	j;

	i = 0;
	if (str == NULL)
		return (1);
	while (str[i] != '\0')
	{
		j = 0;
		while (invalid_char[j] != '\0')
		{
			if (str[i] == invalid_char[j])
				break ;
			j++;
		}
		if (invalid_char[j] == '\0')
			return (0);
		i++;
	}
	return (1);
}

int	ft_check_full_cmd(char *cmd)
{
	char	*invalid_char;

	invalid_char = ";(){}<>|&.";
	if (only_misuse(cmd, invalid_char))
	{
		ft_fprintf(2, "syntax error near unexpected token `%c%c'\n", cmd[0],
			cmd[1]);
		g_code_exit = MISUSE;
		return (1);
	}
	else if (ft_strcmp(cmd, "!") == 0)
	{
		g_code_exit = ERROR;
		return (1);
	}
	return (0);
}

void	ft_check_not_found(char **cmd)
{
	struct stat	path_stat;

	if (lstat(cmd[0], &path_stat) == 0)
		ft_get_exit_code_lstat(cmd, &path_stat);
	else if (errno == ENOENT)
	{
		ft_fprintf(2, "bash: %s: command not found\n", cmd[0]);
		g_code_exit = NOTFOUND;
	}
	else
	{
		ft_fprintf(2, "bash: %s: error: %s\n", cmd[0], strerror(errno));
		g_code_exit = ERROR;
	}
}

void	ft_get_exit_code_lstat(char *const *cmd, struct stat *path_stat)
{
	if (S_ISDIR((*path_stat).st_mode))
	{
		ft_fprintf(2, "bash: %s: is a directory\n", cmd[0]);
		g_code_exit = CANTEXEC;
	}
	else if (errno == EACCES)
	{
		ft_fprintf(2, "bash: %s: Permission denied\n", cmd[0]);
		g_code_exit = CANTEXEC;
	}
	else
	{
		ft_fprintf(2, "bash: %s: command not found\n", cmd[0]);
		g_code_exit = NOTFOUND;
	}
}

void	ft_check_error_exec(char **cmd)
{
	char	*full_cmd;
	int		i;

	full_cmd = ft_strdup(cmd[0]);
	i = 1;
	while (cmd[i] != NULL)
	{
		full_cmd = ft_strjoin(full_cmd, cmd[i]);
		i++;
	}
	if (ft_is_error_full_cmd(cmd, full_cmd))
		return ;
	ft_check_not_found(cmd);
	free(full_cmd);
}

int	ft_is_error_full_cmd(char *const *cmd, char *full_cmd)
{
	if (full_cmd == NULL)
	{
		ft_fprintf(2, "bash: %s: command not found\n", cmd[0]);
		g_code_exit = NOTFOUND;
		free(full_cmd);
		return (1);
	}
	if (ft_strcmp(full_cmd, ">") == 0)
	{
		g_code_exit = SUCCESS;
		free(full_cmd);
		return (1);
	}
	if (ft_check_full_cmd(full_cmd))
	{
		free(full_cmd);
		return (1);
	}
	return (0);
}

void	gestion_pipe2(char ***env, t_global_parsing **g_parsing, int *nb_args)
{
	char	*additional_input;
	char	*new_line;

	if (ft_strcmp((*g_parsing)->args[0], "echo") == 0)
	{
		g_code_exit = ERROR_PIPE;
		return ;
	}
	additional_input = readline(">");
	new_line = ft_strjoin((*g_parsing)->line, additional_input);
	(*g_parsing)->line = new_line;
	(*g_parsing)->args = ft_lexeur((*g_parsing)->line);
	(*g_parsing)->info_args = ft_get_info_args((*g_parsing)->args, nb_args);
	(*g_parsing)->args = ft_parsing(nb_args, g_parsing, env);
	free(additional_input);
}

void	gestion_unclosed_quote(char ***env, t_global_parsing **g_parsing,
		int *nb_args)
{
	char	*additional_input;
	char	*new_line;

	additional_input = readline(">");
	new_line = ft_strjoin((*g_parsing)->line, additional_input);
	if (g_code_exit == ERROR_QUOTE_D)
	{
		if (ft_strchr(additional_input, '"') != NULL)
			g_code_exit = SUCCESS;
	}
	else if (g_code_exit == ERROR_QUOTE_S)
	{
		if (ft_strchr(additional_input, '\'') != NULL)
			g_code_exit = SUCCESS;
	}
	(*g_parsing)->line = new_line;
	(*g_parsing)->args = ft_lexeur((*g_parsing)->line);
	(*g_parsing)->info_args = ft_get_info_args((*g_parsing)->args, nb_args);
	(*g_parsing)->args = ft_parsing(nb_args, g_parsing, env);
	free(additional_input);
}

int	ft_custom_error(char **args)
{
	if (g_code_exit == ERROR_PIPE)
	{
		ft_fprintf(2, "bash: syntax error near unexpected token '%c'\n", '|');
		return (1);
	}
	if (g_code_exit == ERROR_REDIRECT)
	{
		ft_print_error_redirect(args);
		return (1);
	}
	if (g_code_exit == ERROR_QUOTE_D)
	{
		ft_fprintf(2, "bash: command not found (unclosed quote) : '\"'\n");
		return (1);
	}
	if (g_code_exit == ERROR_QUOTE_S)
	{
		ft_fprintf(2, "bash: command not found (unclosed quote) : '\''\n");
		return (1);
	}
	return (0);
}

void	ft_print_error_redirect(char **args)
{
	if (ft_db_arr_len(args) == 1 && ft_strlen(args[0]) <= 2)
		ft_fprintf(2, "bash: syntax error near unexpected token 'newline'\n");
	else if (args[0][0] == '>' || args[0][0] == '<')
		ft_fprintf(2, "bash: syntax error near unexpected token '%c%c'\n",
			args[0][0], args[0][1]);
	else
		ft_fprintf(2, "bash: syntax error near unexpected token '>'\n");
}

void	minishell_loop(char ***env, t_global_exec *g_exec,
		t_global_parsing *g_parsing)
{
	int	nb_args;

	ft_init_global_parsing(g_parsing);
	g_parsing->env = env;
	while (1)
	{
		g_parsing->exec = g_exec;
		g_parsing->line = readline("minishell > ");
		if (g_parsing->line == NULL)
		{
			ft_free_g_parsing_total(g_parsing);
			break ;
		}
		rl_replace_line("", 0);
		if (ft_general_error(g_parsing))
			continue ;
		add_history(g_parsing->line);
		ft_do_parsing(env, g_parsing, &nb_args);
		if (ft_is_error_parsing(g_parsing, nb_args))
			continue ;
		ft_do_exec(g_parsing->env, g_parsing, nb_args);
		printf("\033[1;36mexit code FINAL = %d\n\033[0m", g_code_exit);
		ft_free_g_parsing(g_parsing);
		ft_init_global_parsing(g_parsing);
	}
}

int	ft_general_error(t_global_parsing *g_parsing)
{
	if (ft_strlen(g_parsing->line) == 0 || ft_strcmp(g_parsing->line, ":") == 0
		|| ft_strcmp(g_parsing->line, ";") == 0
		|| is_only_space(g_parsing->line) == 1)
	{
		g_code_exit = SUCCESS;
		return (1);
	}
	return (0);
}

void	ft_do_exec(char ***env, t_global_parsing *g_parsing, int nb_args)
{
	g_parsing->tokens = ft_get_tokens_with_infos(g_parsing->args, nb_args);
	g_parsing->commande = cmd_complete(g_parsing->tokens);
	ft_set_index_for_exec(&g_parsing->tokens);
	t_token	*head;
	head = NULL;
	FT_PRINT_TOKEN_DEBUG(g_parsing, head);
	exec(g_parsing->tokens, g_parsing->commande, env, &g_parsing);
	rl_replace_line("", 0);
}

void	ft_do_parsing(char ***env, t_global_parsing *g_parsing, int *nb_args)
{
	g_parsing->args = ft_lexeur(g_parsing->line);
	g_parsing->info_args = ft_get_info_args(g_parsing->args, nb_args);
	g_parsing->args = ft_parsing(nb_args, &g_parsing, env);
}

int	ft_is_error_parsing(t_global_parsing *g_parsing, int nb_args)
{
	if (g_code_exit == ERROR_REDIRECT2)
	{
		g_code_exit = ERROR;
		ft_free_g_parsing(g_parsing);
		ft_init_global_parsing(g_parsing);
		return (1);
	}
	else if (g_code_exit == NOTFOUND)
	{
		ft_fprintf(2, "bash: : command not found\n");
		return (1);
	}
	if (nb_args == 0)
	{
		g_code_exit = SUCCESS;
		ft_free_g_parsing(g_parsing);
		return (1);
	}
	if (ft_is_error_par_part2(g_parsing))
		return (1);
	return (0);
}

int	ft_is_error_par_part2(t_global_parsing *g_parsing)
{
	if (g_code_exit != SUCCESS)
	{
		if (!ft_custom_error(g_parsing->args))
			ft_fprintf(2, "bash: syntax error near unexpected token '%c%c'\n",
				g_parsing->args[0][0], g_parsing->args[0][1]);
		g_code_exit = MISUSE;
		ft_free_g_parsing(g_parsing);
		return (1);
	}
	if (!g_parsing->args)
		return (1);
	else
		return (0);
}

char	**ft_db_array_dup(char **db_array)
{
	int		i;
	char	**db_array_cpy;

	i = 0;
	while (db_array[i])
		i++;
	db_array_cpy = malloc(sizeof(char *) * (i + 1));
	if (!db_array_cpy)
		return (NULL);
	i = 0;
	while (db_array[i])
	{
		db_array_cpy[i] = ft_strdup(db_array[i]);
		i++;
	}
	db_array_cpy[i] = NULL;
	return (db_array_cpy);
}

char	*ft_add_quotes_export(char *str)
{
	int		equal_found;
	int		len;
	char	*new_str;

	equal_found = 0;
	len = ft_strlen(str);
	new_str = malloc(sizeof(char) * (len + 3));
	if (new_str == NULL)
		return (NULL);
	ft_add_quote_exp(str, equal_found, new_str);
	free(str);
	return (new_str);
}

void	ft_add_quote_exp(const char *str, int equal_found, char *new_str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '=' && !equal_found)
		{
			new_str[j++] = str[i++];
			new_str[j++] = '"';
			equal_found = 1;
			continue ;
		}
		new_str[j++] = str[i++];
	}
	if (equal_found)
		new_str[j++] = '"';
	new_str[j] = '\0';
}

char	**ft_get_export(char **env)
{
	int		i;
	int		j;
	char	**export;
	char	*suffix;

	suffix = ft_strdup("export ");
	if (!suffix)
		return (NULL);
	i = 0;
	j = 0;
	while (env[i])
		i++;
	export = malloc(sizeof(char *) * (i + 1));
	export[i] = NULL;
	while (j < i)
	{
		export[j] = ft_strjoin(suffix, env[j]);
		export[j] = ft_add_quotes_export(export[j]);
		suffix = ft_strdup("export ");
		if (!suffix)
			return (NULL);
		j++;
	}
	free(suffix);
	return (export);
}

// Gestionnaire de signal SIGINT //
void	int_handler(int sig)
{
	int	infile;

	infile = 0;
	if (g_code_exit >= CHILD)
	{
		infile = g_code_exit - 1000;
		close(infile);
		exit(0);
		g_code_exit = 998;
	}
	else if (sig == SIGINT && g_code_exit != FORK)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_code_exit = CSIGINT;
	}
}

void	quit_handler(int sig)
{
	if (sig == SIGQUIT && g_code_exit >= CHILD && g_code_exit != FORK)
	{
		if (rl_line_buffer && ft_strlen(rl_line_buffer) > 0)
		{
			ft_fprintf(2, "Quit (core dumped)\n");
			ft_strlen(rl_line_buffer);
			exit(131);
		}
	}
}

int	main(int argc, char **argv, char **env)
{
	char				**env_cpy;
	t_global_exec		*g_exec;
	t_global_parsing	g_parsing;
	struct sigaction	s_sigaction;

	(void)argc;
	(void)argv;
	g_code_exit = 0;
	env_cpy = ft_db_array_dup(env);
	g_exec = malloc(sizeof(t_global_exec));
	g_exec->export = ft_get_export(env_cpy);
	ft_bzero(&s_sigaction, sizeof(struct sigaction));
	unlink("/tmp/here_doc_minishell");
	s_sigaction.sa_handler = int_handler;
	rl_catch_signals = 0;
	sigaction(SIGINT, &s_sigaction, NULL);
	sigaction(EOF, &s_sigaction, NULL);
	s_sigaction.sa_handler = quit_handler;
	sigaction(SIGQUIT, &s_sigaction, NULL);
	minishell_loop(&env_cpy, g_exec, &g_parsing);
	rl_clear_history();
	rl_catch_signals = 1;
	return (0);
}
