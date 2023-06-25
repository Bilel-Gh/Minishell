/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recipe_for_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 22:49:57 by ncharii           #+#    #+#             */
/*   Updated: 2023/06/25 16:31:33 by ncharii          ###   ########.fr       */
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
    int z = 0;
    while (cmd_join[z])
    {
        printf("@@@@@@@@cmd_join[%d] = %s\n", z, cmd_join[z]);
        z++;
    }
	return (cmd_join);
}

// char **ft_join_for_export(char **cmd_join)
// {
//     int i = 0;
//     int l = 0;
//     char *tmp;
//     char **cmd_join2;

//     int count = 0;
//     while (cmd_join[count])
//         count++;

//     cmd_join2 = malloc(sizeof(char *) * (count + 1));
//     if (!cmd_join2)
//         return NULL;

//     while (cmd_join[i])
//     {
//         if (ft_strchr(cmd_join[i], '"') || ft_strchr(cmd_join[i], '\''))
//         {
//             tmp = ft_strdup(cmd_join[i]);
//             i++;
//             while (cmd_join[i] && !ft_strchr(cmd_join[i], '"') && !ft_strchr(cmd_join[i], '\''))
//             {
//                 tmp = ft_strjoin(tmp, " ");
//                 tmp = ft_strjoin(tmp, cmd_join[i]);
//                 free(cmd_join[i]);
//                 i++;
//             }
//             if (cmd_join[i])
//             {
//                 tmp = ft_strjoin(tmp, " ");
//                 tmp = ft_strjoin(tmp, cmd_join[i]);
//                 free(cmd_join[i]);
//                 i++;
//             }
//             cmd_join2[l] = tmp;
//             l++;
//         }
//         else
//         {
//             cmd_join2[l] = cmd_join[i];
//             l++;
//             i++;
//         }
//     }
//     cmd_join2[l] = NULL; // Terminer le tableau de sortie
//     free(cmd_join);
//     return cmd_join2;
// }

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
    unsigned int	i;

    i = 0;
    while (src[i] != '\0' && i < n)
    {
        dest[i] = src[i];
        ++i;
    }
    while (i < n)
    {
        dest[i] = '\0';
        i++;
    }
    return (dest);
}

void	ft_free_db_by_len(char **array, int len)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (i < len)
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = 0;
}

char** ft_custom_split(char* str) {
    int count;
    char** result;
    int space_count;
    int i;
    int len;

    result = NULL;
    // Vérifier si la chaîne est nulle ou vide
    if (str == NULL || ft_strlen(str) == 0)
        return NULL;

    // Calculer le nombre d'espaces potentiels de séparation
    len = ft_strlen(str);
    space_count = 0;
    count = 0;
    i = 0;
    while (i < len)
    {
        if (str[i] == ' ')
            space_count++;
        i++;
    }

    // Allouer de la mémoire pour le tableau de chaînes de caractères
    result = (char**)malloc((space_count + 1) * sizeof(char*));
    b_zero_for_cmd_join(result, space_count, 0);

    // Parcourir la chaîne de caractères et effectuer le découpage
    int in_quotes = 0;
    int start = 0;
    i = 0;
    while (i < len) {
        if (str[i] == '"' || str[i] == '\'')
            in_quotes = !in_quotes;

        if (str[i] == ' ' && !in_quotes) {
            int len_str_to_add = i - start;
            if (len_str_to_add > 0) {
                result[count] = (char*)malloc((len_str_to_add + 1) * sizeof(char));
                ft_strncpy(result[count], str + start, len_str_to_add);
                result[count][len_str_to_add] = '\0';
                (count)++;
            }
            start = i + 1;
        }
        i++;
    }
    // Traiter la dernière sous-chaîne
    int len_str_to_add = i - start;
    if (len_str_to_add > 0) {
        result[count] = (char*)malloc((len_str_to_add + 1) * sizeof(char));
        ft_strncpy(result[count], str + start, len_str_to_add);
        result[count][len_str_to_add] = '\0';
        (count)++;
    }
    
    i = 0;
    return result;
}


