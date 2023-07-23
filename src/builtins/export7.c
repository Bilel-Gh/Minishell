/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 20:17:40 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 20:19:49 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_get_index_to_export(char *const *tab, char *name, int i,
		t_var_add_to_export *vars)
{
	while (tab[i] != NULL)
	{
		vars->current_name = get_current_name(tab[i]);
		if (ft_strncmp(vars->current_name, name, ft_strlen(name)) == 0)
			vars->found = 1;
		if (ft_strcmp(vars->current_name, vars->name_s_equal) == 0)
			vars->found = 1;
		if (vars->found == 1)
			return (i);
		i++;
		free(vars->current_name);
		vars->current_name = NULL;
	}
	return (i);
}

void	ft_init_vars(char **tab, char *name, int *i, t_var_add_to_export *vars)
{
	(*i) = 0;
	vars->found = 0;
	vars->current_name = NULL;
	vars->db_tablen = ft_db_tablen(tab);
	vars->name_copy = ft_strdup(name);
	if (vars->name_copy[ft_strlen(vars->name_copy) - 1] == '=')
		vars->name_s_equal = ft_substr(vars->name_copy, 0,
				ft_strlen(vars->name_copy) - 1);
	else
		vars->name_s_equal = ft_strdup(vars->name_copy);
	free(vars->name_copy);
}

char	**ft_get_new_tab_export(char **tab, char *str, int i, int db_tablen)
{
	char	**new_tab;

	if (i < db_tablen)
	{
		free(tab[i]);
		tab[i] = ft_strdup(str);
		return (tab);
	}
	else
	{
		new_tab = malloc(sizeof(char *) * (db_tablen + 2));
		i = 0;
		while (i < db_tablen)
		{
			new_tab[i] = ft_strdup(tab[i]);
			free(tab[i]);
			i++;
		}
		new_tab[i] = ft_strdup(str);
		new_tab[i + 1] = NULL;
		free(tab);
		return (new_tab);
	}
}

char	**ft_add_to_db_tab(char **tab, char *str, char *name)
{
	int		i;
	char	**new_tab;
	int		db_tablen;
	int		len_name;

	i = 0;
	if (tab == NULL)
	{
		new_tab = malloc(sizeof(char *) * 2);
		new_tab[0] = ft_strdup(str);
		new_tab[1] = NULL;
		return (new_tab);
	}
	db_tablen = ft_db_tablen(tab);
	len_name = ft_strlen(name);
	while (tab[i] != NULL && ft_strncmp(tab[i], name, len_name) != 0)
		i++;
	return (ft_get_new_tab(tab, str, i, db_tablen));
}

char	**ft_get_new_tab(char **tab, char *str, int i, int db_tablen)
{
	char	**new_tab;

	if (i < db_tablen)
	{
		free(tab[i]);
		tab[i] = ft_strdup(str);
		free(str);
		return (tab);
	}
	else
	{
		new_tab = malloc(sizeof(char *) * (db_tablen + 2));
		i = 0;
		while (i < db_tablen)
		{
			new_tab[i] = ft_strdup(tab[i]);
			free(tab[i]);
			i++;
		}
		new_tab[i] = ft_strdup(str);
		free(str);
		new_tab[i + 1] = NULL;
		free(tab);
		return (new_tab);
	}
}
