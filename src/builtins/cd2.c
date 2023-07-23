/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 19:51:46 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 20:09:27 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_init_cd_var(char **const *env, char **home, char **prev_dir)
{
	(*home) = ft_getenv("HOME", *env);
	(*prev_dir) = ft_getenv("OLDPWD", *env);
}

void	ft_cd_tiret(char **const *env, char *prev_dir)
{
	(void)env;
	if (prev_dir != NULL)
	{
		if (chdir(prev_dir) != 0)
		{
			free(prev_dir);
			perror("chdir");
			g_code_exit = ERROR;
		}
	}
	else
	{
		free(prev_dir);
		ft_fprintf(2, "cd: prev dir not found\n");
		g_code_exit = ERROR;
	}
	printf("%s\n", prev_dir);
}

void	ft_cd_tild(char **const *env, char *home)
{
	(void)env;
	if (home != NULL && home[0] != '\0')
	{
		if (home != NULL)
		{
			if (chdir(home) != 0)
			{
				free(home);
				perror("chdir");
				g_code_exit = ERROR;
			}
		}
		else
		{
			ft_fprintf(2, "cd: home not found\n");
			g_code_exit = ERROR;
		}
	}
}
