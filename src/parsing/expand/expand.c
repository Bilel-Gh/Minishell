/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 02:10:02 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/14 21:04:10 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*expand_exit_code(char *expande)
{
	char	*nb_exit_code;
	char	*expand;
	int		i;
	int		j;

	i = 0;
	j = 0;
	nb_exit_code = ft_itoa(g_code_exit);
	expand = malloc(ft_strlen(expande) + ft_strlen(nb_exit_code));
	while (nb_exit_code[i])
	{
		expand[j] = nb_exit_code[i];
		j++;
		i++;
	}
	i = 1;
	while (expande[i])
	{
		expand[j] = expande[i];
		j++;
		i++;
	}
	expand[j] = 0;
	return (expand);
}

char	*traslate_expand(char *arg_ct, char *ext_exp, int size_ext, char **env)
{
	char	*get_expande;
	char	*new_args;
	char	*exit_code;

	exit_code = NULL;
	new_args = NULL;
	get_expande = NULL;
	if (ext_exp[0] == '?')
	{
		exit_code = expand_exit_code(ext_exp);
		get_expande = exit_code;
	}
	else
		get_expande = give_env_expand(ext_exp, size_ext, env);
	if (get_expande)
	{
		new_args = join_and_rp_args(arg_ct, get_expande, size_ext);
		return (new_args);
	}
	else
	{
		new_args = join_and_rp_args(arg_ct, NULL, size_ext);
		return (new_args);
	}
}

char	*replace_expande(char *args, int i, char **env, int *info)
{
	char	*expend_recherche;
	int		size_of_expende;
	char	*new_args;

	*info = 0;
	new_args = NULL;
	size_of_expende = ft_size_of_expende(&args[i]);
	expend_recherche = get_expende_detect(size_of_expende, &args[i]);
	new_args = traslate_expand(args, expend_recherche, size_of_expende, env);
	return (new_args);
}

char	*importe_expande(char *args, char **env)
{
	int		i;
	char	*tmp;
	char	*new_args;
	int		info;

	i = 0;
	new_args = NULL;
	while (args[i])
	{
		info = 0;
		if (args[i] == '$' && back_slach(args, i))
		{
			tmp = ft_strdup(args);
			new_args = replace_expande(tmp, i, env, &info);
			free(args);
			if (new_args == NULL)
				return (new_args);
			args = new_args;
			if (info != 1)
				i = 0;
			else
				i++;
		}
		else
			i++;
		if (new_args == NULL)
			new_args = args;
	}
	return (new_args);
}

bool	have_expande(char *args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (args[i] == '$')
			return (true);
		i++;
	}
	return (false);
}

int	ft_nb_args_out_null(char **old_arg, int size_db_arr)
{
	int	i;
	int	new_size;

	i = 0;
	new_size = 0;
	while (i < size_db_arr)
	{
		if (old_arg[i] == 0)
			i++;
		else if (old_arg[i][0] == ' ' && old_arg[i + 1] == NULL
			&& (i + 2 < size_db_arr) && old_arg[i + 2][0] == ' ')
			i = i + 2;
		else
		{
			i++;
			new_size++;
		}
	}
	return (new_size);
}

char	**ft_clean_null_db_array(char **old_arg, int *size_db_arr)
{
	int		nb_new_args;
	char	**new_args;
	int		i;
	int		j;

	i = 0;
	j = 0;
	nb_new_args = ft_nb_args_out_null(old_arg, *size_db_arr);
	new_args = malloc(sizeof(char *) * (nb_new_args + 1));
	new_args[nb_new_args] = NULL;
	printf("size old db_arr = %d\n", *size_db_arr);
	printf("size out null db_ar = %d\n", nb_new_args);
	while (i < *size_db_arr)
	{
		if (old_arg[i] == 0)
			free(old_arg[i]);
		else if (old_arg[i][0] == ' ' && old_arg[i + 1] == NULL
			&& (i + 2 < *size_db_arr) && old_arg[i + 2][0] == ' ')
			free(old_arg[i]);
		else if (old_arg[i] != 0)
		{
			printf(" clean array null ======= [%s]*****\n", old_arg[i]);
			new_args[j] = ft_strdup(old_arg[i]);
			free(old_arg[i]);
			j++;
		}
		i++;
	}
	if (old_arg[i])
		free(old_arg[i]);
	free(old_arg);
	return (*size_db_arr = nb_new_args, new_args);
}

bool	solo_dolard(char *str)
{
	if (!str)
		return (false);
	if (str[0] == '$' && str[1] == 0)
		return (true);
	return (false);
}

void	expande(int **type_args, int *nb_args, t_global_parsing **g_pars, char **env)
{
	int		i;
	char	*suite;
	char	*new_args;
	int		*new_type_args;

	i = 0;
	suite = NULL;
	new_args = NULL;
	new_type_args = NULL;
	while (i < *nb_args)
	{
		if ((*type_args)[i] == ALPHANUM || (*type_args)[i] == QUOTE_D)
		{
			if (have_expande((*g_pars)->args[i]))
			{
				printf("\n \n PRESANCE EXPANDE\n");
				if (ft_is_digit((*g_pars)->args[i][1]))
				{
					new_args = ft_strdup((*g_pars)->args[i] + 2);
					free((*g_pars)->args[i]);
					(*g_pars)->args[i] = ft_strdup(new_args);
					free(new_args);
				}
				else if (solo_dolard((*g_pars)->args[i]))
				{
					if (i + 1 < *nb_args && ((*type_args)[i + 1] == ALPHANUM
						|| (*type_args)[i + 1] == QUOTE_S
						|| (*type_args)[i + 1] == QUOTE_D))
					{
						free((*g_pars)->args[i]);
						(*g_pars)->args[i] = NULL;
					}
					i++;
					continue ;
				}
				else
					(*g_pars)->args[i] = importe_expande((*g_pars)->args[i], env);
			}
		}
		i++;
	}
	(*g_pars)->args = ft_clean_null_db_array((*g_pars)->args, nb_args);
	new_type_args = ft_get_info_args((*g_pars)->args, nb_args);
	free(*type_args);
	*type_args = new_type_args;
}
