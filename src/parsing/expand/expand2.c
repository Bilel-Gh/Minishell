/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 02:13:01 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/13 17:21:21 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_size_of_expende(char *expande)
{
	int	size;

	size = 1;
	while (expande[size] != ' ' && expande[size] != 0 && expande[size] != '$'
		&& expande[size] != 34 && expande[size] != 39)
	{
		if ((expande[size] == '@' && size > 1)
			|| (expande[size] == '#' && size > 1))
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
	while (i < size_of_expende)
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

	if (expand_search[0] == '@')
	{
		sp_expand = &expand_search[1];
		return (sp_expand);
	}
	return (NULL);
}

char	*give_env_expand(char *expande_search, int size)
{
	int		i;
	char	*sp_expand;

	i = 0;
	sp_expand = NULL;
	sp_expand = is_sp_expand(expande_search);
	if (sp_expand)
		return (sp_expand);
	while (environ[i])
	{
		if (!strncmp(expande_search, environ[i], size - 1)) // ! a changer
			break ;
		i++;
	}
	free(expande_search);
	if (environ[i] == NULL)
		return (NULL);
	return (&environ[i][size]);
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

char	*join_and_rp_args(char *args_con, char *expande, int s_extract)
{
	char	*new_args;
	int		i;
	int		j;
	bool	expande_in;

	expande_in = false;
	i = 0;
	j = 0;
	if (!expande)
		new_args = malloc(strlen(args_con) - s_extract);
	else
		new_args = malloc(strlen(args_con) + (strlen(expande) - s_extract) + 1);
	if (!new_args)
		exit (0);// renplacer par la fonction free_all;
	while (args_con[i])
	{
		if (args_con[i] == '$' && expande_in == false)
			i = i + join_expand(expande, &j, s_extract, new_args);
		new_args[j] = args_con[i];
		if (args_con[i])
			i++;
		j++;
	}
	free(args_con);
	return (new_args);
}
