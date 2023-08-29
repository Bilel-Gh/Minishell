/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_for_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:19:36 by ncharii           #+#    #+#             */
/*   Updated: 2023/08/29 14:21:48 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_exec(t_exec *exec)
{
	exec->infile = NULL;
	exec->outfile = NULL;
	exec->limiteur = NULL;
	exec->path_cmd = NULL;
	exec->fd_infile = 0;
	exec->fd_outfile = 1;
	exec->fd_in_last_pipe = -1;
	exec->fd_heredoc = -1;
	exec->path_input = 0;
	exec->pos = FIRST;
}

void	free_name_file(t_exec *exec)
{
	if (exec->infile)
	{
		free(exec->infile);
		exec->infile = NULL;
	}
	if (exec->outfile)
	{
		free(exec->outfile);
		exec->outfile = NULL;
	}
	if (exec->limiteur)
	{
		free(exec->limiteur);
		exec->limiteur = NULL;
	}
}

void	ft_free_g_parsing_total(t_global_parsing *g_parsing)
{
	rl_clear_history();
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
	if (g_parsing->exec)
	{
		if (g_parsing->exec->export)
			free_db_array(g_parsing->exec->export);
		free(g_parsing->exec);
	}
	if (*(g_parsing->env))
		free_db_array(*(g_parsing->env));
}

void	free_path(t_exec *exec)
{
	if (exec->path_cmd)
	{
		free(exec->path_cmd);
		exec->path_cmd = NULL;
	}
}
