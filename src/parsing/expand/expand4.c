/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 23:02:00 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 23:04:08 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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
