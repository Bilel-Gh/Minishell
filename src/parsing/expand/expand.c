/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 02:10:02 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/13 00:08:39 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*traslate_expand(char *args_ct, char *extract_expend, int size_extract)
{
	char	*get_expande;
	char	*new_args;

	get_expande = give_env_expand(extract_expend, size_extract);
	if (get_expande)
		printf("\n my extratc = %s\n", get_expande);
	else
		printf("\n my extratc no\n");
	new_args = join_and_rp_args(args_ct, get_expande, size_extract);
	return (new_args);
}

char	*replace_expande(char *args, int i)
{
	char	*expend_recherche;
	int		size_of_expende;
	char	*new_args;

	size_of_expende = ft_size_of_expende(&args[i]);
	if (size_of_expende < 2)
		return (args);
	printf("size = %d\n \n ", size_of_expende);
	expend_recherche = get_expende_detect(size_of_expende, &args[i]);
	printf("expend_recherche == %s\n \n", expend_recherche);
	new_args = traslate_expand(args, expend_recherche, size_of_expende);
	return (new_args);
}

char	*importe_expande(char *args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (args[i] == '$')
			args = replace_expande(args, i);
		i++;
	}
	return (args);
}

bool	have_expande(char *args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (args[i] == '$')
			return (true);
		i++;
	}
	return (false);
}

void	expande(int *type_args, int nb_args, char **args)
{
	int	i;

	i = 0;
	while (i < nb_args)
	{
		if (type_args[i] == ALPHANUM || type_args[i] == QUOTE_D)
		{
			if (have_expande(args[i]))
			{
				printf("\n \n PRESANCE EXPANDE\n");
				args[i] = importe_expande(args[i]);
			}
		}
		i++;
	}
}
