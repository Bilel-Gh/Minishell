/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 16:18:20 by ncharii           #+#    #+#             */
/*   Updated: 2023/06/01 13:47:36 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	error_grammaticale(int *type_node, int nb_node)
{
	int	i;

	i = 0;
	if(!type_node)
		return(true);
	printf("*********check grammaticale error ???***************\n");
	if (nb_node == 1)
	{
		if ((type_node[i] != 0))
			return (true);
	}
	while (i < nb_node - 1)
	{
		//printf("type_node %d i = %d\n",type_node[i], i);
		if (type_node[i] == 5)
			i++;
		if (i == 0)
		{
			if (type_node[i] != 0 && type_node[i] != 1)
				return (true);
			if (type_node[i] == 1 && type_node[i + 1] != 0)
				return (true);
		}
		if (i != 0)
		{
			//   printf("parsing 2 = %d nb = %d\n", type_node[i], i);
			if (type_node[i] == 0)
			{
				//  printf("i parsing.c = %d\n",i);
				if (type_node[i + 1] != 4 && type_node[i + 1] != 0
					&& type_node[i + 1] != 1)
					return (true);
			}
			//!!!!!!! ATTENTION ls | > outfile !!!!!!!!
			if (type_node[i] == 4 && type_node[i + 1] != 0
				&& type_node[i + 1] != 1)
				return (true);
			if (type_node[i] == 1 && type_node[i + 1] != 0)
				return (true);
		}
		i++;
	}
	//   printf("*********ft_parsing***************\n");
	return (false);
}

bool	error_size(char *cont_node, int size_max)
{
	int	i;

	i = 0;
	while (cont_node[i])
		i++;
	if (i > size_max)
		return (true);
	return (false);
}

bool	error_no_only_type(char *str)
{
	char	style;
	int		i;

	i = 0;
	style = str[0];
	while (str[i])
	{
		if (str[i] != style)
			return (true);
		i++;
	}
	return (false);
}

void	give_sp_node_redi(char *node, int *type_node, int pos)
{
	int	size_node;

	size_node = strlen(node);
	if (size_node == 1 && node[0] == '<')
		type_node[pos] = 2;
	else if (size_node == 2 && node[0] == '<')
		type_node[pos] = 6;
	else if (size_node == 1 && node[0] == '>')
		type_node[pos] = 3;
	else if (size_node == 2 && node[0] == '>')
		type_node[pos] = 7;
	return ;
}

bool	error_size_or_spe_redi(int *type_node, int nb_node, char **cont_node)
{
	int	i;

	i = 0;
	printf("\n*********check parsing redirection ???***************\n");
	while (i < nb_node)
	{
		//      printf("sp redirection i = %d", i);
		if (type_node[i] == 1)
		{
			if (error_size(cont_node[i], 2))
				return (true);
			if (error_no_only_type((cont_node[i])))
				return (true);
			give_sp_node_redi(cont_node[i], type_node, i);
		}
		i++;
	}
	return (false);
}

bool	error_pipe(int *type_node, int nb_node, char **cont_node)
{
	int	i;

	i = 0;
	printf("\n*********check parsing pipe ???***************\n");
	while (i < nb_node)
	{
		//      printf("sp redirection i = %d", i);
		if (type_node[i] == 4)
		{
			if (error_size(cont_node[i], 1))
				return (true);
			if (error_no_only_type((cont_node[i])))
				return (true);
		}
		i++;
	}
	return (false);
}

bool	quote_is_open(char *node)
{
	int	state;
	int	i;

	state = 0;
	i = 0;
	while (node[i])
	{
		if (node[i] == 34 && state == 1)
			state = 0;
		else if (node[i] == 34 && state == 0)
			state = 1;
		else if (node[i] == 39 && state == 2)
			state = 0;
		else if (node[i] == 39 && state == 0)
			state = 2;
		i++;
	}
	//   printf("state = %d\n", state);
	if (state == 0)
		return (false);
	return (true);
}

bool	error_quote(int *type_node, int nb_node, char **cont_node)
{
	int	i;

	i = 0;
	printf("\n*********check parsing quote ???***************\n");
	while (i < nb_node)
	{
		if (cont_node[i][0] == 34 || cont_node[i][0] == 39)
		{
			if (quote_is_open(cont_node[i]))
				return (true);
			if (cont_node[i][0] == 34)
				type_node[i] = 9;
			else
				type_node[i] = 8;
		}
		i++;
	}
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

char	*join_and_rp_node(char *node_con, char *expande, int size_extract)
{
	char	*new_node;
	int		i;
	int		j;
	int		index;
	bool	expande_in;

	expande_in = false;
	index = 0;
	i = 0;
	j = 0;
	if (!expande)
		new_node = malloc(strlen(node_con) - size_extract);
	else
		new_node = malloc(strlen(node_con) + strlen(expande) - size_extract);
	if (!new_node)
		exit (0);// renplacer par la fonction free_all;
	while (node_con[i])
	{
		if (node_con[i] == '$' && expande_in == false)
		{
			if (expande)
			{
				while (expande[index])
				{
					new_node[j] = expande[index];
					index++;
					j++;
				}
			}
			i = i + size_extract;
		}
		new_node[j] = node_con[i];
		i++;
		j++;
	}
	new_node[j] = 0;
	printf("join = %s\n", new_node);
	free(node_con);
	return (new_node);
}

char	*traslate_expand(char *node_ct, char *extract_expend, int size_extract)
{
	char	*get_expande;
	char    *new_node;

	get_expande = give_env_expand(extract_expend, size_extract);
	if (get_expande)
		printf("\n my extratc = %s\n", get_expande);
	else
		printf("\n my extratc no\n");
	new_node = join_and_rp_node(node_ct, get_expande, size_extract);
	return (new_node);
}

char	*replace_expande(char *cont_node, int i)
{
	char	*expend_recherche;
	int		size_of_expende;
	char    *new_cont_node;
	size_of_expende = ft_size_of_expende(&cont_node[i]);
	if (size_of_expende < 2)
		return (cont_node);
	printf("size = %d\n \n ", size_of_expende);
	expend_recherche = get_expende_detect(size_of_expende, &cont_node[i]);
	printf("expend_recherche == %s\n \n", expend_recherche);
	new_cont_node = traslate_expand(cont_node, expend_recherche, size_of_expende);
	return (new_cont_node);
}

char    *importe_expande(char *cont_node)
{
	int	i;

	i = 0;
	while (cont_node[i])
	{
		if (cont_node[i] == '$')
			cont_node = replace_expande(cont_node, i);
		i++;
	}
	return (cont_node);
}

bool	have_expande(char *cont_node)
{
	int	i;

	i = 0;
	while (cont_node[i])
	{
		if (cont_node[i] == '$')
			return (true);
		i++;
	}
	return (false);
}

void	expande(int *type_node, int nb_node, char **cont_node)
{
	int	i;

	i = 0;
	while (i < nb_node)
	{
		if (type_node[i] == ALPHANUM || type_node[i] == QUOTE_D)
		{
			if (have_expande(cont_node[i]))
			{
				printf("\n \n PRESANCE EXPANDE\n");
				cont_node[i] = importe_expande(cont_node[i]);
			}
		}
		i++;
	}
}

bool is_word(int type_node)
{
	if (type_node == ALPHANUM
		|| type_node == QUOTE_S || type_node == QUOTE_D)
		return (true);
	return (false);
}

int count_new_node(int *type_node, int nb_node)
{
	int i;
	int j;

	i = 0;
	j = 0;
while (i < nb_node)
	{
		printf("nb node = %d\n", nb_node);
		if (is_word(type_node[i]))
			{
				printf("nb_node new node = %d\n type_", nb_node);
				while (is_word(type_node[i]) && i < nb_node)
				{
					printf("i new node = %d\n", i);
					i++;
					if (i == nb_node)
						return (printf("FSFSFSFF\n"),j + 1);

				}
				j++;
				continue;
			}

		(i++, j++);
	}
	printf("j new node = %d\n", j);
	return (j);
}



int malloc_new_node(char **new_cont_node, char **cont_node, int *type_node, int nb_node)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < nb_node)
	{
		if (is_word(type_node[i]))
			{
				while (is_word(type_node[i]) && i < nb_node)
				{
					new_cont_node[j] = ft_strjoin(new_cont_node[j], cont_node[i]);
					i++;
					if (i == nb_node)
						return (new_cont_node[j + 1] = 0, 1);
				}
				j++;
				continue;
			}
		new_cont_node[j] = ft_strjoin(new_cont_node[j], cont_node[i]);
		(i++, j++);
	}
	new_cont_node[j] = 0;
	return (1);

}
//CHAR **
char *copy_sans_quote(char *cont_node)// vas malloc et copier sans quote
{
	char *new_no_quote;
	int size_cont_node;
	int size_no_quote;
	int i;
	int j;

	i = 1;
	j = 0;
	size_cont_node = ft_strlen(cont_node);
	size_no_quote = size_cont_node - 2;
	new_no_quote = malloc(size_no_quote + 1);
	new_no_quote[size_no_quote] = 0;
	while (j < size_no_quote)
	{
		new_no_quote[j] = cont_node[i];
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

char **kick_quote(int *type_node, int nb_node, char **cont_node)
{
	char **no_quote;
	int i;

	i = 0;
	no_quote = malloc (sizeof(char*) * (nb_node + 1));
	no_quote[nb_node] = 0;
	while (i < nb_node)
	{
		if (type_node[i] == QUOTE_D || type_node[i] == QUOTE_S)
			no_quote[i] = copy_sans_quote(cont_node[i]);
		else
			no_quote[i] = copy_cont(cont_node[i]);
		i++;
	}
	return (no_quote);

}
char **join_inter_space(char **cont_node, int *type_node, int *nb_node)
{
	char ** new_cont_node;
	int i;
	int nb_new_node;

	nb_new_node = count_new_node(type_node, *nb_node);
	//printf("nb space =  %d\n", nb_new_node(type_node, nb_node));
	printf("\n \n nb node_join %d\n", nb_new_node);
	//prinft("NB node _join = %d  \n",nb_new_node);

	if (nb_new_node == *nb_node)
		return(cont_node) ;//return (cont_node);
	i = 0;
	new_cont_node = malloc( sizeof(char*) * (nb_new_node + 1));
	while (i <= nb_new_node)
	{
		new_cont_node[i] = malloc(1);
		new_cont_node[i] = NULL;
		i++;
	}
	i = 0;
	if (!new_cont_node)
		return(printf("error"), NULL) ; //ft_free;
	if (!malloc_new_node(new_cont_node, cont_node, type_node, *nb_node))
		return(printf("error malloc_new_node"), NULL) ;//ft_free;
	printf("new node %d = %s", i, new_cont_node[i]);
	while ( i < nb_new_node)
	{
		printf("new node %d = %s", i, new_cont_node[i]);
		i++;
	}
	*nb_node = nb_new_node;
	return (new_cont_node);
}


//void	kick_quote(char **cont_node)
//char **join_inter_space(char **cont_node, int *type_node, int nb_node)

//
/*

char    **good_nodenisation(int *type_node, int nb_node, char **cont_node)
{
    int nb_new_node;
    int i;
    char **new_nodeisation

    i = 0
    nb_new_node = 1;
    while (i < nb_node)
    {
        if(t_node[i] == SPACE)
            nb_new_node++;
        if(t_node[i] == ALPHANUM)
            nb_new_node = nb_new_node + nb_space(cont_node[i]);
    }
    new_nodeisation = (nb_new_node);
    i = 0;
    while (new_nodeisation[i])
    {
        if (!new_nodeisation[i])
            remplir_le_nouveau(new_nodeisation, i);
        i++;
    }
    return (good_nodenisation);
}*/

