/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_info_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 17:34:33 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/21 04:35:29 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int is_only_space(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] != ' ')
            return (0);
        i++;
    }
    return (1);
}

int	ft_get_type(char **line_split, int i, int *type)
{
	while (line_split[i])
	{
		if (line_split[i][0] == 34)
			type[i] = ALPHANUM;
		else if (line_split[i][0] == 39)
			type[i] = ALPHANUM;
		else if (is_only_space(line_split[i]))
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

int	*ft_get_info_args(char **line_split, int *give_nb_args)
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
	i = ft_get_type(line_split, i, type);
	*give_nb_args = i;
	i = 0;
	return (type);
}
