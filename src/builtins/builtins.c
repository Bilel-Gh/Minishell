/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 01:44:34 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/14 17:43:03 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_exec_bultins(char **args, char **env)
{
    (void)env;
    if (ft_strcmp(args[0], "echo") == 0)
        builtin_echo(args);
    else if (ft_strcmp(args[0], "cd") == 0)
        builtin_cd(args);
    else if (ft_strcmp(args[0], "pwd") == 0)
        builtin_pwd();
    else if (ft_strcmp(args[0], "unset") == 0)
        builtin_unset(args);
    else if (ft_strcmp(args[0], "env") == 0)
        builtin_env();
    else if (ft_strcmp(args[0], "exit") == 0)
        builtin_exit();
}

int ft_is_option_echo(char *str)
{
    if (str[0] == '-')
    {
        if (str[1] == 'n')
        {
            str++;
            while (*str == 'n')
                str++;
            if (*str == '\0')
                return (1);
            else
                return (0);
        }
        else
            return (0);
    }
    else
        return (0);
}

void ft_get_full_arg(char **args, char **full_arg, int i)
{
    int j;
    j = i;
    if (args[j] == NULL)
        return;
    *full_arg = ft_strdup(args[j]);
    j++;
    while (args[j] != NULL)
    {
        *full_arg = ft_join_cmd(*full_arg, args[j]);
        j++;
    }
}

void builtin_echo(char **args)
{
    int i;
    int len;
    int print_newline;
    char *full_arg;

    i = 1;
    len = 0;
    print_newline = 1; // Variable pour gérer l'option -n
    full_arg = NULL;
    // Vérifier si l'option -n est spécifiée
    if (args[1] != NULL && ft_is_option_echo(args[1]))
    {
        print_newline = 0;
        i = 2;
    }
    ft_get_full_arg(args, &full_arg, i);
    if (full_arg != NULL)
    {
        len = strlen(full_arg);
        write(1, full_arg, len);
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




