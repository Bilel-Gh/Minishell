/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 16:18:20 by ncharii           #+#    #+#             */
/*   Updated: 2023/07/23 23:36:24 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
