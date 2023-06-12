/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_error_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 20:08:15 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/12 22:49:07 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

bool	error_quote(int *type_args, int nb_args, char **args)
{
	int	i;

	i = 0;
	printf("\n*********check parsing quote ???***************\n");
	while (i < nb_args)
	{
		if (args[i][0] == 34 || args[i][0] == 39)
		{
			if (quote_is_open(args[i]))
				return (true);
			if (args[i][0] == 34)
				type_args[i] = 9;
			else
				type_args[i] = 8;
		}
		i++;
	}
	return (false);
}

bool	error_size_or_spe_redi(int *type_args, int nb_args, char **args)
{
	int	i;

	i = 0;
	printf("\n*********check parsing redirection ???***************\n");
	while (i < nb_args)
	{
		if (type_args[i] == 1)
		{
			if (error_size(args[i], 2))
				return (true);
			if (error_no_only_type((args[i])))
				return (true);
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
	printf("\n*********check parsing pipe ???***************\n");
	while (i < nb_args)
	{
		if (type_args[i] == 4)
		{
			if (error_size(args[i], 1))
				return (true);
			if (error_no_only_type((args[i])))
				return (true);
		}
		i++;
	}
	return (false);
}

bool	search_error_args(int *type_args, int *nb_args, char **args)
{
	if (error_size_or_spe_redi(type_args, *nb_args, args))
	{
		printf("^^^^^^^^^^^ no error redirection ^^^^^^^^^^^^^^^^^^\n");
		return (1);
	}
	if (error_pipe(type_args, *nb_args, args))
	{
		printf("^^^^^^^^^^^     no error pipe    ^^^^^^^^^^^^^^^^^^\n");
		return (1);
	}
	if (error_quote(type_args, *nb_args, args))
	{
		printf("\n*********    no error quote    ***************\n");
		return (1);
	}
	return (0);
}
