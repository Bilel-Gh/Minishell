/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 04:16:16 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/09 15:25:17 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
    str[i] = 0;
    return (str);
}

// fonction qui copie n octets de src dans dst
void	*ft_memcpy(void *dst, const void *src, size_t n)
{
    size_t i;

    if (!dst && !src)
        return (0);
    i = 0;
    while (i < n)
    {
        ((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
        i++;
    }
    return (dst);
}

// fonction qui duplique une chaine de caracteres
char	*ft_strndup(const char *s1, size_t n)
{
    char *s2;

    if (!(s2 = (char *)malloc(n + 1)))
        return (0);
    ft_memcpy(s2, s1, n);
    s2[n] = 0;
    return (s2);
}

char	*ft_strcpy(char *dest, char *src)
{
    int i;

    i = 0;
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return (dest);
}