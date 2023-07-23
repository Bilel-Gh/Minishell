/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 20:17:17 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 20:20:11 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_value_for_export(char *value)
{
	int		i;
	int		len;
	char	*new_value;

	if (value == NULL)
		return (NULL);
	i = 0;
	len = ft_strlen(value) + 2;
	new_value = malloc(sizeof(char) * (len + 1));
	new_value[0] = '"';
	while (value[i] != '\0')
	{
		new_value[i + 1] = value[i];
		i++;
	}
	new_value[i + 1] = '"';
	new_value[i + 2] = '\0';
	return (new_value);
}

char	*get_current_name(char *long_name)
{
	char	*long_name_cpy;
	char	*solo_name;
	int		len_solo;
	int		i;

	long_name_cpy = ft_strdup(&long_name[7]);
	len_solo = 0;
	solo_name = NULL;
	len_solo = ft_get_len_solo(long_name_cpy, len_solo);
	if (len_solo == ft_strlen(long_name_cpy))
		return (long_name_cpy);
	solo_name = malloc(sizeof(char) * len_solo + 1);
	i = 0;
	while (long_name_cpy[i] != '\0' && long_name_cpy[i] != '=')
	{
		solo_name[i] = long_name_cpy[i];
		i++;
	}
	if (long_name_cpy[i] == '=')
		solo_name[i++] = '=';
	solo_name[i] = '\0';
	free(long_name_cpy);
	return (solo_name);
}

int	ft_get_len_solo(const char *long_name_cpy, int len_solo)
{
	while (long_name_cpy[len_solo] != '\0' && long_name_cpy[len_solo] != '=')
		len_solo++;
	if (long_name_cpy[len_solo] == '=')
		len_solo++;
	return (len_solo);
}

char	**ft_add_to_db_tab_export(char **tab, char *str, char *name)
{
	int					i;
	t_var_add_to_export	vars;

	ft_init_vars(tab, name, &i, &vars);
	if (tab == NULL)
	{
		vars.new_tab = malloc(sizeof(char *) * 2);
		vars.new_tab[0] = ft_strdup(str);
		vars.new_tab[1] = NULL;
		return (vars.new_tab);
	}
	i = ft_get_index_to_export(tab, name, i, &vars);
	if (vars.current_name != NULL)
		free(vars.current_name);
	if (vars.found == 1 && vars.name_s_equal[ft_strlen(name) - 1] == '=')
	{
		free(str);
		return (tab);
	}
	free(vars.name_s_equal);
	if (ft_no_egal_export(tab, str, i))
		return (tab);
	return (ft_get_new_tab(tab, str, i, vars.db_tablen));
}

int	ft_no_egal_export(char *const *tab, char *str, int i)
{
	if (tab[i] != NULL && ft_strchr(str, '=') == NULL)
	{
		free(str);
		return (1);
	}
	return (0);
}
