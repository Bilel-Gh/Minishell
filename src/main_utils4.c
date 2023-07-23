/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 00:23:37 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/24 00:24:00 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_g_parsing(t_global_parsing *g_parsing)
{
	if (g_parsing->line)
		free(g_parsing->line);
	if (g_parsing->args)
		free_db_array(g_parsing->args);
	if (g_parsing->info_args)
		free(g_parsing->info_args);
	if (g_parsing->tokens)
		free_list_tokens(g_parsing->tokens);
	if (g_parsing->commande)
		free_list_commande(g_parsing->commande);
}

void	ft_set_index_for_exec(t_token **tokens)
{
	t_token	*head;
	int		i;

	i = 0;
	head = *tokens;
	while (*tokens)
	{
		while ((*tokens)->info->type != 2)
		{
			(*tokens)->token_index = i;
			*tokens = (*tokens)->next;
			if (!*tokens)
				break ;
		}
		if (!*tokens)
			break ;
		(*tokens)->token_index = i;
		i++;
		*tokens = (*tokens)->next;
	}
	*tokens = head;
}

int	only_misuse(char *str, char *invalid_char)
{
	int	i;
	int	j;

	i = 0;
	if (str == NULL)
		return (1);
	while (str[i] != '\0')
	{
		j = 0;
		while (invalid_char[j] != '\0')
		{
			if (str[i] == invalid_char[j])
				break ;
			j++;
		}
		if (invalid_char[j] == '\0')
			return (0);
		i++;
	}
	return (1);
}

int	ft_check_full_cmd(char *cmd)
{
	char	*invalid_char;

	invalid_char = ";(){}<>|&.";
	if (only_misuse(cmd, invalid_char))
	{
		ft_fprintf(2, "syntax error near unexpected token `%c%c'\n", cmd[0],
			cmd[1]);
		g_code_exit = MISUSE;
		return (1);
	}
	else if (ft_strcmp(cmd, "!") == 0)
	{
		g_code_exit = ERROR;
		return (1);
	}
	return (0);
}

void	ft_check_not_found(char **cmd)
{
	struct stat	path_stat;

	if (lstat(cmd[0], &path_stat) == 0)
		ft_get_exit_code_lstat(cmd, &path_stat);
	else if (errno == ENOENT)
	{
		ft_fprintf(2, "bash: %s: command not found\n", cmd[0]);
		g_code_exit = NOTFOUND;
	}
	else
	{
		ft_fprintf(2, "bash: %s: error: %s\n", cmd[0], strerror(errno));
		g_code_exit = ERROR;
	}
}
