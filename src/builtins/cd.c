/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:58:56 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/21 16:15:16 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_cd_tild(char **const *env, char *home);

void	ft_cd_tiret(char **const *env, char *prev_dir);

void	ft_change_env_after_cd(char ***env, char *target_dir);

void	ft_move_to_directory(char ***env, char *home, char *prev_dir,
			char *target_dir);

void	ft_init_cd_var(char **const *env, char **home, char **prev_dir);

int		ft_gestion_cd_error2(char **args, char *home, char *prev_dir);

int		ft_gestion_cd_error1(char *home, char *prev_dir,
			const char *target_dir);

char	*ft_getenv(char *name, char **env)
{
	char	*value;
	int		i;
	int		j;

	i = 0;
	while (env[i] != NULL)
	{
		j = 0;
		while (env[i][j] != '=' && env[i][j] == name[j])
		{
			j++;
		}
		if (env[i][j] == '=')
		{
			value = ft_strdup(env[i] + j + 1);
			return (value);
		}
		i++;
	}
	return (NULL);
}

char	*ft_replace_env(char *name, char *value)
{
	char	*new_env;
	int		i;
	int		j;

	new_env = (char *)malloc((ft_strlen(name) + ft_strlen(value) + 2)
			* sizeof(char));
	i = 0;
	while (name[i] != '\0')
	{
		new_env[i] = name[i];
		i++;
	}
	new_env[i] = '=';
	i++;
	if (value == NULL)
	{
		new_env[i] = '\0';
		return (new_env);
	}
	j = 0;
	while (value[j] != '\0')
		new_env[i++] = value[j++];
	new_env[i] = '\0';
	return (new_env);
}

int	ft_setenv(char *name, char *value, char ***env)
{
	int		i;
	int		j;
	char	*new_env;

	i = 0;
	while ((*env)[i] != NULL)
	{
		j = 0;
		while ((*env)[i][j] != '=' && (*env)[i][j] == name[j])
		{
			j++;
		}
		if ((*env)[i][j] == '=')
		{
			new_env = ft_replace_env(name, value);
			free((*env)[i]);
			(*env)[i] = new_env;
			return (0);
		}
		i++;
	}
	return (-1);
}

int	ft_is_db_array(char *name, char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		if (ft_strcmp(name, array[i]) == 0)
		{
			return (1);
		}
		i++;
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
	if (ft_strlen(args[1]) > 1 && args[1][0] == '-' && args[1][1] == '-')
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

void	ft_init_cd_var(char **const *env, char **home, char **prev_dir)
{
	(*home) = ft_getenv("HOME", *env);
	(*prev_dir) = ft_getenv("OLDPWD", *env);
}

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
	else
	{
		if (chdir(target_dir) != 0)
		{
			printf("\033[1;31m CD FINAL ERROR TARGET_DIR = '%s'\n\033[0m",
				target_dir);
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

void	ft_cd_tiret(char **const *env, char *prev_dir)
{
	(void)env;
	if (prev_dir != NULL)
	{
		if (chdir(prev_dir) != 0)
		{
			free(prev_dir);
			perror("chdir");
			g_code_exit = ERROR;
		}
	}
	else
	{
		free(prev_dir);
		ft_fprintf(2, "cd: prev dir not found\n");
		g_code_exit = ERROR;
	}
	printf("%s\n", prev_dir);
}

void	ft_cd_tild(char **const *env, char *home)
{
	(void)env;
	if (home != NULL && home[0] != '\0')
	{
		if (home != NULL)
		{
			if (chdir(home) != 0)
			{
				free(home);
				perror("chdir");
				g_code_exit = ERROR;
			}
		}
		else
		{
			ft_fprintf(2, "cd: home not found\n");
			g_code_exit = ERROR;
		}
	}
}
