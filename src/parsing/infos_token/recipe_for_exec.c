/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recipe_for_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 22:49:57 by ncharii           #+#    #+#             */
/*   Updated: 2023/06/14 16:42:47 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	init_struc_cmd(t_commande *commande)
{
	commande->cmd = 0;
}

int	nb_pipe(t_token *info)
{
	int		nbr_pipe;
	t_token	*search_pipe;

	search_pipe = info;
	nbr_pipe = 0;
	while (search_pipe)
	{
		if (search_pipe->info->type == T_PIPE)
			nbr_pipe++;
		search_pipe = search_pipe->next;
	}
	return (nbr_pipe);
}

void	creat_cmd_list(t_commande *cmd, int nb_node)
{
	int			i;
	t_commande	*tmp;

	i = 1;
	while (i < nb_node)
	{
		tmp = malloc(sizeof(t_commande)); //security!!!
		if (!tmp)
		{
			printf("##############################################\n");
			return ;
		}
		init_struc_cmd(tmp);
		tmp->next = 0;
		cmd->next = tmp;
		cmd = cmd->next;
		i++;
	}
}

char	*ft_join_cmd(char *s1, char *s2)
{
	char	*str;
	int		size_s1;
	int		size_s2;
	int		i;

	i = 0;
	size_s1 = ft_strlen(s1);
	size_s2 = ft_strlen(s2);
	str = malloc(sizeof(char) * (size_s1 + size_s2 + 2));
	if (!str)
		return (0);
	ft_complete(s1, str, &i);
	str[i] = ' ';
	i++;
	ft_complete(s2, str, &i);
	if (s1)
		free(s1);
	str[i] = '\0';
	return (str);
}

void	b_zero_for_cmd_join(char **cmd_join, int nb_cdm, int i)
{
	cmd_join[nb_cdm] = 0;
	while (i < nb_cdm)
	{
		cmd_join[i] = 0;
		i++;
	}
	i = 0;
}

char	**give_cmd_join(t_token *token, int nb_cdm)
{
	char	**cmd_join;
	int		i;
	t_token	*tok_sch;

	i = 0;
	tok_sch = token;
	cmd_join = malloc(sizeof(char *) * (nb_cdm + 1));
	if (!cmd_join)
		return (0);
	b_zero_for_cmd_join(cmd_join, nb_cdm, i);
	while (tok_sch && i != nb_cdm)
	{
		while (tok_sch)
		{
			if (tok_sch->info->type == COMMANDE || tok_sch->info->type == ARG)
				cmd_join[i] = ft_join_cmd(cmd_join[i], tok_sch->value);
			if (tok_sch->info->type == T_PIPE)
				break ;
			tok_sch = tok_sch->next;
		}
		i++;
		if (i != nb_cdm)
			tok_sch = tok_sch->next;
	}
	return (cmd_join);
}

void	add_cmd_to_list_commande(t_commande *list_commande, char **cmd_join)
{
	int			i;
	t_commande	*list_cmd;

	list_cmd = list_commande;
	i = 0;
	while (list_cmd)
	{
		list_cmd->cmd = ft_split(cmd_join[i], ' ');
		i++;
		if (list_cmd->next == 0)
			break ;
		list_cmd = list_cmd->next;
	}
}

void	free_list_commande(t_commande *commande)
{
	t_commande	*head;

	while (commande->next)
	{
		head = commande;
		commande = commande->next;
		free_db_array(head->cmd);
		free(head);
	}
	free_db_array(commande->cmd);
	free(commande);
}

t_commande	*cmd_complete(t_token *token)
{
	t_commande	*list_commande;
	int			nb_node;
	t_commande	*head;
	char		**cmd_join;

	nb_node = nb_pipe(token) + 1;
	list_commande = malloc(sizeof(t_commande));
	if (!list_commande)
		return (list_commande);
	head = list_commande;
	list_commande->next = 0;
	creat_cmd_list(list_commande, nb_node);
	list_commande = head;
	cmd_join = give_cmd_join(token, nb_node);
	add_cmd_to_list_commande(list_commande, cmd_join);
	list_commande = head;
	free_db_array(cmd_join);
	return (head);
}
