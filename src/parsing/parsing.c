/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 16:18:20 by ncharii           #+#    #+#             */
/*   Updated: 2023/06/01 20:46:14 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	error_grammaticale(int *type_args, int nb_args)
{
	int	i;

	i = 0;
	if(!type_args)
		return(true);
	printf("*********check grammaticale error ???***************\n");
	if (nb_args == 1)
	{
		if ((type_args[i] != 0))
			return (true);
	}
	while (i < nb_args - 1)
	{
		//printf("type_args %d i = %d\n",type_args[i], i);
		if (type_args[i] == 5)
			i++;
		if (i == 0)
		{
			if (type_args[i] != 0 && type_args[i] != 1)
				return (true);
			if (type_args[i] == 1 && type_args[i + 1] != 0)
				return (true);
		}
		if (i != 0)
		{
			//   printf("parsing 2 = %d nb = %d\n", type_args[i], i);
			if (type_args[i] == 0)
			{
				//  printf("i parsing.c = %d\n",i);
				if (type_args[i + 1] != 4 && type_args[i + 1] != 0
					&& type_args[i + 1] != 1)
					return (true);
			}
			//!!!!!!! ATTENTION ls | > outfile !!!!!!!!
			if (type_args[i] == 4 && type_args[i + 1] != 0
				&& type_args[i + 1] != 1)
				return (true);
			if (type_args[i] == 1 && type_args[i + 1] != 0)
				return (true);
		}
		i++;
	}
	//   printf("*********ft_parsing***************\n");
	return (false);
}

int	ft_size_of_expende(char *expande)
{
	int	size;

	size = 1;
	while (expande[size] != ' ' && expande[size] != 0 && expande[size] != '$'
		&& expande[size] != 34 && expande[size] != 39)
	{
		if ((expande[size] == '@' && size > 1)
			|| (expande[size] == '#' && size > 1)
			)
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
	txt_expande = malloc(sizeof(char) * size_of_expende);
	while (i < size_of_expende)
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

	if (expand_search[0] == '@')
	{
		sp_expand = &expand_search[1];
		return (sp_expand);
	}
	return (NULL);
}

char	*give_env_expand(char *expande_search, int size)
{
	int		i;
	char	*sp_expand;

	i = 0;
	sp_expand = NULL;
	sp_expand = is_sp_expand(expande_search);
	if (sp_expand)
		return (sp_expand);
	while (environ[i])
	{
		if (!strncmp(expande_search, environ[i], size - 1)) // ! a changer
			break ;
		i++;
	}
	free(expande_search);
	if (environ[i] == NULL)
		return (NULL);
	return (&environ[i][size]);
}

char	*join_and_rp_args(char *args_con, char *expande, int size_extract)
{
	char	*new_args;
	int		i;
	int		j;
	int		index;
	bool	expande_in;

	expande_in = false;
	index = 0;
	i = 0;
	j = 0;
	if (!expande)
		new_args = malloc(strlen(args_con) - size_extract);
	else
		new_args = malloc(strlen(args_con) + strlen(expande) - size_extract);
	if (!new_args)
		exit (0);// renplacer par la fonction free_all;
	while (args_con[i])
	{
		if (args_con[i] == '$' && expande_in == false)
		{
			if (expande)
			{
				while (expande[index])
				{
					new_args[j] = expande[index];
					index++;
					j++;
				}
			}
			i = i + size_extract;
		}
		new_args[j] = args_con[i];
		i++;
		j++;
	}
	new_args[j] = 0;
	printf("join = %s\n", new_args);
	free(args_con);
	return (new_args);
}

char	*traslate_expand(char *args_ct, char *extract_expend, int size_extract)
{
	char	*get_expande;
	char    *new_args;

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
	char    *new_args;
	size_of_expende = ft_size_of_expende(&args[i]);
	if (size_of_expende < 2)
		return (args);
	printf("size = %d\n \n ", size_of_expende);
	expend_recherche = get_expende_detect(size_of_expende, &args[i]);
	printf("expend_recherche == %s\n \n", expend_recherche);
	new_args = traslate_expand(args, expend_recherche, size_of_expende);
	return (new_args);
}

char    *importe_expande(char *args)
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

bool is_word(int type_args)
{
	if (type_args == ALPHANUM
		|| type_args == QUOTE_S || type_args == QUOTE_D)
		return (true);
	return (false);
}

int count_new_args(int *type_args, int nb_args)
{
	int i;
	int j;

	i = 0;
	j = 0;
while (i < nb_args)
	{
		printf("nb args = %d\n", nb_args);
		if (is_word(type_args[i]))
			{
				printf("nb_args new args = %d\n type_", nb_args);
				while (is_word(type_args[i]) && i < nb_args)
				{
					printf("i new args = %d\n", i);
					i++;
					if (i == nb_args)
						return (printf("FSFSFSFF\n"),j + 1);

				}
				j++;
				continue;
			}

		(i++, j++);
	}
	printf("j new args = %d\n", j);
	return (j);
}



int malloc_new_args(char **new_args, char **args, int *type_args, int nb_args)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < nb_args)
	{
		if (is_word(type_args[i]))
			{
				while (is_word(type_args[i]) && i < nb_args)
				{
					new_args[j] = ft_strjoin(new_args[j], args[i]);
					i++;
					if (i == nb_args)
						return (new_args[j + 1] = 0, 1);
				}
				j++;
				continue;
			}
		new_args[j] = ft_strjoin(new_args[j], args[i]);
		(i++, j++);
	}
	new_args[j] = 0;
	return (1);

}
//CHAR **
char *copy_sans_quote(char *args)// vas malloc et copier sans quote
{
	char *new_no_quote;
	int size_args;
	int size_no_quote;
	int i;
	int j;

	i = 1;
	j = 0;
	size_args = ft_strlen(args);
	size_no_quote = size_args - 2;
	new_no_quote = malloc(size_no_quote + 1);
	new_no_quote[size_no_quote] = 0;
	while (j < size_no_quote)
	{
		new_no_quote[j] = args[i];
		i++;
		j++;
	}
	return (new_no_quote);
}

char *copy_cont(char *str)
{
	char *copy;
	int size_str;
	int i;

	i = 0;
	size_str = ft_strlen(str);
	copy = malloc(size_str + 1);
	if (!copy)
		return (NULL);
	while (str[i])
	{
		copy[i] = str[i];
		i++;
	}
	copy[i] = 0;
	return (copy);
}

char **kick_quote(int *type_args, int nb_args, char **args)
{
	char **no_quote;
	int i;

	i = 0;
	no_quote = malloc (sizeof(char*) * (nb_args + 1));
	no_quote[nb_args] = 0;
	while (i < nb_args)
	{
		if (type_args[i] == QUOTE_D || type_args[i] == QUOTE_S)
			no_quote[i] = copy_sans_quote(args[i]);
		else
			no_quote[i] = copy_cont(args[i]);
		i++;
	}
	return (no_quote);

}
char **join_inter_space(char **args, int *type_args, int *nb_args)
{
	char ** new_args;
	int i;
	int nb_new_args;

	nb_new_args = count_new_args(type_args, *nb_args);
	//printf("nb space =  %d\n", nb_new_args(type_args, nb_args));
	printf("\n \n nb args_join %d\n", nb_new_args);
	//prinft("NB args _join = %d  \n",nb_new_args);

	if (nb_new_args == *nb_args)
		return(args) ;//return (args);
	i = 0;
	new_args = malloc( sizeof(char*) * (nb_new_args + 1));
	while (i <= nb_new_args)
	{
		new_args[i] = malloc(1);
		new_args[i] = NULL;
		i++;
	}
	i = 0;
	if (!new_args)
		return(printf("error"), NULL) ; //ft_free;
	if (!malloc_new_args(new_args, args, type_args, *nb_args))
		return(printf("error malloc_new_args"), NULL) ;//ft_free;
	printf("new args %d = %s", i, new_args[i]);
	while ( i < nb_new_args)
	{
		printf("new args %d = %s", i, new_args[i]);
		i++;
	}
	*nb_args = nb_new_args;
	return (new_args);
}


//void	kick_quote(char **args)
//char **join_inter_space(char **args, int *type_args, int nb_args)

//
/*

char    **good_argsnisation(int *type_args, int nb_args, char **args)
{
    int nb_new_args;
    int i;
    char **new_argsisation

    i = 0
    nb_new_args = 1;
    while (i < nb_args)
    {
        if(t_args[i] == SPACE)
            nb_new_args++;
        if(t_args[i] == ALPHANUM)
            nb_new_args = nb_new_args + nb_space(args[i]);
    }
    new_argsisation = (nb_new_args);
    i = 0;
    while (new_argsisation[i])
    {
        if (!new_argsisation[i])
            remplir_le_nouveau(new_argsisation, i);
        i++;
    }
    return (good_argsnisation);
}*/

