/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 05:04:11 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/02 19:05:04 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

#include <stdlib.h>

int		has_char(char c, char *str)
{
    while (*str)
    {
        if (*str++ == c)
            return (1);
    }
    return (0);
}

int		strs_l(char *str, char *charset)
{
    int	part;
    int count;

    part = 1;
    count = 0;
    while (*str)
    {
        if (!has_char(*str, charset) && part)
        {
            count++;
            part = 0;
        }
        else if (has_char(*str, charset))
            part = 1;
        str++;
    }
    return (count);
}

char	*ft_strdup_split(char *src, char *charset)
{
    char	*dest;
    char	*buffer;
    int		length;

    length = 0;
    buffer = src;
    while (*buffer && !has_char(*buffer++, charset))
        length++;
    dest = (char*)malloc(sizeof(*src) * length);
    buffer = dest;
    while (*src && length-- > 0)
        *buffer++ = *src++;
    *buffer = '\0';
    return (dest);
}

char	**ft_split(char *str, char *charset)
{
    char	**strs;
    char	**tmp;
    int		part;

    strs = (char**)malloc(strs_l(str, charset) * sizeof(*strs) + 1);
    tmp = strs;
    part = 1;
    while (*str)
    {
        if (!has_char(*str, charset) && part)
        {
            part = 0;
            *tmp = ft_strdup_split(str, charset);
            tmp++;
        }
        else if (has_char(*str, charset))
            part = 1;
        str++;
    }
    *tmp = 0;
    return (strs);
}
