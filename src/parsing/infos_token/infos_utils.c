/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infos_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 04:18:57 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/06 09:58:08 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

// version a completer demain
int ft_is_command(t_token *token)
{
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

int ft_get_infos_by_pos(t_token *token)
{
    if (token->token_index == 0)
        return UNDEFINED_CMD;
    if (token->prev != NULL && token->prev->info->type == T_PIPE)
        return UNDEFINED_CMD;
    return ARG;
}

int ft_is_infile(t_token *token)
{
    if (token->prev == NULL)
        return 0;
    if (token->prev->info->type == REDIRECT_IN || token->prev->info->type == REDIRECT_D_IN)
        return 1;
    return 0;
}

int ft_is_outfile(t_token *token)
{
    if (token->prev == NULL)
        return 0;
    if (token->prev->info->type == REDIRECT_OUT || token->prev->info->type == REDIRECT_D_OUT)
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