int count_nb_space(int *type_node, int nb_node)
{
	int i;
	int nb_space;

	i = 0;
	nb_space = 0;
	while (i < nb_node)
	{
		if (type_node[i] == ESPACE)
			nb_space++;
		i++;
	}
	return (nb_space);
}

char **kick_node_space(char **new_cont_node, int *type_node, int *nb_node)
{
	char **last_cont_node;
	int nb_space;
	int nb_new_node;
	int i;
	int j;

	i = 0;
	j = 0;
	nb_space = count_nb_space(type_node, *nb_node);
	nb_new_node = *nb_node - nb_space;
	//printf ("$$$$$$$$$$$$$$$$$$$$$$$$ nb_space %d\n", nb_space);
	last_cont_node = malloc( sizeof(char*) * (nb_new_node + 1));
	if (!last_cont_node)
		return (NULL);// FT_ERROR_malloc_parsing
	last_cont_node[nb_new_node] = 0;
	while (i < *nb_node)
	{
		printf("111111111111\n");
		if (type_node[i] == ESPACE)
		{
				i++;
				if (i == *nb_node)
					return (*nb_node = nb_new_node, last_cont_node);
		}
		last_cont_node[j] = ft_strjoin(last_cont_node[j], new_cont_node[i]);
		(i++, j++);
	}
	while ( i < nb_new_node)
	{
		printf("new node kick_space %d = %s", i, new_cont_node[i]);
		i++;
	}
	*nb_node = nb_new_node;
	return (last_cont_node);
}

