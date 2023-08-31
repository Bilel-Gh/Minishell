/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 02:10:02 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 23:03:13 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_init_var_clean_null(char **old_arg, const int *size_db_arr,
		t_clean_null_var *var)
{
	var->i = 0;
	var->j = 0;
	var->nb_new_args = ft_nb_args_out_null(old_arg, *size_db_arr);
	var->new_args = malloc(sizeof(char *) * (var->nb_new_args + 1));
	var->new_args[var->nb_new_args] = NULL;
}

bool	solo_dolard(char *str)
{
	if (!str)
		return (false);
	if (str[0] == '$' && str[1] == 0)
		return (true);
	return (false);
}

bool	is_limiteur(int *type_args, int i)
{
	if ((i - 1) >= 0 && type_args[i - 1] == RED_D_IN)
		return (true);
	if ((i - 2) >= 0 && type_args[i - 2] == RED_D_IN)
		return (true);
	return (false);
}

char	*exit_code_expande(char *args, int exit)
{
	char	*exit_code;
	char	*new_args;
	char	*suite;

	exit_code = ft_itoa(exit);
	if (ft_strlen(args) > 2)
	{
		suite = ft_strdup(args + 2);
		if (!suite)
			return (NULL);
		new_args = ft_strcat(exit_code, suite);
		free(args);
		args = ft_strdup(new_args);
		free(suite);
	}
	else
	{
		free(args);
		args = ft_strdup(exit_code);
	}
	free(exit_code);
	return (args);
}

void	expande(int **type_args, int *nb_args, t_global_parsing **g_pars,
		char **env)
{
	t_expande_var	v;

	v.nb_args = nb_args;
	v.i = 0;
	v.new_args = NULL;
	v.new_type_args = NULL;
	while (v.i < *nb_args)
	{
		if ((*type_args)[v.i] == QUOTE_S)
		{
			printf("C EST ICI LE BOSS\n");
			v.i++;
		}
		if (v.i < *nb_args && ((*type_args)[v.i] == ALPHANUM
			|| (*type_args)[v.i] == QUOTE_D))
		{
			if (ft_do_gestion_expand(type_args, g_pars, env, &v))
				continue ;
		}
		if (v.i < *nb_args)
			v.i++;
	}
	(*g_pars)->args = ft_clean_null_db_array((*g_pars)->args, nb_args);
	v.new_type_args = ft_get_info_args((*g_pars)->args, nb_args);
	free(*type_args);
	*type_args = v.new_type_args;
}
