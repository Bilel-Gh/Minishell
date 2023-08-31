/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_solo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:19:34 by ncharii           #+#    #+#             */
/*   Updated: 2023/08/29 15:14:44 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	gestion_file_last(t_exec *info)
{
	int	error;

	error = 1;
	if (info->fd_infile == -1 || info->fd_outfile == -1)
	{
		g_code_exit = 1;
		error = -1;
	}
	if ((!info->infile && !info->limiteur) || info->fd_infile == -1)
	{
		info->fd_infile = info->fd_in_last_pipe;
	}
	if (!info->outfile || info->fd_outfile == -1)
		info->fd_outfile = 1;
	return (error);
}

int	dup_in_fork_last_solo(t_exec *info)
{
	if (info->outfile)
	{
		if (dup2(info->fd_outfile, 1) == -1)
			return (perror("error dup last"), -1);
		close(info->fd_outfile);
	}
	return (0);
}

void	close_for_solo_and_free(t_exec *info)
{
	while (waitpid(-1, &g_code_exit, 0) != -1 || g_code_exit == 355)
		;
	if (g_code_exit == 131)
		ft_fprintf(2, "Quit (core dumped)\n");
	if (g_code_exit == 2)
		g_code_exit = CSIGINT;
	if (g_code_exit > 255)
		g_code_exit = g_code_exit / 256;
	if (g_code_exit == 13)
		g_code_exit = 2;
	if ((info->infile || info->limiteur) && info->fd_infile > 0)
		close(info->fd_infile);
	if (info->outfile && info->fd_outfile != 1)
		close(info->fd_outfile);
	if (info->path_cmd)
		free(info->path_cmd);
}

int	file_solo(t_exec *info)
{
	int	error;

	error = 1;
	if (info->fd_infile == -1 || info->fd_outfile == -1)
	{
		g_code_exit = 1;
		error = -1;
	}
	if ((!info->infile && !info->limiteur) || info->fd_infile == -1)
		info->fd_infile = 0;
	if (!info->outfile || info->fd_outfile == -1)
		info->fd_outfile = 1;
	return (error);
}

int	solo_exec(char **cmd, t_exec *info, char ***env, t_token *tokens)
{
	pid_t	pid;

	if (file_solo(info) < 0)
	{
		close_for_solo_and_free(info);
		return (-1);
	}
	find_path(info->path, cmd[0], info);
	if (is_bultins_not_fork(cmd, env, info, DERNIER))
	{
		g_code_exit = FORK;
		pid = fork();
		if (pid == -1)
			return (perror("error fork"), -1);
		if (pid == 0)
		{
			if (dup_in_fork_last_solo(info) == -1)
				return (-1);
			if (exec_cmd(info, env, cmd, tokens) == 1)
				return (-1);
		}
	}
	close_for_solo_and_free(info);
	return (0);
}
