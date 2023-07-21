/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexeur.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 07:23:43 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/21 22:08:46 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*addtoken(t_token *head, char *value)
{
	t_token	*newtoken;
	t_token	*current;

	newtoken = (t_token *)malloc(sizeof(t_token));
	newtoken->value = value;
	newtoken->info = NULL;
	newtoken->next = NULL;
	if (head == NULL)
	{
		head = newtoken;
	}
	else
	{
		current = head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = newtoken;
	}
	return (head);
}

char	**get_args(t_token *head)
{
	int		count;
	t_token	*current;
	char	**args;
	int		i;

	count = 0;
	current = head;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	args = (char **)malloc((count + 1) * sizeof(char *));
	args[count] = NULL;
	current = head;
	i = 0;
	while (current != NULL)
	{
		args[i] = ft_strdup(current->value);
		i++;
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

void	free_list_characters(t_character *characters)
{
	t_character	*head;

	while (characters->next)
	{
		head = characters;
		characters = characters->next;
		free(head);
	}
	free(characters);
}

void	free_list_tokens(t_token *tokens)
{
	t_token	*head;

	if (!tokens)
		return ;
	while (tokens->next)
	{
		head = tokens;
		tokens = tokens->next;
		free(head->value);
		if (head->info)
			free(head->info);
		free(head);
	}
	if (tokens->info)
		free(tokens->info);
	free(tokens->value);
	free(tokens);
}

char	**ft_lexeur(char *line)
{
	t_character	*characters;
	t_token		*tokens;
	char		**args;

	characters = ft_parse_string(line);
	tokens = merge_characters(characters);
	args = get_args(tokens);
	free_list_characters(characters);
	free_list_tokens(tokens);
    char **tmp = args;
    while (*tmp)
    {
        printf("\033[0;31m arg = %s\n \033[0m", *tmp);
        tmp++;
    }
	return (args);
}
