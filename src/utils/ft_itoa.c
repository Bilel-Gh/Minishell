/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 17:03:50 by ncharii           #+#    #+#             */
/*   Updated: 2023/07/01 16:37:30 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "../../includes/minishell.h"

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
/*
   int main()
   {
   int n = -1234;

   printf("%s", ft_itoa(n));
   return (0);
   }
   */
