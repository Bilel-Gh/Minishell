/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 02:13:01 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/18 02:50:49 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_size_of_expende(char *expande)
{
	int	size;

	size = 1;
	while (1)
	{
		if ((expande[size] == '@' && size > 1)
			|| (expande[size] == '#' && size > 1)
			|| (expande[size] == '[' && size > 1)
			|| (expande[size] == ']' && size > 1)
			|| (expande[size] == '/' && size > 1)
			|| (expande[size] == '=')
			|| (expande[size] == ':' && size > 1)
			|| (expande[size] == '-' && size > 1)
			|| (expande[size] == '%' && size > 1)
			|| (expande[size] == '$' && size > 1)
			|| (expande[size] == 92 && size > 1)
			|| (expande[size] == ' ' && size > 1)
			|| (expande[size] == 0)
			|| (expande[size] == 39)
			|| (expande[size] == 34))
		{
			size--;
			break ;
		}
		size++;
	}
	return (size);
}

char	*get_expende_detect(int size_of_expende, char *expande)
{
	char	*txt_expande;
	int		i;

	i = 0;
	txt_expande = malloc(sizeof(char) * (size_of_expende + 1));
	while (i < size_of_expende && expande)
	{
		txt_expande[i] = expande[i + 1];
		i++;
	}
	txt_expande[i] = 0;
	return (txt_expande);
}

char	*is_sp_expand(char *expand_search)
{
	char	*sp_expand;

	if (!expand_search)
		return (NULL);
	if (expand_search[0] == '@')
	{
		sp_expand = &expand_search[1];
		return (sp_expand);
	}
	return (NULL);
}

char	*give_env_expand(char *expande_search, int size, char **env)
{
	int		i;
	char	*sp_expand;
	char	*for_search;

	i = 0;
	sp_expand = NULL;
//	if (!expande_search)
//		return (NULL);
	sp_expand = is_sp_expand(expande_search);
	for_search = ft_strdup(expande_search);
	for_search = ft_strjoin(for_search, "=");
	if (sp_expand)
		return (sp_expand);
	while (env[i])
	{
		if (!strncmp(for_search, env[i], size + 1)) // ! a changer
			break ;
		i++;
	}
	free(for_search);
	free(expande_search);
	if (env[i] == NULL)
		return (NULL);
	return (&env[i][size + 1]);
}

int	join_expand(char *expande, int *j, int s_extract, char *new_args)
{
	int	index;

	index = 0;
	if (expande)
	{
		while (expande[index])
		{
			new_args[*j] = expande[index];
			index++;
			*j = *j + 1;
		}
	}
	return (s_extract);
}

bool back_slach(char *args, int i)
{
	int nb_back_s;

	nb_back_s = 0;
	if (i == 0)
		return (true);
	i = i - 1;
	while (args[i] == 92)
	{
		i--;
		nb_back_s++;
		if (i < 0)
			break;
	}
	if (nb_back_s % 2 == 0)
		return (true);
	else
		return (false);
}

char	*join_and_rp_args(char *args_con, char *expande, int s_extract)
{
	char	*new_args;
	int		i;
	int		j;
	bool	expande_in;

	expande_in = false;
	i = 0;
	i = strlen(args_con) - s_extract;
	i = 0;
	j = 0;
	new_args = NULL;
	if ((strlen(args_con) - (s_extract + 1) == 0) && (expande == NULL))
		return (new_args);
	if (!expande)
		new_args = malloc((strlen(args_con) - s_extract) + 1);
	else
		new_args = malloc(strlen(args_con) + (strlen(expande) - s_extract) + 1);
	if (!new_args)
		exit (0);
	while (args_con[i])
	{
		if (args_con[i] == '$' && expande_in == false && back_slach(args_con, i))
		{
			i = i + join_expand(expande, &j, s_extract, new_args);
			if (args_con[i] != '$' )
				i++;
			expande_in = true;
		}
		new_args[j] = args_con[i];
		if (args_con[i] == 0)
			break;
		i++;
		j++;
	}
	new_args[j] = 0;
	free(args_con);
	return (new_args);
}
