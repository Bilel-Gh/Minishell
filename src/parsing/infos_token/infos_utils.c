/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infos_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 04:18:57 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/05 04:19:21 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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
