/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:19:27 by ncharii           #+#    #+#             */
/*   Updated: 2023/08/29 15:36:35 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_bultins_fork(char **cmd, char ***env, t_exec *info)
{
	int					is_bultin;
	t_global_parsing	**info_parsing;
	t_global_exec		**g_exec;

	info_parsing = &(info->g_parsing);
	g_exec = &(info->g_parsing->exec);
	is_bultin = 0;
	g_exec = &(info->g_parsing->exec);
	if ((ft_strcmp(cmd[0], "cd") == 0) && add_one(&is_bultin))
		g_code_exit = builtin_cd(cmd, env);
	if (ft_strcmp(cmd[0], "echo") == 0 && add_one(&is_bultin))
		builtin_echo(cmd);
	else if (ft_strcmp(cmd[0], "pwd") == 0 && add_one(&is_bultin))
		builtin_pwd(cmd);
	else if (ft_strcmp(cmd[0], "exit") == 0 && add_one(&is_bultin))
		builtin_exit(cmd, info_parsing, info);
	else if (ft_strcmp(cmd[0], "env") == 0 && add_one(&is_bultin))
		builtin_env(cmd, *env);
	else if (ft_strcmp(cmd[0], "export") == 0 && add_one(&is_bultin))
		builtin_export(cmd, env, g_exec);
	return (is_bultin);
}

int	is_bultins_not_fork(char **cmd, char ***env, t_exec *info, int pos)
{
	t_global_parsing	**info_parsing;
	t_global_exec		**g_exec;
	int					info_return;

	info_return = 1;
	info_parsing = &(info->g_parsing);
	g_exec = &(info->g_parsing->exec);
	if ((ft_strcmp(cmd[0], "cd") == 0) && (pos == DERNIER)
		&& add_zero(&info_return))
		g_code_exit = builtin_cd(cmd, env);
	else if (ft_strcmp(cmd[0], "unset") == 0 && add_zero(&info_return))
		builtin_unset(cmd, env, g_exec);
	else if (ft_strcmp(cmd[0], "exit") == 0
		&& add_zero(&info_return) && (pos == DERNIER))
		builtin_exit(cmd, info_parsing, info);
	else if ((ft_strcmp(cmd[0], "export") == 0) && (cmd[1] != NULL)
		&& add_zero(&info_return) && (pos == DERNIER))
		builtin_export(cmd, env, g_exec);
	return (info_return);
}

void	null_cmd(t_exec *info)
{
	ft_fprintf(2, "bash: : command not found\n");
	g_code_exit = NOTFOUND;
	free(info->path_cmd);
	ft_free_g_parsing_total(info->g_parsing);
	return (exit(g_code_exit));
}

void	fail_execve(t_exec *info, char **cmd, char *path, t_token *tokens)
{
	free(info->path_cmd);
	if (path == NULL)
	{
		ft_fprintf(2, "bash: %s: No such file or directory\n", cmd[0]);
		g_code_exit = NOTFOUND;
	}
	else
		ft_check_error_exec(cmd);
	free_list_tokens(tokens);
	free_name_file(info);
	free_db_array(info->path);
	ft_free_g_parsing_total(info->g_parsing);
	free(path);
	exit(g_code_exit);
}

int	exec_cmd(t_exec *info, char ***env, char **cmd, t_token *tokens)
{
	int		exec_bultins;
	char	*path;

	g_code_exit = CSIGINT;
	path = ft_getenv("PATH", *env);
	if (info->fd_infile != 0)
	{
		dup2(info->fd_infile, 0);
		close(info->fd_infile);
	}
	exec_bultins = ft_bultins_fork(cmd, env, info);
	if (exec_bultins != 0)
	{	
		free(info->path_cmd);
		free_db_array(info->path);
		free_list_tokens(tokens);
		ft_free_g_parsing_total(info->g_parsing);
		return (free_name_file(info), free(path), exit(g_code_exit), -1);
	}
	if (cmd[0] == NULL)
		null_cmd(info);
	if (execve(info->path_cmd, cmd, *env) == -1)
		fail_execve(info, cmd, path, tokens);
	g_code_exit = SUCCESS;
	return (1);
}
