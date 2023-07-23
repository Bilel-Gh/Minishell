/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 01:44:34 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 22:10:46 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*str;

	str = (char *)malloc(sizeof(*s) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (i >= start && j < len)
		{
			str[j] = s[i];
			j++;
		}
		i++;
	}
	str[j] = 0;
	return (str);
}

// permet d'ajouter un caractère à la fin d'une chaine de caractère
char	*append_value(char *str, char c)
{
	char	*temp;

	temp = (char *)malloc((ft_strlen(str) + 2) * sizeof(char));
	ft_strcpy(temp, str);
	temp[ft_strlen(str)] = c;
	temp[ft_strlen(str) + 1] = '\0';
	free(str);
	return (temp);
}
