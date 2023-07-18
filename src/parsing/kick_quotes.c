/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 03:35:14 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/10 21:18:30 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*copy_sans_quote(char *args)
{
	char	*new_no_quote;
	int		size_args;
	int		size_no_quote;
	int		i;
	int		j;

	i = 1;
	j = 0;
	size_args = ft_strlen(args);
	size_no_quote = size_args - 2;
	new_no_quote = malloc(size_no_quote + 1);
	new_no_quote[size_no_quote] = 0;
	while (j < size_no_quote)
	{
		new_no_quote[j] = args[i];
		i++;
		j++;
	}
	return (new_no_quote);
}

char	*copy_cont(char *str)
{
	char	*copy;
	int		size_str;
	int		i;

	i = 0;
	size_str = ft_strlen(str);
	copy = malloc(size_str + 1);
	if (!copy)
		return (NULL);
	while (str[i])
	{
		copy[i] = str[i];
		i++;
	}
	copy[i] = 0;
	return (copy);
}

char	**kick_quote(int *type_args, int nb_args, char **args)
{
	char	**no_quote;
	int		i;

	i = 0;
	no_quote = malloc(sizeof(char *) * (nb_args + 1));
	no_quote[nb_args] = 0;

	while (i < nb_args)
	{
		if (type_args[i] == QUOTE_D || type_args[i] == QUOTE_S)
			no_quote[i] = copy_sans_quote(args[i]);
		else
			no_quote[i] = copy_cont(args[i]);
		printf("no quote = %s \n", no_quote[i]);
		printf("args = %s \n", args[i]);
		i++;
	}
	no_quote[i] = 0;
	no_quote = ft_clean_null_db_array(no_quote, &nb_args);
	return (no_quote);
}
