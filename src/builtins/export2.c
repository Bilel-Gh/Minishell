/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 20:15:10 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 20:16:36 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_get_name(char *full_clean_str, int *i, char **name)
{
	char	*name_to_check;

	(*name) = get_name(full_clean_str);
	name_to_check = ft_remove_egal_name(name);
	if (!ft_check_name(name_to_check))
	{
		g_code_exit = ERROR;
		if (*name != NULL)
		{
			ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n",
				full_clean_str);
		}
		else
			ft_fprintf(2, "minishell: export: `': not a valid identifier\n");
		(*i)++;
		free(name_to_check);
		return (0);
	}
	free(name_to_check);
	return (1);
}

char	*ft_remove_egal_name(char *const *name)
{
	char	*name_to_check;

	name_to_check = NULL;
	if (*name != NULL)
	{
		if ((*name)[ft_strlen((*name)) - 1] == '=')
			name_to_check = ft_substr((*name), 0, ft_strlen((*name)) - 1);
		else
			name_to_check = ft_strdup((*name));
	}
	return (name_to_check);
}

void	ft_free_export_var(char *name, char *value, char *full_clean_str)
{
	if (name != NULL)
		free(name);
	if (value != NULL)
		free(value);
	if (full_clean_str != NULL)
		free(full_clean_str);
}

void	ft_add_and_sort(char ***env, t_global_exec **g_exec, char *name,
		char *value)
{
	ft_add_env(name, value, env, g_exec);
	ft_sort_env(env);
	ft_sort_env(&(*g_exec)->export);
}

int	ft_is_alnum(char c)
{
	if (ft_is_alpha(c) == 1)
		return (1);
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
