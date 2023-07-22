/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 02:13:01 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/22 02:29:47 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char		*ft_malloc_expand(const char *args_con, const char *expande,
				int s_extract, char *new_args);

int			ft_init_s_ex_join(char *args_con, const char *expande,
				int s_extract, t_expand_join *s_ex_join);

void		ft_fill_new_args(const char *args_con, const int *info,
				t_expand_join *s_ex_join);

int			ft_go_next_arg(const char *args_con, t_expand_join *s_ex_join);

int	ft_size_of_expende(char *expande)
{
	int	size;

	size = 1;
	while (1)
	{
		printf("+1\n");
		if ((expande[size] == '@' && size > 1) || (expande[size] == '#'
				&& size > 1) || (expande[size] == '[' && size > 1)
			|| (expande[size] == ']' && size > 1) || (expande[size] == '/'
				&& size > 1) || (expande[size] == '=') || (expande[size] == ':'
				&& size > 1) || (expande[size] == '-' && size > 1)
			|| (expande[size] == '%' && size > 1) || (expande[size] == '$'
				&& size > 1) || (expande[size] == 92 && size > 1)
			|| (expande[size] == ' ' && size > 1) || (expande[size] == 0)
			|| (expande[size] == '.') || (expande[size] == 39)
			|| (expande[size] == 34))
		{
			size--;
			break ;
		}
		size++;
	}
	return (size);
}

char	*get_expende_detect(int size_of_expende, char *expande)
{
	char	*txt_expande;
	int		i;

	i = 0;
	txt_expande = malloc(sizeof(char) * (size_of_expende + 1));
	while (i < size_of_expende && expande)
	{
		txt_expande[i] = expande[i + 1];
		i++;
	}
	txt_expande[i] = 0;
	return (txt_expande);
}

char	*is_sp_expand(char *expand_search)
{
	char	*sp_expand;

	if (!expand_search)
		return (NULL);
	if (expand_search[0] == '@')
	{
		sp_expand = &expand_search[1];
		return (sp_expand);
	}
	return (NULL);
}

char	*give_env_expand(char *expande_search, int size, char **env)
{
	int		i;
	char	*sp_expand;
	char	*for_search;

	i = 0;
	sp_expand = NULL;
	sp_expand = is_sp_expand(expande_search);
	for_search = ft_strdup(expande_search);
	for_search = ft_strjoin(for_search, "=");
	if (sp_expand)
		return (sp_expand);
	while (env[i])
	{
		if (!strncmp(for_search, env[i], size + 1))
			break ;
		i++;
	}
	free(for_search);
	free(expande_search);
	if (env[i] == NULL)
		return (NULL);
	return (&env[i][size + 1]);
}

int	join_expand(char *expande, int *j, int s_extract, char *new_args)
{
	int	index;

	(void)s_extract;
	index = 0;
	if (expande)
	{
		printf("expand = ________ %s\n", expande);
		while (expande[index])
		{
			new_args[*j] = expande[index];
			index++;
			*j = *j + 1;
		}
	}
	return (index);
}

bool	back_slach(char *args, int i)
{
	int	nb_back_s;

	nb_back_s = 0;
	if (i == 0)
		return (true);
	i = i - 1;
	while (args[i] == 92)
	{
		i--;
		nb_back_s++;
		if (i < 0)
			break ;
	}
	if (nb_back_s % 2 == 0)
		return (true);
	else
		return (false);
}

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
