/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 01:44:34 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/01 19:30:07 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// pour recuperer une partie d'une chaine de caracteres
// par exemple pour recuperer tout ce qui a apres le = dans la variable d'environnement PATH
char *ft_substr(char *s, unsigned int start, size_t len)
{
    size_t	i;
    size_t	j;
    char	*str;

    str = (char*)malloc(sizeof(*s) * (len + 1));
    if (!str)
        return (NULL);
    i = 0;
    j = 0;
    while (s[i])
    {
        if (i >= start && j < len)
        {
            str[j] = s[i];
            j++;
        }
        i++;
    }
    str[j] = 0;
    return (str);
}

char *ft_get_path_cmd(char **args, char **env)
{
    char *path;
    char **path_tab;
    char *path_cmd;
    int i;

    path = NULL;
    i = 0;
    while (env[i] != NULL) {
        if (ft_strncmp(env[i], "PATH=", 5) == 0) {
            path = ft_substr(env[i], 5, ft_strlen(env[i]));
            break ;
        }
        i++;
    }
    path_tab = ft_split(path, ':');
    i = 0;
    while (path_tab[i] != NULL) {
        path_cmd = ft_strjoin(ft_strdup(path_tab[i]), "/");
        path_cmd = ft_strjoin(path_cmd, args[0]);
        if (access(path_cmd, F_OK) == 0)
            break ;
        else
            path_cmd = NULL;
        i++;
    }
    free_db_array(path_tab);
    return (path_cmd);
}

void ft_exec_cmd(char **args, char ***env)
{
    pid_t pid;
    int status;
    char *path_cmd;

    path_cmd = ft_get_path_cmd(args, *env);
    if (path_cmd == NULL) {
        printf("minishell: %s: command not found\n", args[0]);
        return ;
    }
    printf("path_cmd: %s\n", path_cmd);
    pid = fork();
    if (pid == 0) {
        if (execve(path_cmd, args, *env) == -1)
            perror("execve");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork");
    } else {
        waitpid(pid, &status, 0);
    }
}

char *ft_strrchr(const char *s, int c)
{
    int	i;

    i = 0;
    while (s[i])
        i++;
    while (i >= 0)
    {
        if (s[i] == (char)c)
            return ((char*)(s + i));
        i--;
    }
    return (NULL);
}

void ft_exec_bultins(char **args, char ***env, t_global_parsing **g_pars, t_global_exec **g_exec)
{
    char *command;
    if (args[0] == NULL)
        return;
    if(access(args[0], F_OK) == 0)
    {
        command = ft_strrchr(args[0], 'bin/');
        command = ft_substr(command, 1, ft_strlen(command));
        printf("                     command: %s\n", command);
    }
    else
        command = args[0];
    if (ft_strcmp(command, "echo") == 0)
        builtin_echo(args);
    else if (ft_strcmp(command, "cd") == 0)
        builtin_cd(args, env, (*g_exec)->export);
    else if (ft_strcmp(command, "pwd") == 0)
        builtin_pwd(args);
    else if (ft_strcmp(command, "unset") == 0)
        builtin_unset(args, env, g_exec);
    else if (ft_strcmp(command, "env") == 0)
        builtin_env(args, *env);
    else if (ft_strcmp(command, "exit") == 0)
        builtin_exit(args, g_pars);
    else if (ft_strcmp(command, "export") == 0)
        builtin_export(args, env, g_exec);
//    else
//        ft_exec_cmd(args, env);
}




