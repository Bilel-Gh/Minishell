/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 02:10:02 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/22 04:51:52 by bghandri         ###   ########.fr       */
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

char	*replace_expande(char *args, int *i, char **env, int *info)
{
	char	*expend_recherche;
	int		size_of_expende;
	char	*new_args;

	*info = 0;
	new_args = NULL;
	size_of_expende = ft_size_of_expende(&args[*i]);
	printf("size = %d\n \n ", size_of_expende);
	expend_recherche = get_expende_detect(size_of_expende, &args[*i]);
	new_args = traslate_expand(args, expend_recherche, i, env);
	return (new_args);
}

char	*importe_expande(char *args, char **env)
{
	t_importe_expande	imp;
	int					i;

	i = 0;
	imp.info = 0;
	imp.tmp = NULL;
	imp.new_args = NULL;
	while (args[i])
	{
		imp.info = 0;
		if (args[i] == '$' && back_slach(args, i))
		{
			imp.tmp = ft_strdup(args);
			imp.new_args = replace_expande(imp.tmp, &i, env, &(imp.info));
			free(args);
			if (imp.new_args == NULL)
				return (free(imp.tmp), imp.new_args);
			args = imp.new_args;
		}
		else
			i++;
		if (imp.new_args == NULL)
			imp.new_args = args;
	}
	return (imp.new_args);
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
		if (old_arg[i] == 0 || !old_arg[i][0])
			i++;
		else if (old_arg[i][0] == ' ' && old_arg[i + 1] == NULL && (i
					+ 2 < size_db_arr) && old_arg[i + 2][0] == ' ')
			i = i + 2;
		else
		{
			i++;
			new_size++;
		}
	}
	return (new_size);
}

void		ft_init_var_clean_null(char **old_arg, const int *size_db_arr,
			t_clean_null_var *var);

void		ft_gestion_digit_expand(t_global_parsing *const *g_pars,
			t_expande_var *var);

void		ft_gestion_dollar_expand(int *const *type_args, const int *nb_args,
			t_global_parsing *const *g_pars, t_expande_var *var);

int			ft_do_gestion_expand(int **type_args, t_global_parsing **g_pars,
			char **env, t_expande_var *var);

char	**ft_clean_null_db_array(char **old_arg, int *size_db_arr)
{
	t_clean_null_var	var;

	ft_init_var_clean_null(old_arg, size_db_arr, &var);
	while (var.i < *size_db_arr)
	{
		if (old_arg[var.i] == 0 || !old_arg[var.i][0])
			free(old_arg[var.i]);
		else if (old_arg[var.i][0] == ' ' && old_arg[var.i + 1] == NULL
				&& (var.i + 2 < *size_db_arr) && old_arg[var.i + 2][0] == ' ')
			free(old_arg[var.i]);
		else if (old_arg[var.i] != 0)
		{
			var.new_args[var.j++] = ft_strdup(old_arg[var.i]);
			free(old_arg[var.i]);
		}
		var.i++;
	}
	if (old_arg[var.i])
		free(old_arg[var.i]);
	free(old_arg);
	return (*size_db_arr = var.nb_new_args, var.new_args);
}

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
			v.i++;
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

int	ft_do_gestion_expand(int **type_args, t_global_parsing **g_pars, char **env,
		t_expande_var *v)
{
	if (have_expande((*g_pars)->args[(*v).i]))
	{
		if (ft_is_digit((*g_pars)->args[(*v).i][1]))
			ft_gestion_digit_expand(g_pars, v);
		else if (solo_dolard((*g_pars)->args[(*v).i]))
		{
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
