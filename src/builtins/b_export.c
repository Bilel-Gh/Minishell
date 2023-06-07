/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 02:56:40 by bghandri          #+#    #+#             */
/*   Updated: 2023/05/25 08:05:34 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern char **environ;

void builtin_export(char **args)
{
    char *var = args[1];
    int i = 0;

    if (var == NULL) {
        // Afficher toutes les variables d'environnement
        while (environ[i] != NULL) {
            printf("%s\n", environ[i]);
            i++;
        }
    } else {
        // Rechercher le caractère '=' pour diviser la variable en clé et valeur
        char *equal_sign = ft_strchr(var, '=');
        if (equal_sign == NULL) {
            // Pas de '=' trouvé, afficher un message d'erreur
            printf("export: '%s': invalide format de variable\n", var);
            return;
        }

        // Diviser la variable en clé et valeur
        *equal_sign = '\0';
        char *key = var;
        char *value = equal_sign + 1;

        // Définir la variable d'environnement avec la clé et la valeur spécifiées
        if (setenv(key, value, 1) != 0) {
            perror("export");
        }
    }
}

