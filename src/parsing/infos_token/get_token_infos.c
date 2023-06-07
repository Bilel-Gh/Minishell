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

int	ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

t_token_info*    add_infos_to_token(char* value, t_token* token, int* is_commande_in_pipe)
{
//    printf("\033[0;32m token value : %s\033[0m\n", token->value);
//    if (token->prev != NULL)
//        printf("\033[0;31m token prev value : %s\033[0m\n", token->prev->value);
//    printf("\033[0;32m token index : %d\033[0m\n", token->token_index);
//    if (token->prev != NULL)
//        printf("\033[0;31m token prev index : %d\033[0m\n", token->prev->token_index);
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

char	*ft_strchr(const char *s, int c)
{
    char	*str;

    str = (char *)s;
    while (*str != c)
    {
        if (*str == '\0')
        {
            return (NULL);
        }
        str++;
    }
    return (str);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
    unsigned int	i;

    i = 0;
    if (!dst || !src)
        return (0);
    if (size > 0)
    {
        while (--size && src[i])
        {
            dst[i] = src[i];
            i++;
        }
        dst[i] = '\0';
    }
    while (src[i])
        i++;
    return (i);
}

// fonction qui enleve les charactere de set au debut et a la fin de s1
char	*ft_strtrim(char *s1, char *set)
{
    int		start;
    int		end;
    char	*str;

    if (!s1 || !set)
        return (NULL);
    start = 0;
    end = ft_strlen(s1) - 1;
    while (ft_strchr(set, s1[start]) && start <= end)
        start++;
    if (start > end)
        return (ft_strdup(s1 + end + 1));
    while (ft_strchr(set, s1[end]) && end >= 0)
        end--;
    str = malloc(end - start + 2);
    if (!str)
        return (NULL);
    ft_strlcpy(str, &s1[start], end - start + 2);
    return (str);
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

t_token* ft_verif_cmd(t_token** tokenHead)
{
    t_token* current = *tokenHead;
//    t_token* save_next_token;
    t_token* tmp;
    char **value_splited;

    while (current != NULL)
    {
        if (current->info->type == COMMANDE)
        {
            if (ft_get_nb_space_in_value(current->value) > 0)
            {
//                 if (current->next != NULL)
//                 {
//
//                 }
                value_splited = ft_split(current->value, " ");
                tmp = current;
                current = current->next;

                free(tmp->value);
                tmp->value = ft_strdup(value_splited[0]);
                tmp->info->type = COMMANDE;

                tmp->next = create_token(tmp->next, value_splited[1], tmp->token_index + 1);
                tmp->next->info = add_infos_to_token(value_splited[1], tmp->next, 0);
                tmp->next->prev = tmp;

                int i = 2;

                while (value_splited[i] != NULL)
                {
                    t_token* newToken = create_token(NULL, value_splited[i], tmp->token_index + i);
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

                i = 0;

                while (value_splited[i] != NULL)
                {
                    free(value_splited[i]);
                    i++;
                }

                free(value_splited);
            }
        }

        if (current->next == NULL)
            break;

        current = current->next;
    }

        // Ajout du token suivant et tous les tokens suivants à la fin de la manipulation
    // if (save_next_token != NULL) {
    //     t_token* lastToken = *tokenHead;
    //     while (lastToken->next != NULL) {
    //         lastToken = lastToken->next;
    //     }
    //     lastToken->next = save_next_token;
    //     save_next_token->prev = lastToken;
    // }
    return *tokenHead;
}


// TODO :
// il faut dabbord cree les token avec prev et next add_token simple
// apres ajouter les infos avec une nouvelle fonction add_infos_to_token
t_token* ft_get_tokens_with_infos(char **args, int *info_args, int nb_args)
{
    (void)info_args;
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
