/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_error_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 20:08:15 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/16 17:37:47 by bghandri         ###   ########.fr       */
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
            {
                if (args[i][0] == '"')
                    g_code_exit = ERROR_QUOTE_D;
                else
                    g_code_exit = ERROR_QUOTE_S;
                return (true);
            }
            if (ft_strlen(args[0]) == 2 && args[1] == NULL) // marche que pour "" et ''  sans rien apres
            {
                g_code_exit = NOTFOUND;
                return (true);
            }
			if (args[i][0] == 34)
				type_args[i] = 9;
			else
				type_args[i] = 8;
		}
		i++;
	}
	return (false);
}


char *get_prev_arg(char **arg, int i)
{
    i--;
    while (i >= 0 && arg[i][0] == ' ')
        i--;
    if (i < 0)
        return (NULL);
    return (arg[i]);
}

char *get_next_arg(char **arg, int i)
{
    i++;
    while (arg[i] && arg[i][0] == ' ')
        i++;
    if (arg[i] == NULL)
        return (NULL);
    return (arg[i]);
}

bool prev_next_error(char **args, int i)
{
    char *get_first_prev_arg;
    char *get_first_next_arg;

    get_first_prev_arg = get_prev_arg(args, i);
    get_first_next_arg = get_next_arg(args, i);

//    printf("get_first_prev_arg = %s\n", get_first_prev_arg);
//    printf("get_first_next_arg = %s\n", get_first_next_arg);
    if (get_first_prev_arg != NULL && get_first_next_arg == NULL)
    {
        g_code_exit = ERROR_PIPE2;
        return (true);
    }
    if (get_first_prev_arg == NULL || get_first_next_arg == NULL)
    {
        g_code_exit = ERROR_PIPE;
        return (true);
    }
    if (get_first_prev_arg[0] == '<' || get_first_prev_arg[0] == '>')
    {
        g_code_exit = ERROR_PIPE;
        return (true);
    }
    return (false);
}

bool prev_next_redi_error(char **args, int i)
{
    char *get_first_prev_arg;
    char *get_first_next_arg;

    get_first_prev_arg = get_prev_arg(args, i);
    get_first_next_arg = get_next_arg(args, i);

//    printf("get_first_prev_arg = %s\n", get_first_prev_arg);
//    printf("get_first_next_arg = %s\n", get_first_next_arg);
    if (get_first_prev_arg == NULL || get_first_next_arg == NULL)
    {
        g_code_exit = ERROR_REDIRECT;
        return (true);
    }
    if (get_first_prev_arg[0] == '<' || get_first_prev_arg[0] == '>')
    {
        g_code_exit = ERROR_REDIRECT;
        return (true);
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
            if (prev_next_redi_error(args, i))
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
        printf("PIPE args[%d] = %s\n", i, args[i]);
        i++;
    }
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

int is_only_bs(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] != '\\')
            return (0);
        i++;
    }
    return (1);
}

bool	error_back_slash(int nb_args, char **args)
{
    int	i;
    int nb_bs;
    int len_last_arg;

    i = 0;
    nb_bs = 0;
    len_last_arg = ft_strlen(args[nb_args -1]);
    printf("\n*********check parsing backslash ???***************\n");
    printf("args[nb_args - 1][len_last_arg - 1] = %c\n", args[nb_args - 1][len_last_arg - 1]);
    while (i < nb_args)
    {
        nb_bs = ft_strlen(args[i]);
        if (is_only_bs(args[i]))
        {
            if (nb_bs == 1 || nb_bs % 2 == 0)
            {
                i++;
                continue;
            }
            if (nb_bs % 2 != 0)
            {
                g_code_exit = ERROR_BACKSLASH;
                return (true);
            }
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

bool	search_error_args(int *type_args, int *nb_args, char **args)
{
    if (error_pipe(type_args, *nb_args, args))
    {
        printf("^^^^^^^^^^^     no error pipe    ^^^^^^^^^^^^^^^^^^\n");
        return (1);
    }
	if (error_size_or_spe_redi(type_args, *nb_args, args))
	{
		printf("^^^^^^^^^^^ no error redirection ^^^^^^^^^^^^^^^^^^\n");
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
