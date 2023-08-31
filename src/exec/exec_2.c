/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:19:19 by ncharii           #+#    #+#             */
/*   Updated: 2023/08/29 14:23:14 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	nb_token_whis_index(t_token *tokens, int index)
{
	int		i;
	t_token	*seach;

	i = 0;
	seach = tokens;
	while (seach)
	{
		if (seach->token_index == index)
			i++;
		seach = seach->next;
	}
	return (i);
}

t_token	*creat_info_token_list(t_token *tokens, int nb_node)
{
	int		i;
	t_token	*tmp;
	t_token	*for_creat;
	t_token	*head;

	for_creat = tokens;
	head = tokens;
	i = 1;
	while (i < nb_node)
	{
		tmp = malloc(sizeof(t_token));
		if (!tmp)
		{
			free_list_tokens(tokens);
			return (NULL);
		}
		tmp->next = 0;
		for_creat->next = tmp;
		for_creat = for_creat->next;
		i++;
	}
	return (head);
}

int	copy_cont_token(t_token *dest, t_token *src)
{
	dest->token_index = src->token_index;
	dest->value = ft_strdup(src->value);
	dest->info = (t_token_info *)malloc(sizeof(t_token_info));
	if (!dest->info)
		return (1);
	dest->info->type = src->info->type;
	dest->prev = NULL;
	return (0);
}

t_token	*dup_info(t_token *info_token, t_token *tokens, int index)
{
	t_token	*tokens_value_index;
	t_token	*all;

	all = tokens;
	tokens_value_index = info_token;
	while (all)
	{
		if (all->token_index == index)
		{
			if (copy_cont_token(tokens_value_index, all))
			{
				return (NULL);
			}
			tokens_value_index = tokens_value_index->next;
		}
		all = all->next;
	}
	return (info_token);
}

t_token	*get_info_token(t_token *tokens, int index)
{
	t_token	*info_token;
	int		nb_token;

	nb_token = nb_token_whis_index(tokens, index);
	info_token = NULL;
	if (nb_token == 0)
		return (info_token);
	info_token = malloc(sizeof(t_token));
	info_token->next = NULL;
	if (!info_token)
		return (NULL);
	info_token = creat_info_token_list(info_token, nb_token);
	if (!info_token)
		return (NULL);
	info_token = dup_info(info_token, tokens, index);
	return (info_token);
}