int count_nb_space(int *type_args, int nb_args)
{
	int i;
	int nb_space;

	i = 0;
	nb_space = 0;
	while (i < nb_args)
	{
		if (type_args[i] == ESPACE)
			nb_space++;
		i++;
	}
	return (nb_space);
}

char **kick_args_space(char **new_args, int *type_args, int *nb_args)
{
	char **last_args;
	int nb_space;
	int nb_new_args;
	int i;
	int j;

	i = 0;
	j = 0;
	nb_space = count_nb_space(type_args, *nb_args);
	nb_new_args = *nb_args - nb_space;
	//printf ("$$$$$$$$$$$$$$$$$$$$$$$$ nb_space %d\n", nb_space);
	last_args = malloc( sizeof(char*) * (nb_new_args + 1));
	if (!last_args)
		return (NULL);// FT_ERROR_malloc_parsing
	last_args[nb_new_args] = 0;
	while (i < *nb_args)
	{
		printf("111111111111\n");
		if (type_args[i] == ESPACE)
		{
				i++;
				if (i == *nb_args)
					return (*nb_args = nb_new_args, last_args);
		}
		last_args[j] = ft_strjoin(last_args[j], new_args[i]);
		(i++, j++);
	}
	while ( i < nb_new_args)
	{
		printf("new args kick_space %d = %s", i, new_args[i]);
		i++;
	}
	*nb_args = nb_new_args;
	return (last_args);
}

char    **ft_parsing(int *type_args, int *nb_args, char **args, int *error)
{
	char **new_args;
	char **no_quote_args;
	int *new_type_args;

	*error = 0;
	if (search_error_args(type_args, nb_args, args))
		return (args);
	expande(type_args, *nb_args, args);
    no_quote_args = kick_quote(type_args, *nb_args, args);
	new_args = join_inter_space(no_quote_args, type_args, nb_args);
	//free no_quote
	new_type_args = ft_get_info_args(new_args, nb_args);
	//ft_free(args);
	args = kick_args_space(new_args,type_args, nb_args);
	free (type_args);
	free(new_type_args);
	new_type_args = ft_get_info_args(args, nb_args);

	//ft_free (args);
	 if (error_grammaticale(new_type_args, *nb_args))
	   return (args);
	printf("^^^^^^^^^^^ no error grammaticale ^^^^^^^^^^^^^^^^\n");
    *error = 1;
	return (args);
}

//execve("/usr/bin/jjy", {"echo", "$USER"}, argv)
