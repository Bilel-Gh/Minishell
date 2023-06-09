/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 16:18:20 by ncharii           #+#    #+#             */
/*   Updated: 2023/06/09 19:23:17 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	error_grammaticale(int *type_args, int nb_args)
{
	int	i;

	i = 0;
	if(!type_args)
		return(false);
	printf("*********check grammaticale error ???***************\n");
	if (nb_args == 1 && type_args[i] != 0)
		return (true);
	while (i < nb_args - 1)
	{
		if (type_args[i] == 5)
			i++;
		if (i == 0)
		{
			if (type_args[i] != 0 && type_args[i] != 1
				|| type_args[i] == 1 && type_args[i + 1] != 0)
				return (true);
		}
		if (i != 0)
		{
			if (type_args[i] == 0)
			{
				if (type_args[i + 1] != 4 && type_args[i + 1] != 0
					&& type_args[i + 1] != 1)
					return (true);
			}
			if (type_args[i] == 4 && type_args[i + 1] != 0
				&& type_args[i + 1] != 1)
				return (true);
			if (type_args[i] == 1 && type_args[i + 1] != 0)
				return (true);
		}
		i++;
	}
	return (false);
}

//CHAR **

//void	kick_quote(char **args)
//char **join_inter_space(char **args, int *type_args, int nb_args)

//
/*

char    **good_argsnisation(int *type_args, int nb_args, char **args)
{
    int nb_new_args;
    int i;
    char **new_argsisation

    i = 0
    nb_new_args = 1;
    while (i < nb_args)
    {
        if(t_args[i] == SPACE)
            nb_new_args++;
        if(t_args[i] == ALPHANUM)
            nb_new_args = nb_new_args + nb_space(args[i]);
    }
    new_argsisation = (nb_new_args);
    i = 0;
    while (new_argsisation[i])
    {
        if (!new_argsisation[i])
            remplir_le_nouveau(new_argsisation, i);
        i++;
    }
    return (good_argsnisation);
}*/

int count_nb_space(int *type_args, int nb_args)
{
	int i;
	int nb_space;

	i = 0;
	nb_space = 0;
	while (i < nb_args)
	{
		if (type_args[i] == ESPACE)
			nb_space++;
		i++;
	}
	return (nb_space);
}

char **kick_args_space(char **new_args, int *type_args, int *nb_args)
{
	char **last_args;
	int nb_space;
	int nb_new_args;
	int i;
	int j;

	i = 0;
	j = 0;
	nb_space = count_nb_space(type_args, *nb_args);
	nb_new_args = *nb_args - nb_space;
	if(nb_new_args == 0)
		return (NULL);
	//printf ("$$$$$$$$$$$$$$$$$$$$$$$$ nb_space %d\n", nb_space);
	last_args = malloc( sizeof(char*) * (nb_new_args + 1));
	if (!last_args)
		return (NULL);// FT_ERROR_malloc_parsing
	last_args[nb_new_args] = 0;
	while ( i < nb_new_args)
	{
		last_args[i] = 0;
		i++;
	}
	i = 0;
	while (i < *nb_args)
	{
		printf("111111111111\n");
		if (type_args[i] == ESPACE)
		{
				i++;
				if (i == *nb_args)
					return (*nb_args = nb_new_args, last_args);
		}
		last_args[j] = ft_strjoin(last_args[j], new_args[i]);
		(i++, j++);
	}
	while ( i < nb_new_args)
	{
		printf("new args kick_space %d = %s", i, new_args[i]);
		i++;
	}
	*nb_args = nb_new_args;
	return (last_args);
}

char    **ft_parsing(int *type_args, int *nb_args, char **args, int *error)
{
	char **new_args;
	char **no_quote_args;
	int *new_type_args;

	*error = 0;
	if (search_error_args(type_args, nb_args, args))
		return (args);
	expande(type_args, *nb_args, args);
    no_quote_args = kick_quote(type_args, *nb_args, args);
	new_args = join_inter_space(no_quote_args, type_args, nb_args);
	//free_db_array(no_quote_args);
	new_type_args = ft_get_info_args(new_args, nb_args);
	free_db_array(args);
	args = kick_args_space(new_args,type_args, nb_args);
	free (type_args);
	free(new_type_args);
	new_type_args = ft_get_info_args(args, nb_args);
	 if (error_grammaticale(new_type_args, *nb_args))
	   return (args);
	printf("^^^^^^^^^^^ no error grammaticale ^^^^^^^^^^^^^^^^\n");
    *error = 1;
	free(new_type_args);
	free_db_array(no_quote_args);
	return (args);
}

//execve("/usr/bin/jjy", {"echo", "$USER"}, argv)
