/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif_cmd2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:26:45 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/10 21:10:09 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_token	*ft_save_next_tokens(t_token *current)
{
	t_token	*save_next_token;

	save_next_token = NULL;
	if (current->next != NULL)
	{
		save_next_token = current->next;
		current->next = NULL;
	}
	return (save_next_token);
}

void	ft_free_value_splited(char **value_splited)
{
	int	i;

	i = 0;
	while (value_splited[i] != NULL)
	{
		free(value_splited[i]);
		i++;
	}
	free(value_splited);
}

void	ft_free_paths(char **path_splited, char *pathCopy)
{
	free(pathCopy);
	free_db_array(path_splited);
}

void	ft_free_paths2(char **path_splited, char *pathCopy, char *fullPath)
{
	ft_free_paths(path_splited, pathCopy);
	free(fullPath);
}
