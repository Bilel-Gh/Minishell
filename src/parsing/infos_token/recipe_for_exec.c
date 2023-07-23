/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recipe_for_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 22:49:57 by ncharii           #+#    #+#             */
/*   Updated: 2023/07/23 23:21:15 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*ft_db_array_join(char **strings, int count)
{
	int		total_len;
	int		i;
	char	*result;
	int		len;

	if (strings == NULL || count == 0)
		return (NULL);
	total_len = 0;
	i = 0;
	len = 0;
	while (i < count)
	{
		total_len += ft_strlen(strings[i]);
		i++;
	}
	result = (char *)malloc((total_len + 1) * sizeof(char));
	ft_get_result_db_join(strings, count, result, len);
	return (result);
}

void	ft_get_result_db_join(char **strings, int count, char *result, int len)
{
	int	current_index;
	int	i;

	current_index = 0;
	i = 0;
	while (i < count)
	{
		len = ft_strlen(strings[i]);
		ft_strncpy(result + current_index, strings[i], len);
		current_index += len;
		i++;
	}
	result[current_index] = '\0';
}

void	change_cmd_list(t_commande *list_commande)
{
	int	i;

	while (list_commande)
	{
		if (ft_strcmp(list_commande->cmd[0], "export") != 0)
		{
			i = 0;
			while (list_commande->cmd[i])
				ft_change_cmds(list_commande, &i);
			list_commande->cmd = ft_clean_null_db_array(list_commande->cmd, &i);
			list_commande = list_commande->next;
		}
		else
			list_commande = list_commande->next;
	}
}

void	ft_change_cmds(t_commande *list_commande, int *i)
{
	char	**arg_to_unquote;
	int		*type_arg_to_unquote;
	int		nb_arg_to_unquote;
	char	**no_quote_args;

	arg_to_unquote = ft_lexeur(list_commande->cmd[*i]);
	nb_arg_to_unquote = ft_db_arr_len(arg_to_unquote);
	type_arg_to_unquote = ft_get_info_args2(arg_to_unquote,
			&nb_arg_to_unquote);
	no_quote_args = kick_quote(type_arg_to_unquote,
			nb_arg_to_unquote,
			arg_to_unquote);
	free_db_array(arg_to_unquote);
	free(list_commande->cmd[*i]);
	if (no_quote_args != NULL)
	{
		list_commande->cmd[*i] = ft_db_array_join(no_quote_args,
				ft_db_arr_len(no_quote_args));
	}
	free_db_array(no_quote_args);
	free(type_arg_to_unquote);
	*i = *i + 1;
}

t_commande	*cmd_complete(t_token *token)
{
	t_commande	*list_commande;
	int			nb_node;
	t_commande	*head;
	char		**cmd_join;

	cmd_join = NULL;
	nb_node = nb_pipe(token) + 1;
	list_commande = malloc(sizeof(t_commande));
	if (!list_commande)
		return (list_commande);
	head = list_commande;
	list_commande->next = 0;
	creat_cmd_list(list_commande, nb_node);
	list_commande = head;
	cmd_join = give_cmd_join(token, nb_node);
	if (!cmd_join)
		return (NULL);
	add_cmd_to_list_commande(list_commande, cmd_join);
	list_commande = head;
	change_cmd_list(list_commande);
	free_db_array(cmd_join);
	return (head);
}