char    **ft_parsing(int *type_node, int *nb_node, char **cont_node, int *error)
{
	char **new_cont_node;
	char **no_quote_cont_node;
	int *new_type_node;

	*error = 0;
	if (error_size_or_spe_redi(type_node, *nb_node, cont_node))
		return (cont_node);
	printf("^^^^^^^^^^^ no error redirection ^^^^^^^^^^^^^^^^^^\n");
	if (error_pipe(type_node, *nb_node, cont_node))
		return (cont_node);
	printf("^^^^^^^^^^^     no error pipe    ^^^^^^^^^^^^^^^^^^\n");
	if (error_quote(type_node, *nb_node, cont_node))
		return (cont_node);
	printf("\n*********    no error quote    ***************\n");
	expande(type_node, *nb_node, cont_node);
    no_quote_cont_node = kick_quote(type_node, *nb_node, cont_node);
	new_cont_node = join_inter_space(no_quote_cont_node, type_node, nb_node);
	//free no_quote
	new_type_node = ft_info_node(new_cont_node, nb_node);
	//ft_free(cont_node);
	cont_node = kick_node_space(new_cont_node,type_node, nb_node);
	free (type_node);
	free(new_type_node);
	new_type_node = ft_info_node(cont_node, nb_node);

	//ft_free (cont_node);
	 if (error_grammaticale(new_type_node, *nb_node))
	   return (cont_node);
	printf("^^^^^^^^^^^ no error grammaticale ^^^^^^^^^^^^^^^^\n");
    *error = 1;
	return (cont_node);
}

//execve("/usr/bin/jjy", {"echo", "$USER"}, argv)
