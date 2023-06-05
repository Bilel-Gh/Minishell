/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_node_infos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 05:03:47 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/02 19:04:30 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strdup(char *src)
{
    int		i;
    int		len;
    char	*str;

    len = 0;
    while (src[len])
        len++;
    str = (char*)malloc(sizeof(*str) * (len + 1));
    i = 0;
    while (i < len)
    {
        str[i] = src[i];
        i++;
    }
    return (str);
}

// version a completer demain
int ft_is_command(char* value)
{
    (void)value;
    // il faut regarder si DANS LE PATH la chaine correspond a une commande

    char *path;
    char **token;

    path = getenv("PATH");
    char* pathCopy = ft_strdup(path);
    token = ft_split(pathCopy, ":");

    while (*token)
    {
        char *fullPath; // 256 est le max de char dans un path
        fullPath = ft_strjoin(*token, "/");
        fullPath = ft_strjoin(fullPath, value);
        if (access(fullPath, F_OK) == 0)
        {
            printf("\033[0;33m[OK CMD]\033[0m\n");
            free(fullPath);
            free(pathCopy);
            return 1;
        }
        free(fullPath);
        token++;
    }
    free(pathCopy);
    printf("\033[0;31m[NOT OK CMD]\033[0m\n");
    return 0;
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

enum e_node_type get_value_type(t_node* node)
{
    // si c'est une commande
    if (access(node->value, F_OK) == 0)
        return FICHIER;
    else if (node->prev == NULL && node->node_index == 0 && ft_is_command(node->value))
        return COMMANDE;
    else if (ft_is_argument(node->value))
        return ARG;
    else if (ft_is_option(node->value))
        return OPTION;
    else
        return ERROR;
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

t_node_info*    ft_get_value_infos(char* value, t_node* node)
{
//    printf("\033[0;32m node value : %s\033[0m\n", node->value);
//    if (node->prev != NULL)
//        printf("\033[0;31m node prev value : %s\033[0m\n", node->prev->value);
//    printf("\033[0;32m node index : %d\033[0m\n", node->node_index);
//    if (node->prev != NULL)
//        printf("\033[0;31m node prev index : %d\033[0m\n", node->prev->node_index);
//    if (value == NULL)
//        return NULL;
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
        infos->type = get_value_type(node);

    return infos;
}

t_node* create_node(t_node* head, char* value, int index)
{
    t_node* newNode = (t_node*)malloc(sizeof(t_node));
    newNode->value = value;
    newNode->node_index = index;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        t_node* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        newNode->prev = current;
    }
    return head;
}

// TODO :
// il faut dabbord cree les node avec prev et next add_node simple
// apres ajouter les infos avec une nouvelle fonction add_infos_to_node
t_node* ft_get_nodes_with_infos(char **args, int *info_args, int nb_args)
{
    (void)info_args;
    int i;
    i = 0;
    t_node* nodeHead = NULL;

    while (i < nb_args)
    {
        nodeHead = create_node(nodeHead, args[i], i);
        i++;
    }
    i = 0;
    t_node* current = nodeHead;
    while (current != NULL)
    {
        current->info = ft_get_value_infos(current->value, current);
        current = current->next;
    }
    return nodeHead;
}
