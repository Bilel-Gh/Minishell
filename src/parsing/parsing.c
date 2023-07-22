/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 16:18:20 by ncharii           #+#    #+#             */
/*   Updated: 2023/07/22 18:57:55 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void		ft_get_result_bs(const char *str, char *result, int *i, int *j);

bool	fist_error(int *type_args)
{
	if ((type_args[0] != 0 && type_args[0] != 1) || (type_args[0] == 1
			&& type_args[1] != 0))
		return (true);
	return (false);
}

bool	error_grammaticale(int *type_args, int nb_args)
{
	int	i;

	i = 0;
	if (!type_args)
		return (false);
	printf("*********check grammaticale error ???***************\n");
	if (nb_args == 1 && type_args[i] != 0)
		return (true);
	while (i < nb_args - 1)
	{
		if (type_args[i] == 5)
			i++;
		if (i == 0 && fist_error(type_args))
			return (true);
		if (i != 0)
		{
			if (type_args[i] == 4 && type_args[i + 1] != 0 && type_args[i
					+ 1] != 1)
				return (true);
			if (type_args[i] == 1 && type_args[i + 1] != 0)
				return (true);
		}
		i++;
	}
	return (false);
}

int	count_nb_space(int *type_args, int nb_args)
{
	int	i;
	int	nb_space;

	i = 0;
	nb_space = 0;
	printf("nb_args = %d", nb_args);
	if (type_args == NULL)
		return (nb_space);
	while (i < nb_args - 1)
	{
		if (type_args[i] == ESPACE)
			nb_space++;
		i++;
	}
	return (nb_space);
}

void	b_zero_last_args(char **last_args, int nb_new_args, int i)
{
	last_args[nb_new_args] = 0;
	while (i < nb_new_args)
	{
		last_args[i] = 0;
		i++;
	}
}

char	**kick_args_space(char **new_args, int *type_args, int *nb_args)
{
	char	**last_args;
	int		nb_new_args;
	int		i;
	int		j;

	i = 0;
	j = 0;
	nb_new_args = *nb_args - count_nb_space(type_args, *nb_args);
	if (nb_new_args == 0)
		return (NULL);
	last_args = malloc(sizeof(char *) * (nb_new_args + 1));
	if (!last_args)
		return (NULL);
	b_zero_last_args(last_args, nb_new_args, i);
	while (i < *nb_args)
	{
		if (type_args[i] == ESPACE)
			i++;
		if (i == *nb_args)
			return (*nb_args = nb_new_args, last_args);
		last_args[j] = ft_strjoin(last_args[j], new_args[i]);
		i++;
		j++;
	}
	return (*nb_args = nb_new_args, last_args);
}

typedef struct s_bachslash
{
	int		i;
	int		j;
	int		inquote;
	int		max;
	char	*result;
}			t_bachslash;

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
	*type_args = ft_get_info_args((*g_pars)->args, nb_args);
}

char	**ft_parsing(int *nb_args, t_global_parsing **g_pars, char ***env)
{
	char	**new_args;
	int		*new_type_args;
	int		*type_args;

	type_args = ft_get_info_args((*g_pars)->args, nb_args);
	if (search_error_args(type_args, nb_args, (*g_pars)->args, *env))
		return (free(type_args), (*g_pars)->args);
	expande(&type_args, nb_args, g_pars, *env);
	ft_gestion_backslash(&type_args, nb_args, g_pars);
	g_code_exit = SUCCESS;
	if (*nb_args == 0)
		return (free(type_args), (*g_pars)->args);
	new_args = join_inter_space((*g_pars)->args, type_args, nb_args);
	new_type_args = ft_get_info_args(new_args, nb_args);
	free_db_array((*g_pars)->args);
	(*g_pars)->args = kick_args_space(new_args, new_type_args, nb_args);
	free_db_array(new_args);
	free(type_args);
	free(new_type_args);
	new_type_args = ft_get_info_args((*g_pars)->args, nb_args);
	if (error_grammaticale(new_type_args, *nb_args))
		return ((*g_pars)->args);
	free(new_type_args);
	return ((*g_pars)->args);
}
