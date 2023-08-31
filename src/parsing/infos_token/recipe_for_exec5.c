/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recipe_for_exec5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 23:18:15 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 23:20:51 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_init_s(char *str, t_split *s)
{
	s->len = ft_strlen(str);
	s->space_count = count_spaces(str, s->len);
	s->result = allocate_result_array(s->space_count);
	s->in_quotes = 0;
	s->type_inquote = '\0';
	s->start = 0;
	s->i = 0;
	s->count = 0;
}

char	**ft_custom_split(char *str)
{
	t_split	*s;
	char	**final_result;

	s = malloc(sizeof(t_split));
	if (!ft_init_result(str, s))
	{
		final_result = s->result;
		free(s);
		return (final_result);
	}
	ft_init_s(str, s);
	while (s->i < s->len)
	{
		ft_do_split_custom(str, s);
		s->i++;
	}
	s->len_str_to_add = s->i - s->start;
	ft_add_last_str_split(str, s);
	s->clean_result = ft_db_array_dup(s->result);
	ft_free_db_by_len(s->result, s->space_count + 1);
	final_result = s->clean_result;
	free(s);
	return (final_result);
}

void	ft_add_last_str_split(char *str, t_split *s)
{
	if (s->len_str_to_add > 0)
	{
		s->result[s->count] = (char *)malloc((s->len_str_to_add + 1)
				* sizeof(char));
		ft_strncpy(s->result[s->count], str + s->start, s->len_str_to_add);
		s->result[s->count][s->len_str_to_add] = '\0';
	}
}

void	add_cmd_to_list_commande(t_commande *list_commande, char **cmd_join)
{
	int			i;
	t_commande	*list_cmd;

	list_cmd = list_commande;
	i = 0;
	while (list_cmd)
	{
		list_cmd->cmd = ft_custom_split(cmd_join[i]);
		i++;
		if (list_cmd->next == 0)
			break ;
		list_cmd = list_cmd->next;
	}
}

void	free_list_commande(t_commande *commande)
{
	t_commande	*head;

	while (commande->next)
	{
		head = commande;
		commande = commande->next;
		free_db_array(head->cmd);
		free(head);
	}
	if (commande->cmd)
		free_db_array(commande->cmd);
	free(commande);
}
