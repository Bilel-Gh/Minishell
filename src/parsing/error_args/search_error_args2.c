/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_error_args2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 22:21:52 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 22:26:03 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

bool	error_quote(int *type_args, int nb_args, char **args)
{
	int	i;

	i = 0;
	printf("\n*********check parsing quote ???***************\n");
	while (i < nb_args)
	{
		if (args[i][0] == 34 || args[i][0] == 39)
		{
			if (quote_is_open(args[i]))
			{
				if (args[i][0] == '"')
					g_code_exit = ERROR_QUOTE_D;
				else
					g_code_exit = ERROR_QUOTE_S;
				return (true);
			}
			if (args[i][0] == 34)
				type_args[i] = 9;
			else
				type_args[i] = 8;
		}
		i++;
	}
	return (false);
}

char	*get_prev_arg(char **arg, int i)
{
	i--;
	while (i >= 0 && arg[i][0] == ' ')
		i--;
	if (i < 0)
		return (NULL);
	return (arg[i]);
}

char	*get_next_arg(char **arg, int i)
{
	i++;
	while (arg[i] && arg[i][0] == ' ')
		i++;
	if (arg[i] == NULL)
		return (NULL);
	return (arg[i]);
}

bool	prev_next_error(char **args, int i)
{
	char	*get_first_prev_arg;
	char	*get_first_next_arg;

	get_first_prev_arg = get_prev_arg(args, i);
	get_first_next_arg = get_next_arg(args, i);
	if (get_first_prev_arg != NULL && get_first_next_arg == NULL)
	{
		g_code_exit = ERROR_PIPE;
		return (true);
	}
	if (get_first_prev_arg == NULL || get_first_next_arg == NULL)
	{
		g_code_exit = ERROR_PIPE;
		return (true);
	}
	if (get_first_prev_arg[0] == '<' || get_first_prev_arg[0] == '>')
	{
		g_code_exit = ERROR_PIPE;
		return (true);
	}
	return (false);
}

bool	ft_is_file(char *str)
{
	if (str == NULL)
		return (false);
	if (str[0] == 34)
		return (true);
	else if (str[0] == 39)
		return (true);
	else if (ft_isprint(str[0]))
		return (true);
	return (false);
}
