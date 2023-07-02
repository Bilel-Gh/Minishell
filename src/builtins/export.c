/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 02:56:40 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/26 13:30:19 by bghandri         ###   ########.fr       */
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
    int in_quote;
    char type_quote;
    int len_name;

    i = 0;
    j = 0;
    in_quote = 0;
    type_quote = 0;
    len_name = 0;
    while (arg[i] != '=' && arg[i] != '\0')
    {
        if (arg[i] == '"' || arg[i] == '\'')
        {
            in_quote = !in_quote;
            type_quote = arg[i];
            i++;
            while (arg[i] != '\0' && arg[i] != type_quote)
                i++;
            if (arg[i] == type_quote)
                in_quote = 0;
            i++;
        }
        else
        {
            len_name = i - len_name + 1;
            i++;
        }
    }
    if (i == 0)
        return (NULL);
    name = malloc(sizeof(char) * (i + 1));
    if (name == NULL)
        return (NULL);
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
    while (arg[i] != '=' && arg[i] != 0)
        i++;
    if (arg[i] == 0)
        return (NULL);
    i++;
    value = malloc(sizeof(char) * ((ft_strlen(arg) - i) + 1));
    if (value == NULL)
        return (NULL);
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
    if (value == NULL)
    {
        new_env[i] = '\0';
        return (new_env);
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

char **ft_add_to_db_tab_export(char **tab, char *str, char *name)
{
    int i;
    char **new_tab;
    int db_tablen;
    int len_name;

    i = 0;
    if (tab == NULL)
    {
        new_tab = malloc(sizeof(char *) * 2);
        new_tab[0] = ft_strdup(str);
        new_tab[1] = NULL;
        return (new_tab);
    }
    db_tablen = ft_db_tablen(tab);
    len_name = ft_strlen(name);
    // stocker la valeur de tab[i] dans une variable sans les 6 premiers char
//    char *test = ft_strdup(tab[i] + 7);
//    printf("\033[0;31m name: %s\n \033[0m", name);
//    printf("\033[0;31m tab[i] sans export: %s\n \033[0m", test);
    while (tab[i] != NULL && ft_strncmp(ft_strdup(tab[i] + 7), name, len_name) != 0)
        i++;
    if (i < db_tablen)
    {
        free(tab[i]);
        tab[i] = ft_strdup(str);
        return (tab);
    }
    else
    {
        new_tab = malloc(sizeof(char *) * (db_tablen + 2));
        i = 0;
        while (i < db_tablen)
        {
            new_tab[i] = ft_strdup(tab[i]);
            free(tab[i]);
            i++;
        }
        new_tab[i] = ft_strdup(str);
        new_tab[i + 1] = NULL;
        free(tab);
        return (new_tab);
    }
}

char **ft_add_to_db_tab(char **tab, char *str, char *name)
{
    int i;
    char **new_tab;
    int db_tablen;
    int len_name;

    i = 0;
    if (tab == NULL)
    {
        new_tab = malloc(sizeof(char *) * 2);
        new_tab[0] = ft_strdup(str);
        new_tab[1] = NULL;
        return (new_tab);
    }
    db_tablen = ft_db_tablen(tab);
    len_name = ft_strlen(name);
    // stocker la valeur de tab[i] dans une variable sans les 6 premiers char
//    char *test = ft_strdup(tab[i] + 7);
//    printf("\033[0;31m name: %s\n \033[0m", name);
//    printf("\033[0;31m tab[i] sans export: %s\n \033[0m", test);
    while (tab[i] != NULL && ft_strncmp(ft_strdup(tab[i]), name, len_name) != 0)
        i++;
    if (i < db_tablen)
    {
        free(tab[i]);
        tab[i] = ft_strdup(str);
        return (tab);
    }
    else
    {
        new_tab = malloc(sizeof(char *) * (db_tablen + 2));
        i = 0;
        while (i < db_tablen)
        {
            new_tab[i] = ft_strdup(tab[i]);
            free(tab[i]);
            i++;
        }
        new_tab[i] = ft_strdup(str);
        new_tab[i + 1] = NULL;
        free(tab);
        return (new_tab);
    }
}

void ft_add_to_export(t_global_exec **g_exec, char *new_env, char *name)
{
    char *env_to_add_to_export;
    char *pre_export;
    env_to_add_to_export = ft_strdup(new_env);
    pre_export = ft_strdup("export ");
    (*g_exec)->export = ft_add_to_db_tab_export((*g_exec)->export, ft_strjoin(pre_export, env_to_add_to_export), name);
}

char *ft_clean_value(char *value)
{
    int i;
    char *new_value;
    int len;
    int new_len;
    char first_char;
    char last_char;

    len = ft_strlen(value);
    i = 0;
    first_char = value[0];
    last_char = value[len - 1];
    if ((first_char == '"' && last_char == '"') || (first_char == '\'' && last_char == '\''))
    {
        new_len = len - 2;
        new_value = malloc(sizeof(char) * (new_len + 1));
        if (new_value == NULL)
            return (NULL);
        while (value[i + 1] != '\0')
        {
            new_value[i] = value[i + 1];
            i++;
        }
        new_value[i - 1] = '\0';
        return (new_value);
    }
    return (value);
}

void ft_add_env(char *name, char *value, char ***env, t_global_exec **g_exec)
{
//    int i;
//    int j;
//    char **new_env;
    char *env_to_add;
    char *env_to_add_export;

//    i = 0;
//    j = 0;
//    new_env = NULL;
//    while ((*env)[j] != NULL && ft_strcmp((*env)[j], name) != 0)
//        j++;
//    printf("\033[0;31m j EXIST : %d\n \033[0m", j);
    env_to_add_export = ft_get_env_to_add(name, value);
    if (value == NULL)
    {
        ft_add_to_export(g_exec, env_to_add_export, name);
        return ;
    }
    value = ft_clean_value(value);
    while (value != NULL && (value[0] == '"' || value[0] == '\''))
        value = ft_clean_value(value);
    env_to_add = ft_get_env_to_add(name, value);
    (*env) = ft_add_to_db_tab((*env), env_to_add, name);
    ft_add_to_export(g_exec, env_to_add_export, name);
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

int ft_is_alpha(char c)
{
    if (c >= 'a' && c <= 'z')
        return (1);
    if (c >= 'A' && c <= 'Z')
        return (1);
    return (0);
}

int ft_is_alnum(char c)
{
    if (ft_is_alpha(c) == 1)
        return (1);
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

bool ft_check_name(char *name)
{
    int i ;
    // Vérification de la première lettre
    if (!ft_is_alpha(name[0]) && name[0] != '_') {
        return false;
    }
    i = 1;
    // Vérification des caractères suivants
    while (name[i] != '\0') {
        if (!ft_is_alnum(name[i]) && name[i] != '_') {
            return false;
        }
        i++;
    }
    return true;
}

int ft_check_equal_error(char **args, int nb_args)
{
    int i;
    i = 0;
    if (nb_args > 2)
    {
        while (args[i] != NULL)
        {
            if (args[i][0] == '=')
            {
                printf("minishell: export: `%s': not a valid identifier\n", args[i]);
                return (1);
            }
            i++;
        }
    }
    return (0);
}

int ft_check_no_args(char **args, t_global_exec **g_exec)
{
    int i;
    if (args[1] == NULL)
    {
        i = 0;
        if ((*g_exec)->export == NULL)
            return 1;
        while ((*g_exec)->export[i] != NULL)
        {
            printf("%s\n", (*g_exec)->export[i]);
            i++;
        }
        return 1;
    }
    return 0;
}

int ft_check_solo_invalid_arg(char *args)
{
    char *invalid_chars;
    invalid_chars = "=+%?-@!*#$&(){}[]^~|\\<>;,./123456789";
    if(args[0] == '\0' || ft_strchr(invalid_chars, args[0]) != NULL)
    {
        if (args[0] == '-' && args[1] != '\0')
        {
            g_code_exit = MISUSE;
            printf("minishell: export: `%c%c': invalid option\n", args[0], args[1]);
            return 1;
        }
        printf("minishell: `%s': not a valid identifier\n", args);
        g_code_exit = ERROR;
        return 1;
    }
    return 0;
}

int ft_check_export_err(char **args, t_global_exec **g_exec, int nb_args) {
    if (ft_check_equal_error(args, nb_args))
        return 1;
    if (ft_check_no_args(args, g_exec))
        return 1;
    return 0;
}

void builtin_export(char **args, char ***env, t_global_exec **g_exec)
{
    char *name;
    char *value;
    int nb_args;
    int i;

    i = 1;
    nb_args = ft_db_tablen(args);
    value = NULL;
    if (ft_check_export_err(args, g_exec, nb_args))
        return ;
    while (args[i] != NULL)
    {
        if (ft_check_solo_invalid_arg(args[i]))
            return ;
        name = get_name(args[i]);
        printf("name = %s\n", name);
        name = ft_clean_value(name);
        printf("name2 = %s\n", name);
        if (!ft_check_name(name))
        {
            g_code_exit = ERROR;
            printf("minishell: export: `%s': not a valid identifier\n", args[i]);
            i++;
            continue;
        }
        value = get_value(args[i]);
        ft_add_env(name, value, env, g_exec);
        ft_sort_env(env);
        ft_sort_env(&(*g_exec)->export);
        i++;
    }
    if (name != NULL)
        free(name);
    if (value != NULL)
        free(value);
}
