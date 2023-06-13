/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infos_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 04:18:57 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/10 23:06:10 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	get_paths(char *path, char ***path_splited, char **path_copy)
{
	(*path_copy) = NULL;
	path = getenv("PATH");
	if (path)
		(*path_copy) = ft_strdup(path);
	else
		path = "/";
	(*path_splited) = ft_split((*path_copy), ':');
}

char	*ft_get_full_path(t_token *token, char *const *path_splited,
		char *full_path, int i)
{
	full_path = NULL;
	full_path = ft_strjoin(full_path, path_splited[i]);
	full_path = ft_strjoin(full_path, "/");
	full_path = ft_strjoin(full_path, token->value);
	return (full_path);
}

// version a completer demain
int	ft_is_command(t_token *token)
{
	char	**path_splited;
	char	*path;
	char	*path_copy;
	char	*full_path;
	int		i;

	if (token->token_index == 0 || (token->prev
			&& token->prev->info->type == T_PIPE))
		return (1);
	i = 0;
	path = NULL;
	get_paths(path, &path_splited, &path_copy);
	while (path_splited[i])
	{
		full_path = ft_get_full_path(token, path_splited, full_path, i);
		if (access(full_path, 0) == 0)
		{
			ft_free_paths2(path_splited, path_copy, full_path);
			return (1);
		}
		free(full_path);
		i++;
	}
	ft_free_paths(path_splited, path_copy);
	return (0);
}

int	ft_is_infile(t_token *token)
{
	if (token->prev == NULL)
		return (0);
	if (token->prev->info->type == REDIRECT_IN
		|| token->prev->info->type == REDIRECT_D_IN)
		return (1);
	return (0);
}

int	ft_is_outfile(t_token *token)
{
	if (token->prev == NULL)
		return (0);
	if (token->prev->info->type == REDIRECT_OUT)
		return (1);
	return (0);
}
