/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infos_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 04:18:57 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/05 10:21:27 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int ft_is_prefixe_ok(t_token *token)
{
    if (token->token_index == 0 || token->prev->info->type == PATH || token->prev->info->type == ENV)
        return 1;
    return 0;
}

int ft_is_suffixe_ok(t_token *token)
{
    if (token->next->info->type == REDIRECT_D_OUT || token->next->info->type == REDIRECT_OUT
        || token->next->info->type == N_PIPE || token->next->info->type == ARG
        || token->next->info->type == OPTION )
        return 1;
    return 0;
}

// version a completer demain
int ft_is_command(t_token *token)
{
    if(!ft_is_prefixe_ok(token) && !ft_is_suffixe_ok(token))
        return 0;
    char **path_splited;
    char *path;
    path = getenv("PATH");
    char* pathCopy = ft_strdup(path);
    path_splited = ft_split(pathCopy, ":");

    while (*path_splited)
    {
        char *fullPath; // 256 est le max de char dans un path
        fullPath = ft_strjoin(*path_splited, "/");
        fullPath = ft_strjoin(fullPath, token->value);
        if (access(fullPath, 0) == 0)
        {
            printf("\033[0;33m[OK CMD]\033[0m\n");
            free(fullPath);
            free(pathCopy);
            return 1;
        }
        free(fullPath);
        path_splited++;
    }
    free(pathCopy);
    printf("\033[0;31m[NOT OK CMD]\033[0m\n");
    return 0;
}

int ft_is_upper(char c)
{
    if (c >= 'A' && c <= 'Z')
        return 1;
    return 0;
}

int is_env(t_token* token)
{
    char* value;
    int len;
    int i;

    value = token->value;
    len = strlen(value);
    i = 0;
    // Je Vérifie si le token est dans le format "NOM=valeur" a voir si je rajoute d'autre verif par la suite
    while (i < len) {
        if (value[i] == '=')
        {
            int j = 0;
            while (j < i)
            {
                if (!ft_is_upper(value[j]))
                    return 0;
                j++;
            }
            return 1;
        }
        i++;
    }
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
