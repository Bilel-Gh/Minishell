/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recipe_for_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 22:49:57 by ncharii           #+#    #+#             */
/*   Updated: 2023/07/21 07:11:47 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

typedef struct s_split
{
    int		count;
    char	**result;
    char	**clean_result;
    int		space_count;
    int		i;
    int		len;
    int		in_quotes;
    char	type_inquote;
    int		start;
    int		len_str_to_add;
}			t_split;

void		ft_init_s(char *str, t_split *s);

int			ft_init_result(char *str, t_split *s);

void		ft_do_split_custom(char *str, t_split *s);

void		ft_get_result_db_join(char **strings, int count, char *result,
                                  int len);

void		ft_change_cmds(t_commande *list_commande, int *i);

void ft_is_in_quote_split(const char *str, t_split *s);

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
        tmp = malloc(sizeof(t_commande));
        if (!tmp)
            return ;
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
    printf("JOIN CMD S1 = %s    |     SIZE = %d \n", s1, size_s1);
    printf("JOIN CMD S2 = %s    |     SIZE = %d \n", s2, size_s2);
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
    printf("STR JOIN = %s\n\n\n", str);
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
    int is_first;

    i = 0;
    is_first = 1;
    tok_sch = token;
    cmd_join = malloc(sizeof(char *) * (nb_cdm + 1));
    if (!cmd_join)
        return (0);
    b_zero_for_cmd_join(cmd_join, nb_cdm, i);
    while (tok_sch)
    {
        while (tok_sch && i < nb_cdm)
        {
            printf("************** tok_sch->value = %s\n", tok_sch->value);
            printf("************** tok_sch->info->type = %d\n\n\n", tok_sch->info->type);
            if (is_first && (tok_sch->info->type == REDIRECT_IN || tok_sch->info->type == REDIRECT_OUT))
            {
                cmd_join[i] = ft_join_cmd(cmd_join[i], tok_sch->value);
                is_first = 0;
            }
            if (tok_sch->info->type == COMMANDE || tok_sch->info->type == ARG)
                cmd_join[i] = ft_join_cmd(cmd_join[i], tok_sch->value);
            is_first = 0;
            if (tok_sch->info->type == T_PIPE)
                break ;
            tok_sch = tok_sch->next;
        }
        i++;
        if (i != nb_cdm)
            tok_sch = tok_sch->next;
    }
    printf("************** cmd_join[0] = %s\n\n\n", cmd_join[0]);
    printf("************** cmd_join[1] = %s\n\n\n", cmd_join[1]);
    return (cmd_join);
}

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

int	count_spaces(char *str, int len)
{
    int	space_count;
    int	i;

    space_count = 0;
    i = 0;
    while (i < len)
    {
        if (str[i] == ' ')
            space_count++;
        i++;
    }
    return (space_count);
}

char	**allocate_result_array(int space_count)
{
    char	**result;

    result = (char **)malloc((space_count + 1) * sizeof(char *));
    b_zero_for_cmd_join(result, space_count, 0);
    return (result);
}

void	ft_do_split_custom(char *str, t_split *s)
{
    ft_is_in_quote_split(str, s);
    if (str[s->i] == ' ' && !s->in_quotes)
    {
        s->len_str_to_add = s->i - s->start;
        if (s->len_str_to_add > 0)
        {
            s->result[s->count] = (char *)malloc((s->len_str_to_add + 1)
                                                 * sizeof(char));
            ft_strncpy(s->result[s->count], str + s->start, s->len_str_to_add);
            s->result[s->count][s->len_str_to_add] = '\0';
            (s->count)++;
        }
        s->start = s->i + 1;
    }
}

void ft_is_in_quote_split(const char *str, t_split *s) {
    if (str[s->i] == '"' || str[s->i] == '\'')
    {
        if (s->in_quotes && str[s->i] == s->type_inquote)
        {
            s->in_quotes = 0;
            s->type_inquote = '\0';
        }
        else if (!s->in_quotes)
        {
            s->in_quotes = 1;
            s->type_inquote = str[s->i];
        }
    }
}

int	ft_init_result(char *str, t_split *s)
{
    s->result = NULL;
    if (str == NULL || ft_strlen(str) == 0)
    {
        s->result = (char **)malloc(sizeof(char *));
        s->result[0] = NULL;
        return (0);
    }
    return (1);
}

void	ft_init_s(char *str, t_split *s)
{
    s->len = ft_strlen(str);
    s->space_count = count_spaces(str, s->len);
    s->result = allocate_result_array(s->space_count);
    s->in_quotes = 0;
    s->type_inquote = '\0';
    s->start = 0;
    s->i = 0;
    s->count = 0;
}

char	**ft_custom_split(char *str)
{
    t_split	*s;
    char **final_result;

    printf("str custom split = %s\n", str);
    s = malloc(sizeof(t_split));
    if (!ft_init_result(str, s))
    {
        final_result = s->result;
        free(s);
        return (final_result);
    }
    ft_init_s(str, s);
    while (s->i < s->len)
    {
        ft_do_split_custom(str, s);
        s->i++;
    }
    s->len_str_to_add = s->i - s->start;
    if (s->len_str_to_add > 0)
    {
        s->result[s->count] = (char *)malloc((s->len_str_to_add + 1)
                                             * sizeof(char));
        ft_strncpy(s->result[s->count], str + s->start, s->len_str_to_add);
        s->result[s->count][s->len_str_to_add] = '\0';
    }
    s->clean_result = ft_db_array_dup(s->result);
    ft_free_db_by_len(s->result, s->space_count + 1);
    final_result = s->clean_result;
    free(s);
    return (final_result);
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
    if (commande->cmd)
        free_db_array(commande->cmd);
    free(commande);
}

int	ft_db_arr_len(char **arr)
{
    int	i;

    i = 0;
    while (arr[i])
        i++;
    return (i);
}

int	ft_get_type2(char *const *line_split, int i, int *type)
{
    while (line_split[i])
    {
        if (line_split[i][0] == 34)
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

char	*ft_db_array_join(char **strings, int count)
{
    int		total_len;
    int		i;
    char	*result;
    int		len;

    if (strings == NULL || count == 0)
    {
        printf(" c est ici \n");
        return (NULL);
    }
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
            {
                printf("$$$$$$$$$$ %s /n",list_commande->cmd[i]);
                ft_change_cmds(list_commande, &i);
            }
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
                               int y = 0;
    while (no_quote_args[y])
    {
        printf("\033[1;36mno_quote_args[%d] = %s\n\033[0m", y, no_quote_args[y]);
        y++;
    }
    free_db_array(arg_to_unquote);
    free(list_commande->cmd[*i]);
    if (no_quote_args != NULL)
    {
        list_commande->cmd[*i] = ft_db_array_join(no_quote_args,
                                             ft_db_arr_len(no_quote_args));
        printf("\033[1;36mlist_commande->cmd[%d] = %s\n\033[0m", *i, list_commande->cmd[*i]);

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
    {
        return (NULL);
    }
    add_cmd_to_list_commande(list_commande, cmd_join);
    list_commande = head;
    change_cmd_list(list_commande);
    free_db_array(cmd_join);
    return (head);
}

