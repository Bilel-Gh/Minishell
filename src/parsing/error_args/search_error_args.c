/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_error_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 20:08:15 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 22:31:56 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	is_only_bs(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '\\')
			return (0);
		i++;
	}
	return (1);
}

int	ft_is_impair_bs(int nb_bs)
{
	if (nb_bs % 2 != 0)
	{
		g_code_exit = ERROR_BACKSLASH;
		return (1);
	}
	return (0);
}

bool	ft_gestion_bs(int nb_args, char *const *args, int i, int len_last_arg)
{
	int	nb_bs;

	nb_bs = 0;
	while (i < nb_args)
	{
		nb_bs = ft_strlen(args[i]);
		if (is_only_bs(args[i]))
		{
			if (nb_bs == 1 || nb_bs % 2 == 0)
			{
				i++;
				continue ;
			}
			if (ft_is_impair_bs(nb_bs))
				return (true);
		}
		if (args[nb_args - 1][len_last_arg - 1] == '\\')
		{
			g_code_exit = ERROR_BACKSLASH;
			return (true);
		}
		i++;
	}
	return (false);
}

bool	error_back_slash(int nb_args, char **args)
{
	int	i;
	int	len_last_arg;

	i = 0;
	len_last_arg = ft_strlen(args[nb_args - 1]);
	return (ft_gestion_bs(nb_args, args, i, len_last_arg));
}

bool	search_error_args(int *type_args, int *nb_args, char **args, char **env)
{
	if (error_pipe(type_args, *nb_args, args))
	{
		printf("^^^^^^^^^^^     no error pipe    ^^^^^^^^^^^^^^^^^^\n");
		return (1);
	}
	if (error_size_or_spe_redi(type_args, *nb_args, args, env))
	{
		printf("^^^^^^^^^^^ no error redirection ^^^^^^^^^^^^^^^^^^\n");
		if (g_code_exit != ERROR_REDIRECT2)
			g_code_exit = ERROR_REDIRECT;
		return (1);
	}
	if (error_quote(type_args, *nb_args, args))
	{
		printf("\n*********    no error quote    ***************\n");
		return (1);
	}
	if (error_back_slash(*nb_args, args))
	{
		printf("\n*********    no error back_slash    ***************\n");
		return (1);
	}
	return (0);
}
