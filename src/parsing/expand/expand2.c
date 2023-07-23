/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 02:13:01 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 23:08:00 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*join_and_rp_args(char *args_con, char *expande, int s_extract,
		int *info)
{
	t_expand_join	s_ex_join;

	if (ft_init_s_ex_join(args_con, expande, s_extract, &s_ex_join))
		return (s_ex_join.new_args);
	ft_fill_new_args(args_con, info, &s_ex_join);
	while (args_con[s_ex_join.i])
	{
		if (args_con[s_ex_join.i] == '$' && s_ex_join.expande_in == false
			&& back_slach(args_con, s_ex_join.i))
		{
			*info = s_ex_join.i + join_expand(expande, &s_ex_join.j,
					s_extract, s_ex_join.new_args);
			s_ex_join.i = s_ex_join.i + s_extract;
			if (args_con[s_ex_join.i] != '$')
				s_ex_join.i++;
			s_ex_join.expande_in = true;
		}
		if (ft_go_next_arg(args_con, &s_ex_join))
			break ;
	}
	s_ex_join.new_args[s_ex_join.j] = 0;
	free(args_con);
	return (s_ex_join.new_args);
}

int	ft_go_next_arg(const char *args_con, t_expand_join *s_ex_join)
{
	s_ex_join->new_args[s_ex_join->j] = args_con[s_ex_join->i];
	if (args_con[s_ex_join->i] == 0)
		return (1);
	s_ex_join->i++;
	s_ex_join->j++;
	return (0);
}

void	ft_fill_new_args(const char *args_con, const int *info,
		t_expand_join *s_ex_join)
{
	while (s_ex_join->i < *info)
	{
		s_ex_join->new_args[s_ex_join->j] = args_con[s_ex_join->i];
		if (args_con[s_ex_join->i] == 0)
			break ;
		s_ex_join->i++;
		s_ex_join->j++;
	}
}

int	ft_init_s_ex_join(char *args_con, const char *expande, int s_extract,
		t_expand_join *s_ex_join)
{
	s_ex_join->expande_in = false;
	s_ex_join->i = 0;
	s_ex_join->j = 0;
	s_ex_join->new_args = NULL;
	if ((ft_strlen(args_con) - (s_extract + 1) == 0) && (expande == NULL))
		return (1);
	s_ex_join->new_args = ft_malloc_expand(args_con, expande, s_extract,
			s_ex_join->new_args);
	return (0);
}

char	*ft_malloc_expand(const char *args_con, const char *expande,
		int s_extract, char *new_args)
{
	if (!expande)
		new_args = malloc((strlen(args_con) - s_extract) + 1);
	else
		new_args = malloc(strlen(args_con) + (strlen(expande) - s_extract) + 1);
	if (!new_args)
		exit(0);
	return (new_args);
}
