/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recipe_for_exec4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 23:18:11 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 23:20:31 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	count_spaces(char *str, int len)
{
	int	space_count;
	int	i;

	space_count = 0;
	i = 0;
	while (i < len)
	{
		if (str[i] == ' ')
			space_count++;
		i++;
	}
	return (space_count);
}

char	**allocate_result_array(int space_count)
{
	char	**result;

	result = (char **)malloc((space_count + 1) * sizeof(char *));
	b_zero_for_cmd_join(result, space_count, 0);
	return (result);
}

void	ft_do_split_custom(char *str, t_split *s)
{
	ft_is_in_quote_split(str, s);
	if (str[s->i] == ' ' && !s->in_quotes)
	{
		s->len_str_to_add = s->i - s->start;
		if (s->len_str_to_add > 0)
		{
			s->result[s->count] = (char *)malloc((s->len_str_to_add + 1)
					* sizeof(char));
			ft_strncpy(s->result[s->count], str + s->start, s->len_str_to_add);
			s->result[s->count][s->len_str_to_add] = '\0';
			(s->count)++;
		}
		s->start = s->i + 1;
	}
}

void	ft_is_in_quote_split(const char *str, t_split *s)
{
	if (str[s->i] == '"' || str[s->i] == '\'')
	{
		if (s->in_quotes && str[s->i] == s->type_inquote)
		{
			s->in_quotes = 0;
			s->type_inquote = '\0';
		}
		else if (!s->in_quotes)
		{
			s->in_quotes = 1;
			s->type_inquote = str[s->i];
		}
	}
}

int	ft_init_result(char *str, t_split *s)
{
	s->result = NULL;
	if (str == NULL || ft_strlen(str) == 0)
	{
		s->result = (char **)malloc(sizeof(char *));
		s->result[0] = NULL;
		return (0);
	}
	return (1);
}
