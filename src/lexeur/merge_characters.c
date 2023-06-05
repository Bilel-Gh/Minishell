/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_characters.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 17:34:41 by bghandri          #+#    #+#             */
/*   Updated: 2023/05/31 17:02:01 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_delete_quotes(t_token *currenttoken, int len_value, int type_start, int type_end);

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
    s2[n] = 0;
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
    dest[i] = '\0';
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

t_token	*ft_group_spaces(t_token *tokenHead, t_character **currentcharacterPtr)
{
    char	*value;
    t_character	*nextcharacter;

    value = ft_strndup(&(*currentcharacterPtr)->value, 1); // ! gerer le cas ou le character est un seule espace
    nextcharacter = (*currentcharacterPtr)->next;
    if (!nextcharacter)
    {
        *currentcharacterPtr = NULL;
        return (addtoken(tokenHead, value));
    }
    if (nextcharacter && nextcharacter->type != ESPACE)
    {
        *currentcharacterPtr = nextcharacter;
        return addtoken(tokenHead, value);
    }
    while (nextcharacter && nextcharacter->type == ESPACE)
    {
        value = append_value(value, nextcharacter->value);
        nextcharacter = nextcharacter->next;
        *currentcharacterPtr = nextcharacter ? nextcharacter : NULL;
        if (!(nextcharacter) || nextcharacter->type != ESPACE)
            return addtoken(tokenHead, value);
    }
    tokenHead = addtoken(tokenHead, value);
    return tokenHead;
}

// permet de savoir si la quote est fermée
// si la quote est fermée, on retourne 1
// si la quote n'est pas fermée, on retourne 0
int	is_quote_end(int curr_state, t_character *next_character)
{
    return ((curr_state == QUOTE_D && next_character->type == QUOTE_D) ||
            (curr_state == QUOTE_S && next_character->type == QUOTE_S));
}

int ft_is_quote_closed(t_character *currentcharacter)
{
    enum e_character_type state;
    int is_quote_closed;

    state = (currentcharacter->type == QUOTE_D) ? QUOTE_D : QUOTE_S;
    currentcharacter = currentcharacter->next;
    is_quote_closed = 0;
    while (currentcharacter)
    {
        if (currentcharacter->type == state)
            is_quote_closed = 1;
        currentcharacter = currentcharacter->next;
    }
    return is_quote_closed;
}

t_token	*ft_handle_quotes(t_token *tokenHead, t_character **currentcharacterPtr)
{
    enum e_character_type		state;
    char	*value;
    t_character	*nextcharacter;
    int is_quote_closed;

    is_quote_closed = ft_is_quote_closed(*currentcharacterPtr);
    state = ((*currentcharacterPtr)->type == QUOTE_D) ? QUOTE_D : QUOTE_S;
    value = ft_strndup(&(*currentcharacterPtr)->value, 1);
    nextcharacter = (*currentcharacterPtr)->next;
    if (!nextcharacter || (!is_quote_closed && nextcharacter->type != ALPHANUM))
    {
        *currentcharacterPtr = nextcharacter ? nextcharacter : NULL;
        return (addtoken(tokenHead, value));
    }
    while (!is_quote_end(state, nextcharacter))
    {
        value = append_value(value, nextcharacter->value);
        nextcharacter = nextcharacter->next;
        *currentcharacterPtr = nextcharacter ? nextcharacter : NULL;
        if (!nextcharacter || (!is_quote_closed && nextcharacter->type != ALPHANUM))
            return (addtoken(tokenHead, value));
    }
    if (is_quote_end(state, nextcharacter))
    {
        value = append_value(value, nextcharacter->value);
        *currentcharacterPtr = (nextcharacter->next) ? nextcharacter->next : NULL;
    }
    return (addtoken(tokenHead, value));
}

t_token	*ft_group_rest(t_token *tokenHead, t_character **currentcharacterPtr)
{
    char	*value;
    t_character	*nextcharacter;
    enum e_character_type type;

    value = ft_strndup(&(*currentcharacterPtr)->value, 1);
    type = (*currentcharacterPtr)->type;
    nextcharacter = (*currentcharacterPtr)->next;
    while (nextcharacter && nextcharacter->type == type)
    {
        value = append_value(value, nextcharacter->value);
        nextcharacter = nextcharacter->next;
    }
    *currentcharacterPtr = nextcharacter ? nextcharacter : NULL;
    return addtoken(tokenHead, value);
}

void ft_delete_quotes(t_token *currenttoken, int len_value, int type_start, int type_end)
{
    len_value = ft_strlen(currenttoken->value);
    type_start = currenttoken->value[0] == 34 ? QUOTE_D : QUOTE_S;
    if (currenttoken->value[len_value - 1] == 34 || currenttoken->value[len_value - 1] == 39)
    {
        type_end = currenttoken->value[len_value - 1] == 34 ? QUOTE_D : QUOTE_S;
    }
    if (type_start == type_end)
    {
        currenttoken->value[len_value - 1] = '\0';
        currenttoken->value = currenttoken->value + 1;
    }
}

// permet de supprimer les quotes
void	ft_clean_quotes(t_token *tokenHead)
{
    t_token	*currenttoken;
    int		len_value;
    int     type_start;
    int     type_end;

    currenttoken = tokenHead;
    len_value = 0;
    type_start = 0;
    type_end = 1;
    while (currenttoken)
    {
        if (currenttoken->value[1] == 0)
        {
            currenttoken = currenttoken->next;
            continue;
        }
        if (currenttoken->value[0] == 34 || currenttoken->value[0] == 39)
        {
            ft_delete_quotes(currenttoken, len_value, type_start, type_end);
        }
        currenttoken = currenttoken->next;
    }
}

t_token* mergecharacters(t_character* head) {
    t_token* tokenHead = NULL;
    t_character* currentcharacter = head;
    while (currentcharacter != NULL)
    {
        if (currentcharacter->type == ESPACE) {
            tokenHead = ft_group_spaces(tokenHead, &currentcharacter);
            // printf avec ANSI color
            //printf("\033[0;32m currentcharacter->value = '%c'\n\033[0m", currentcharacter->value);
            if (currentcharacter == NULL) // a gerer
            {
                printf("currentcharacter == NULL\n");
                return tokenHead;
            }
        }
        if (currentcharacter->type == QUOTE_D || currentcharacter->type == QUOTE_S) {
            tokenHead = ft_handle_quotes(tokenHead, &currentcharacter);
            // ft_clean_quotes(tokenHead);
            if (currentcharacter == NULL)
                return tokenHead;
        }
        tokenHead = ft_group_rest(tokenHead, &currentcharacter);
        if (currentcharacter == NULL)
            return tokenHead;
    }

    return tokenHead;
}
