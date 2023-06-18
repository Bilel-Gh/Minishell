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

void ft_exec_bultins(char **args, char ***env, t_global_parsing **g_pars)
{
    if (args[0] == NULL)
        return;
    (void)env;
    if (ft_strcmp(args[0], "echo") == 0)
        builtin_echo(args);
    else if (ft_strcmp(args[0], "cd") == 0)
        builtin_cd(args, env);
    else if (ft_strcmp(args[0], "pwd") == 0)
        builtin_pwd();
    else if (ft_strcmp(args[0], "unset") == 0)
        builtin_unset(args, env);
    else if (ft_strcmp(args[0], "env") == 0)
        builtin_env(args, *env);
    else if (ft_strcmp(args[0], "exit") == 0)
        builtin_exit(args, g_pars);
    else if (ft_strcmp(args[0], "export") == 0)
        builtin_export(args, env);
    else
        execve(args[0], args, *env);
}




