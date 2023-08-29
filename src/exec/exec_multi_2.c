/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multi_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:19:33 by ncharii           #+#    #+#             */
/*   Updated: 2023/08/29 14:28:58 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_for_first(int *pipefd, t_exec *info)
{
	close(pipefd[1]);
	if ((info->infile || info->limiteur) && info->fd_infile != 0)
	{
		printf("close infile first= %d\n", info->fd_infile);
		close(info->fd_infile);
	}
	if (info->outfile && info->fd_outfile != 1)
	{
		printf("close outfile first= %d\n", info->fd_outfile);
		close(info->fd_outfile);
	}
	info->fd_in_last_pipe = pipefd[0];
}

int	creat_pipe_and_file(t_exec *info, int *pipefd)
{
	int	error;

	error = 1;
	if (pipe(pipefd) == -1)
	{
		perror("error pipe");
		error = -1;
	}
	if (info->fd_infile == -1 || info->fd_outfile == -1)
	{
		g_code_exit = 1;
		error = -1;
	}
	if ((!info->infile && !info->limiteur) || info->fd_infile == -1)
	{
		info->fd_infile = 0;
	}
	if (!info->outfile || info->fd_outfile == -1)
		info->fd_outfile = pipefd[1];
	if (error == -1)
		close_for_first(pipefd, info);
	return (error);
}

int	dup_in_fork_first_inter(t_exec *info, int *pipefd)
{
	if (dup2(info->fd_outfile, 1) == -1)
		return (perror("error dup first"), -1);
	if (info->outfile)
		close(info->fd_outfile);
	close(pipefd[0]);
	close(pipefd[1]);
	return (0);
}

void	close_inter(int *pipefd, t_exec *info)
{
	if ((info->infile || info->limiteur)
		&& (info->fd_infile != info->fd_in_last_pipe) && (info->fd_infile != 0))
		close(info->fd_infile);
	if (info->outfile && info->fd_outfile != pipefd[1])
		close(info->fd_outfile);
	close(pipefd[1]);
	if (info->fd_in_last_pipe)
		close(info->fd_in_last_pipe);
	info->fd_in_last_pipe = pipefd[0];
}

int	gestion_file_inter(t_exec *info, int *pipefd)
{
	int	error;

	error = 1;
	if (pipe(pipefd) == -1)
		perror("error pipe");
	if (info->fd_infile == -1 || info->fd_outfile == -1)
	{
		g_code_exit = 1;
		error = -1;
	}
	if ((!info->infile && !info->limiteur) || info->fd_infile == -1)
		info->fd_infile = info->fd_in_last_pipe;
	if (!info->outfile || info->fd_outfile == -1)
		info->fd_outfile = pipefd[1];
	if (error == -1)
		close_inter(pipefd, info);
	return (error);
}
