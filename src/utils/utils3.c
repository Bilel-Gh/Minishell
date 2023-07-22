/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 18:28:35 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/22 22:47:02 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strcat(char *s1, const char *s2)
{
	size_t	dst_len;
	size_t	i;

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

	ptr = (unsigned char *)s;
	while (n--)
		*ptr++ = '\0';
}

static int	size_str(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		i++;
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static void	complete(char *str, unsigned int nb, int size)
{
	while (nb != 0)
	{
		str[size] = (nb % 10) + 48;
		nb = nb / 10;
		size = size - 1;
	}
}

char	*ft_itoa(int n)
{
	char			*str;
	int				size;
	unsigned int	nb;

	size = size_str(n);
	str = malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	if (n < 0)
	{
		str[0] = '-';
		nb = n * -1;
	}
	else
		nb = n;
	str[size] = '\0';
	if (nb == 0)
	{
		str[size -1] = '0';
		str[size] = '\0';
		return (str);
	}
	size = size - 1;
	complete(str, nb, size);
	return (str);
}
