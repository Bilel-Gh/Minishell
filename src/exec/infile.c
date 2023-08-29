/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:19:40 by ncharii           #+#    #+#             */
/*   Updated: 2023/08/29 14:21:55 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_path(t_exec *exec, char **envp)
{
	int		i;
	char	*skip_path;

	i = 0;
	if (envp == NULL)
	{
		exec->path = 0;
	}
	while (envp[i])
	{
		if (!ft_strncmp("PATH", envp[i], 4))
			break ;
		i++;
	}
	if (envp[0] == NULL || envp[i] == NULL)
		exec->path = 0;
	else
	{
		skip_path = &envp[i][5];
		exec->path = ft_split(skip_path, ':');
	}
	return (1);
}

int	set_new_infile(t_exec *exec, t_token *tokens)
{
	t_token	*token;

	token = tokens;
	if (token->info->type == REDIRECT_IN)
		new_infile(exec, token->next);
	else
	{
		if (new_heredoc(exec, token->next) == -1)
			return (-1);
	}
	return (0);
}

int	gestion_infile(t_token *tokens, t_exec *exec)
{
	t_token	*seach_tok_in;
	int		info;

	info = 0;
	seach_tok_in = tokens;
	while (seach_tok_in)
	{
		if (exec->fd_infile == -1)
			break ;
		if (is_token_redi_in(seach_tok_in))
			info = set_new_infile(exec, seach_tok_in);
		if (info == -1)
			return (-1);
		seach_tok_in = seach_tok_in->next;
	}
	return (0);
}

void	new_infile(t_exec *exec, t_token *token)
{
	if (exec->infile)
	{
		close(exec->fd_infile);
		free(exec->infile);
		exec->infile = NULL;
	}
	if (exec->limiteur)
	{
		close(exec->fd_infile);
		free(exec->limiteur);
		unlink("/tmp/here_doc_minishell");
		exec->limiteur = NULL;
	}
	exec->infile = ft_strdup(token->value);
	if (!exec->infile)
		return ;
	exec->fd_infile = open(exec->infile, O_RDONLY);
	if (exec->fd_infile == -1)
		perror(exec->infile);
}

bool	is_token_redi_in(t_token *token)
{
	if (token->info->type == REDIRECT_IN || token->info->type == REDIRECT_D_IN)
		return (true);
	return (false);
}
