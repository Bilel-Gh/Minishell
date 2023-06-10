/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_characters2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 04:45:41 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/10 20:05:58 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// permet de savoir si la quote est fermée
// si la quote est fermée, on retourne 1
// si la quote n'est pas fermée, on retourne 0
int	is_quote_end(int curr_state, t_character *next_character)
{
	return ((curr_state == QUOTE_D && next_character->type == QUOTE_D)
		|| (curr_state == QUOTE_S && next_character->type == QUOTE_S));
}

enum e_character_type	get_state(t_character *const *current_charPtr)
{
	enum e_character_type	state;

	if ((*current_charPtr)->type == QUOTE_D)
		state = QUOTE_D;
	else
		state = QUOTE_S;
	return (state);
}

int	ft_is_quote_closed(t_character *current_char)
{
	enum e_character_type	state;
	int						is_quote_closed;

    state = get_state(&current_char);
	current_char = current_char->next;
	is_quote_closed = 0;
	while (current_char)
	{
		if (current_char->type == state)
			is_quote_closed = 1;
		current_char = current_char->next;
	}
	return (is_quote_closed);
}

char	*get_value_wt_quotes(t_character **current_charPtr,
							enum e_character_type *state,
							char *value,
							t_character *nextcharacter)
{
	if (is_quote_end((*state), nextcharacter))
	{
		value = append_value(value, nextcharacter->value);
		if (nextcharacter->next)
			*current_charPtr = nextcharacter->next;
		else
			*current_charPtr = NULL;
	}
	return (value);
}

t_token	*ft_handle_quotes(t_token *tokenHead, t_character **current_charPtr)
{
	enum e_character_type	state;
	char					*value;
	t_character				*nextcharacter;
	int						is_quote_closed;

	is_quote_closed = ft_is_quote_closed(*current_charPtr);
	state = get_state(current_charPtr);
	value = ft_strndup(&(*current_charPtr)->value, 1);
	nextcharacter = (*current_charPtr)->next;
	if (!nextcharacter || (!is_quote_closed && nextcharacter->type != ALPHANUM))
	{
		get_current_charptr(current_charPtr, nextcharacter);
		return (addtoken(tokenHead, value));
	}
	while (!is_quote_end(state, nextcharacter))
	{
		value = append_value(value, nextcharacter->value);
		nextcharacter = nextcharacter->next;
		get_current_charptr(current_charPtr, nextcharacter);
		if (!nextcharacter || (!is_quote_closed
				&& nextcharacter->type != ALPHANUM))
			return (addtoken(tokenHead, value));
	}
	value = get_value_wt_quotes(current_charPtr, &state, value, nextcharacter);
	return (addtoken(tokenHead, value));
}
