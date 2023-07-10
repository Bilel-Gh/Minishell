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
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i] != '\0')
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

int ft_is_digit_char(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

long long int ft_atoll(char *str)
{
    long long int res = 0;
    int negative = 1;

    if (ft_strncmp(str, "9223372036854775807", 19) == 0)
        return (LLONG_MAX);
    if (ft_strncmp(str, "-9223372036854775808", 20) == 0)
        return (LLONG_MIN);
    while (*str && (*str == ' ' || *str == '\n' || *str == '\t' ||
                    *str == '\v' || *str == '\f' || *str == '\r'))
        ++str;
    if (*str == '-')
    {
        negative = -1;
        ++str;
    }
    else if (*str == '+')
    {
        ++str;
    }
    while (*str && *str >= '0' && *str <= '9')
    {
        // Vérifier si le dépassement de plage se produit
        if (res > LLONG_MAX / 10 || (res == LLONG_MAX / 10 && (*str - '0') > LLONG_MAX % 10))
        {
            if (negative == 1)
            {
                g_code_exit = OVERFLOW;
                return (LLONG_MAX);
            }
            else
            {
                g_code_exit = OVERFLOW;
                return (LLONG_MIN);
            }
        }
        res = res * 10 + (*str - '0');
        ++str;
    }

    return res * negative;
}

void builtin_exit(char** args, t_global_parsing **g_pars)
{
    int nb_args;
    char **args_cpy;
    long long int nb_check;

    nb_args = 0;
    args_cpy = args;
    if (args[1] != NULL)
        nb_check = ft_atoll(args[1]);
    else if (args[1] == NULL)
        nb_check = 0;
    printf("exit\n");
    while (args_cpy[nb_args] != NULL)
        nb_args++;
//    printf("ft_atoll(args[1]): %lld\n", ft_atoll(args[1]));
//    printf("nb_min: %lld\n", LLONG_MIN);
//    printf("nb_max: %lld\n", LLONG_MAX);
    if ((args[1] != NULL && ft_isdigit(args[1]) == 0) || ((nb_check >= LLONG_MAX || nb_check <= LLONG_MIN)&& g_code_exit == OVERFLOW))
    {
        printf("exit: %s: numeric argument required\n", args[1]);
        g_code_exit = MISUSE;
        printf("\033[1;31m CODE EXIT DANS EXIT: %d \033[0m\n", g_code_exit); // A SUPPRIMER
        exit(g_code_exit);
    }
    else if (nb_args > 2)
    {
        printf("exit: too many arguments\n");
        g_code_exit = ERROR;
        printf("\033[1;31m CODE EXIT DANS EXIT: %d \033[0m\n", g_code_exit); // A SUPPRIMER
        exit(g_code_exit);
        return ;
    }
    if (args[1] != NULL)
    {
        g_code_exit = ft_atoi(args[1]) % 256;
        if (g_code_exit < 0)
            g_code_exit = 256 + g_code_exit;
        ft_free_g_parsing(*g_pars);
        printf("\033[1;31m CODE EXIT DANS EXIT: %d \033[0m\n", g_code_exit); // A SUPPRIMER
        // free env aussi
        exit(g_code_exit);
    }
    printf("\033[1;31m CODE EXIT DANS EXIT2: %d \033[0m\n", g_code_exit); // A SUPPRIMER
    exit(0);
}
