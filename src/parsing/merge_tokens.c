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

// fonction qui copie n octets de src dans dst
void	*ft_memcpy(void *dst, const void *src, size_t n)
{
    size_t i;

    if (!dst && !src)
        return (0);
    i = 0;
    while (i < n)
    {
        ((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
        i++;
    }
    return (dst);
}

// fonction qui duplique une chaine de caracteres
char	*ft_strndup(const char *s1, size_t n)
{
    char *s2;

    if (!(s2 = (char *)malloc(n + 1)))
        return (0);
    ft_memcpy(s2, s1, n);
    s2[n + 1] = 0;
    return (s2);
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

// permet d'ajouter un caractère à la fin d'une chaine de caractère
char	*append_value(char *str, char c)
{
    char	*temp = (char *)malloc((ft_strlen(str) + 2) * sizeof(char));
    ft_strcpy(temp, str);
    temp[ft_strlen(str)] = c;
    temp[ft_strlen(str) + 1] = '\0';
    free(str);
    return temp;
}

t_node	*ft_group_spaces(t_node *nodeHead, t_token **currentTokenPtr)
{
    char	*value;
    t_token	*nextToken;

    value = ft_strndup(&(*currentTokenPtr)->value, 1);
    nextToken = (*currentTokenPtr)->next;
    if (!nextToken)
        return addNode(nodeHead, value);
    while (nextToken->type == ESPACE)
    {
        value = append_value(value, nextToken->value);
        *currentTokenPtr = (*currentTokenPtr)->next;
        if (!(nextToken = nextToken->next) || nextToken->type != ESPACE)
        {
            if (nextToken)
                *currentTokenPtr = (*currentTokenPtr)->next;
            return addNode(nodeHead, value);
        }
    }
    nodeHead = addNode(nodeHead, value);
    if (nextToken)
        *currentTokenPtr = (*currentTokenPtr)->next;
    return nodeHead;
}

// permet de savoir si la quote est fermée
// si la quote est fermée, on retourne 1
// si la quote n'est pas fermée, on retourne 0
int	is_quote_end(int curr_state, t_token *next_token)
{
    return ((curr_state == QUOTE_D && next_token->type == QUOTE_D) ||
            (curr_state == QUOTE_S && next_token->type == QUOTE_S));
}

t_node	*ft_handle_quotes(t_node *nodeHead, t_token **currentTokenPtr)
{
    int		state;
    char	*value;
    t_token	*nextToken;

    state = ((*currentTokenPtr)->type == QUOTE_D) ? QUOTE_D : QUOTE_S;
    value = ft_strndup(&(*currentTokenPtr)->value, 1);
    nextToken = (*currentTokenPtr)->next;
    if (!nextToken)
        return (addNode(nodeHead, value));
    while (!is_quote_end(state, nextToken))
    {
        value = append_value(value, nextToken->value);
        nextToken = nextToken->next;
        *currentTokenPtr = (*currentTokenPtr)->next;
        if (!nextToken || !(*currentTokenPtr))
            return (addNode(nodeHead, value));
    }
    if (is_quote_end(state, nextToken))
    {
        value = append_value(value, nextToken->value);
        nextToken = nextToken->next;
        *currentTokenPtr = (*currentTokenPtr)->next;
        if (!nextToken)
            return (addNode(nodeHead, value));
    }
    printf("QUOTE RESET -----> currentToken->value: %c\n", (*currentTokenPtr)->value);
    *currentTokenPtr = (*currentTokenPtr)->next;
    return (addNode(nodeHead, value));
}

t_node	*ft_group_rest(t_node *nodeHead, t_token **currentTokenPtr)
{
    char	*value;
    t_token	*nextToken;
    enum e_token_type type;

    value = ft_strndup(&(*currentTokenPtr)->value, 1);
    type = (*currentTokenPtr)->type;
    nextToken = (*currentTokenPtr)->next;
    while (nextToken && nextToken->type == type)
    {
        value = append_value(value, nextToken->value);
        nextToken = nextToken->next;
        *currentTokenPtr = (*currentTokenPtr)->next;
    }
    printf("REST RESET -----> currentToken->value: %c\n", (*currentTokenPtr)->value);
    if (!nextToken)
        return addNode(nodeHead, value);
    nodeHead = addNode(nodeHead, value);
    *currentTokenPtr = (*currentTokenPtr)->next;
    return nodeHead;
}

t_node* mergeTokens(t_token* head) {
    t_node* nodeHead = NULL;
    t_token* currentToken = head;
    while (currentToken != NULL)
    {
        if (currentToken->type == ESPACE) {
            nodeHead = ft_group_spaces(nodeHead, &currentToken);
            if ((currentToken == NULL || currentToken->next == NULL) && currentToken->type == ESPACE) // a gerer
                return nodeHead;
        }
        if (currentToken->type == QUOTE_D || currentToken->type == QUOTE_S) {
            nodeHead = ft_handle_quotes(nodeHead, &currentToken);
            printf("currentToken->value: %c\n", currentToken->value);
            printf("currentToken->type: %d\n", currentToken->type);
            if ((currentToken == NULL || currentToken->next == NULL) && (currentToken->type == QUOTE_D || currentToken->type == QUOTE_S)) // A gerer
                return nodeHead;
        }
        nodeHead = ft_group_rest(nodeHead, &currentToken);
        if (currentToken->next == NULL || currentToken == NULL)
            return nodeHead;
    }

    return nodeHead;
}