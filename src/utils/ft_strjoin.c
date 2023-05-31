/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 12:54:47 by ncharii           #+#    #+#             */
/*   Updated: 2022/12/24 18:44:50 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include "../../includes/minishell.h"

int	ft_strlen(const char *str)
{
	int	nb;

	nb = 0;
	if (!str)
		return (0);
	while (str[nb])
	{
		nb++;
	}
	return (nb);
}

static void	ft_complete( char const *s, char *str, int *i)
{
	int	x;

	x = 0;
	if (!s)
		return;
	while (s[x])
	{
		str[*i] = s[x];
		x++;
		*i = *i + 1;
	}
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		size_s1;
	int		size_s2;
	int		i;

	//if (!s1 || !s2)
	//	return (NULL);
	i = 0;
	size_s1 = ft_strlen(s1);
	size_s2 = ft_strlen(s2);
	str = malloc(sizeof(char) * (size_s1 + size_s2 + 1));
	if (!str)
		return (NULL);
	ft_complete(s1, str, &i);
	ft_complete(s2, str, &i);
	if (s1)
		free(s1);
	str[i] = '\0';
	return (str);
}
/*
int main()
{
	char s1[] = "nassim";
	char s2[] = "charii";

	printf("%s",ft_strjoin(s1, s2));
	return (0);
}*/
