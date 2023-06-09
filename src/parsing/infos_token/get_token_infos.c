/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token_infos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 05:03:47 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/05 04:04:56 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

enum e_token_type get_value_type(t_token* token, int* is_commande_in_pipe)
{
    if (ft_is_infile(token))
        return INFILE;
    else if (ft_is_outfile(token))
        return OUTFILE;
    else if (token->prev && token->prev->info->type == REDIRECT_D_OUT)
        return LIMITEUR;
    else if (ft_is_command(token) && *is_commande_in_pipe == 0)
    {
        *is_commande_in_pipe = 1;
        return COMMANDE;
    }
    else
        return ARG;
}

t_token_info*    add_infos_to_token(char* value, t_token* token, int* is_commande_in_pipe)
{
    if (value == NULL)
        return NULL;
    t_token_info* infos = (t_token_info*)malloc(sizeof(t_token_info));
    if (ft_strncmp(value, "|", 1) == 0)
    {
        *is_commande_in_pipe = 0;
        infos->type = T_PIPE;
    }
    else if (ft_strcmp(value, ">") == 0)
        infos->type = REDIRECT_OUT;
    else if (ft_strcmp(value, "<") == 0)
        infos->type = REDIRECT_IN;
    else if (ft_strcmp(value, ">>") == 0)
        infos->type = REDIRECT_D_OUT;
    else if (ft_strcmp(value, "<<") == 0)
        infos->type = REDIRECT_D_IN;
    else
        infos->type = get_value_type(token, is_commande_in_pipe);
    return infos;
}

t_token* create_token(t_token* head, char* value, int index)
{
    // enlever les espaces en trop au debut et a la fin
    value = ft_strtrim(value, " ");
    t_token* newtoken = (t_token*)malloc(sizeof(t_token));
    newtoken->value = value;
    newtoken->token_index = index;
    newtoken->next = NULL;
    newtoken->prev = NULL;

    if (head == NULL) {
        head = newtoken;
    } else {
        t_token* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newtoken;
        newtoken->prev = current;
    }
    return head;
}

// TODO :
// il faut dabbord cree les token avec prev et next add_token simple
// apres ajouter les infos avec une nouvelle fonction add_infos_to_token
t_token* ft_get_tokens_with_infos(char **args, int nb_args)
{
    int i;
    i = 0;
    t_token* tokenHead = NULL;
    t_token* current;

    while (i < nb_args)
    {
        tokenHead = create_token(tokenHead, args[i], i);
        i++;
    }
    i = 0;
    current = tokenHead;
    int is_commande_in_pipe = 0;
    while (current != NULL)
    {
        current->info = add_infos_to_token(current->value, current, &is_commande_in_pipe);
        current = current->next;
    }
    tokenHead = ft_verif_cmd(&tokenHead);
    return tokenHead;
}
