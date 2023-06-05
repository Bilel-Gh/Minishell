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

#include "../../includes/minishell.h"

char	*ft_strdup(char *src)
{
    int		i;
    int		len;
    char	*str;

    len = 0;
    while (src[len])
        len++;
    str = (char*)malloc(sizeof(*str) * (len + 1));
    i = 0;
    while (i < len)
    {
        str[i] = src[i];
        i++;
    }
    return (str);
}

// version a completer demain
int ft_is_command(char* value)
{
    (void)value;
    // il faut regarder si DANS LE PATH la chaine correspond a une commande

    char *path;
    char **token;

    path = getenv("PATH");
    char* pathCopy = ft_strdup(path);
    token = ft_split(pathCopy, ":");

    while (*token)
    {
        char *fullPath; // 256 est le max de char dans un path
        fullPath = ft_strjoin(*token, "/");
        fullPath = ft_strjoin(fullPath, value);
        if (access(fullPath, F_OK) == 0)
        {
            printf("\033[0;33m[OK CMD]\033[0m\n");
            free(fullPath);
            free(pathCopy);
            return 1;
        }
        free(fullPath);
        token++;
    }
    free(pathCopy);
    printf("\033[0;31m[NOT OK CMD]\033[0m\n");
    return 0;
}
// version a completer demain
int ft_is_argument(char* value)
{
    if (value[1] == '\0')
        return 1;
    if (value[0] != '-' && value[0] != '|' && value[0] != '>'
        && value[0] != '<')
        return 1;
    return 0;
}

// version a completer demain
int ft_is_option(char* value)
{
    if (value[0] == '-')
        return 1;
    return 0;
}

enum e_token_type get_value_type(t_token* token)
{
    // si c'est une commande
    if (access(token->value, F_OK) == 0)
        return FICHIER;
    else if (token->prev == NULL && token->token_index == 0 && ft_is_command(token->value))
        return COMMANDE;
    else if (ft_is_argument(token->value))
        return ARG;
    else if (ft_is_option(token->value))
        return OPTION;
    else
        return ERROR;
}

int		ft_strncmp(char *s1, char *s2, unsigned int n)
{
    unsigned int	i;
    int				r;

    r = 0;
    i = 0;
    while ((s1[i] || s2[i]) && r == 0 && i < n)
    {
        if (s1[i] != s2[i])
        {
            r = s1[i] - s2[i];
        }
        i++;
    }
    return (r);
}

t_token_info*    add_infos_to_token(char* value, t_token* token)
{
//    printf("\033[0;32m token value : %s\033[0m\n", token->value);
//    if (token->prev != NULL)
//        printf("\033[0;31m token prev value : %s\033[0m\n", token->prev->value);
//    printf("\033[0;32m token index : %d\033[0m\n", token->token_index);
//    if (token->prev != NULL)
//        printf("\033[0;31m token prev index : %d\033[0m\n", token->prev->token_index);
//    if (value == NULL)
//        return NULL;
    t_token_info* infos = (t_token_info*)malloc(sizeof(t_token_info));
    if (ft_strncmp(value, "|", 1) == 0)
        infos->type = N_PIPE;
    else if (ft_strncmp(value, ">", 1) == 0)
        infos->type = REDIRECT_OUT;
    else if (ft_strncmp(value, "<", 1) == 0)
        infos->type = REDIRECT_IN;
    else if (ft_strncmp(value, ">>", 2) == 0)
        infos->type = REDIRECT_D_OUT;
    else if (ft_strncmp(value, "<<", 2) == 0)
        infos->type = REDIRECT_D_IN;
    else
        infos->type = get_value_type(token);

    return infos;
}

t_token* create_token(t_token* head, char* value, int index)
{
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
t_token* ft_get_tokens_with_infos(char **args, int *info_args, int nb_args)
{
    (void)info_args;
    int i;
    i = 0;
    t_token* tokenHead = NULL;

    while (i < nb_args)
    {
        tokenHead = create_token(tokenHead, args[i], i);
        i++;
    }
    i = 0;
    t_token* current = tokenHead;
    while (current != NULL)
    {
        current->info = add_infos_to_token(current->value, current);
        current = current->next;
    }
    return tokenHead;
}
