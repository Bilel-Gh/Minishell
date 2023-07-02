/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 02:10:02 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/02 14:22:55 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char *expand_exit_code(char *expande)
{
	char *nb_exit_code;
	char *expand;
	int i;
	int j;

	i = 0;
	j = 0;
	nb_exit_code = ft_itoa(g_code_exit);
	expand = malloc(ft_strlen(expande) + ft_strlen(nb_exit_code));
	while (nb_exit_code[i])
	{
		expand[j] = nb_exit_code[i];
		j++;
		i++;
	}
	i = 1;
	while(expande[i])
	{
		expand[j] = expande[i];
		j++;
		i++;
	}
	expand[j] = 0;
	return (expand);
}

char	*traslate_expand(char *arg_ct, char *ext_exp, int size_ext, char **env)
{
	char	*get_expande;
	char	*new_args;
	char	*exit_code;

	exit_code = NULL;
	if (ext_exp[0] == '?')
	{
		exit_code = expand_exit_code(ext_exp);
		get_expande = exit_code;
	}
	else
		get_expande = give_env_expand(ext_exp, size_ext, env);
	if (get_expande)
		printf("\n my extratc = %s\n", get_expande);
	else
		printf("\n my extratc no\n");
	new_args = join_and_rp_args(arg_ct, get_expande, size_ext);
	if (exit_code)
		free(exit_code);
	return (new_args);
}

char	*replace_expande(char *args, int i, char **env)
{
	char	*expend_recherche;
	int		size_of_expende;
	char	*new_args;

	size_of_expende = ft_size_of_expende(&args[i]);
	if (size_of_expende < 2)
		return (args);
	printf("size = %d\n \n ", size_of_expende);
	expend_recherche = get_expende_detect(size_of_expende - 1, &args[i]);
	printf("expend_recherche == %s\n \n", expend_recherche);
	new_args = traslate_expand(args, expend_recherche, size_of_expende, env);
	return (new_args);
}

char	*importe_expande(char *args, char **env)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (args[i] == '$')
			args = replace_expande(args, i, env);
		if (args == NULL)
			return (args);
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

void	expande(int *type_args, int nb_args, char **args, char **env)
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
				args[i] = importe_expande(args[i], env);
			}
		}
		i++;
	}
}
