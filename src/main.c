/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:36:06 by ncharii           #+#    #+#             */
/*   Updated: 2023/08/28 22:22:04 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <readline/history.h>  // pour faire fonctionner readline sur mac
#include <readline/readline.h> // pour faire fonctionner readline sur mac
#include <string.h>            // a supprimer

#define MAX_ARGS 64
#define MAX_ARG_LENGTH 256

int		g_code_exit = 0;

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
		// TODO dosier null
		// if(getpwd == NULL)
		//	chdir(== home);
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

// Gestionnaire de signal SIGINT //
void	int_handler(int sig)
{
	int	infile;

	infile = 0;
	if (g_code_exit > CHILD)
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
