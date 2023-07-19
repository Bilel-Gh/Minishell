/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 20:43:23 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/19 00:57:36 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

bool	quote_is_open(char *args)
{
	int	state;
	int	i;

	state = 0;
	i = 0;
	while (args[i])
	{
		if (args[i] == 34 && state == 1)
			state = 0;
		else if (args[i] == 34 && state == 0)
			state = 1;
		else if (args[i] == 39 && state == 2)
			state = 0;
		else if (args[i] == 39 && state == 0)
			state = 2;
		i++;
	}
	if (state == 0)
		return (false);
	return (true);
}

void	give_sp_args_redi(char *args, int *type_args, int pos)
{
	int	size_args;

	size_args = ft_strlen(args);
	if (size_args == 1 && args[0] == '<')
		type_args[pos] = 2;
	else if (size_args == 2 && args[0] == '<')
		type_args[pos] = 6;
	else if (size_args == 1 && args[0] == '>')
		type_args[pos] = 3;
	else if (size_args == 2 && args[0] == '>')
		type_args[pos] = 7;
	return ;
}

bool	error_size(char *args, int size_max)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	if (i > size_max)
		return (true);
	return (false);
}

bool	error_no_only_type(char *str)
{
	char	style;
	int		i;

	i = 0;
	style = str[0];
	while (str[i])
	{
		if (str[i] != style)
			return (true);
		i++;
	}
	return (false);
}
