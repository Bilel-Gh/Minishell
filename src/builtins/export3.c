/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 20:16:10 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 20:16:19 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ft_check_name(char *name)
{
	int	i;

	if (name == NULL)
		return (false);
	if (!ft_is_alpha(name[0]) && name[0] != '_')
	{
		return (false);
	}
	i = 1;
	while (name[i] != '\0')
	{
		if (!ft_is_alnum(name[i]) && name[i] != '_')
		{
			return (false);
		}
		i++;
	}
	return (true);
}

int	ft_check_equal_error(char **args, int nb_args)
{
	int	i;

	i = 0;
	if (nb_args > 2)
	{
		while (args[i] != NULL)
		{
			if (args[i][0] == '=')
			{
				ft_fprintf(2,
					"minishell: export: `%s': not a valid identifier\n",
					args[i]);
				g_code_exit = ERROR;
				return (1);
			}
			i++;
		}
	}
	return (0);
}

int	ft_check_no_args(char **args, t_global_exec **g_exec)
{
	int	i;

	if (args[1] == NULL)
	{
		i = 0;
		if ((*g_exec)->export == NULL)
			return (1);
		while ((*g_exec)->export[i] != NULL)
		{
			printf("%s\n", (*g_exec)->export[i]);
			i++;
		}
		return (1);
	}
	return (0);
}

int	ft_check_solo_invalid_arg(char *args)
{
	char	*invalid_chars;

	invalid_chars = "=+%?-@!*#$&(){}[]^~|\\<>;,./123456789";
	if (args == NULL)
		return (0);
	if (args[0] == '\0' || ft_strchr(invalid_chars, args[0]) != NULL)
	{
		if (args[0] == '-' && args[1] != '\0')
		{
			g_code_exit = MISUSE;
			ft_fprintf(2, "minishell: export: `%c%c': invalid option\n", args[0],
				args[1]);
			return (1);
		}
		ft_fprintf(2, "minishell: `%s': not a valid identifier\n", args);
		g_code_exit = ERROR;
		return (1);
	}
	return (0);
}

int	ft_check_export_err(char **args, t_global_exec **g_exec, int nb_args)
{
	if (ft_check_equal_error(args, nb_args))
	{
		g_code_exit = ERROR;
		return (1);
	}
	if (ft_check_no_args(args, g_exec))
		return (1);
	return (0);
}
