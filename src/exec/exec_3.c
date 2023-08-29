/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:19:24 by ncharii           #+#    #+#             */
/*   Updated: 2023/08/29 14:24:21 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*for_test(char *test, char *argv)
{
	char	*str;
	int		i;
	int		y;

	i = 0;
	y = 0;
	str = malloc(ft_strlen(test) + ft_strlen(argv) + 2);
	while (test[i])
	{
		str[i] = test[i];
		i++;
	}
	str[i] = '/';
	i++;
	while (argv[y])
	{
		str[i] = argv[y];
		i++;
		y++;
	}
	str[i] = '\0';
	return (str);
}

int	find_path(char **path, char *cmd, t_exec *info)
{
	char	*test;

	info->i = 0;
	if (cmd == 0)
		return (0);
	test = ft_strchr(cmd, 47);
	if (test != 0 || path == NULL)
	{
		info->path_cmd = ft_strdup(cmd);
		info->path_input = 1;
		return (1);
	}
	if (path == NULL)
		return (0);
	while (path[info->i] && path[0] != NULL)
	{
		test = for_test(path[info->i], cmd);
		if (access(test, F_OK) == 0)
			return (info->path_cmd = test, 1);
		info->i++;
		if (path[info->i])
			free(test);
	}
	info->path_cmd = test;
	return (1);
}

int	add_one(int *is_bultin)
{
	*is_bultin = 1;
	return (1);
}

int	add_zero(int *info_return)
{
	*info_return = 0;
	return (1);
}

int	wait_heredoc(int info, t_exec *exec, int tmp_error, pid_t pid)
{
	while (waitpid(pid, &info, 0) == -1 && info != 32718)
		;
	g_code_exit = tmp_error;
	if (info > 255)
		info = info / 256;
	if (info == 0)
		return (g_code_exit = 0, -1); // ! TODO ERROR le fait de renvoyer -1 ici fait exit le parent apres un controle c dans le heredoc je pense pas que ce soit voulu mais je change rien pr l'instant
	exec->fd_infile = open("/tmp/here_doc_minishell", O_RDONLY);
	if (exec->fd_infile == -1)
		perror("error open heredoc");
	return (0);
}
