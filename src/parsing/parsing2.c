/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 23:32:47 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 23:35:37 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*remove_backslash(char *str)
{
	t_bachslash	var;

	var.inquote = 0;
	var.i = 0;
	var.j = 0;
	var.max = 2 * ft_strlen(str) + 1;
	var.result = malloc(sizeof(char) * var.max);
	ft_bzero(var.result, var.max);
	if (var.result == NULL)
		return (NULL);
	while (str[var.i])
	{
		if (str[var.i] == '\'')
			var.inquote = !var.inquote;
		if (str[var.i] == '\\' && var.inquote)
			var.i++;
		else if (str[var.i] == '\\' && !var.inquote)
		{
			ft_get_result_bs(str, var.result, &var.i, &var.j);
			continue ;
		}
		var.result[var.j++] = str[var.i++];
	}
	var.result[var.j] = '\0';
	return (var.result);
}

void	ft_get_result_bs(const char *str, char *result, int *i, int *j)
{
	if (str[(*i) + 1])
	{
		result[(*j)++] = '\'';
		result[(*j)++] = str[(*i) + 1];
		result[(*j)++] = '\'';
		(*i) += 2;
	}
	else
		result[(*j)++] = str[(*i)++];
}

int	is_backslash(char *str)
{
	int	get_index_bs;

	get_index_bs = 0;
	if (ft_strchr(str, '\\'))
	{
		while (str[get_index_bs] != '\\')
			get_index_bs++;
		if (str[get_index_bs + 1] != '\'' && str[get_index_bs - 1] != '\'')
			return (1);
	}
	return (0);
}

void	ft_gestion_backslash(int **type_args, int *nb_args,
		t_global_parsing **g_pars)
{
	char	*cleaned_arg;
	int		i;

	i = 0;
	while (i < *nb_args)
	{
		while (is_backslash((*g_pars)->args[i])
			&& (*g_pars)->args[i][1] != '\0')
		{
			cleaned_arg = remove_backslash((*g_pars)->args[i]);
			free((*g_pars)->args[i]);
			(*g_pars)->args[i] = cleaned_arg;
		}
		i++;
	}
	free(*type_args);
	*type_args = ft_get_info_args((*g_pars)->args, nb_args);
}
