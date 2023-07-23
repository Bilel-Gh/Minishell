/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 23:33:09 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 23:33:25 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	fist_error(int *type_args)
{
	if ((type_args[0] != 0 && type_args[0] != 1) || (type_args[0] == 1
			&& type_args[1] != 0))
		return (true);
	return (false);
}

bool	error_grammaticale(int *type_args, int nb_args)
{
	int	i;

	i = 0;
	if (!type_args)
		return (false);
	printf("*********check grammaticale error ???***************\n");
	if (nb_args == 1 && type_args[i] != 0)
		return (true);
	while (i < nb_args - 1)
	{
		if (type_args[i] == 5)
			i++;
		if (i == 0 && fist_error(type_args))
			return (true);
		if (i != 0)
		{
			if (type_args[i] == 4 && type_args[i + 1] != 0 && type_args[i
					+ 1] != 1)
				return (true);
			if (type_args[i] == 1 && type_args[i + 1] != 0)
				return (true);
		}
		i++;
	}
	return (false);
}

int	count_nb_space(int *type_args, int nb_args)
{
	int	i;
	int	nb_space;

	i = 0;
	nb_space = 0;
	printf("nb_args = %d", nb_args);
	if (type_args == NULL)
		return (nb_space);
	while (i < nb_args - 1)
	{
		if (type_args[i] == ESPACE)
			nb_space++;
		i++;
	}
	return (nb_space);
}

void	b_zero_last_args(char **last_args, int nb_new_args, int i)
{
	last_args[nb_new_args] = 0;
	while (i < nb_new_args)
	{
		last_args[i] = 0;
		i++;
	}
}

char	**kick_args_space(char **new_args, int *type_args, int *nb_args)
{
	char	**last_args;
	int		nb_new_args;
	int		i;
	int		j;

	i = 0;
	j = 0;
	nb_new_args = *nb_args - count_nb_space(type_args, *nb_args);
	if (nb_new_args == 0)
		return (NULL);
	last_args = malloc(sizeof(char *) * (nb_new_args + 1));
	if (!last_args)
		return (NULL);
	b_zero_last_args(last_args, nb_new_args, i);
	while (i < *nb_args)
	{
		if (type_args[i] == ESPACE)
			i++;
		if (i == *nb_args)
			return (*nb_args = nb_new_args, last_args);
		last_args[j] = ft_strjoin(last_args[j], new_args[i]);
		i++;
		j++;
	}
	return (*nb_args = nb_new_args, last_args);
}
