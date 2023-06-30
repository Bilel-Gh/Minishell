/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 22:51:14 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/17 22:51:15 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_check_args(char **args)
{
    int i;
    int j;

    i = 1;
    j = 0;
    if (args[i] == NULL)
        return (0);
    if (args[i][0] == '-')
    {
        // temps que c'est un -
        while (args[i][j] == '-')
            j++;
        if (args[i][j] != 0)
        {
            printf("pwd: -%c: invalid option\n", args[i][j]);
            return (1);
        }
    }
    return (0);
}

void builtin_pwd(char **args)
{
    if (ft_check_args(args))
        return;
    char cwd[SIZE_PATH]; // Taille maximale du chemin d'accès = 4096 octets

    if (getcwd(cwd, SIZE_PATH))
        printf("%s\n", cwd);
    else
    {
        printf("pwd: error retrieving current directory");
        printf("getcwd: cannot access parent directories\n");
    }
}