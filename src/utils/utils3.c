/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 18:28:35 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/29 18:28:43 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strcat(char *s1, const char *s2)
{
    size_t dst_len;
    size_t i;

    dst_len = ft_strlen(s1);
    i = 0;
    while (s2[i] != '\0')
    {
        s1[dst_len + i] = s2[i];
        i++;
    }
    s1[dst_len + i] = '\0';
    return (s1);
}

void	ft_bzero(void *s, size_t n)
{
    unsigned char	*ptr;

    ptr = (unsigned char*)s;
    while (n--)
        *ptr++ = '\0';
}

char	*ft_strnew(size_t size)
{
    char	*str;

    if (!(str = (char*)malloc(sizeof(*str) * (size + 1))))
        return (NULL);
    ft_bzero(str, size + 1);
    return (str);
}

static int	count_size(int n)
{
    int i;

    i = 0;
    if (n < 0)
        n *= -1;
    while (n != 0)
    {
        n /= 10;
        i++;
    }
    return (i);
}

char		*ft_itoa(int num)
{
    char		*dst;
    int			count;
    int			i;
    long int	n;

    n = num;
    count = count_size(n);
    i = 0;
    if (n < 0 || count == 0)
        count++;
    if (!(dst = ft_strnew(count)))
        return (NULL);
    if (n < 0)
    {
        n *= -1;
        dst[0] = '-';
        i++;
    }
    while (count > i)
    {
        count--;
        dst[count] = (n % 10) + '0';
        n /= 10;
    }
    return (dst);
}
