/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 01:44:34 by bghandri          #+#    #+#             */
/*   Updated: 2023/05/25 03:24:14 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

void builtin_echo(char **args)
{
    int i = 1;
    int len = 0;
    int print_newline = 1; // Variable pour gérer l'option -n

    // Vérifier si l'option -n est spécifiée
    if (args[1] != NULL && strcmp(args[1], "-n") == 0)
    {
        print_newline = 0;
        i = 2;
    }

    // Parcourir les arguments et les afficher
    while (args[i] != NULL)
    {
        len = strlen(args[i]);
        fwrite(args[i], sizeof(char), len, stdout);
        i++;
        if (args[i] != NULL)
            putchar(' '); // Ajouter un espace entre les arguments
    }

    if (print_newline)
        putchar('\n');
}

void builtin_cd(char **args)
{
    if (args[1] == NULL) {
        // Pas d'argument, revenir au répertoire home de l'utilisateur
        const char *home = getenv("HOME");
        if (home != NULL) {
            if (chdir(home) != 0) {
                perror("cd");
            }
        } else {
            fprintf(stderr, "cd: Impossible de trouver le répertoire home de l'utilisateur\n");
        }
    } else {
        // Changer de répertoire en fonction de l'argument spécifié
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
    }
}

void builtin_pwd()
{
    char cwd[4096]; // Taille maximale du chemin d'accès = 4096 octets

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}
extern char **environ;

void builtin_unset(char **args)
{
    char *var = args[1];

    if (var == NULL) {
        // Pas d'argument spécifié, afficher un message d'erreur
        printf("unset: nom de variable manquant\n");
        return;
    }

    // Supprimer la variable d'environnement spécifiée
    if (unsetenv(var) != 0) {
        perror("unset");
    }
}

void builtin_env()
{
    char **env = environ;
    while (*env != NULL) {
        printf("%s\n", *env);
        env++;
    }
}
void builtin_exit()
{
    exit(0);
}




