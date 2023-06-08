/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:16:18 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/08 22:42:44 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int ft_get_nb_space_in_value(char* value)
{
    int i;
    int nb_space;
    i = 0;
    nb_space = 0;
    while (value[i] != '\0')
    {
        if (value[i] == ' ')
        {
            while (value[i] == ' ' && value[i] != '\0')
                i++;
            nb_space++;
        }
        i++;
    }
    return nb_space;
}

void ft_insert_tokens(t_token *tmp, char **value_splited) {
    int i;
    free(tmp->value);
    tmp->value = ft_strdup(value_splited[0]);
    tmp->info->type = COMMANDE;
    i = 1;
    while (value_splited[i] != NULL)
    {
        t_token* newToken = create_token(NULL, value_splited[i], i);
        newToken->info = add_infos_to_token(value_splited[i], newToken, 0);
        newToken->prev = tmp;
        if (tmp->next != NULL) {
            newToken->next = tmp->next;
            tmp->next->prev = newToken;
        }

        tmp->next = newToken;
        tmp = newToken;
        i++;
    }
}

void ft_add_end_of_list(t_token *current, t_token *save_next_token) {
    if (save_next_token != NULL) {
        t_token* lastToken = current;
        while (lastToken->next != NULL) {
            lastToken = lastToken->next;
        }
        lastToken->next = save_next_token;
        save_next_token->prev = lastToken;
    }
}

t_token* ft_verif_cmd(t_token** tokenHead)
{
    t_token* current;
    t_token* save_next_tokens;
    t_token* tmp;
    int nb_space;
    char **value_splited;

    current = *tokenHead;
    save_next_tokens = NULL;
    while (current != NULL)
    {
        nb_space = ft_get_nb_space_in_value(current->value);
        if (current->info->type == COMMANDE && nb_space > 0)
        {
            save_next_tokens = ft_save_next_tokens(current);
            value_splited = ft_split(current->value, " ");
            tmp = current;
            ft_insert_tokens(tmp, value_splited);
            ft_free_value_splited(value_splited);
        }
        if (current->next == NULL)
            break;
        current = current->next;
    }
    ft_add_end_of_list(current, save_next_tokens);
    return *tokenHead;
}
