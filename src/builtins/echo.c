/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 15:59:33 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/18 02:36:34 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		ft_print_echo_arg(char **args, int i, int print_newline);

int		ft_print_fullarg(int j, char *full_arg, char *tmp);

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
	g_code_exit = ft_print_echo_arg(args, i, print_newline);
}

int	ft_print_echo_arg(char **args, int i, int print_newline)
{
	int		j;
	char	*full_arg;
	char	*tmp;
	int		error;

	j = 0;
	error = SUCCESS;
	full_arg = NULL;
	tmp = full_arg;
	ft_get_full_arg(args, &full_arg, i);
	free(tmp);
	error = ft_print_fullarg(j, full_arg, tmp);
	if (print_newline)
		write(1, "\n", 1);
	return (error);
}

int	ft_print_fullarg(int j, char *full_arg, char *tmp)
{
	int	error;

	error = SUCCESS;
	if (full_arg != NULL)
	{
		tmp = full_arg;
		full_arg = ft_substr(full_arg, j, ft_strlen(full_arg));
		j = 0;
		while (full_arg[j])
		{
			if (write(1, &full_arg[j], 1) == -1)
			{
				perror("echo: write error");
				error = ERROR;
				break ;
			}
			j++;
		}
		free(full_arg);
		free(tmp);
	}
	return (error);
}
