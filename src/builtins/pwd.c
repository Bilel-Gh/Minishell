/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 22:51:14 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/14 16:52:02 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_check_args(char **args)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	if (args[i] == NULL)
		return (0);
	if (args[i][0] == '-')
	{
		while (args[i][j] == '-')
			j++;
		if (args[i][j] != 0)
		{
			ft_fprintf(2, "pwd: -%c: invalid option\n", args[i][j]);
			g_code_exit = MISUSE;
			return (1);
		}
	}
	return (0);
}

void	builtin_pwd(char **args)
{
	char	cwd[SIZE_PATH];

	if (ft_check_args(args))
		return ;
	if (getcwd(cwd, SIZE_PATH))
		printf("%s\n", cwd);
	else
	{
		ft_fprintf(2, "pwd: error retrieving current directory");
		ft_fprintf(2, "getcwd: cannot access parent directories\n");
	}
}
