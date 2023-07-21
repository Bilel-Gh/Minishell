/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 12:54:47 by ncharii           #+#    #+#             */
/*   Updated: 2023/06/10 21:27:33 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>

int	ft_strlen(char *str)
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

void	ft_complete(char const *s, char *str, int *i)
{
	int	x;

	x = 0;
	if (!s)
		return ;
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
