/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_info_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 17:34:33 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/01 13:12:27 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// demander a nassim a quoi sert cette fonction et si c'est pas mieux de mettre les infos des nodes dans la structure prevu a cet effet (t_node_info)
int *ft_info_node(char **line_split , int *give_nb_node)
{
    int nb_node;
    int i;
    int *type ;

    nb_node = 0;
    i = 0;
    while (line_split[nb_node])
        nb_node++;
    printf("nb_node = %d\n",nb_node);
    type = malloc(sizeof(int) * nb_node);
    while (line_split[i])
    {
        if (line_split[i][0] == 34)
			type[i] = ALPHANUM;
		else if (line_split[i][0] == 39)
			type[i] = ALPHANUM;
        else if (line_split[i][0] == ' ')
			type[i] = ESPACE;
		else if (line_split[i][0] == '>' || line_split[i][0] == '<') {
			type[i] = REDIRECT;
		}  else if (line_split[i][0] == '|') {
			type[i] = PIPE;
		} else if (ft_isprint(line_split[i][0])) {
			type[i] = ALPHANUM;
         }
         i++;
    }
    printf ( "nb node == %d\n" ,i);
    *give_nb_node = i;
    i = 0;
    while (i < nb_node)
    {
        printf("type nb %d = %d\n", i, type[i]);
        i++;
    }

    return (type);
 //   exit (0);
}
