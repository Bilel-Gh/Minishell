/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:58:56 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/29 13:52:57 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	j = 0;
	while (value[j] != '\0')
	{
		new_env[i] = value[j];
		i++;
		j++;
	}
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
			free(env[i]);
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

void	builtin_cd(char **args, char ***env, char **export)
{
	char	*home;
	char	*in_env;
	int		in_export;
	char	*prev_dir;
	char	*current_dir;
	char	*new_pwd;
	char	*export_value;
    char *target_dir;

    target_dir = args[1];

	if (ft_db_tablen(args) > 2)
	{
		printf("cd: Trop d'arguments\n");
		return ;
	}
	if (target_dir == NULL || ft_strcmp(target_dir, "~") == 0)
	{
		home = ft_getenv("HOME", *env);
		if (home != NULL)
		{
			if (chdir(home) != 0)
			{
				perror("chdir");
			}
		}
		else
		{
			printf("cd: Impossible de trouver le répertoire home de l'utilisateur\n");
		}
	}
	else if (ft_strcmp(target_dir, "-") == 0)
	{
		prev_dir = ft_getenv("OLDPWD", *env);
		if (prev_dir != NULL)
		{
			if (chdir(prev_dir) != 0)
			{
				perror("chdir");
			}
		}
		else
		{
			printf("cd: Impossible de trouver le répertoire précédent\n");
		}
	}
	else
	{
        if (chdir(target_dir) != 0)
        {
            in_env = ft_getenv(target_dir, *env);
            export_value = ft_strjoin(ft_strdup("export "), target_dir);
            in_export = ft_is_db_array(export_value, export);
            if (in_env == NULL && in_export == 1)
                printf("cd: %s not set\n", target_dir);
            else
                perror("cd");
        }
	}
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
            printf("chdir: error retrieving current directory");
            printf("getcwd: cannot access parent directories\n");
        }
        else
		    perror("getcwd");
	}
}
