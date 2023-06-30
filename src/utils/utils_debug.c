/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_gnl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 17:37:32 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/26 17:37:35 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
