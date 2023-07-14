/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 21:38:03 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/14 02:11:05 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	builtin_env(char **args, char **env)
{
	int	i;

	i = 0;
	while (ft_strcmp(args[i], "env") == 0)
	{
		if (args[i + 1] == NULL || ft_strcmp(args[i + 1], "env") != 0)
			break ;
		i++;
	}
	if (args[i + 1] != NULL)
	{
		printf("env: ʻ%s’: No such file or directory\n", args[i + 1]);
		printf("env: should not have arguments\n");
		g_code_exit = NOTFOUND;
		return ;
	}
	while (env[i] != NULL)
	{
		printf("%s\n", env[i]);
		i++;
	}
}
