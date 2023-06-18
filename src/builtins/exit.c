/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 05:17:04 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/18 05:17:10 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_atoi(const char *str)
{
    int	res;
    int	negative;

    negative = 1;
    res = 0;
    while (*str && (*str == ' ' || *str == '\n' || *str == '\t' ||
                    *str == '\v' || *str == '\f' || *str == '\r'))
        ++str;
    if (*str == '-')
        negative = -1;
    if (*str == '-' || *str == '+')
        ++str;
    while (*str && *str >= '0' && *str <= '9')
    {
        res = res * 10 + (*str - 48);
        ++str;
    }
    return (res * negative);
}

int ft_isdigit(char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

void builtin_exit(char** args, t_global_parsing **g_pars)
{
    int exit_code;
    int nb_args;
    char **args_cpy;

    nb_args = 0;
    args_cpy = args;
    while (args_cpy[nb_args] != NULL)
        nb_args++;
    if (nb_args > 2)
    {
        printf("exit: too many arguments\n");
        return ;
    }
    if (args[1] != NULL && ft_isdigit(args[1]) == 0)
    {
        printf("exit: numeric argument required\n");
        return ;
    }
    if (args[1] != NULL)
    {
        exit_code = ft_atoi(args[1]);
        ft_free_g_parsing(*g_pars);
        exit(exit_code);
    }
    exit(0);
}
