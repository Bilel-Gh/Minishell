/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recipe_for_exec6.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 23:18:19 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 23:21:18 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_db_arr_len(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

int	ft_get_type2(char *const *line_split, int i, int *type)
{
	while (line_split[i])
	{
		if (line_split[i][0] == 34)
			type[i] = QUOTE_D;
		else if (line_split[i][0] == 39)
			type[i] = QUOTE_S;
		else if (line_split[i][0] == ' ')
			type[i] = ESPACE;
		else if (line_split[i][0] == '>' || line_split[i][0] == '<')
		{
			type[i] = REDIRECT;
		}
		else if (line_split[i][0] == '|')
		{
			type[i] = PIPE;
		}
		else if (ft_isprint(line_split[i][0]))
		{
			type[i] = ALPHANUM;
		}
		i++;
	}
	return (i);
}

int	*ft_get_info_args2(char **line_split, int *give_nb_args)
{
	int	nb_args;
	int	i;
	int	*type;

	nb_args = 0;
	i = 0;
	if (line_split == NULL)
		return (NULL);
	while (line_split[nb_args])
		nb_args++;
	type = malloc(sizeof(int) * nb_args);
	i = ft_get_type2(line_split, i, type);
	*give_nb_args = i;
	i = 0;
	return (type);
}
