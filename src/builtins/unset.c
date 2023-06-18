/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 23:39:03 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/17 23:39:09 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_unsetenv(char *name, char ***env)
{
    int i;
    int j;

    i = 0;
    while ((*env)[i] != NULL) {
        j = 0;
        while ((*env)[i][j] != '=' && (*env)[i][j] == name[j]) {
            j++;
        }
        if ((*env)[i][j] == '=') {
            while ((*env)[i] != NULL) {
                (*env)[i] = (*env)[i + 1];
                i++;
            }
            return (0);
        }
        i++;
    }
    return (-1);
}

void builtin_unset(char **args, char ***env)
{
    char *name = args[1];

    if (name == NULL) {
        // Pas d'argument spécifié, afficher un message d'erreur
        printf("unset: nom de variable manquant\n");
        return;
    }

    // Supprimer la variable d'environnement spécifiée
    if (ft_unsetenv(name, env) != 0) {
        perror("unset");
    }
}
