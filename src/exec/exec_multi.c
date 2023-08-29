/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:19:31 by ncharii           #+#    #+#             */
/*   Updated: 2023/08/29 14:23:09 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	start_exec(char **cdm, t_exec *info, char ***env)
{
	if (info->pos == FIRST)
		first(cdm, info, env);
	else if (info->nb_cmd == DERNIER)
		last(cdm, info, env);
	else if (info->pos == INTER)
		inter(cdm, info, env);
	info->pos = INTER;
	info->nb_cmd--;
	return (1);
}

int	first(char **cmd, t_exec *info, char ***env)
{
	int		pipefd[2];
	pid_t	pid;

	if (creat_pipe_and_file(info, pipefd) < 0)
		return (-1);
	if (cmd[0] != NULL)
	{
		find_path(info->path, cmd[0], info);
		if (is_bultins_not_fork(cmd, env, info, FIRST))
		{
			g_code_exit = FORK;
			pid = fork();
			if (pid == -1)
				return (perror("error fork"), -1);
			if (pid == 0)
			{
				if (dup_in_fork_first_inter(info, pipefd) == -1)
					return (-1);
				if (exec_cmd(info, env, cmd) == 1)
					return (1);
			}
		}
	}
	close_for_first(pipefd, info);
	return (0);
}

int	inter(char **cmd, t_exec *info, char ***env)
{
	int		pipefd[2];
	pid_t	pid;

	if (gestion_file_inter(info, pipefd) < 0)
		return (-1);
	if (cmd[0] != NULL)
	{
		find_path(info->path, cmd[0], info);
		if (is_bultins_not_fork(cmd, env, info, INTER))
		{
			g_code_exit = FORK;
			pid = fork();
			if (pid == -1)
				return (perror("error fork"), -1);
			if (pid == 0)
			{
				if (dup_in_fork_first_inter(info, pipefd) == -1)
					return (-1);
				if (exec_cmd(info, env, cmd) == -1)
					return (1);
			}
		}
	}
	close_inter(pipefd, info);
	return (0);
}

int	last(char **cmd, t_exec *info, char ***env)
{
	pid_t	pid;

	if (gestion_file_last(info) < 0 && close_last(info))
		return (-1);
	if (cmd[0] != NULL)
	{
		find_path(info->path, cmd[0], info);
		if (is_bultins_not_fork(cmd, env, info, INTER))
		{
			g_code_exit = FORK;
			pid = fork();
			if (pid == -1)
				return (perror("error fork"), -1);
			if (pid == 0)
			{
				if (dup_in_fork_last_solo(info) == -1)
					return (-1);
				if (exec_cmd(info, env, cmd) == 1)
					return (1);
			}
		}
	}
	close_last(info);
	return (0);
}

int	close_last(t_exec *info)
{
	if ((info->infile || info->limiteur) && info->fd_infile != 0)
		close(info->fd_infile);
	if (info->outfile && info->fd_outfile != 1)
		close(info->fd_outfile);
	if (info->fd_in_last_pipe)
		close(info->fd_in_last_pipe);
	while (waitpid(-1, &g_code_exit, 0) != -1)
		;
	printf("g_code_exit sortie de wait = %d\n", g_code_exit);
	if (g_code_exit == 2)
		g_code_exit = CSIGINT; // ! TODO ERROR code d'erreur modifie builtins
	if (g_code_exit > 255)
		g_code_exit = g_code_exit / 256;
	return (1);
}
