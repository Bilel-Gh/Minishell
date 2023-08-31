/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 23:07:31 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 23:07:42 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_size_of_expende(char *expande)
{
	int	size;

	size = 1;
	while (1)
	{
		if ((expande[size] == '@' && size > 1) || (expande[size] == '#'
				&& size > 1) || (expande[size] == '[' && size > 1)
			|| (expande[size] == ']' && size > 1) || (expande[size] == '/'
				&& size > 1) || (expande[size] == '=') || (expande[size] == ':'
				&& size > 1) || (expande[size] == '-' && size > 1)
			|| (expande[size] == '%' && size > 1) || (expande[size] == '$'
				&& size > 1) || (expande[size] == 92 && size > 1)
			|| (expande[size] == ' ' && size > 1) || (expande[size] == 0)
			|| (expande[size] == '.') || (expande[size] == 39)
			|| (expande[size] == 34))
		{
			size--;
			break ;
		}
		size++;
	}
	return (size);
}

char	*get_expende_detect(int size_of_expende, char *expande)
{
	char	*txt_expande;
	int		i;

	i = 0;
	txt_expande = malloc(sizeof(char) * (size_of_expende + 1));
	while (i < size_of_expende && expande)
	{
		txt_expande[i] = expande[i + 1];
		i++;
	}
	txt_expande[i] = 0;
	return (txt_expande);
}

char	*is_sp_expand(char *expand_search)
{
	char	*sp_expand;

	if (!expand_search)
		return (NULL);
	if (expand_search[0] == '@')
	{
		sp_expand = &expand_search[1];
		return (sp_expand);
	}
	return (NULL);
}

char	*give_env_expand(char *expande_search, int size, char **env)
{
	int		i;
	char	*sp_expand;
	char	*for_search;

	i = 0;
	sp_expand = NULL;
	sp_expand = is_sp_expand(expande_search);
	for_search = ft_strdup(expande_search);
	for_search = ft_strjoin(for_search, "=");
	if (sp_expand)
		return (sp_expand);
	while (env[i])
	{
		if (!strncmp(for_search, env[i], size + 1))
			break ;
		i++;
	}
	free(for_search);
	free(expande_search);
	if (env[i] == NULL)
		return (NULL);
	return (&env[i][size + 1]);
}

int	join_expand(char *expande, int *j, int s_extract, char *new_args)
{
	int	index;

	(void)s_extract;
	index = 0;
	if (expande)
	{
		printf("expand = ________ %s\n", expande);
		while (expande[index])
		{
			new_args[*j] = expande[index];
			index++;
			*j = *j + 1;
		}
	}
	return (index);
}
