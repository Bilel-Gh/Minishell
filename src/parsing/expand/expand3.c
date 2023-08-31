/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 23:01:47 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 23:02:21 by bghandri         ###   ########.fr       */
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
		expand[j++] = expande[i++];
	expand[j] = 0;
	free(expande);
	free(nb_exit_code);
	return (expand);
}

char	*traslate_expand(char *arg_ct, char *ext_exp, int *i, char **env)
{
	char	*get_expande;
	char	*new_args;
	char	*exit_code;
	int		size_ext;

	exit_code = NULL;
	new_args = NULL;
	get_expande = NULL;
	size_ext = ft_size_of_expende(&arg_ct[*i]);
	if (ext_exp[0] == '?')
	{
		exit_code = expand_exit_code(ext_exp);
		get_expande = exit_code;
		new_args = join_and_rp_args(arg_ct, get_expande, size_ext, i);
		return (free(get_expande), new_args);
	}
	else
		get_expande = give_env_expand(ext_exp, size_ext, env);
	if (get_expande)
		new_args = join_and_rp_args(arg_ct, get_expande, size_ext, i);
	else
		new_args = join_and_rp_args(arg_ct, NULL, size_ext, i);
	return (new_args);
}

int	ft_do_gestion_expand(int **type_args, t_global_parsing **g_pars, char **env,
		t_expande_var *v)
{
	if (have_expande((*g_pars)->args[(*v).i]))
	{
		if (ft_is_digit((*g_pars)->args[(*v).i][1]))
			ft_gestion_digit_expand(g_pars, v);
		else if (solo_dolard((*g_pars)->args[(*v).i]))
		{
			ft_fprintf(2,"HSJAKFKJSHFLSDKJLKSHDKH\n \n");
			ft_gestion_dollar_expand(type_args, v->nb_args, g_pars, v);
			return (1);
		}
		else if (ft_strncmp((*g_pars)->args[(*v).i], "$?", 2) == 0)
			(*g_pars)->args[(*v).i] = exit_code_expande((*g_pars)->args[(*v).i],
					g_code_exit);
		else if (!is_limiteur(*type_args, (*v).i))
			(*g_pars)->args[(*v).i] = importe_expande((*g_pars)->args[(*v).i],
					env);
	}
	return (0);
}

void	ft_gestion_dollar_expand(int *const *type_args, const int *nb_args,
		t_global_parsing *const *g_pars, t_expande_var *v)
{
	if ((*v).i + 1 < *nb_args && ((*type_args)[(*v).i + 1] == ALPHANUM
		|| (*type_args)[(*v).i + 1] == QUOTE_S || (*type_args)[(*v).i
			+ 1] == QUOTE_D))
	{
		free((*g_pars)->args[(*v).i]);
		(*g_pars)->args[(*v).i] = NULL;
	}
	(*v).i++;
}

void	ft_gestion_digit_expand(t_global_parsing *const *g_pars,
								t_expande_var *v)
{
	(*v).new_args = ft_strdup((*g_pars)->args[(*v).i] + 2);
	free((*g_pars)->args[(*v).i]);
	(*g_pars)->args[(*v).i] = ft_strdup((*v).new_args);
	free((*v).new_args);
}
