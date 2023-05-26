/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexeur.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 07:23:43 by bghandri          #+#    #+#             */
/*   Updated: 2023/05/26 19:22:18 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token* addToken(t_token* head, char value, enum e_token_type type) {
	t_token* newToken = (t_token*)malloc(sizeof(t_token));
	newToken->value = value;
	newToken->type = type;
	newToken->next = NULL;

	if (head == NULL) {
		head = newToken;
	} else {
		t_token* current = head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newToken;
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

t_token* parseString(const char* input) {
	t_token* head = NULL;

	while (*input != '\0') {
		char currentChar = *input;

		// Déterminer le type du caractère
		enum e_token_type type;
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
		head = addToken(head, currentChar, type);

		input++;
	}

	return head;
}

t_node* addNode(t_node* head, char* value) {
	t_node* newNode = (t_node*)malloc(sizeof(t_node));
	newNode->value = value;
	newNode->next = NULL;

	if (head == NULL) {
		head = newNode;
	} else {
		t_node* current = head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newNode;
	}

	return head;
}

t_node* mergeTokens(t_token* head) {
	t_node* nodeHead = NULL;
	int state;
	t_token* currentToken = head;
	while (currentToken != NULL) 
	{
		state = 0;
		if (currentToken->type == ESPACE) {
			currentToken = currentToken->next;
			continue;
		}
		if (currentToken->type == QUOTE_D || currentToken->type == QUOTE_S)
		{
			if (currentToken->type == QUOTE_D)
				state = 1;
			else
				state = 2;
			char* value = (char*)malloc(2 * sizeof(char)); // Pour stocker le caractère et le terminateur null
			value[0] = currentToken->value;
			value[1] = '\0';
			//printf("c = %c\n",currentToken->value);
			//enum e_token_type type = currentToken->type;

			t_token* nextToken = currentToken->next;
			if (nextToken == NULL)
				return nodeHead;

			while ((state == 1 && nextToken->type != QUOTE_D) || (state == 2 && nextToken->type != QUOTE_S))
			{
				char* temp = (char*)malloc((strlen(value) + 2) * sizeof(char)); // +2 pour le caractère et le terminateur null
				strcpy(temp, value);
				temp[strlen(value)] = nextToken->value;
				temp[strlen(value) + 1] = '\0';
			//	printf("intra c1 = %s\n",temp);
				free(value);
				value = temp;


				nextToken = nextToken->next;
				currentToken = currentToken->next;
				if (state == 0 && nextToken->type == ESPACE)
					printf("jj\n");
				if (nextToken == NULL)
				{
					nodeHead = addNode(nodeHead, value);
					return nodeHead;
				}
				continue ;              
			}
			if ((state == 1 && nextToken->type == QUOTE_D) || (state == 2 && nextToken->type == QUOTE_S))
			{
				char* temp = (char*)malloc((strlen(value) + 2) * sizeof(char)); // +2 pour le caractère et le terminateur null
				strcpy(temp, value);
				temp[strlen(value)] = nextToken->value;
				temp[strlen(value) + 1] = '\0';
//				printf("intra c2 = %s\n",temp);
				free(value);
				value = temp;


				nextToken = nextToken->next;
				currentToken = currentToken->next;
				if (state == 0 && nextToken->type == ESPACE)
					printf("jj\n");
				if (nextToken == NULL) 
				{
					nodeHead = addNode(nodeHead, value);
					return nodeHead;
				}

			}

			nodeHead = addNode(nodeHead, value);
			currentToken = currentToken->next;

		}
		char* value = (char*)malloc(2 * sizeof(char)); // Pour stocker le caractère et le terminateur null
		value[0] = currentToken->value;
		value[1] = '\0';

		enum e_token_type type = currentToken->type;

		t_token* nextToken = currentToken->next;

		while (nextToken != NULL && nextToken->type == type) {
			char* temp = (char*)malloc((strlen(value) + 2) * sizeof(char)); // +2 pour le caractère et le terminateur null
			strcpy(temp, value);
			temp[strlen(value)] = nextToken->value;
			temp[strlen(value) + 1] = '\0';

			free(value);
			value = temp;

			nextToken = nextToken->next;
			currentToken = currentToken->next;
		}

		nodeHead = addNode(nodeHead, value);

		currentToken = currentToken->next;
	}

	return nodeHead;
}

char** get_args(t_node* head) {
	int count = 0;
	t_node* current = head;
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
	// ici il faudra spliter correctement la ligne de commande avec la tokenisation et les listes chainées
	// pour l'instant on se contente de séparer les mots par des espaces
	t_token* tokens;
	t_node* nodes;
	char **args;

	tokens = parseString(line); // Ici on récupère une liste chaînée de tokens un token et un char
	nodes = mergeTokens(tokens); // ici on groupe les tokens adjacents de même type dans des nœuds
	args = get_args(nodes); // on cree un double tableau avec chaque argument = un node
	return (args);
	// return (ft_split(line));
}
