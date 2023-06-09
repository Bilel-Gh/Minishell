/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexeur.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 07:23:43 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/09 19:21:20 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_character* add_char(t_character* head, char value, enum e_character_type type) {
	t_character* newcharacter = (t_character*)malloc(sizeof(t_character));
	newcharacter->value = value;
	newcharacter->type = type;
	newcharacter->next = NULL;

	if (head == NULL) {
		head = newcharacter;
	} else {
		t_character* current = head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newcharacter;
	}

	return head;
}

int	ft_isprint(int chara)
{
	if (chara < 32 || chara > 126)
	{
		return (0);
	}
	return (1);
}

t_character* parseString(const char* input)
{
	t_character* head = NULL;

	while (*input != '\0') {
		char current_char = *input;
		enum e_character_type type;
		if (current_char == 34)
			type = QUOTE_D;
		else if (current_char == 39)
			type = QUOTE_S;
		else if (current_char == '>' || current_char == '<') {
			type = REDIRECT;
		}  else if (current_char == '|') {
			type = PIPE;
		} else if (current_char == ' ') { // ! a voir si on inclut les tabulations ou pas
			type = ESPACE;
		} else if (ft_isprint(current_char)) {
			type = ALPHANUM;
		} else {
			input++;
			continue;
		}
		head = add_char(head, current_char, type);
		input++;
	}
	return head;
}

t_token* addtoken(t_token* head, char* value)
{
	t_token* newtoken = (t_token*)malloc(sizeof(t_token));
	newtoken->value = value;
	newtoken->info = NULL;
	newtoken->next = NULL;

	if (head == NULL) {
		head = newtoken;
	} else {
		t_token* current = head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newtoken;
	}

	return head;
}

char** get_args(t_token* head)
{
	int count = 0;
	t_token* current = head;
	char	**args;

	// while (current != NULL) {
	// 	current = current->next;
	// }
	while (current != NULL) {
		count++;
		current = current->next;
	}
	args = (char**)malloc((count + 1) * sizeof(char*));
	args[count] = NULL; // Marquer la fin du tableau avec NULL
	current = head;
	int i = 0;
	while (current != NULL) {
		args[i] = ft_strdup(current->value);
		i++;
		current = current->next;
	}
	args[i] = NULL;

	return args;
}

void free_list_characters(t_character* characters)
{
	t_character* head;
	while (characters->next)
	{
		head = characters;
		characters = characters->next;
		free(head);
	}
	free(characters);
}

void free_list_tokens(t_token* tokens)
{
	t_token* head;
	if (!tokens)
		return ;
	while(tokens->next)
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

char **ft_lexeur(char *line)
{
	t_character* characters;
	t_token* tokens;
	char **args;

	characters = parseString(line); // Ici on récupère une liste chaînée de characters
    // afficher tous les characters
     t_character* current = characters;
     // *debug*
     int i = 0;
     while (current != NULL) {
         printf("character %d ---------------> '%c'\n", i, current->value);
         i++;
         current = current->next;
     }
    // *debug*
	tokens = merge_characters(characters); // ici on groupe les characters adjacents de même type dans des nœuds
	args = get_args(tokens); // on cree un double tableau avec chaque argument = un token
	free_list_characters(characters);
	free_list_tokens(tokens);
	return (args);
	// return (ft_split(line));
}
