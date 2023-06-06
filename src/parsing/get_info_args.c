/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_info_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 17:34:33 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/05 10:34:58 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// demander a nassim a quoi sert cette fonction et si c'est pas mieux de mettre les infos des tokens dans la structure prevu a cet effet (t_token_info)
int *ft_get_info_args(char **line_split , int *give_nb_args)
{
    int nb_args;
    int i;
    int *type ;

    nb_args = 0;
    i = 0;
    while (line_split[nb_args])
        nb_args++;
    printf("nb_args = %d\n",nb_args);
    type = malloc(sizeof(int) * nb_args);
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
    *give_nb_args = i;
    i = 0;
//    while (i < nb_args)
//    {
//        printf("type tableau nb %d = %d\n", i, type[i]);
//        i++;
//    }

    return (type);
 //   exit (0);
}
