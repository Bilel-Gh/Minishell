/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 02:10:02 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/19 00:51:14 by bghandri         ###   ########.fr       */
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
	new_args = NULL;
	get_expande = NULL;
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
		new_args = join_and_rp_args(arg_ct, NULL, size_ext);
		printf ("nil ? = %s", new_args);
		return (new_args);
	}

}

char	*replace_expande(char *args, int i, char **env, int *info)
{
	char	*expend_recherche;
	int		size_of_expende;
	char	*new_args;

	*info = 0;
	new_args = NULL;
	size_of_expende = ft_size_of_expende(&args[i]);
	if (size_of_expende < 2)
		return (*info = 1, args);
	printf("size = %d\n \n ", size_of_expende);
	expend_recherche = get_expende_detect(size_of_expende , &args[i]);
	printf("expend_recherche == %s\n \n", expend_recherche);
	new_args = traslate_expand(args, expend_recherche, size_of_expende, env);
	return (new_args);
}

char	*importe_expande(char *args, char **env)
{
	int	i;
	char *tmp;
	char *new_args;
	int info;

	i = 0;
	info = 0;
	tmp = NULL;
	new_args = NULL;
	while (args[i])
	{
		info = 0;
		printf("in importe_expande args[%d] = %c \n", i ,args[i]);
		if (args[i] == '$' && back_slach(args, i))
		{
			printf("args ________ %s\n", args);
			tmp = ft_strdup(args);
			new_args = replace_expande(tmp, i, env, &info);
			printf(" replace_expande ______ %s\n", new_args);
			free(args);
			if (new_args == NULL)
				return (free(tmp), new_args);
			args = new_args;
			if (info != 1)
				i = 0;
			else
				i++;
		}
		else
			i++;
		if (new_args == NULL)
			new_args = args;
	}
	return (new_args);
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

/*int delete_if_no_expand(int *nb_args, t_global_parsing **g_pars, int i) {
  if (ft_strncmp((*g_pars)->args[i], "NO EXPAND", 9) == 0)
  {
  remove_db_tab((*g_pars)->args, i);
  (*nb_args)--;
  if (i > 0);
  }
  */
int ft_nb_args_out_null(char **old_arg, int size_db_arr)
{
	int i;
	int new_size;

	i = 0;
	new_size = 0;
	while (i < size_db_arr)
	{
		if (old_arg[i] == 0 || !old_arg[i][0])
			i++;
		else if ( old_arg[i][0] == ' ' && old_arg[i + 1] == NULL && (i + 2 < size_db_arr) && old_arg[i + 2][0] == ' ')
			i = i + 2;
		else
		{
			i++;
			new_size++;
		}
	}
	return (new_size);
}

char **ft_clean_null_db_array(char **old_arg, int *size_db_arr)
{
	int nb_new_args;
	char **new_args;
	int i;
	int	j;

	i = 0;
	j = 0;
	nb_new_args = ft_nb_args_out_null(old_arg, *size_db_arr);
	new_args = malloc(sizeof(char *) * (nb_new_args + 1));
	new_args[nb_new_args] = NULL;
	printf("size old db_arr = %d\n", *size_db_arr);
	printf("size out null db_ar = %d\n", nb_new_args);
	while (i < *size_db_arr)
	{
		if (old_arg[i] == 0 || !old_arg[i][0])
			free(old_arg[i]);
		else if ( old_arg[i][0] == ' ' && old_arg[i + 1] == NULL && (i + 2 < *size_db_arr) && old_arg[i + 2][0] == ' ')
			free(old_arg[i]);
		else if (old_arg[i] != 0)
		{
			printf(" clean array null ======= [%s]*****\n", old_arg[i]);
			new_args[j] = ft_strdup(old_arg[i]);
			free(old_arg[i]);
			j++;
		}
		i++;
	}
	if (old_arg[i])
		free(old_arg[i]);
	free(old_arg);
	return (*size_db_arr = nb_new_args, new_args);
}

bool solo_dolard(char *str)
{
	if (!str)
		return (false);
	if (str[0] == '$' && str[1] == 0)
		return (true);
	return (false);
}

bool is_limiteur(int *type_args, int i)
{
	if ((i - 1) >= 0 && type_args[i - 1] == RED_D_IN)
		return (true);
	if ((i - 2) >= 0 && type_args[i - 2] == RED_D_IN)
		return (true);
	return (false);
}
char  *exit_code_expande(char *args, int exit)
{
	char *exit_code;
	char *new_args;
	char *suite;

	exit_code = ft_itoa(exit);
	if (ft_strlen(args) > 2)
	{
		suite = ft_strdup(args + 2);
		if (!suite)
			return (NULL);
		new_args = ft_strcat(exit_code, suite);
		free(args);
		args = ft_strdup(new_args);
		free(suite);
	}
	else
	{
		free(args);
		args = ft_strdup(exit_code);
	}
	free(exit_code);
	return (args);
}

void	expande(int **type_args, int *nb_args, t_global_parsing **g_pars, char **env)
{
	int	i;
	char *new_args;
	int *new_type_args;
	//int len_expande;

	i = 0;
	new_args = NULL;
	new_type_args = NULL;
	while (i < *nb_args)
	{
		printf("^^^^^^^^^^^^^^^%d\n",(*type_args)[i]);
		i++;
	}
	i = 0;
	while (i < *nb_args)
	{
		printf("######### (*g_pars)->args[i] = %s \n", (*g_pars)->args[i]);
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
				else if (solo_dolard((*g_pars)->args[i]))
				{
					if (i + 1 < *nb_args && ((*type_args)[i + 1] == ALPHANUM
					|| (*type_args)[i + 1] == QUOTE_S
					|| (*type_args)[i + 1] == QUOTE_D))
					{
						free((*g_pars)->args[i]);
						(*g_pars)->args[i] = NULL;
					}
					i++;
					continue;
				}
				else if (ft_strncmp((*g_pars)->args[i], "$?", 2) == 0)
					(*g_pars)->args[i] = exit_code_expande((*g_pars)->args[i], g_code_exit);
				else
				{
					if (!is_limiteur(*type_args, i))
					{
						(*g_pars)->args[i] = importe_expande((*g_pars)->args[i], env);
					}
					// on supprimer l'argument du tableau si l'expand n'est pas trouver
					//					i = delete_if_no_expand(nb_args, g_pars, i);
				}
			}
		}
		printf("&&&&&&&& %s &&&&&&\n",(*g_pars)->args[i]);
		i++;
	}
	(*g_pars)->args = ft_clean_null_db_array((*g_pars)->args, nb_args);
	new_type_args = ft_get_info_args((*g_pars)->args, nb_args);
	printf("++++++++++++ nb_args = %d", *nb_args);
	free(*type_args);
	*type_args = new_type_args;
}

