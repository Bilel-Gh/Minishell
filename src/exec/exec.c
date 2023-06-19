/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:21:17 by ncharii           #+#    #+#             */
/*   Updated: 2023/06/19 17:56:06 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int nb_token_whis_index(t_token *tokens, int index)
{
	int i;
	t_token *seach;

	i = 0;
	seach = tokens;
	while (seach)
	{
		if (seach->token_index == index)
			i++;
		seach = seach->next;
	}
	return (i);
}

void	creat_info_token_list(t_token *tokens, int nb_node)
{
	int			i;

	t_token	*tmp;
	t_token	*for_creat;

	i = 1;
	while (i < nb_node)
	{
		tmp = malloc(sizeof(t_token));
		if (!tmp)
		{
			free_list_tokens(tokens);
			return ;
		}
		tmp->next = 0;
		for_creat->next = tmp;
		for_creat = for_creat->next;
		i++;
	}
}

void	copy_cont_token(t_token *dest, t_token *src)
{
	dest->token_index = src->token_index;
	dest->value = ft_strdup(src->value);
	dest->info->type = src->info->type;
}

t_token	*dup_info(t_token *info_token, t_token *tokens, int index)
{
	t_token *tokens_value_index;
	t_token *all;

	all = tokens;
	tokens_value_index = info_token;
	while (all)
	{
		if (all->token_index == index)
		{
			copy_cont_token(tokens_value_index, all);
			tokens_value_index = tokens_value_index->next;
		}
		all = all->next;
	}
	return (info_token);
}

t_token *get_info_token(t_token *tokens, int index)
{
	t_token *info_token;
	int nb_token;

	nb_token = nb_token_whis_index(tokens, index);
	info_token = NULL;
	if (nb_token == 0)
		return (info_token);
	info_token = malloc(sizeof(t_token));
	if (!info_token)
		return (NULL);
	creat_info_token_list(info_token, nb_token);
	if (!info_token)
		return (NULL);
	info_token = dup_info(info_token, tokens, index);
}



//########################################################################
typedef struct		exec
{
	char *infile;
	char **outfile;
	int	fd_infile;
	int	fd_outfile;
	int	fd_in_last_pipe;
	int heredoc;

	char	**path;
}					t_exec;

void	new_infile(t_exec *exec, t_token *token)
{
	if (exec->infile)
		close(fd_infile);
	

}

void	new_heredoc(t_exec *exec, t_token *token)
{
	if (exec->infile)
		close(fd_infile);
}

void	set_new_infile(t_exec *exec, t_token *tokens)
{
	t_token *token;

	tokens = token;
	if (token->info->type == INFILE)
		new_infile(exec, token->next);
	else
		new_heredoc(exec, token->next);
}

void	gestion_infile(t_token *tokens, t_exec *exec)
{
	t_token *seach_tok_in;

	seach_tok_in = tokens;
	while (seach_tok_in)
	{
		if (is_token_redi_in)
			set_new_infile(exec, seach_tok_in)

	}
}

void	set_exec_and_exec(t_token *tokens, char **cmd, t_exec *exec)
{
	gestion_infile;
	gestion_outfile;
	start_exec;


}
void	exec(t_token *tokens, t_commande *commande, char **env)
{
	t_token *info_token;
	t_exec exec;
	int i;

	i = 0;
//get_path;
	while (commande)
	{
		info_token = get_info_token(tokens, i);
		if (!info_token)
			return ;
		i++;
		//set_exec_and_(info_token, commande->cmd, exec)
		commande = commande->next;
		free_list_tokens(info_token);
	}
}
