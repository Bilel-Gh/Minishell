/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 00:22:52 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/24 00:23:01 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
