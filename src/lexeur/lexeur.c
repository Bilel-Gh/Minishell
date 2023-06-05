/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexeur.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 07:23:43 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/05 04:05:06 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_character* addcharacter(t_character* head, char value, enum e_character_type type) {
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

t_character* parseString(const char* input) {
	t_character* head = NULL;

	while (*input != '\0') {
		char currentChar = *input;

		// Déterminer le type du caractère
		enum e_character_type type;
		if (currentChar == 34)
			type = QUOTE_D;
		else if (currentChar == 39)
			type = QUOTE_S;
		else if (currentChar == '>' || currentChar == '<') {
			type = REDIRECT;
		}  else if (currentChar == '|') {
			type = PIPE;
		} else if (currentChar == ' ') { // ! a voir si on inclut les tabulations ou pas
			type = ESPACE;
		} else if (ft_isprint(currentChar)) {
			type = ALPHANUM;
		} else {
			input++;
			continue;
		}

		// Ajouter le caractère à la liste chaînée
		head = addcharacter(head, currentChar, type);

		input++;
	}

	return head;
}

t_token* addtoken(t_token* head, char* value)
{
	t_token* newtoken = (t_token*)malloc(sizeof(t_token));
	newtoken->value = value;
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

char** get_args(t_token* head) {
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
		args[i] = current->value;
		i++;
		current = current->next;
	}
	args[i] = NULL;

	return args;
}


char **ft_lexeur(char *line)
{
	t_character* characters;
	t_token* tokens;
	char **args;

	characters = parseString(line); // Ici on récupère une liste chaînée de characters un character et un char
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
	tokens = mergecharacters(characters); // ici on groupe les characters adjacents de même type dans des nœuds
	args = get_args(tokens); // on cree un double tableau avec chaque argument = un token
	return (args);
	// return (ft_split(line));
}
