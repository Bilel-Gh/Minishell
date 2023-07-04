/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 15:59:33 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/03 19:39:37 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_is_option_echo(char *str)
{
    if (str[0] == '-')
    {
        if (str[1] == 'n')
        {
            str++;
            while (*str == 'n')
                str++;
            if (*str == '\0')
                return (1);
            else
                return (0);
        }
        else
            return (0);
    }
    else
        return (0);
}

void ft_get_full_arg(char **args, char **full_arg, int i)
{
    int j;
    j = i;
    if (args[j] == NULL)
        return;
    *full_arg = ft_strdup(args[j]);
    j++;
    while (args[j] != NULL)
    {
        *full_arg = ft_join_cmd(*full_arg, args[j]);
        j++;
    }
}

void builtin_echo(char **args)
{
    int i;
    int print_newline;
    char *full_arg;

    i = 1;
	printf("42 echo\
    print_newline = 1; // Variable pour gérer l'option -n
    full_arg = NULL;
    // Vérifier si l'option -n est spécifiée
    if (args[i] != NULL && ft_is_option_echo(args[i]))
    {
        print_newline = 0;
        i = 2;
        while (args[i] != NULL && ft_is_option_echo(args[i]))
            i++;
    }
    ft_get_full_arg(args, &full_arg, i);
    if (full_arg != NULL)
    {
        // si full_arg commence par un $ et qu'il y a des charactères après on enleve le dollard
        if (full_arg[0] == '$' && full_arg[1] != '\0')
            full_arg = ft_substr(full_arg, 1, ft_strlen(full_arg) - 1);
        printf("%s", full_arg);
        free(full_arg);
    }
    if (print_newline)
        printf("\n");
}
