/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 17:37:32 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/01 01:44:18 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *ft_strstr(char *str, char *to_find)
{
    int i;
    int j;

    i = 0;
    while (str[i] != '\0')
    {
        j = 0;
        while (to_find[j] == str[i + j])
        {
            if (to_find[j + 1] == '\0')
            {
                return (str + i);
            }
            j++;
        }
        i++;
    }
    return (0);
}

int ft_is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

void verifyString(const char* str)
{
    int i = 0;
    while (str[i] != '\0') {
        printf("%c", str[i]);
        i++;
    }
    printf("\n");

    if (str[i] == '\0') {
        printf("La chaîne se termine correctement avec le caractère nul '\\0' a l'index %d.\n", i);
    } else {
        printf("La chaîne ne se termine pas avec le caractère nul '\\0'.\n");
    }
}
