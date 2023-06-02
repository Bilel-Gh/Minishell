/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_node_infos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 05:03:47 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/02 05:03:48 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// version a completer demain
int ft_is_command(char* value)
{
    (void)value;
    // il faut regarder si DANS LE PATH la chaine correspond a une commande

    char *path;

    path = getenv("PATH");
    printf("path = %s\n", path);
    return 1;
}
// version a completer demain
int ft_is_argument(char* value)
{
    if (value[1] == '\0')
        return 1;
    if (value[0] != '-' && value[0] != '|' && value[0] != '>'
        && value[0] != '<')
        return 1;
    return 0;
}

// version a completer demain
int ft_is_option(char* value)
{
    if (value[0] == '-')
        return 1;
    return 0;
}

enum e_node_type get_value_type(char* value)
{
    // si c'est une commande
    if (access(value, F_OK) == 0)
        return FICHIER;
    else if (ft_is_command(value))
        return COMMANDE;
    else if (ft_is_argument(value))
        return ARG;
    else if (ft_is_option(value))
        return OPTION;
    else
        return ERROR;
    // si c'est un fichier

}

int		ft_strncmp(char *s1, char *s2, unsigned int n)
{
    unsigned int	i;
    int				r;

    r = 0;
    i = 0;
    while ((s1[i] || s2[i]) && r == 0 && i < n)
    {
        if (s1[i] != s2[i])
        {
            r = s1[i] - s2[i];
        }
        i++;
    }
    return (r);
}

t_node_info*    ft_get_value_infos(char* value)
{
    if (value == NULL)
        return NULL;
    t_node_info* infos = (t_node_info*)malloc(sizeof(t_node_info));
    if (ft_strncmp(value, "|", 1) == 0)
        infos->type = N_PIPE;
    else if (ft_strncmp(value, ">", 1) == 0)
        infos->type = REDIRECT_OUT;
    else if (ft_strncmp(value, "<", 1) == 0)
        infos->type = REDIRECT_IN;
    else if (ft_strncmp(value, ">>", 2) == 0)
        infos->type = REDIRECT_D_OUT;
    else if (ft_strncmp(value, "<<", 2) == 0)
        infos->type = REDIRECT_D_IN;
    else
        infos->type = get_value_type(value);

    return infos;
}

t_node* addNode_with_infos(t_node* head, char* value)
{
    t_node* newNode = (t_node*)malloc(sizeof(t_node));
    newNode->value = value;
    newNode->info = ft_get_value_infos(value);
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        t_node* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    return head;
}

t_node* ft_get_nodes_with_infos(char **args, int *info_args, int nb_args)
{
    (void)info_args;
    int i;
    i = 0;
    t_node* nodeHead = NULL;

    while (i < nb_args)
    {
        nodeHead = addNode_with_infos(nodeHead, args[i]);
        i++;
    }
    return nodeHead;
}
