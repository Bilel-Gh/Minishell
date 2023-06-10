/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexeur2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 17:34:25 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/10 19:48:55 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_character	*add_char(t_character *head, char value, enum e_character_type type)
{
	t_character	*newcharacter;
	t_character	*current;

	newcharacter = (t_character *)malloc(sizeof(t_character));
	newcharacter->value = value;
	newcharacter->type = type;
	newcharacter->next = NULL;
	if (head == NULL)
	{
		head = newcharacter;
	}
	else
	{
		current = head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = newcharacter;
	}
	return (head);
}

int	ft_isprint(int chara)
{
	if (chara < 32 || chara > 126)
	{
		return (0);
	}
	return (1);
}

t_character	*ft_get_type_char(const char *input, t_character *head,
		char current_char, enum e_character_type *type)
{
	while (*input != '\0')
	{
		current_char = *input;
		if (current_char == 34)
			(*type) = QUOTE_D;
		else if (current_char == 39)
			(*type) = QUOTE_S;
		else if (current_char == '>' || current_char == '<')
			(*type) = REDIRECT;
		else if (current_char == '|')
			(*type) = PIPE;
		else if (current_char == ' ')
			(*type) = ESPACE;
		else if (ft_isprint(current_char))
			(*type) = ALPHANUM;
		else
		{
			input++;
			continue ;
		}
		head = add_char(head, current_char, (*type));
		input++;
	}
	return (head);
}

t_character	*ft_parse_string(const char *input)
{
	t_character				*head;
	char					current_char;
	enum e_character_type	type;

	head = NULL;
	current_char = 0;
	head = ft_get_type_char(input, head, current_char, &type);
	return (head);
}
