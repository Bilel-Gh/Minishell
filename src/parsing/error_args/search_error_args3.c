/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_error_args3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 22:21:56 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 22:27:12 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

bool	ft_is_solo_expand(char *str)
{
	if (str != NULL && str[0] == '$' && str[1] == 0)
		return (true);
	return (false);
}

bool	prev_next_redi_error(char **args, int i, int *type_args, int nb_args)
{
	char	*get_first_prev_arg;
	char	*get_first_next_arg;

	get_first_prev_arg = get_prev_arg(args, i);
	get_first_next_arg = get_next_arg(args, i);
	if (type_args[i] == REDIRECT)
	{
		if ((i + 1) > nb_args)
		{
			g_code_exit = ERROR_REDIRECT;
			return (true);
		}
		if (!ft_is_file(get_first_next_arg))
		{
			g_code_exit = ERROR_REDIRECT;
			return (true);
		}
	}
	if (get_first_prev_arg != NULL && (get_first_prev_arg[0] == '<'
			|| get_first_prev_arg[0] == '>'))
	{
		g_code_exit = ERROR_REDIRECT;
		return (true);
	}
	return (false);
}

bool	error_expand(int type, char *next, char **env)
{
	char	*expand_value;
	char	*expand_to_search;

	if (type == REDIRECT)
	{
		if (ft_is_solo_expand(next))
		{
			if (solo_dolard(next))
				return (false);
			expand_to_search = ft_strdup(&next[1]);
			expand_value = give_env_expand(expand_to_search,
					ft_strlen(expand_to_search),
					env);
			printf(" error_expande == %s\n", expand_value);
			if (expand_value == NULL)
			{
				g_code_exit = ERROR_REDIRECT2;
				return (true);
			}
		}
	}
	return (false);
}

bool	error_size_or_spe_redi(int *type_args, int nb_args, char **args,
		char **env)
{
	int		i;
	char	*get_first_next_arg;

	i = 0;
	printf("\n*********check parsing redirection ???***************\n");
	while (i < nb_args)
	{
		if (type_args[i] == 1)
		{
			get_first_next_arg = get_next_arg(args, i);
			if (error_size(args[i], 2) || error_no_only_type((args[i])))
				return (true);
			if (prev_next_redi_error(args, i, type_args, nb_args))
				return (true);
			if (error_expand(type_args[i], get_first_next_arg, env))
			{
				ft_fprintf(2, "bash: %s: ambiguous redirect\n",
					get_first_next_arg);
				return (true);
			}
			give_sp_args_redi(args[i], type_args, i);
		}
		i++;
	}
	return (false);
}

bool	error_pipe(int *type_args, int nb_args, char **args)
{
	int	i;

	i = 0;
	while (i < nb_args)
	{
		if (type_args[i] == 4)
		{
			if (error_size(args[i], 1))
			{
				g_code_exit = ERROR_PIPE;
				return (true);
			}
			if (error_no_only_type((args[i])))
			{
				g_code_exit = ERROR_PIPE;
				return (true);
			}
			if (prev_next_error(args, i))
				return (true);
		}
		i++;
	}
	return (false);
}
