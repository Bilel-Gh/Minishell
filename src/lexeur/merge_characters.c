/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_characters.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 19:57:00 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/10 20:02:51 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	get_current_charptr(t_character **current_charPtr,
		t_character *next_char)
{
	if (next_char)
		*current_charPtr = next_char;
	else
		*current_charPtr = NULL;
}

// permet d'ajouter un caractère à la fin d'une chaine de caractère
char	*append_value(char *str, char c)
{
	char	*temp;

	temp = (char *)malloc((ft_strlen(str) + 2) * sizeof(char));
	ft_strcpy(temp, str);
	temp[ft_strlen(str)] = c;
	temp[ft_strlen(str) + 1] = '\0';
	free(str);
	return (temp);
}

t_token	*ft_group_spaces(t_token *token_head, t_character **current_charPtr)
{
	char		*value;
	t_character	*next_char;

	value = ft_strndup(&(*current_charPtr)->value, 1);
	next_char = (*current_charPtr)->next;
	if (!next_char)
	{
		*current_charPtr = NULL;
		return (addtoken(token_head, value));
	}
	if (next_char && next_char->type != ESPACE)
	{
		*current_charPtr = next_char;
		return (addtoken(token_head, value));
	}
	while (next_char && next_char->type == ESPACE)
	{
		value = append_value(value, next_char->value);
		next_char = next_char->next;
		get_current_charptr(current_charPtr, next_char);
		if (!(next_char) || next_char->type != ESPACE)
			return (addtoken(token_head, value));
	}
	token_head = addtoken(token_head, value);
	return (token_head);
}

t_token	*ft_group_rest(t_token *token_head, t_character **current_charPtr)
{
	char					*value;
	t_character				*next_char;
	enum e_character_type	type;

	value = ft_strndup(&(*current_charPtr)->value, 1);
	type = (*current_charPtr)->type;
	next_char = (*current_charPtr)->next;
	while (next_char && next_char->type == type)
	{
		value = append_value(value, next_char->value);
		next_char = next_char->next;
	}
	if (next_char)
		*current_charPtr = next_char;
	else
		*current_charPtr = NULL;
	return (addtoken(token_head, value));
}

t_token	*merge_characters(t_character *head)
{
	t_token		*token_head;
	t_character	*current_char;

	token_head = NULL;
	current_char = head;
	while (current_char != NULL)
	{
		if (current_char->type == ESPACE)
		{
			token_head = ft_group_spaces(token_head, &current_char);
			if (current_char == NULL)
				return (token_head);
		}
		if (current_char->type == QUOTE_D || current_char->type == QUOTE_S)
		{
			token_head = ft_handle_quotes(token_head, &current_char);
			if (current_char == NULL)
				return (token_head);
		}
		token_head = ft_group_rest(token_head, &current_char);
		if (current_char == NULL)
			return (token_head);
	}
	return (token_head);
}
