/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recipe_for_exec3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 23:18:07 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/25 12:35:29 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	**give_cmd_join(t_token *token, int nb_cdm)
{
	char	**cmd_join;
	int		i;
	t_token	*tok_sch;
	int		is_first;

	i = 0;
	is_first = 1;
	tok_sch = token;
	if (!ft_malloc_cmd_join(nb_cdm, &cmd_join, i))
		return (0);
	while (tok_sch)
	{
		while (tok_sch && i < nb_cdm)
		{
			ft_do_join_cmd(cmd_join, i, tok_sch, &is_first);
			if (tok_sch->info->type == T_PIPE)
				break ;
			tok_sch = tok_sch->next;
		}
		i++;
		if (i != nb_cdm)
			tok_sch = tok_sch->next;
	}
	return (cmd_join);
}

int	ft_malloc_cmd_join(int nb_cdm, char ***cmd_join, int i)
{
	*cmd_join = malloc(sizeof(char *) * (nb_cdm + 1));
	if (!(*cmd_join))
		return (0);
	b_zero_for_cmd_join(*cmd_join, nb_cdm, i);
	return (1);
}

void	ft_do_join_cmd(char **cmd_join, int i, t_token *tok_sch, int *is_first)
{
	if (tok_sch->info->type == COMMANDE || tok_sch->info->type == ARG)
		cmd_join[i] = ft_join_cmd(cmd_join[i], tok_sch->value);
	*is_first = 0;
}

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		dest[i] = src[i];
		++i;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

void	ft_free_db_by_len(char **array, int len)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (i < len)
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = 0;
}
