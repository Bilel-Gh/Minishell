/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:58:56 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 19:56:09 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_move_to_directory(char ***env, char *home, char *prev_dir,
		char *target_dir)
{
	if (ft_strcmp(target_dir, "~") == 0)
	{
		ft_cd_tild(env, home);
	}
	else if (ft_strcmp(target_dir, "-") == 0)
	{
		ft_cd_tiret(env, prev_dir);
	}
	else if (ft_strcmp(target_dir, "--") == 0)
		return ;
	else
	{
		if (chdir(target_dir) != 0)
		{
			perror("cd");
			g_code_exit = ERROR;
		}
	}
}

void	ft_change_env_after_cd(char ***env, char *target_dir)
{
	char	*current_dir;
	char	*new_pwd;

	current_dir = ft_getenv("PWD", *env);
	new_pwd = getcwd(NULL, 0);
	if (new_pwd != NULL)
	{
		ft_setenv("OLDPWD", current_dir, env);
		ft_setenv("PWD", new_pwd, env);
		free(new_pwd);
		free(current_dir);
	}
	else
	{
		free(current_dir);
		if (ft_strncmp(target_dir, "..", 2) == 0)
		{
			ft_fprintf(2, "chdir: error retrieving current directory");
			ft_fprintf(2, "getcwd: cannot access parent directories\n");
		}
		else
			perror("getcwd");
		g_code_exit = ERROR;
	}
}

int	ft_gestion_cd_error1(char *home, char *prev_dir, const char *target_dir)
{
	if (target_dir == NULL)
	{
		ft_fprintf(2, "bash: cd: HOME not set\n");
		g_code_exit = ERROR;
		if (home)
			free(home);
		if (prev_dir)
			free(prev_dir);
		return (ERROR);
	}
	return (0);
}

int	ft_gestion_cd_error2(char **args, char *home, char *prev_dir)
{
	if (ft_db_tablen(args) > 2)
	{
		ft_fprintf(2, "bash: cd: too many arguments\n");
		g_code_exit = ERROR;
		if (home)
			free(home);
		if (prev_dir)
			free(prev_dir);
		return (ERROR);
	}
	if (ft_strlen(args[1]) > 2 && args[1][0] == '-'
		&& !error_no_only_type(args[1]))
	{
		ft_fprintf(2, "bash: cd: invalid option\n");
		g_code_exit = MISUSE;
		if (home)
			free(home);
		if (prev_dir)
			free(prev_dir);
		return (ERROR);
	}
	return (0);
}

int	builtin_cd(char **args, char ***env)
{
	char	*home;
	char	*prev_dir;
	char	*target_dir;

	target_dir = args[1];
	ft_init_cd_var(env, &home, &prev_dir);
	if (target_dir == NULL || target_dir[0] == '\0')
	{
		target_dir = ft_getenv("HOME", *env);
		if (ft_gestion_cd_error1(home, prev_dir, target_dir))
			return (g_code_exit);
	}
	if (ft_gestion_cd_error2(args, home, prev_dir))
		return (g_code_exit);
	ft_move_to_directory(env, home, prev_dir, target_dir);
	ft_change_env_after_cd(env, target_dir);
	if (home)
		free(home);
	if (prev_dir)
		free(prev_dir);
	return (g_code_exit);
}
