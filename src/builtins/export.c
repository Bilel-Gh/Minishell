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

int ft_db_tablen(char **tab)
{
    int i;

    i = 0;
    while (tab[i] != NULL)
        i++;
    return (i);
}

char *get_name(char *arg)
{
    int i;
    int j;
    char *name;

    i = 0;
    j = 0;
    while (arg[i] != '=')
        i++;
    name = malloc(sizeof(char) * i);
    while (j < i)
    {
        name[j] = arg[j];
        j++;
    }
    name[j] = 0;
    return (name);
}

char *get_value(char *arg)
{
    int i;
    int j;
    char *value;

    i = 0;
    j = 0;
    while (arg[i] != '=')
        i++;
    i++;
    value = malloc(sizeof(char) * (ft_strlen(arg) - i));
    while (arg[i] != 0)
    {
        value[j] = arg[i];
        i++;
        j++;
    }
    value[j] = 0;
    return (value);
}

char *ft_get_env_to_add(char *name, char *value)
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

void ft_add_env(char *name, char *value, char ***env)
{
    int i;
    char **new_env;

    i = 0;
    new_env = malloc(sizeof(char *) * (ft_db_tablen(*env) + 2));
    while ((*env)[i] != NULL)
    {
        new_env[i] = ft_strdup((*env)[i]);
        i++;
    }
    new_env[i] = ft_get_env_to_add(name, value);
    printf("new_env[i]: %s\n", new_env[i]);
    new_env[i + 1] = NULL;
    *env = new_env;
}

void ft_sort_env(char ***env) // a voir comment trier car sur mac je n'ai pas les variable de env trier
{
    int i;
    int j;
    char *tmp;

    i = 0;
    while ((*env)[i] != NULL)
    {
        j = i + 1;
        while ((*env)[j] != NULL)
        {
            if (ft_strcmp((*env)[i], (*env)[j]) > 0)
            {
                tmp = (*env)[i];
                (*env)[i] = (*env)[j];
                (*env)[j] = tmp;
            }
            j++;
        }
        i++;
    }
}

void builtin_export(char **args, char ***env)
{
    (void)env;
    char *name;
    char *value;
    (void )value;

    name = get_name(args[1]);
    printf("name: %s\n", name);
    value = get_value(args[1]);
    printf("value: %s\n", value);

    ft_add_env(name, value, env);
    ft_sort_env(env);
    // printf env
    int i;
    i = 0;
    while ((*env)[i] != NULL)
    {
        printf("%s\n", (*env)[i]);
        i++;
    }
}
