/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_info_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 17:34:33 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/01 13:12:27 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// demander a nassim a quoi sert cette fonction et si c'est pas mieux de mettre les infos des tokens dans la structure prevu a cet effet (t_node_info)
int *ft_info_token(char **line_split , int *give_nb_token)
{
    int nb_token;
    int i;
    int *type ;

    nb_token = 0;
    i = 0;
    while (line_split[nb_token])
        nb_token++;
    printf("nb_token = %d\n",nb_token);
    type = malloc(sizeof(int) * nb_token);
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
    printf ( "nb token == %d\n" ,i);
    *give_nb_token = i;
    i = 0;
    while (i < nb_token)
    {
        printf("type nb %d = %d\n", i, type[i]);
        i++;
    }

    return (type);
 //   exit (0);
}
