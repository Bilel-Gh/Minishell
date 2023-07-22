/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token_infos.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 21:08:43 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/22 05:03:38 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

enum e_token_type	get_value_type(t_token *token, int *is_commande_in_pipe)
{
	(void)is_commande_in_pipe;
	if (ft_is_infile(token))
		return (INFILE);
	else if (ft_is_outfile(token))
		return (OUTFILE);
	else if (token->prev && token->prev->info->type == REDIRECT_D_OUT)
		return (LIMITEUR);
	else
		return (ARG);
}

t_token_info	*add_infos_to_token(char *value, t_token *token,
		int *is_commande_in_pipe)
{
	t_token_info	*infos;

	if (value == NULL)
		return (NULL);
	infos = (t_token_info *)malloc(sizeof(t_token_info));
	if (ft_strncmp(value, "|", 1) == 0)
	{
		*is_commande_in_pipe = 0;
		infos->type = T_PIPE;
	}
	else if (ft_strcmp(value, ">") == 0)
		infos->type = REDIRECT_OUT;
	else if (ft_strcmp(value, "<") == 0)
		infos->type = REDIRECT_IN;
	else if (ft_strcmp(value, ">>") == 0)
		infos->type = REDIRECT_D_OUT;
	else if (ft_strcmp(value, "<<") == 0)
		infos->type = REDIRECT_D_IN;
	else
		infos->type = get_value_type(token, is_commande_in_pipe);
	return (infos);
}

t_token	*create_token(t_token *head, char *value, int index)
{
	t_token	*new_token;
	t_token	*current;

	value = ft_strtrim(value, " ");
	new_token = (t_token *)malloc(sizeof(t_token));
	new_token->value = value;
	new_token->token_index = index;
	new_token->next = NULL;
	new_token->prev = NULL;
	if (head == NULL)
	{
		head = new_token;
	}
	else
	{
		current = head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = new_token;
		new_token->prev = current;
	}
	return (head);
}

void	handle_kick_quote(t_token *token)
{
	t_token	*head;
	char	*tmp;

	head = token;
	while (head)
	{
		tmp = head->value;
		if ((head->info->type == INFILE || head->info->type == OUTFILE
				|| head->info->type == LIMITEUR) && (head->value[0] == '\''
				|| head->value[0] == '\"'))
			head->value = copy_sans_quote(head->value);
		else
			head->value = copy_cont(head->value);
		free(tmp);
		head = head->next;
	}
}

t_token	*ft_get_tokens_with_infos(char **args, int nb_args)
{
	int		i;
	t_token	*token_head;
	t_token	*current;
	int		is_commande_in_pipe;

	i = 0;
	token_head = NULL;
	while (i < nb_args)
	{
		token_head = create_token(token_head, args[i], i);
		i++;
	}
	i = 0;
	current = token_head;
	is_commande_in_pipe = 0;
	while (current != NULL)
	{
		current->info = add_infos_to_token(current->value, current,
				&is_commande_in_pipe);
		current = current->next;
	}
	token_head = ft_verif_cmd(&token_head);
	handle_kick_quote(token_head);
	return (token_head);
}
