/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 20:17:37 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 20:18:15 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_db_tablen(char **tab)
{
	int	i;

	i = 0;
	while (tab[i] != NULL)
		i++;
	return (i);
}

char	*get_name(char *arg)
{
	int		i;
	int		j;
	char	*name;

	i = 0;
	j = 0;
	i = ft_get_len_name(arg, i);
	if (i == 0)
		return (NULL);
	name = malloc(sizeof(char) * (i + 1));
	if (name == NULL)
		return (NULL);
	while (j < i)
	{
		name[j] = arg[j];
		j++;
	}
	name[j] = 0;
	return (name);
}

int	ft_get_len_name(const char *arg, int i)
{
	char	type_quote;
	int		in_quote;

	in_quote = 0;
	if (arg == NULL)
		return (i);
	while (arg[i] != '=' && arg[i] != '\0')
	{
		if (arg[i] == '"' || arg[i] == '\'')
		{
			in_quote = !in_quote;
			type_quote = arg[i];
			i++;
			while (arg[i] != '\0' && arg[i] != type_quote)
				i++;
			if (arg[i] == type_quote)
				in_quote = 0;
			i++;
		}
		else
			i++;
	}
	if (arg[i] == '=')
		i++;
	return (i);
}

char	*get_value(char *arg)
{
	int		i;
	int		j;
	char	*value;

	i = 0;
	j = 0;
	while (arg[i] != '=' && arg[i] != 0)
		i++;
	if (arg[i] == 0)
		return (NULL);
	i++;
	value = malloc(sizeof(char) * ((ft_strlen(arg) - i) + 1));
	if (value == NULL)
		return (NULL);
	while (arg[i] != 0)
	{
		value[j] = arg[i];
		i++;
		j++;
	}
	value[j] = 0;
	return (value);
}

char	*ft_get_env_to_add(char *name, char *value)
{
	char	*new_env;
	int		i;
	int		j;

	new_env = (char *)malloc((ft_strlen(name) + ft_strlen(value) + 1)
			* sizeof(char));
	i = 0;
	while (name[i] != '\0')
	{
		new_env[i] = name[i];
		i++;
	}
	if (value == NULL)
	{
		new_env[i] = '\0';
		return (new_env);
	}
	j = 0;
	while (value[j] != '\0')
		new_env[i++] = value[j++];
	new_env[i] = '\0';
	return (new_env);
}
