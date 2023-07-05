/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 02:10:02 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/05 12:28:13 by ncharii          ###   ########.fr       */
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
    {
        printf("\n my extratc yes\n");
        new_args = join_and_rp_args(arg_ct, get_expande, size_ext);
        return (new_args);
    }
	else
    {
        printf("\n my extratc no\n");
        new_args = join_and_rp_args(arg_ct, "NO EXPAND", size_ext);
        return (new_args);
    }
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

void remove_db_tab(char **str, int index_to_remove) {
    int i;
    int size;

    size = 0;
    // Afficher le tableau résultant
    i = 0;
//    while (str[i] != NULL) {
//        printf("\033[0;32m str_BEFORE[%d] = %s\n\033[0m", i, str[i]);
//        i++;
//    }
//    printf("\n");
    while (str[size] != NULL) {
        size++;
    }
    if (index_to_remove < 0 || index_to_remove >= size) {
        printf("Index à supprimer invalide.\n");
        return;
    }
    free(str[index_to_remove]);
    i = index_to_remove;
    while (i < size - 1) {
        str[i] = str[i + 1];
        i++;
    }
    str[size - 1] = NULL;

//    // Afficher le tableau résultant
//    i = 0;
//    while (str[i] != NULL) {
//        printf("\033[0;32m str_AFTER[%d] = %s\n\033[0m", i, str[i]);
//        i++;
//    }
//    printf("\n");
}

int delete_if_no_expand(int *nb_args, t_global_parsing **g_pars, int i) {
    if (ft_strncmp((*g_pars)->args[i], "NO EXPAND", 9) == 0)
    {
        remove_db_tab((*g_pars)->args, i);
        (*nb_args)--;
        if (i > 0)
            i--;
        if (ft_strcmp((*g_pars)->args[i + 1], " ") == 0 || (*g_pars)->args[i + 1] == NULL)
        {
            remove_db_tab((*g_pars)->args, i);
            (*nb_args)--;
            if (i > 0)
                i--;
        }
    }
    return i;
}


void	expande(int **type_args, int *nb_args, t_global_parsing **g_pars, char **env)
{
	int	i;
     char *suite;
     char *exit_code;
     char *new_args;
    int *new_type_args;
    //int len_expande;

	i = 0;
    suite = NULL;
    new_args = NULL;
    new_type_args = NULL;
	while (i < *nb_args)
	{
		if ((*type_args)[i] == ALPHANUM || (*type_args)[i] == QUOTE_D)
		{
			if (have_expande((*g_pars)->args[i]))
			{
                printf("\n \n PRESANCE EXPANDE\n");
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
                    // on supprimer l'argument du tableau si l'expand n'est pas trouver
                    i = delete_if_no_expand(nb_args, g_pars, i);
                    new_type_args = ft_get_info_args((*g_pars)->args, nb_args);
                    free(*type_args);
                    *type_args = new_type_args;
                }
			}
		}
		i++;
	}
}
