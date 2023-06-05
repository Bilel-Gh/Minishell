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

t_token	*ft_group_spaces(t_token *tokenHead, t_character **current_charPtr)
{
    char	*value;
    t_character	*next_char;

    value = ft_strndup(&(*current_charPtr)->value, 1); // ! gerer le cas ou le character est un seule espace
    next_char = (*current_charPtr)->next;
    if (!next_char)
    {
        *current_charPtr = NULL;
        return (addtoken(tokenHead, value));
    }
    if (next_char && next_char->type != ESPACE)
    {
        *current_charPtr = next_char;
        return addtoken(tokenHead, value);
    }
    while (next_char && next_char->type == ESPACE)
    {
        value = append_value(value, next_char->value);
        next_char = next_char->next;
        *current_charPtr = next_char ? next_char : NULL;
        if (!(next_char) || next_char->type != ESPACE)
            return addtoken(tokenHead, value);
    }
    tokenHead = addtoken(tokenHead, value);
    return tokenHead;
}

t_token	*ft_group_rest(t_token *tokenHead, t_character **current_charPtr)
{
    char	*value;
    t_character	*next_char;
    enum e_character_type type;

    value = ft_strndup(&(*current_charPtr)->value, 1);
    type = (*current_charPtr)->type;
    next_char = (*current_charPtr)->next;
    while (next_char && next_char->type == type)
    {
        value = append_value(value, next_char->value);
        next_char = next_char->next;
    }
    *current_charPtr = next_char ? next_char : NULL;
    return addtoken(tokenHead, value);
}

t_token* merge_characters(t_character* head) {
    t_token* tokenHead = NULL;
    t_character* current_char = head;
    while (current_char != NULL)
    {
        if (current_char->type == ESPACE) {
            tokenHead = ft_group_spaces(tokenHead, &current_char);
            // printf avec ANSI color
            //printf("\033[0;32m current_char->value = '%c'\n\033[0m", current_char->value);
            if (current_char == NULL) // a gerer
            {
                printf("current_char == NULL\n");
                return tokenHead;
            }
        }
        if (current_char->type == QUOTE_D || current_char->type == QUOTE_S) {
            tokenHead = ft_handle_quotes(tokenHead, &current_char);
            //ft_clean_quotes(tokenHead);
            if (current_char == NULL)
                return tokenHead;
        }
        tokenHead = ft_group_rest(tokenHead, &current_char);
        if (current_char == NULL)
            return tokenHead;
    }

    return tokenHead;
}
