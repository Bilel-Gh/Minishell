/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outfile.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:19:41 by ncharii           #+#    #+#             */
/*   Updated: 2023/08/29 14:23:07 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_new_outfile(t_exec *exec, t_token *token)
{
	if (exec->outfile)
	{
		close(exec->fd_outfile);
		free(exec->outfile);
	}
	if (token->next)
		exec->outfile = ft_strdup(token->next->value);
	if (token->info->type == REDIRECT_OUT)
	{
		exec->fd_outfile = open(exec->outfile,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (exec->fd_outfile == -1)
			perror(exec->outfile);
	}
	else if (token->info->type == REDIRECT_D_OUT)
	{
		exec->fd_outfile = open(exec->outfile,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (exec->fd_outfile == -1)
			perror(exec->outfile);
	}
}

bool	is_token_redi_out(t_token *token)
{
	if (token->info->type == REDIRECT_OUT
		|| token->info->type == REDIRECT_D_OUT)
		return (true);
	return (false);
}

void	gestion_outfile(t_token *tokens, t_exec *exec)
{
	t_token	*seach_tok_out;

	seach_tok_out = tokens;
	while (seach_tok_out)
	{
		if (exec->fd_outfile == -1)
			break ;
		if (is_token_redi_out(seach_tok_out))
			set_new_outfile(exec, seach_tok_out);
		seach_tok_out = seach_tok_out->next;
	}
}

void	destruction_infile(t_exec *exec)
{
	close(exec->fd_infile);
	free(exec->infile);
	exec->infile = NULL;
}

void	dectruction_heredoc(t_exec *exec)
{
	close(exec->fd_infile);
	free(exec->limiteur);
	unlink("/tmp/here_doc_minishell");
	exec->limiteur = NULL;
}
