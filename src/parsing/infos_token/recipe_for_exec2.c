/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recipe_for_exec2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 23:17:57 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 23:19:43 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	init_struc_cmd(t_commande *commande)
{
	commande->cmd = 0;
}

int	nb_pipe(t_token *info)
{
	int		nbr_pipe;
	t_token	*search_pipe;

	search_pipe = info;
	nbr_pipe = 0;
	while (search_pipe)
	{
		if (search_pipe->info->type == T_PIPE)
			nbr_pipe++;
		search_pipe = search_pipe->next;
	}
	return (nbr_pipe);
}

void	creat_cmd_list(t_commande *cmd, int nb_node)
{
	int			i;
	t_commande	*tmp;

	i = 1;
	while (i < nb_node)
	{
		tmp = malloc(sizeof(t_commande));
		if (!tmp)
			return ;
		init_struc_cmd(tmp);
		tmp->next = 0;
		cmd->next = tmp;
		cmd = cmd->next;
		i++;
	}
}

char	*ft_join_cmd(char *s1, char *s2)
{
	char	*str;
	int		size_s1;
	int		size_s2;
	int		i;

	i = 0;
	size_s1 = ft_strlen(s1);
	size_s2 = ft_strlen(s2);
	str = malloc(sizeof(char) * (size_s1 + size_s2 + 2));
	if (!str)
		return (0);
	ft_complete(s1, str, &i);
	str[i] = ' ';
	i++;
	ft_complete(s2, str, &i);
	if (s1)
		free(s1);
	str[i] = '\0';
	return (str);
}

void	b_zero_for_cmd_join(char **cmd_join, int nb_cdm, int i)
{
	cmd_join[nb_cdm] = 0;
	while (i < nb_cdm)
	{
		cmd_join[i] = 0;
		i++;
	}
	i = 0;
}
