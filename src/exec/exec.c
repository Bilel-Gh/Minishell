/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:21:17 by ncharii           #+#    #+#             */
/*   Updated: 2023/08/29 14:23:54 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	start_exec_one(t_token *tokens, char **cmd, t_exec *exec, char ***env)
{
	int	info;

	info = 0;
	exec->tokens = tokens;
	info = gestion_infile(tokens, exec);
	if (info == -1)
		return (-1);
	gestion_outfile(tokens, exec);
	if (cmd[0] == NULL)
	{
		file_solo(exec);
		close_for_solo_and_free(exec);
		return (0);
	}
	if (solo_exec(cmd, exec, env, tokens) == -1)
		return (-2);
	return (info);
}

int	start_exec_mult(t_token *tokens, char **cmd, t_exec *exec, char ***env)
{
	int	info;

	info = 0;
	exec->tokens = tokens;
	info = gestion_infile(tokens, exec);
	if (info == -1)
		return (-1);
	gestion_outfile(tokens, exec);
	if (start_exec(cmd, exec, env, tokens) == -1)
		return (-2);
	return (info);
}

int	exec_one(t_token *tokens, t_commande *cmd, char ***env, t_exec *exec)
{
	t_token		*info_token;
	t_commande	*commande;
	int			i;

	i = 0;
	commande = cmd;
	info_token = get_info_token(tokens, i);
	if (!info_token)
		return (0);
	if (start_exec_one(info_token, commande->cmd, exec, env) == -1)
	{
		free_list_tokens(info_token);
		free_name_file(exec);
		free_db_array(exec->path);
		return (0);
	}
	commande = commande->next;
	free_list_tokens(info_token);
	free_name_file(exec);
	return (1);
}

int	exec_multi(t_token *tokens, t_commande *cmd, char ***env, t_exec *exec)
{
	t_token		*info_token;
	t_commande	*commande;
	int			i;

	i = 0;
	commande = cmd;
	while (commande)
	{
		info_token = get_info_token(tokens, i);
		if (!info_token)
			return (0);
		i++;
		if (start_exec_mult(info_token, commande->cmd, exec, env) == -1)
		{
			free_list_tokens(info_token);
			return (0);
		}
		free_path(exec);
		commande = commande->next;
		free_list_tokens(info_token);
		free_name_file(exec);
	}
	return (1);
}

void	exec(t_token *tokens, t_commande *cmd, char ***env,
	t_global_parsing **g_pars)
{
	t_exec		exec;
	int			i;

	i = 0;
	get_path(&exec, *env);
	init_exec(&exec);
	exec.g_parsing = *g_pars;
	exec.nb_cmd = nb_pipe(tokens) + 1;
	if (exec.nb_cmd == 1)
	{
		if (!exec_one(tokens, cmd, env, &exec))
		{
			free_db_array(exec.path);
			return ;
		}	
	}
	else
	{
		if (!exec_multi(tokens, cmd, env, &exec))
		{
			free_db_array(exec.path);
			return ;
		}
	}
	free_db_array(exec.path);
}
