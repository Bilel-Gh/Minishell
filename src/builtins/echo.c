/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 15:59:33 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/13 20:19:43 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_print_echo_arg(char **args, int i, int print_newline);

int	ft_is_option_echo(char *str)
{
	if (str[0] == '-')
	{
		if (str[1] == 'n')
		{
			str++;
			while (*str == 'n')
				str++;
			if (*str == '\0')
				return (1);
			else
				return (0);
		}
		else
			return (0);
	}
	else
		return (0);
}

void	ft_get_full_arg(char **args, char **full_arg, int i)
{
	int	j;

	j = i;
	if (args[j] == NULL)
		return ;
	while (args[j] != NULL)
	{
		*full_arg = ft_strjoin(*full_arg, args[j]);
		j++;
		if (args[j] != NULL)
			*full_arg = ft_strjoin(*full_arg, " ");
	}
}

void	builtin_echo(char **args)
{
	int	i;
	int	print_newline;

	i = 1;
	print_newline = 1;
	if (args[i] != NULL && ft_is_option_echo(args[i]))
	{
		print_newline = 0;
		i = 2;
		while (args[i] != NULL && ft_is_option_echo(args[i]))
			i++;
	}
	ft_print_echo_arg(args, i, print_newline);
	g_code_exit = SUCCESS;
}

void	ft_print_echo_arg(char **args, int i, int print_newline)
{
	int		j;
	char	*full_arg;
	char	*tmp;

	j = 0;
	full_arg = NULL;
	tmp = full_arg;
	ft_get_full_arg(args, &full_arg, i);
	free(tmp);
	if (full_arg != NULL)
	{
		tmp = full_arg;
		full_arg = ft_substr(full_arg, j, ft_strlen(full_arg));
		printf("%s", full_arg);
		free(full_arg);
		free(tmp);
	}
	if (print_newline)
		printf("\n");
}
