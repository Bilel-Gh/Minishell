/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:19:38 by ncharii           #+#    #+#             */
/*   Updated: 2023/08/29 15:33:24 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	write_in_heredoc(int fd_heredoc, char *line, int size_line)
{
	write(fd_heredoc, line, size_line);
	write(fd_heredoc, "\n", 1);
	free(line);
}

void	quit_heredoc(t_exec *exec, char *line)
{
	rl_catch_signals = 0;
	free(line);
	close(exec->fd_infile);
	free(exec->limiteur);
	free_db_array(exec->path);
	free_list_tokens(exec->tokens);
	ft_free_g_parsing_total(exec->g_parsing);
	exit(exec->fd_infile);
}

void	heredoc_loop(t_exec *exec, int exit_code, char *line, char ***env)
{
	while (1)
	{
		line = readline("> ");
		if (!line || g_code_exit == CSIGINT)
		{
			ft_fprintf(2, "bash: warning: here-document \
delimited by end-of-file (wanted `%s')", exec->limiteur);
			g_code_exit = SUCCESS;
			close(exec->fd_infile);
			free_db_array(exec->path);
			free_list_tokens(exec->tokens);
			ft_free_g_parsing_total(exec->g_parsing);
			return (free(exec->limiteur), exit(0));
		}
		if (!ft_strcmp(line, exec->limiteur))
		{
			g_code_exit = SUCCESS;
			break ;
		}
		else if (ft_strncmp(line, "$?", 2) == 0)
			line = exit_code_expande(line, exit_code);
		else
			line = importe_expande(line, *env);
		write_in_heredoc(exec->fd_infile, line, ft_strlen(line));
	}
}

void	start_heredoc(t_exec *exec, int exit_code)
{
	char	*line;
	char	***env;

	env = exec->g_parsing->env;
	g_code_exit = CHILD;
	line = NULL;
	exec->fd_infile = open("/tmp/here_doc_minishell",
			O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (exec->fd_infile == -1)
	{
		perror("open");
		return ;
	}
	g_code_exit = g_code_exit + exec->fd_infile;
	rl_catch_signals = 1;
	heredoc_loop(exec, exit_code, line, env);
	quit_heredoc(exec, line);
}

int	new_heredoc(t_exec *exec, t_token *token)
{
	int		info;
	int		tmp_error;
	pid_t	pid;

	info = 0;
	tmp_error = g_code_exit;
	if (exec->infile)
		destruction_infile(exec);
	if (exec->limiteur)
		dectruction_heredoc(exec);
	exec->limiteur = ft_strdup(token->value);
	if (!exec->limiteur)
		return (-1);
	else
	{
		g_code_exit = FORK;
		pid = fork();
		if (pid == -1)
			return (perror("error fork"), -1);
		if (pid == 0)
			start_heredoc(exec, tmp_error);
		wait_heredoc(info, exec, tmp_error, pid);
	}
	return (0);
}
