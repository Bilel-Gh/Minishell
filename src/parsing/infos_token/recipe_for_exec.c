#include "../../../includes/minishell.h"





void init_struc_cmd(t_commande *commande)
{
	commande->cmd = 0;
}

int nb_pipe(t_token *info)
{
	int nbr_pipe;
	t_token *search_pipe;

	search_pipe = info;
	nbr_pipe = 0;
	while(search_pipe)
	{
		if (search_pipe->info->type == T_PIPE)
			nbr_pipe++;
		search_pipe =search_pipe->next;
	}
	return(nbr_pipe);
}

void creat_cmd_list(t_commande *cmd, int nb_node)
{
	int i;
	t_commande *tmp;

	i = 1;
	while (i < nb_node)
	{
		tmp = malloc(sizeof(t_commande));//security!!!
		if (!tmp)
		{
			printf("########################################################################\n");
			return ;
		}
		init_struc_cmd(tmp);
		tmp->next = 0;
		cmd->next = tmp;
		cmd = cmd->next;
		i++;
	}
}

char *ft_join_cmd(char *s1, char *s2)
{

	char *str;
	int size_s1;
	int size_s2;
	int i;

	// if (!s1 || !s2)
	//	return (NULL);
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


char **give_cmd_join(t_token *token, int nb_cdm)
{
	char **cmd_join;
	int i;
	t_token *token_search;

	i = 0;
	token_search = token;
	cmd_join = malloc(sizeof(char *) * (nb_cdm + 1));
	if(!cmd_join)
		return(0);
	cmd_join[nb_cdm] = 0;
	while(i < nb_cdm)
	{
		cmd_join[i] = 0;
		i++;
	}
	i = 0;
	while (token_search)
	{
		while (token_search)
		{
			if (token_search->info->type == COMMANDE || token_search->info->type == ARG)
				cmd_join[i] = ft_join_cmd(cmd_join[i], token_search->value);
			if (token_search->info->type == T_PIPE)
			{
				token_search = token_search->next;
				break;
			}
			token_search = token_search->next;
		}
		i++;
		if (i == nb_cdm)
			break;
	}
	return(cmd_join);
}

void add_cmd_to_list_commande(t_commande *list_commande, char **cmd_join)
{
	int i;
	t_commande *list_cmd;

	list_cmd = list_commande;
	i = 0;
	while(list_cmd)
	{
		list_cmd->cmd = ft_split(cmd_join[i], ' ');
		i++;
		if (list_cmd->next == 0)
		    break;
		list_cmd = list_cmd->next;
	}
}

void free_list_commande(t_commande *commande)
{
	t_commande* head;
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

t_commande *cmd_complete(t_token *token)
{
	t_commande *list_commande;
	int nb_node;
	t_commande *head;
	char **cmd_join;

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
	int i;
	i = 0;
	int y;
	y = 0;
	while (y < nb_node)
	{
		i = 0;
		while (list_commande->cmd[i])
		{

			printf("nb cdm %d :  %s\n", y, list_commande->cmd[i]);
			i++;
		}
		list_commande = list_commande->next;
		y++;
	}
	return (head);
}
