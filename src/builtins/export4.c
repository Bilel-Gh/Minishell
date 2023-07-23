/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 20:16:58 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 20:17:30 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_add_env(char *name, char *value, char ***env, t_global_exec **g_exec)
{
	char	*value_for_export;
	char	*env_to_add;
	char	*env_to_add_export;
	char	*value_cpy;

	value_cpy = NULL;
	if (value != NULL)
		value_cpy = ft_strdup(value);
	value_for_export = get_value_for_export(value_cpy);
	env_to_add_export = ft_get_env_to_add(name, value_for_export);
	ft_set_quotes_if_nul(name, &value_for_export, &env_to_add_export);
	free(value_cpy);
	if (value == NULL)
	{
		ft_add_and_free_export(name, g_exec, value_for_export,
			env_to_add_export);
		return ;
	}
	env_to_add = ft_get_env_to_add(name, value);
	(*env) = ft_add_to_db_tab((*env), env_to_add, name);
	ft_add_and_free_export(name, g_exec, value_for_export, env_to_add_export);
}

void	ft_set_quotes_if_nul(char *name, char **value_for_export,
		char **env_to_add_export)
{
	if ((*env_to_add_export)[ft_strlen((*env_to_add_export)) - 1] == '='
		&& (*value_for_export) == NULL)
	{
		free((*env_to_add_export));
		(*value_for_export) = ft_strdup("\"\"");
		(*env_to_add_export) = ft_get_env_to_add(name, (*value_for_export));
	}
}

void	ft_add_and_free_export(char *name, t_global_exec **g_exec,
		char *value_for_export, char *env_to_add_export)
{
	ft_add_to_export(g_exec, env_to_add_export, name);
	if (env_to_add_export)
		free(env_to_add_export);
	if (value_for_export)
		free(value_for_export);
}

void	ft_sort_env(char ***env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while ((*env)[i] != NULL)
	{
		j = i + 1;
		while ((*env)[j] != NULL)
		{
			if (ft_strcmp((*env)[i], (*env)[j]) > 0)
			{
				tmp = (*env)[i];
				(*env)[i] = (*env)[j];
				(*env)[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	ft_is_alpha(char c)
{
	if (c >= 'a' && c <= 'z')
		return (1);
	if (c >= 'A' && c <= 'Z')
		return (1);
	return (0);
}
