/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 20:04:46 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 20:08:54 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	ft_do_exit(char *const *args, t_global_parsing *const *g_pars, t_exec *exec)
{
	if (args[1] != NULL)
	{
		g_code_exit = ft_atoi(args[1]) % 256;
		if (g_code_exit < 0)
			g_code_exit = 256 + g_code_exit;
		free(exec->path_cmd);
		free_db_array(exec->path);
		free_list_tokens(exec->tokens);
		ft_free_g_parsing_total(*g_pars);
		exit(g_code_exit);
	}
}

int	ft_gestion_exit_error(char *const *args, int nb_args,
		long long int nb_check, t_global_parsing **g_pars)
{
	if ((args[1] != NULL && ft_isdigit(args[1]) == 0) || ((nb_check >= LLONG_MAX
				|| nb_check <= LLONG_MIN) && g_code_exit == OVERFLOW))
	{
		ft_fprintf(2, "exit: %s: numeric argument required\n", args[1]);
		g_code_exit = MISUSE;
		ft_free_g_parsing_total(*g_pars);
		return (1);
	}
	else if (nb_args > 2)
	{
		ft_fprintf(2, "exit: too many arguments\n");
		g_code_exit = ERROR;
		ft_free_g_parsing_total(*g_pars);
		return (1);
	}
	return (0);
}
