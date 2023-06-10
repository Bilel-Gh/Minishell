/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 05:04:11 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/10 21:27:39 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>

static int	nbr_string(char *s, char c)
{
	int	nb_str;
	int	i;
	int	ok;

	ok = 0;
	i = 0;
	nb_str = 1;
	while (s[i])
	{
		if (s[i] != c)
		{
			ok++;
		}
		if (((s[i] == c) || (s[i + 1] == '\0')) && ok > 0)
		{
			nb_str++;
			ok = 0;
		}
		i++;
	}
	return (nb_str);
}

static void	reset(int *a, int *b)
{
	*a = 0;
	*b = 0;
}

static int	malloc_str(char *s, char **str, char c, int i)
{
	int	nb_str;
	int	nb_word;
	int	ok;

	nb_str = 0;
	nb_word = 0;
	ok = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			ok++;
			nb_word++;
		}
		if (((s[i] == c) || (s[i + 1] == '\0')) && ok > 0)
		{
			str[nb_str] = malloc(sizeof(char) * nb_word + 1);
			if (!str[nb_str])
				return (1);
			nb_str++;
			reset(&nb_word, &ok);
		}
		i++;
	}
	return (0);
}

static void	complete(char *s, char **str, char c, int i)
{
	int	nb_str;
	int	nb_word;
	int	ok;

	nb_str = 0;
	nb_word = 0;
	ok = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			ok++;
			str[nb_str][nb_word] = s[i];
			nb_word++;
		}
		if (((s[i] == c) || (s[i + 1] == '\0')) && ok > 0)
		{
			str[nb_str][nb_word] = '\0';
			nb_str++;
			nb_word = 0;
			ok = 0;
		}
		i++;
	}
}

char	**ft_split(char *s, char c)
{
	char	**str;
	int		nb_str;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	nb_str = nbr_string(s, c);
	str = malloc(sizeof(char *) * nb_str);
	if (!str)
		return (NULL);
	str[nb_str - 1] = NULL;
	nb_str = malloc_str(s, str, c, i);
	if (nb_str == 1)
		return (NULL);
	complete(s, str, c, i);
	return (str);
}
