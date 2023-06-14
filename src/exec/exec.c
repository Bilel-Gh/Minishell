/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:21:17 by ncharii           #+#    #+#             */
/*   Updated: 2023/06/14 18:13:51 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int nb_token_whis_index(t_token *tokens, int index)
{
	int i;
	t_token *seach;

	i = 0;
	seach = tokens;
	while (seach)
	{
		if (seach->token_index = index)
			i++;
		seach = seach->next;
	}
	return (i);
}

void	creat_info_token_list(t_token *tokens, int nb_node)
{
	int			i;

	t_token	*tmp;
	t_token	*for_creat;

	i = 1;
	while (i < nb_node)
	{
		tmp = malloc(sizeof(t_token));
		if (!tmp)
		{
			free_list_tokens(tokens);
			return ;
		}
		tmp->next = 0;
		for_creat->next = tmp;
		for_creat = for_creat->next;
		i++;
	}
}

t_token dup_info(t_token *info_token, t_token *tokens, index)
{
	je dois dup les token dans l ordre qui coresponde a l index;
}

t_token *get_info_token(t_token *tokens, int index)
{
	t_token *info_token;
	int nb_token;

	nb_token = nb_token_whis_index(tokens, index);
	info_token = NULL;
	if (nb_token == 0)
		return (info_token);
	info_token = malloc(sizeof(t_token));
	if (!info_token)
		return (NULL);
	creat_info_token_list(info_token, nb_token);
	if (!info_token)
		return (NULL);
	info_token = dup_info(info_token, tokens, index);




}

void exec(t_token *tokens, t_commande *commande)
{
	t_token *info_token;
	int i;

	i = 0;
	while (commande)
	{
		info_token = get_info_token(tokens, i);
		i++;
		//start_exec(info_token, commande->cmd)
		commande = commande->next;
	}
}
