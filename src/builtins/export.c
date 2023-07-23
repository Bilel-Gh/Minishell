/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 02:56:40 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 20:20:45 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_add_to_export(t_global_exec **g_exec, char *new_env, char *name)
{
	char	*env_to_add_to_export;
	char	*pre_export;

	env_to_add_to_export = ft_strdup(new_env);
	pre_export = ft_strdup("export ");
	(*g_exec)->export = ft_add_to_db_tab_export((*g_exec)->export,
			ft_strjoin(pre_export, env_to_add_to_export), name);
	if (env_to_add_to_export)
		free(env_to_add_to_export);
}

char	*ft_clean_quote_str(char *value)
{
	int		len;
	char	*new_value;
	int		i;
	int		j;
	char	*cleaned_value;

	i = 0;
	j = 0;
	len = ft_strlen(value);
	new_value = malloc(sizeof(char) * (len + 1));
	ft_bzero(new_value, len + 1);
	if (new_value == NULL)
		return (NULL);
	ft_remove_qt_value(value, new_value, i, j);
	cleaned_value = ft_strdup(new_value);
	free(new_value);
	return (cleaned_value);
}

void	ft_remove_qt_value(const char *value, char *new_value, int i, int j)
{
	int		quote_count;
	char	current_quote;

	quote_count = 0;
	current_quote = '\0';
	while (value[i] != '\0')
	{
		if ((value[i] == '"' || value[i] == '\'') && (quote_count % 2 == 0))
		{
			current_quote = value[i];
			quote_count++;
		}
		else if (value[i] == current_quote)
		{
			quote_count++;
			current_quote = '\0';
		}
		else
		{
			new_value[j] = value[i];
			j++;
		}
		i++;
	}
	new_value[j] = '\0';
}

void	ft_do_export(char *const *args, char ***env, t_global_exec **g_exec)
{
	char	*full_clean_str;
	int		i;
	char	*name;
	char	*value;

	i = 1;
	value = NULL;
	name = NULL;
	full_clean_str = NULL;
	while (args[i] != NULL)
	{
		ft_free_export_var(name, value, full_clean_str);
		full_clean_str = ft_clean_quote_str(args[i]);
		if (ft_check_solo_invalid_arg(full_clean_str))
		{
			ft_free_export_var(name, value, full_clean_str);
			return ;
		}
		if (!ft_get_name(full_clean_str, &i, &name))
			continue ;
		value = get_value(full_clean_str);
		ft_add_and_sort(env, g_exec, name, value);
		i++;
	}
	ft_free_export_var(name, value, full_clean_str);
}

void	builtin_export(char **args, char ***env, t_global_exec **g_exec)
{
	int	nb_args;

	nb_args = ft_db_tablen(args);
	g_code_exit = SUCCESS;
	if (ft_check_export_err(args, g_exec, nb_args))
		return ;
	ft_do_export(args, env, g_exec);
}
