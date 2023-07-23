/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 00:21:09 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/24 00:21:24 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_is_error_parsing(t_global_parsing *g_parsing, int nb_args)
{
	if (g_code_exit == ERROR_REDIRECT2)
	{
		g_code_exit = ERROR;
		ft_free_g_parsing(g_parsing);
		ft_init_global_parsing(g_parsing);
		return (1);
	}
	else if (g_code_exit == NOTFOUND)
	{
		ft_fprintf(2, "bash: : command not found\n");
		return (1);
	}
	if (nb_args == 0)
	{
		g_code_exit = SUCCESS;
		ft_free_g_parsing(g_parsing);
		return (1);
	}
	if (ft_is_error_par_part2(g_parsing))
		return (1);
	return (0);
}

int	ft_is_error_par_part2(t_global_parsing *g_parsing)
{
	if (g_code_exit != SUCCESS)
	{
		if (!ft_custom_error(g_parsing->args))
			ft_fprintf(2, "bash: syntax error near unexpected token '%c%c'\n",
				g_parsing->args[0][0], g_parsing->args[0][1]);
		g_code_exit = MISUSE;
		ft_free_g_parsing(g_parsing);
		return (1);
	}
	if (!g_parsing->args)
		return (1);
	else
		return (0);
}

char	**ft_db_array_dup(char **db_array)
{
	int		i;
	char	**db_array_cpy;

	i = 0;
	while (db_array[i])
		i++;
	db_array_cpy = malloc(sizeof(char *) * (i + 1));
	if (!db_array_cpy)
		return (NULL);
	i = 0;
	while (db_array[i])
	{
		db_array_cpy[i] = ft_strdup(db_array[i]);
		i++;
	}
	db_array_cpy[i] = NULL;
	return (db_array_cpy);
}

char	*ft_add_quotes_export(char *str)
{
	int		equal_found;
	int		len;
	char	*new_str;

	equal_found = 0;
	len = ft_strlen(str);
	new_str = malloc(sizeof(char) * (len + 3));
	if (new_str == NULL)
		return (NULL);
	ft_add_quote_exp(str, equal_found, new_str);
	free(str);
	return (new_str);
}

void	ft_add_quote_exp(const char *str, int equal_found, char *new_str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '=' && !equal_found)
		{
			new_str[j++] = str[i++];
			new_str[j++] = '"';
			equal_found = 1;
			continue ;
		}
		new_str[j++] = str[i++];
	}
	if (equal_found)
		new_str[j++] = '"';
	new_str[j] = '\0';
}
