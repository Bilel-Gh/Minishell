/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:58:56 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/17 18:25:38 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *ft_getenv(char *name, char **env)
{
    char *value;
    int i;
    int j;

    i = 0;
    while (env[i] != NULL) {
        j = 0;
        while (env[i][j] != '=' && env[i][j] == name[j]) {
            j++;
        }
        if (env[i][j] == '=') {
            value = ft_strdup(env[i] + j + 1);
            return (value);
        }
        i++;
    }
    return (NULL);
}

char *ft_replace_env(char *name, char *value)
{
    char *new_env;
    int i;
    int j;

    new_env = (char *)malloc((ft_strlen(name) + ft_strlen(value) + 2) * sizeof(char));
    i = 0;
    while (name[i] != '\0') {
        new_env[i] = name[i];
        i++;
    }
    new_env[i] = '=';
    i++;
    j = 0;
    while (value[j] != '\0') {
        new_env[i] = value[j];
        i++;
        j++;
    }
    new_env[i] = '\0';
    return (new_env);
}

int ft_setenv(char *name, char *value, char ***env)
{
    int i;
    int j;
    char *new_env;

    i = 0;
    while ((*env)[i] != NULL) {
        j = 0;
        while ((*env)[i][j] != '=' && (*env)[i][j] == name[j]) {
            j++;
        }
        if ((*env)[i][j] == '=') {
            new_env = ft_replace_env(name, value);
            free(env[i]);
            (*env)[i] = new_env;
            return (0);
        }
        i++;
    }
    return (-1);
}

void builtin_cd(char **args, char ***env)
{
    char *home;
    //char *my_cwd;
    char *prev_dir;
    char *current_dir;
    char *new_pwd;
    if (args[1] == NULL || ft_strcmp(args[1], "~") == 0) {
        home = ft_getenv("HOME", *env);
        if (home != NULL) {
            if (chdir(home) != 0) {
                perror("cd");
            }
        } else {
            printf("cd: Impossible de trouver le répertoire home de l'utilisateur\n");
        }
    } else if (ft_strcmp(args[1], "-") == 0) {
        prev_dir = ft_getenv("OLDPWD", *env);
        if (prev_dir != NULL) {
            if (chdir(prev_dir) != 0) {
                perror("cd");
            }
        } else {
            printf("cd: Impossible de trouver le répertoire précédent\n");
        }
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
    }

    current_dir = ft_getenv("PWD", *env);
    new_pwd = getcwd(NULL, 0);
    if (new_pwd != NULL) {
        ft_setenv("OLDPWD", current_dir, env);
        ft_setenv("PWD", new_pwd, env);
//        while (*env != NULL) {
//            printf("\033[0;32m%s\033[0m\n", *env);
//            env++;
//        }
        free(new_pwd);
    } else {
        perror("getcwd");
    }
}
