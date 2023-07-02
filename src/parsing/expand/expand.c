/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 02:10:02 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/13 23:45:04 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*traslate_expand(char *arg_ct, char *ext_exp, int size_ext, char **env)
{
	char	*get_expande;
	char	*new_args;

	get_expande = give_env_expand(ext_exp, size_ext, env);
	if (get_expande)
		printf("\n my extratc = %s\n", get_expande);
	else
		printf("\n my extratc no\n");
	new_args = join_and_rp_args(arg_ct, get_expande, size_ext);
    // SEGFAULT VIEN DE LA FONCTION JOIN_AND_RP_ARGS JE CROIS
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
	expend_recherche = get_expende_detect(size_of_expende, &args[i]);
	printf("expend_recherche == %s\n \n", expend_recherche);
	new_args = traslate_expand(args, expend_recherche, size_of_expende, env);
	return (new_args);
}

char	*importe_expande(char *args, char **env)
{
	int	i;
    char *tmp;

	i = 0;
    tmp = NULL;
	while (args[i])
	{
		if (args[i] == '$')
        {
            tmp = ft_strdup(args);
            free(args);
            args = replace_expande(tmp, i, env);
        }
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

void	expande(int *type_args, int nb_args, t_global_parsing **g_pars, char **env)
{
	int	i;
     char *suite;
     char *exit_code;
     char *new_args;
    //int len_expande;

	i = 0;
    suite = NULL;
    new_args = NULL;
	while (i < nb_args)
	{
		if (type_args[i] == ALPHANUM || type_args[i] == QUOTE_D)
		{
			if (have_expande((*g_pars)->args[i]))
			{
//                tmp = ft_strdup((*g_pars)->args[i]);
//                (void)tmp;
                printf("\n \n PRESANCE EXPANDE\n");
                // ou que le deuxieme char est un numero
                if (ft_is_digit((*g_pars)->args[i][1]))
                {
                    new_args = ft_strdup((*g_pars)->args[i] + 2);
                    free((*g_pars)->args[i]);
                    (*g_pars)->args[i] = ft_strdup(new_args);
                    free(new_args);
                }
                else if (ft_strncmp((*g_pars)->args[i], "$?", 2) == 0)
                {
                    exit_code = ft_itoa(g_code_exit);
                    if (ft_strlen((*g_pars)->args[i]) > 2)
                    {
                        suite = ft_strdup((*g_pars)->args[i] + 2);
                        if (!suite)
                            return ;
                        new_args = ft_strcat(exit_code, suite);
                        free((*g_pars)->args[i]);
                        (*g_pars)->args[i] = ft_strdup(new_args);
                        free(suite);
                    }
                    else
                    {
                        free((*g_pars)->args[i]);
                        (*g_pars)->args[i] = ft_strdup(exit_code);
                    }
                    free(exit_code);
                }
                else
                {
                    (*g_pars)->args[i] = importe_expande((*g_pars)->args[i], env);
                }
			}
		}
		i++;
	}
}
