/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd3.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 19:51:48 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 19:55:11 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_getenv(char *name, char **env)
{
	char	*value;
	int		i;
	int		j;

	i = 0;
	while (env[i] != NULL)
	{
		j = 0;
		while (env[i][j] != '=' && env[i][j] == name[j])
		{
			j++;
		}
		if (env[i][j] == '=')
		{
			value = ft_strdup(env[i] + j + 1);
			return (value);
		}
		i++;
	}
	return (NULL);
}

char	*ft_replace_env(char *name, char *value)
{
	char	*new_env;
	int		i;
	int		j;

	new_env = (char *)malloc((ft_strlen(name) + ft_strlen(value) + 2)
			* sizeof(char));
	i = 0;
	while (name[i] != '\0')
	{
		new_env[i] = name[i];
		i++;
	}
	new_env[i] = '=';
	i++;
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

int	ft_setenv(char *name, char *value, char ***env)
{
	int		i;
	int		j;
	char	*new_env;

	i = 0;
	while ((*env)[i] != NULL)
	{
		j = 0;
		while ((*env)[i][j] != '=' && (*env)[i][j] == name[j])
		{
			j++;
		}
		if ((*env)[i][j] == '=')
		{
			new_env = ft_replace_env(name, value);
			free((*env)[i]);
			(*env)[i] = new_env;
			return (0);
		}
		i++;
	}
	return (-1);
}

int	ft_is_db_array(char *name, char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		if (ft_strcmp(name, array[i]) == 0)
		{
			return (1);
		}
		i++;
	}
	return (0);
}