void	add_cmd_to_list_commande(t_commande *list_commande, char **cmd_join)
{
	int			i;
	t_commande	*list_cmd;

	list_cmd = list_commande;
	i = 0;
	while (list_cmd)
	{

		list_cmd->cmd = ft_custom_split(cmd_join[i]);
        // list_cmd->cmd = ft_join_for_export(list_cmd->cmd);
        int z = 0;
        while (list_cmd->cmd[z])
        {
            printf("+++++++++ cmd_join[%d] = %s\n", z, list_cmd->cmd[z]);
            z++;
        }
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

//char **ft_gestion_export(char **cmd_join)
//{
//    char** cmd_join_cpy;
//    char *name;
//    char *value;
//    name = NULL;
//    value = NULL;
//    cmd_join_cpy = ft_db_array_dup(cmd_join);
//    int i = 0;
//
//    while (cmd_join_cpy[i] != 0)
//    {
//        name = get_name(cmd_join_cpy[i]);
//        printf("\033[0;33m name = %s \033[0m\n", name);
//        // si la chaine commance par export
//        if (ft_strncmp(cmd_join_cpy[i], " export", 7) == 0)
//        {
//            printf("\033[0;33m HELLO EXPORT \033[0m\n");
//            // si la chaine contient un =
//            if (ft_strchr(cmd_join_cpy[i], '='))
//            {
//                name = ft_strjoin(name, "=");
//                printf("\033[0;33m name2 = %s \033[0m\n", name);
//                // on stock tout ce qui est apres le =
//                value = ft_strchr(*cmd_join_cpy, '=') + 1;
//                value = ft_strtrim(value, " ");
//                printf("\033[0;33m value sans espace = %s \033[0m\n", value);
//            }
//            // on remplace cmd_join_cpy[i] par name + value
//            free(cmd_join_cpy[i]);
//            cmd_join_cpy[i] = ft_strjoin(name, value);
//        }
//        i++;
//    }
//    return (cmd_join_cpy);
//}

int ft_db_arr_len(char **arr)
{
    int i;

    i = 0;
    while (arr[i])
        i++;
    return (i);
}

int	ft_get_type2(char *const *line_split, int i, int *type)
{
    while (line_split[i])
    {
        if (line_split[i][0] == 34) // "
            type[i] = QUOTE_D;
        else if (line_split[i][0] == 39)
            type[i] = QUOTE_S;
        else if (line_split[i][0] == ' ')
            type[i] = ESPACE;
        else if (line_split[i][0] == '>' || line_split[i][0] == '<')
        {
            type[i] = REDIRECT;
        }
        else if (line_split[i][0] == '|')
        {
            type[i] = PIPE;
        }
        else if (ft_isprint(line_split[i][0]))
        {
            type[i] = ALPHANUM;
        }
        i++;
    }
    return (i);
}

int	*ft_get_info_args2(char **line_split, int *give_nb_args)
{
    int	nb_args;
    int	i;
    int	*type;

    nb_args = 0;
    i = 0;
    if (line_split == NULL)
        return (NULL);
    while (line_split[nb_args])
        nb_args++;
    type = malloc(sizeof(int) * nb_args);
    i = ft_get_type2(line_split, i, type);
    *give_nb_args = i;
    i = 0;
    return (type);
}

char* ft_db_array_join(char** strings, int count)
{
    int total_len;
    int i;
    char* result;
    int current_index;

    if (strings == NULL || count == 0) {
        return NULL;
    }
    total_len = 0;
    i = 0;
    while (i < count) {
        total_len += ft_strlen(strings[i]);
        i++;
    }
    result = (char*)malloc((total_len + 1) * sizeof(char));
	current_index = 0;
    i = 0;
    while (i < count) {
        int len = ft_strlen(strings[i]);
        ft_strncpy(result + current_index, strings[i], len);
        current_index += len;
        i++;
    }
    result[current_index] = '\0';
    return result;
}

void change_cmd_list(t_commande *list_commande)
{
    char **arg_to_unquote;
    int* type_arg_to_unquote;
    int nb_arg_to_unquote;
    char	**no_quote_args;
    // t_commande *cpy_list_commande;
    // cpy_list_commande = ft_dup_list(list_commande);
    // free_list_commande(list_commande);

    while (list_commande)
    {
        if (ft_strcmp(list_commande->cmd[0], "export") != 0)
        {
            int i = 0;

            while (list_commande->cmd[i])
            {
                arg_to_unquote = ft_lexeur(list_commande->cmd[i]);
                nb_arg_to_unquote = ft_db_arr_len(arg_to_unquote);
                type_arg_to_unquote = ft_get_info_args2(arg_to_unquote, &nb_arg_to_unquote);
                no_quote_args = kick_quote(type_arg_to_unquote, nb_arg_to_unquote, arg_to_unquote);
                free_db_array(arg_to_unquote);
                free(list_commande->cmd[i]);
                list_commande->cmd[i] = ft_db_array_join(no_quote_args, ft_db_arr_len(no_quote_args));
                free_db_array(no_quote_args);
                free(type_arg_to_unquote);
//                printf("\033[0;35m            FINAL VALUE = %s \033[0m\n", list_commande->cmd[i]);
//                int z = 0;
//                while (no_quote_args[z])
//                {
//                    printf("\033[0;35m                      no_quote_args[%d] = %s \033[0m\n", z, no_quote_args[z]);
//                    printf("\033[0;35m                           type_arg_to_unquote[%d] = %d \033[0m\n", z, type_arg_to_unquote[z]);
//                    z++;
//                }
                i++;
            }
            list_commande = list_commande->next;
        }
        else
            list_commande = list_commande->next;
    }
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
    // cmd_join = ft_gestion_export(cmd_join);
	add_cmd_to_list_commande(list_commande, cmd_join);
	list_commande = head;
    change_cmd_list(list_commande);
	free_db_array(cmd_join);
	return (head);
}
