/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 05:17:04 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/16 19:44:36 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void			ft_gestion_exit_error(char *const *args, int nb_args,
					long long int nb_check, t_global_parsing **g_pars);

void			ft_do_exit(char *const *args, t_global_parsing *const *g_pars);

void			ft_check_atoll(char **str, int *negative);

long long int	ft_check_overflow(const char *str, long long int res,
					int negative);

int	ft_atoi(const char *str)
{
	int	res;
	int	negative;

	negative = 1;
	res = 0;
	while (*str && (*str == ' ' || *str == '\n' || *str == '\t' || *str == '\v'
			|| *str == '\f' || *str == '\r'))
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

int	ft_isdigit(char *str)
{
	int	i;

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

long long int	ft_atoll(char *str)
{
	long long int	res;
	int				negative;

	res = 0;
	negative = 1;
	if (ft_strncmp(str, "9223372036854775807", 19) == 0)
		return (LLONG_MAX);
	if (ft_strncmp(str, "-9223372036854775808", 20) == 0)
		return (LLONG_MIN);
	ft_check_atoll(&str, &negative);
	while (*str && *str >= '0' && *str <= '9')
	{
		if (ft_check_overflow(str, res, negative) != 0)
			return (ft_check_overflow(str, res, negative));
		res = res * 10 + (*str - '0');
		++str;
	}
	return (res * negative);
}

long long int	ft_check_overflow(const char *str, long long int res,
		int negative)
{
	if (res > LLONG_MAX / 10 || (res == LLONG_MAX / 10 && (*str
				- '0') > LLONG_MAX % 10))
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
	return (0);
}

void	ft_check_atoll(char **str, int *negative)
{
	while (*(*str) && (*(*str) == ' ' || *(*str) == '\n' || *(*str) == '\t'
			|| *(*str) == '\v' || *(*str) == '\f' || *(*str) == '\r'))
		++(*str);
	if (*(*str) == '-')
	{
		(*negative) = -1;
		++(*str);
	}
	else if (*(*str) == '+')
		++(*str);
}

void	builtin_exit(char **args, t_global_parsing **g_pars)
{
	int				nb_args;
	char			**args_cpy;
	long long int	nb_check;

	nb_args = 0;
	args_cpy = args;
	if (args[1] != NULL)
		nb_check = ft_atoll(args[1]);
	else if (args[1] == NULL)
		nb_check = 0;
	printf("exit\n");
	while (args_cpy[nb_args] != NULL)
		nb_args++;
	ft_gestion_exit_error(args, nb_args, nb_check, g_pars);
	ft_do_exit(args, g_pars);
	exit(g_code_exit);
}

void	ft_do_exit(char *const *args, t_global_parsing *const *g_pars)
{
	if (args[1] != NULL)
	{
		g_code_exit = ft_atoi(args[1]) % 256;
		if (g_code_exit < 0)
			g_code_exit = 256 + g_code_exit;
		ft_free_g_parsing_total(*g_pars);
		exit(g_code_exit);
	}
}

void	ft_gestion_exit_error(char *const *args, int nb_args,
		long long int nb_check, t_global_parsing **g_pars)
{
	if ((args[1] != NULL && ft_isdigit(args[1]) == 0) || ((nb_check >= LLONG_MAX
				|| nb_check <= LLONG_MIN) && g_code_exit == OVERFLOW))
	{
		ft_fprintf(2, "exit: %s: numeric argument required\n", args[1]);
		g_code_exit = MISUSE;
		ft_free_g_parsing_total(*g_pars);
		exit(g_code_exit);
	}
	else if (nb_args > 2)
	{
		ft_fprintf(2, "exit: too many arguments\n");
		g_code_exit = ERROR;
		ft_free_g_parsing_total(*g_pars);
		exit(g_code_exit);
	}
}
