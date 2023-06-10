/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:16:18 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/10 21:04:35 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_get_nb_space_in_value(char *value)
{
	int	i;
	int	nb_space;

	i = 0;
	nb_space = 0;
	while (value[i] != '\0')
	{
		if (value[i] == ' ')
		{
			while (value[i] == ' ' && value[i] != '\0')
				i++;
			nb_space++;
		}
		i++;
	}
	return (nb_space);
}

void	ft_insert_tokens(t_token *tmp, char **value_splited)
{
	int		i;
	t_token	*new_token;

	free(tmp->value);
	tmp->value = ft_strdup(value_splited[0]);
	tmp->info->type = COMMANDE;
	i = 1;
	while (value_splited[i] != NULL)
	{
		new_token = create_token(NULL, value_splited[i], i);
		new_token->info = add_infos_to_token(value_splited[i], new_token, 0);
		new_token->prev = tmp;
		if (tmp->next != NULL)
		{
			new_token->next = tmp->next;
			tmp->next->prev = new_token;
		}
		tmp->next = new_token;
		tmp = new_token;
		i++;
	}
}

void	ft_add_end_of_list(t_token *current, t_token *save_next_token)
{
	t_token	*last_token;

	if (save_next_token != NULL)
	{
		last_token = current;
		while (last_token->next != NULL)
		{
			last_token = last_token->next;
		}
		last_token->next = save_next_token;
		save_next_token->prev = last_token;
	}
}

t_token	*ft_verif_cmd(t_token **tokenHead)
{
	t_token	*current;
	t_token	*save_next_tokens;
	t_token	*tmp;
	int		nb_space;
	char	**value_splited;

	current = *tokenHead;
	save_next_tokens = NULL;
	while (current != NULL)
	{
		nb_space = ft_get_nb_space_in_value(current->value);
		if (current->info->type == COMMANDE && nb_space > 0)
		{
			save_next_tokens = ft_save_next_tokens(current);
			value_splited = ft_split(current->value, ' ');
			tmp = current;
			ft_insert_tokens(tmp, value_splited);
			ft_free_value_splited(value_splited);
		}
		if (current->next == NULL)
			break ;
		current = current->next;
	}
	ft_add_end_of_list(current, save_next_tokens);
	return (*tokenHead);
}
