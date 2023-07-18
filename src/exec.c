/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:21:17 by ncharii           #+#    #+#             */
/*   Updated: 2023/07/18 02:34:56 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

typedef struct		exec
{
	char *infile;
	char *outfile;
	int	fd_infile;
	int	fd_outfile;
	int	fd_in_last_pipe;
	int heredoc;

	char	**path;
}					t_exec;

int nb_token_whis_index(t_token *tokens, int index)
{
	int i;
	t_token *seach;
    //test
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

t_token	*creat_info_token_list(t_token *tokens, int nb_node)
{
	int			i;

	t_token	*tmp;
	t_token	*for_creat;
	t_token *head;

	for_creat = tokens;
	head = tokens;

	i = 1;
	while (i < nb_node)
	{
		tmp = malloc(sizeof(t_token));
		if (!tmp)
		{
			free_list_tokens(tokens);
			return (NULL);
		}
		tmp->next = 0;
		for_creat->next = tmp;
		for_creat = for_creat->next;
		i++;
	}
	return (head);
}

void	copy_cont_token(t_token *dest, t_token *src)
{
	dest->token_index = src->token_index;
	dest->value = ft_strdup(src->value);
	dest->info = (t_token_info *)malloc(sizeof(t_token_info));
	dest->info->type = src->info->type;
	// TODO gestion d errerur a faire et il faudrait meme reflechir  aune autre facon de faire
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
	info_token = creat_info_token_list(info_token, nb_token);
	if (!info_token)
		return (NULL);
	info_token = dup_info(info_token, tokens, index);
	return (info_token);
}



//##############################INFILE##########################################
//bool	is_token_redi_in(t_token *token)
//{
//	if (token->info->type == REDIRECT_IN || token->info->type == REDIRECT_D_IN)
//		return (true);
//	return (false);
//
//}
//void	new_infile(t_exec *exec, t_token *token)
//{
//	if (exec->infile)
//	{
//		close(exec->fd_infile);
//		free(exec->infile);
//	}
//
//}
//
//void	new_heredoc(t_exec *exec, t_token *token)
//{
//	if (exec->infile)
//		close(exec->fd_infile);
//}
//
//void	set_new_infile(t_exec *exec, t_token *tokens)
//{
//	t_token *token;
//
//	tokens = token;
//	if (token->info->type == REDIRECT_IN)
//		new_infile(exec, token->next);
//	else
//		new_heredoc(exec, token->next);
//}
//
//void	gestion_infile(t_token *tokens, t_exec *exec)
//{
//	t_token *seach_tok_in;
//
//	seach_tok_in = tokens;
//	while (seach_tok_in)
//	{
//		if (is_token_redi_in)
//			set_new_infile(exec, seach_tok_in);
//
//	}
//}
//////////////////////////OUTFILE////////////////////////////////


void set_new_outfile(t_exec *exec, t_token *token)
{
    if (exec->outfile)
    {
        close(exec->fd_outfile);
        free(exec->outfile);
    }
    exec->outfile = ft_strdup(token->next->value);
	// protection malloc
    if (token->info->type == REDIRECT_OUT)
    {
        exec->fd_outfile = open(exec->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (exec->fd_outfile == -1)
            perror("open");
    }
    else if (token->info->type == REDIRECT_D_OUT)
    {
        exec->fd_outfile = open(exec->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (exec->fd_outfile == -1)
            perror("open");
    }
}

bool	is_token_redi_out(t_token *token)
{
    if (token->info->type == REDIRECT_OUT || token->info->type == REDIRECT_D_OUT)
        return (true);
    return (false);
}

void    gestion_outfile(t_token *tokens, t_exec *exec)
{
    t_token *seach_tok_out;

    seach_tok_out = tokens;
    while (seach_tok_out)
    {
        if (is_token_redi_out(seach_tok_out))
            set_new_outfile(exec, seach_tok_out);
        seach_tok_out = seach_tok_out->next;
    }
}

void	set_exec_and_exec(t_token *tokens, char **cmd, t_exec *exec)
{
	//gestion_infile;
    (void)cmd;
	gestion_outfile(tokens, exec);
    // printf("exec->outfile = %s\n", exec->outfile);
    // printf("exec->fd_outfile = %d\n", exec->fd_outfile);
	//start_exec;


}

void	exec(t_token *tokens, t_commande *commande, char **env)
{
	t_token *info_token;
//	t_token *for_print;
    (void)env;
	t_exec *exec;
    exec = malloc(sizeof(t_exec));
	int i;

	i = 0;
//get_path;
	while (commande)
	{
		info_token = get_info_token(tokens, i);
		if (!info_token)
			return ;
		i++;
        set_exec_and_exec(info_token, commande->cmd, exec);
		commande = commande->next;
	//	for_print = info_token;
	/*	while (for_print)
		{
			printf("\033[1;31mtoken value = %s\n\033[0m", for_print->value);
			printf("\033[1;33mtoken type = %d\n\033[0m", for_print->info->type);
			printf("\033[1;34mtoken index = %d\n\033[0m", for_print->token_index);
			printf("\n\n");
			for_print = for_print->next;
		}
		printf("next \n\n");*/
		free_list_tokens(info_token);
	}
}
