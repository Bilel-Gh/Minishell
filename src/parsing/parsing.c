/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 16:18:20 by ncharii           #+#    #+#             */
/*   Updated: 2023/07/08 21:59:08 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	fist_error(int *type_args)
{
	if ((type_args[0] != 0 && type_args[0] != 1) || (type_args[0] == 1
			&& type_args[1] != 0))
		return (true);
	return (false);
}

bool	error_grammaticale(int *type_args, int nb_args)
{
	int	i;

	i = 0;
	if (!type_args)
		return (false);
	printf("*********check grammaticale error ???***************\n");
	if (nb_args == 1 && type_args[i] != 0)
		return (true);
	while (i < nb_args - 1)
	{
		if (type_args[i] == 5)
			i++;
		if (i == 0 && fist_error(type_args))
			return (true);
		if (i != 0)
		{
			if (type_args[i] == 4 && type_args[i + 1] != 0 && type_args[i
					+ 1] != 1)
				return (true);
			if (type_args[i] == 1 && type_args[i + 1] != 0)
				return (true);
		}
		i++;
	}
	return (false);
}

int	count_nb_space(int *type_args, int nb_args)
{
	int	i;
	int	nb_space;

	i = 0;
	nb_space = 0;
	printf("nb_args = %d",nb_args);
	if (type_args == NULL)
		return (nb_space);
	while (i < nb_args - 1)
	{
		if (type_args[i] == ESPACE)
			nb_space++;
		i++;
	}
	return (nb_space);
}

void	b_zero_last_args(char **last_args, int nb_new_args, int i)
{
	last_args[nb_new_args] = 0;
	while (i < nb_new_args)
	{
		last_args[i] = 0;
		i++;
	}
}

char	**kick_args_space(char **new_args, int *type_args, int *nb_args)
{
	char	**last_args;
	int		nb_new_args;
	int		i;
	int		j;

	i = 0;
	j = 0;
	nb_new_args = *nb_args - count_nb_space(type_args, *nb_args);
	if (nb_new_args == 0)
		return (NULL);
	last_args = malloc(sizeof(char *) * (nb_new_args + 1));
	if (!last_args)
		return (NULL);
	b_zero_last_args(last_args, nb_new_args, i);
	while (i < *nb_args)
	{
		if (type_args[i] == ESPACE)
			i++;
		if (i == *nb_args)
			return (*nb_args = nb_new_args, last_args);
		last_args[j] = ft_strjoin(last_args[j], new_args[i]);
		i++;
		j++;
	}
	return (*nb_args = nb_new_args, last_args);
}

char* remove_double_quotes(char* str) {
    int len;
    int count_quotes;
    int i;
    int j;

    i = 0;
    j = 0;
    count_quotes = 0;
    len = ft_strlen(str);
    while (i < len) {
        if (str[i] == '"' || str[i] == '\'') {
            count_quotes++;
        }
        i++;
    }
    if (count_quotes == len)
        return ft_strdup(str);
    char* result = (char*)malloc((len - count_quotes + 1) * sizeof(char));
    i = 0;
    while (i < len) {
        if (str[i] != '"' && str[i] != '\'') {
            result[j] = str[i];
            j++;
        }
        i++;
    }
    result[j] = '\0';  // Ajouter la terminaison de la chaîne
    return result;
}

char *remove_backslash(char *str)
{
    char *result;
    int inquote;
    int max;
    int i;
    int j;

    inquote = 0;
    i = 0;
    j = 0;
    max = 2 * ft_strlen(str) + 1;

    result = malloc(sizeof(char) * max);
    ft_bzero(result, max);
    if (result == NULL)
        return NULL;
    while (str[i])
    {
        if (str[i] == '\'')
            inquote = !inquote;
        if (str[i] == '\\' && inquote)
            i++;
        else if (str[i] == '\\' && !inquote)
        {
            if (str[i + 1])
            {
                result[j++] = '\'';
                result[j++] = str[i + 1];
                result[j++] = '\'';
                i += 2;
            }
            else
                result[j++] = str[i++];
            continue;
        }
        result[j++] = str[i++];
    }
    result[j] = '\0';
    return result;
}

int is_backslash(char *str)
{
    int get_index_bs;

    get_index_bs = 0;
    if (ft_strchr(str, '\\'))
    {
        while (str[get_index_bs] != '\\')
            get_index_bs++;
        if (str[get_index_bs + 1] != '\'' && str[get_index_bs - 1] != '\'')
            return (1);
    }
    return (0);
}


void ft_gestion_backslash(int **type_args, int *nb_args, t_global_parsing **g_pars)
{
    char *cleaned_arg;
    int i;

    i = 0;
    while (i < *nb_args)
    {
        //si l'argument a un backslash
        while (is_backslash((*g_pars)->args[i]) && (*g_pars)->args[i][1] != '\0')
        {
            cleaned_arg = remove_backslash((*g_pars)->args[i]);
            printf("\033[1;34m ----------------- CLEAN_ARG = %s \033[0m\n", cleaned_arg);
            free((*g_pars)->args[i]);
            (*g_pars)->args[i] = cleaned_arg;
        }

        i++;
    }
    // TODO recree le type_args
    (void )type_args;
}

char	**ft_parsing(int *nb_args, t_global_parsing **g_pars, char ***env)
{
	char	**new_args;
//	char	**no_quote_cmd;
	int		*new_type_args;
	int		*type_args;

	type_args = ft_get_info_args((*g_pars)->args, nb_args);
	if (search_error_args(type_args, nb_args, (*g_pars)->args))
		return ((*g_pars)->args);
    printf("\033[1;34m G_CODE_EXIT = %d \033[0m\n", g_code_exit);
    ft_gestion_backslash(&type_args, nb_args, g_pars);
	expande(&type_args, nb_args, g_pars, *env);
    g_code_exit = SUCCESS;
    printf("\033[1;31m APRES EXPAND \033[0m\n");
    printf("nb_args = %d\n", *nb_args);
    int w = 0;
    while ((*g_pars)->args[w])
    {
        printf("\033[1;31m             APRES EXPAND args[%d] = %s \033[0m\n", w, (*g_pars)->args[w]);
        w++;
    }
    if (*nb_args == 0)
    {
        g_code_exit = SUCCESS;
        return ((*g_pars)->args);
    }
// 	if (ft_strcmp(no_quote_args[0], "export") == 0)
//     {
          //no_quote_args = ft_db_array_dup(args);
//     }
    // printf tout le tableau (*g_pars)->args
	new_args = join_inter_space((*g_pars)->args, type_args, nb_args);
	new_type_args = ft_get_info_args(new_args, nb_args);
	free_db_array((*g_pars)->args);
    (*g_pars)->args = kick_args_space(new_args, new_type_args, nb_args);
	free_db_array(new_args);
	free(type_args);
	free(new_type_args);
	new_type_args = ft_get_info_args((*g_pars)->args, nb_args);
	if (error_grammaticale(new_type_args, *nb_args))
		return ((*g_pars)->args);
	printf("^^^^^^^^^^^ no error grammaticale ^^^^^^^^^^^^^^^^\n");
    int z = 0;
    while ((*g_pars)->args[z])
    {
        printf("\033[0;35m ARGUMENT PARSING : %s\033[0m\n", (*g_pars)->args[z]);
        z++;
    }
    char *first_arg = remove_double_quotes((*g_pars)->args[0]);
    if (ft_strcmp(first_arg, (*g_pars)->args[0]) != 0)
    {
        free((*g_pars)->args[0]);
        (*g_pars)->args[0] = first_arg;
    }
    else
        free(first_arg);
    z = 0;
    while ((*g_pars)->args[z])
    {
        printf("\033[0;35m ARGUMENT PARSING sans quotes: %s\033[0m\n", (*g_pars)->args[z]);
        z++;
    }
	free(new_type_args);
	return ((*g_pars)->args);
}
