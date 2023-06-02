/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_inter_space.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 02:49:46 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/02 02:49:47 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool is_word(int type_args)
{
    if (type_args == ALPHANUM
        || type_args == QUOTE_S || type_args == QUOTE_D)
        return (true);
    return (false);
}

int count_new_args(int *type_args, int nb_args)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (i < nb_args)
    {
        printf("nb args = %d\n", nb_args);
        if (is_word(type_args[i]))
        {
            printf("nb_args new args = %d\n type_", nb_args);
            while (is_word(type_args[i]) && i < nb_args)
            {
                printf("i new args = %d\n", i);
                i++;
                if (i == nb_args)
                    return (printf("FSFSFSFF\n"),j + 1);

            }
            j++;
            continue;
        }

        (i++, j++);
    }
    printf("j new args = %d\n", j);
    return (j);
}

int malloc_new_args(char **new_args, char **args, int *type_args, int nb_args)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (i < nb_args)
    {
        if (is_word(type_args[i]))
        {
            while (is_word(type_args[i]) && i < nb_args)
            {
                new_args[j] = ft_strjoin(new_args[j], args[i]);
                i++;
                if (i == nb_args)
                    return (new_args[j + 1] = 0, 1);
            }
            j++;
            continue;
        }
        new_args[j] = ft_strjoin(new_args[j], args[i]);
        (i++, j++);
    }
    new_args[j] = 0;
    return (1);

}

char **join_inter_space(char **args, int *type_args, int *nb_args)
{
    char ** new_args;
    int i;
    int nb_new_args;

    nb_new_args = count_new_args(type_args, *nb_args);
    //printf("nb space =  %d\n", nb_new_args(type_args, nb_args));
    printf("\n \n nb args_join %d\n", nb_new_args);
    //prinft("NB args _join = %d  \n",nb_new_args);

    if (nb_new_args == *nb_args)
        return(args) ;//return (args);
    i = 0;
    new_args = malloc( sizeof(char*) * (nb_new_args + 1));
    while (i <= nb_new_args)
    {
        new_args[i] = malloc(1);
        new_args[i] = NULL;
        i++;
    }
    i = 0;
    if (!new_args)
        return(printf("error"), NULL) ; //ft_free;
    if (!malloc_new_args(new_args, args, type_args, *nb_args))
        return(printf("error malloc_new_args"), NULL) ;//ft_free;
    printf("new args %d = %s", i, new_args[i]);
    while ( i < nb_new_args)
    {
        printf("new args %d = %s", i, new_args[i]);
        i++;
    }
    *nb_args = nb_new_args;
    return (new_args);
}
