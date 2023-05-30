/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 17:34:41 by bghandri          #+#    #+#             */
/*   Updated: 2023/05/29 17:34:49 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_strlen(char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return (i);
}

char	*ft_strcpy(char *dest, char *src)
{
    int i;

    i = 0;
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i + 1] = src[i];
    return (dest);
}


// fonction qui regroupe tous les tokens de type espace qui se suivent dans un seul node
t_node* ft_group_spaces(t_node* nodeHead, t_token* currentToken) {
    char* value;
    t_token* nextToken;
    char* temp;

    value = (char*)malloc(2 * sizeof(char)); // Pour stocker le caractère et le terminateur null
    value[0] = currentToken->value;
    value[1] = '\0';
    nextToken = currentToken->next;
    if (nextToken == NULL) {
        nodeHead = addNode(nodeHead, value);
        return nodeHead;
    }
    while (nextToken->type == ESPACE)
    {
        temp = (char*)malloc((ft_strlen(value) + 2) * sizeof(char)); // +2 pour le caractère et le terminateur null
        ft_strcpy(temp, value); // copie de value dans temp
        temp[ft_strlen(value)] = nextToken->value; // ajout du caractère suivant à la fin de temp
        temp[ft_strlen(value) + 1] = '\0'; // ajout du terminateur null
        free(value);
        value = temp;
        currentToken = currentToken->next;
        nextToken = nextToken->next;
        if (nextToken == NULL || nextToken->type != ESPACE) {
            nodeHead = addNode(nodeHead, value);
            return nodeHead;
        }
    }
    nodeHead = addNode(nodeHead, value);
    return nodeHead;
}

t_node* ft_handle_quotes(t_node* nodeHead, t_token** currentTokenPtr) {
    int state;
    char* value;
    t_token* nextToken;

    state = ((*currentTokenPtr)->type == QUOTE_D) ? 1 : 2;
    value = (char*)malloc(2 * sizeof(char));
    value[0] = (*currentTokenPtr)->value;
    value[1] = '\0';
    nextToken = (*currentTokenPtr)->next;
    if (nextToken == NULL) {
        nodeHead = addNode(nodeHead, value);
        return nodeHead;
    }
    while ((state == 1 && nextToken->type != QUOTE_D) || (state == 2 && nextToken->type != QUOTE_S)) {
        char* temp = (char*)malloc((ft_strlen(value) + 2) * sizeof(char));
        ft_strcpy(temp, value);
        temp[ft_strlen(value)] = nextToken->value;
        temp[ft_strlen(value) + 1] = '\0';
        free(value);
        value = temp;
        nextToken = nextToken->next;
        (*currentTokenPtr) = (*currentTokenPtr)->next;
        if (nextToken == NULL || (*currentTokenPtr) == NULL) {
            nodeHead = addNode(nodeHead, value);
            return nodeHead;
        }
    }
    if ((state == 1 && nextToken->type == QUOTE_D) || (state == 2 && nextToken->type == QUOTE_S)) {
        char* temp = (char*)malloc((ft_strlen(value) + 2) * sizeof(char));
        ft_strcpy(temp, value);
        temp[ft_strlen(value)] = nextToken->value;
        temp[ft_strlen(value) + 1] = '\0';
        free(value);
        value = temp;
        nextToken = nextToken->next;
        (*currentTokenPtr) = (*currentTokenPtr)->next;
        if (nextToken == NULL) {
            nodeHead = addNode(nodeHead, value);
            return nodeHead;
        }
    }
    nodeHead = addNode(nodeHead, value);
    (*currentTokenPtr) = (*currentTokenPtr)->next;
    return nodeHead;
}

t_node* ft_group_rest(t_node* nodeHead, t_token** currentTokenPtr) {
    char* value;
    t_token* nextToken;
    enum e_token_type type;
    char* temp;

    value = (char*)malloc(2 * sizeof(char)); // Pour stocker le caractère et le terminateur null
    value[0] = (*currentTokenPtr)->value;
    value[1] = '\0';
    type = (*currentTokenPtr)->type;
    nextToken = (*currentTokenPtr)->next;
    while (nextToken != NULL && nextToken->type == type) {
        temp = (char*)malloc((ft_strlen(value) + 2) * sizeof(char)); // +2 pour le caractère et le terminateur null
        ft_strcpy(temp, value);
        temp[ft_strlen(value)] = nextToken->value;
        temp[ft_strlen(value) + 1] = '\0';
        free(value);
        value = temp;
        nextToken = nextToken->next;
        (*currentTokenPtr) = (*currentTokenPtr)->next;
    }
    if (nextToken == NULL)
    {
        nodeHead = addNode(nodeHead, value);
        return nodeHead;
    }
    nodeHead = addNode(nodeHead, value);
    (*currentTokenPtr) = (*currentTokenPtr)->next;
    return nodeHead;
}

t_node* mergeTokens(t_token* head) {
    t_node* nodeHead = NULL;
    t_token* currentToken = head;

    while (currentToken != NULL)
    {
        if (currentToken->type == ESPACE) {
            nodeHead = ft_group_spaces(nodeHead, currentToken);
            if (currentToken->next == NULL)
                return nodeHead;
            while (currentToken->type == ESPACE)
                currentToken = currentToken->next;
        }
        if (currentToken->type == QUOTE_D || currentToken->type == QUOTE_S) {
            nodeHead = ft_handle_quotes(nodeHead, &currentToken);
            if (currentToken->next == NULL || currentToken == NULL)
                return nodeHead;
        }
        nodeHead = ft_group_rest(nodeHead, &currentToken);
        if (currentToken->next == NULL || currentToken == NULL)
            return nodeHead;
    }

    return nodeHead;
}