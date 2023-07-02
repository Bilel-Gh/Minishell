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
        while ((*env)[i][j] != '=' && (*env)[i][j] != 0)
            j++;
        if (ft_strncmp((*env)[i], name, j) == 0)
        {
            free((*env)[i]);
            while ((*env)[i + 1] != NULL)
            {
                (*env)[i] = (*env)[i + 1];
                i++;
            }
            (*env)[i] = NULL;
            return 0;
        }
        i++;
    }
    return (-1);
}

int ft_unset_export(char *name, char **export)
{
    char *to_unset;
    to_unset = ft_strjoin(ft_strdup("export "), name);
    if (to_unset == NULL)
        return (-1);
    int i;

    i = 0;
    while (export[i] != NULL)
    {
        if (ft_strncmp(export[i], to_unset, ft_strlen(to_unset)) == 0)
        {
            free(export[i]);
            while (export[i + 1] != NULL)
            {
                export[i] = export[i + 1];
                i++;
            }
            export[i] = NULL;
            free(to_unset);
            return 0;
        }
        i++;
    }
    return (-1);
}

int ft_is_in_env(char *name, char **env)
{
    int i;
    int j;

    i = 0;
    while (env[i] != NULL) {
        j = 0;
        while (env[i][j] != '=' && env[i][j] != 0)
            j++;
        if (ft_strncmp(env[i], name, j) == 0)
            return (1);
        i++;
    }
    return (0);
}

void builtin_unset(char **args, char ***env, t_global_exec **g_exec)
{
    char *name;
    int i;

    i = 1;
    name = NULL;
    while (args[i] != NULL)
    {
        name = args[i];
        if (name == NULL) {
            // Pas d'argument spécifié, afficher un message d'erreur
            printf("unset: nom de variable manquant\n");
            i++;
            continue;
        }
        if (!ft_check_name(name))
        {
            printf("minishell: unset: `%s': not a valid identifier !\n", name);
            g_code_exit = ERROR;
            i++;
            continue;
        }
        if (ft_check_solo_invalid_arg(args[i]))
        {
            i++;
            continue;
        }

        // Supprimer la variable d'environnement spécifiée
        if (ft_unset_export(name, (*g_exec)->export) != 0)
        {
            i++;
            continue;
        }
        if (ft_is_in_env(name, *env) == 0)
        {
            i++;
            continue;
        }
        if (ft_unsetenv(name, env) != 0)
        {
            i++;
            continue;
        }
        i++;
    }
}
