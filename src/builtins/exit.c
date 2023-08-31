/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 05:17:04 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 20:08:41 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	free_for_err_exit(t_exec *exec)
{
	free(exec->path_cmd);
	free_db_array(exec->path);
	free_list_tokens(exec->tokens);
	exit(g_code_exit);
}

void	builtin_exit(char **args, t_global_parsing **g_pars, t_exec *exec)
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
	if (ft_gestion_exit_error(args, nb_args, nb_check, g_pars))
		free_for_err_exit(exec);
	ft_do_exit(args, g_pars, exec);
	free(exec->path_cmd);
	free_db_array(exec->path);
	free_list_tokens(exec->tokens);
	ft_free_g_parsing_total(*g_pars);
	exit(g_code_exit);
}
