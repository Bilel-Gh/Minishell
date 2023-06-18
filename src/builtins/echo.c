/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 15:59:33 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/16 15:59:40 by bghandri         ###   ########.fr       */
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
    int len;
    int print_newline;
    char *full_arg;

    i = 1;
    len = 0;
    print_newline = 1; // Variable pour gérer l'option -n
    full_arg = NULL;
    // Vérifier si l'option -n est spécifiée
    if (args[1] != NULL && ft_is_option_echo(args[1]))
    {
        print_newline = 0;
        i = 2;
    }
    ft_get_full_arg(args, &full_arg, i);
    if (full_arg != NULL)
    {
        len = strlen(full_arg);
        write(1, full_arg, len);
        free(full_arg);
    }
    if (print_newline)
        putchar('\n');
}
