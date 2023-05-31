/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 16:18:20 by ncharii           #+#    #+#             */
/*   Updated: 2023/05/31 17:33:05 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	error_grammaticale(int *type_token, int nb_token)
{
	int	i;

	i = 0;
	printf("*********check grammaticale error ???***************\n");
	if (nb_token == 1)
	{
		if ((type_token[i] != 0))
			return (true);
	}
	while (i < nb_token - 1)
	{
		//printf("type_token %d i = %d\n",type_token[i], i);
		if (type_token[i] == 5)
			i++;
		if (i == 0)
		{
			if (type_token[i] != 0 && type_token[i] != 1)
				return (true);
			if (type_token[i] == 1 && type_token[i + 1] != 0)
				return (true);
		}
		if (i != 0)
		{
			//   printf("parsing 2 = %d nb = %d\n", type_token[i], i);
			if (type_token[i] == 0)
			{
				//  printf("i parsing.c = %d\n",i);
				if (type_token[i + 1] != 4 && type_token[i + 1] != 0
					&& type_token[i + 1] != 1)
					return (true);
			}
			//!!!!!!! ATTENTION ls | > outfile !!!!!!!!
			if (type_token[i] == 4 && type_token[i + 1] != 0
				&& type_token[i + 1] != 1)
				return (true);
			if (type_token[i] == 1 && type_token[i + 1] != 0)
				return (true);
		}
		i++;
	}
	//   printf("*********ft_parsing***************\n");
	return (false);
}

bool	error_size(char *cont_token, int size_max)
{
	int	i;

	i = 0;
	while (cont_token[i])
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

void	give_sp_token_redi(char *token, int *type_token, int pos)
{
	int	size_token;

	size_token = strlen(token);
	if (size_token == 1 && token[0] == '<')
		type_token[pos] = 2;
	else if (size_token == 2 && token[0] == '<')
		type_token[pos] = 6;
	else if (size_token == 1 && token[0] == '>')
		type_token[pos] = 3;
	else if (size_token == 2 && token[0] == '>')
		type_token[pos] = 7;
	return ;
}

bool	error_size_or_spe_redi(int *type_token, int nb_token, char **cont_token)
{
	int	i;

	i = 0;
	printf("\n*********check parsing redirection ???***************\n");
	while (i < nb_token)
	{
		//      printf("sp redirection i = %d", i);
		if (type_token[i] == 1)
		{
			if (error_size(cont_token[i], 2))
				return (true);
			if (error_no_only_type((cont_token[i])))
				return (true);
			give_sp_token_redi(cont_token[i], type_token, i);
		}
		i++;
	}
	return (false);
}

bool	error_pipe(int *type_token, int nb_token, char **cont_token)
{
	int	i;

	i = 0;
	printf("\n*********check parsing pipe ???***************\n");
	while (i < nb_token)
	{
		//      printf("sp redirection i = %d", i);
		if (type_token[i] == 4)
		{
			if (error_size(cont_token[i], 1))
				return (true);
			if (error_no_only_type((cont_token[i])))
				return (true);
		}
		i++;
	}
	return (false);
}

bool	quote_is_open(char *token)
{
	int	state;
	int	i;

	state = 0;
	i = 0;
	while (token[i])
	{
		if (token[i] == 34 && state == 1)
			state = 0;
		else if (token[i] == 34 && state == 0)
			state = 1;
		else if (token[i] == 39 && state == 2)
			state = 0;
		else if (token[i] == 39 && state == 0)
			state = 2;
		i++;
	}
	//   printf("state = %d\n", state);
	if (state == 0)
		return (false);
	return (true);
}

bool	error_quote(int *type_token, int nb_token, char **cont_token)
{
	int	i;

	i = 0;
	printf("\n*********check parsing quote ???***************\n");
	while (i < nb_token)
	{
		if (cont_token[i][0] == 34 || cont_token[i][0] == 39)
		{
			if (quote_is_open(cont_token[i]))
				return (true);
			if (cont_token[i][0] == 34)
				type_token[i] = 9;
			else
				type_token[i] = 8;
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
		&& expande[size] != 39)
	{
		if ((expande[size] == '@' && size > 1)
			|| (expande[size] == '#' && size > 1))
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
		if (!strncmp(expande_search, environ[i], size - 1))
			break ;
		i++;
	}
	free(expande_search);
	if (environ[i] == NULL)
		return (NULL);
	return (&environ[i][size]);
}

char	*join_and_rp_token(char *token_con, char *expande, int size_extract)
{
	char	*new_token;
	int		i;
	int		j;
	int		index;
	bool	expande_in;

	expande_in = false;
	index = 0;
	i = 0;
	j = 0;
	if (!expande)
		new_token = malloc(strlen(token_con) - size_extract);
	else
		new_token = malloc(strlen(token_con) + strlen(expande) - size_extract);
	if (!new_token)
		exit (0);// renplacer par la fonction free_all;
	while (token_con[i])
	{
		if (token_con[i] == '$' && expande_in == false)
		{
			if (expande)
			{
				while (expande[index])
				{
					new_token[j] = expande[index];
					index++;
					j++;
				}
			}
			i = i + size_extract;
		}
		new_token[j] = token_con[i];
		i++;
		j++;
	}
	new_token[j] = 0;
	printf("join = %s\n", new_token);
	free(token_con);
	return (new_token);
}

char	*traslate_expand(char *token_ct, char *extract_expend, int size_extract)
{
	char	*get_expande;
	char    *new_token;

	get_expande = give_env_expand(extract_expend, size_extract);
	if (get_expande)
		printf("\n my extratc = %s\n", get_expande);
	else
		printf("\n my extratc no\n");
	new_token = join_and_rp_token(token_ct, get_expande, size_extract);
	return (new_token);
}

char	*replace_expande(char *cont_token, int i)
{
	char	*expend_recherche;
	int		size_of_expende;
	char    *new_cont_token;
	size_of_expende = ft_size_of_expende(&cont_token[i]);
	if (size_of_expende < 2)
		return (cont_token);
	printf("size = %d\n \n ", size_of_expende);
	expend_recherche = get_expende_detect(size_of_expende, &cont_token[i]);
	printf("expend_recherche == %s\n \n", expend_recherche);
	new_cont_token = traslate_expand(cont_token, expend_recherche, size_of_expende);
	return (new_cont_token);
}

char    *importe_expande(char *cont_token)
{
	int	i;

	i = 0;
	while (cont_token[i])
	{
		if (cont_token[i] == '$')
			cont_token = replace_expande(cont_token, i);
		i++;
	}
	return (cont_token);
}

bool	have_expande(char *cont_token)
{
	int	i;

	i = 0;
	while (cont_token[i])
	{
		if (cont_token[i] == '$')
			return (true);
		i++;
	}
	return (false);
}

void	expande(int *type_token, int nb_token, char **cont_token)
{
	int	i;

	i = 0;
	while (i < nb_token)
	{
		if (type_token[i] == ALPHANUM || type_token[i] == QUOTE_D)
		{
			if (have_expande(cont_token[i]))
			{
				printf("\n \n PRESANCE EXPANDE\n");
				cont_token[i] = importe_expande(cont_token[i]);
			}
		}
		i++;
	}
}

bool is_word(int type_token)
{
	if (type_token == ALPHANUM
		|| type_token == QUOTE_S || type_token == QUOTE_D)
		return (true);
	return (false);
}

int nb_new_token(int *type_token, int nb_token)
{
	int i;
	int j;

	i = 0;
	j = 0;
while (i < nb_token)
	{
		printf("nb token = %d\n", nb_token);
		if (is_word(type_token[i]))
			{
				while (is_word(type_token[i]) && i < nb_token)
				{
					i++;
				}
				j++;
				continue;
			}

		(i++, j++);
	}
	return (j);
}



int malloc_new_token(char **new_cont_token, char **cont_token, int *type_token, int nb_token)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < nb_token)
	{
		if (is_word(type_token[i]))
			{
				while (is_word(type_token[i]) && i < nb_token)
				{
					new_cont_token[j] = ft_strjoin(new_cont_token[j], cont_token[i]);
					i++;
				}
				j++;
				continue;
			}
		new_cont_token[j] = ft_strjoin(new_cont_token[j], cont_token[i]);
		(i++, j++);
	}
	return (1);

}
//CHAR **
void join_inter_space(char **cont_token, int *type_token, int nb_token)
{
	char ** new_cont_token;
	int i;
	int nb_new__token;

	nb_new__token = nb_new_token(type_token, nb_token);
	//printf("nb space =  %d\n", nb_new_token(type_token, nb_token));
	printf("\n \n nb token_join %d\n", nb_new__token);
	//prinft("NB token _join = %d  \n",nb_new_token);

	if (nb_new__token == 1)
		return ;//return (cont_token);
	i = 0;
	new_cont_token = malloc( sizeof(char*) * (nb_new__token + 1));
	if (!new_cont_token)
		return ; //ft_free;
	if (!malloc_new_token(new_cont_token, cont_token, type_token, nb_token))
		return ;//ft_free;
	while ( i < nb_new__token)
	{
		printf("new token %d = %s", i, new_cont_token[i]);
		i++;
	}


}

//void	kick_quote(char **cont_token)
//char **join_inter_space(char **cont_token, int *type_token, int nb_token)

//
/*

char    **good_tokennisation(int *type_token, int nb_token, char **cont_token)
{
    int nb_new_token;
    int i;
    char **new_tokenisation

    i = 0
    nb_new_token = 1;
    while (i < nb_token)
    {
        if(t_token[i] == SPACE)
            nb_new_token++;
        if(t_token[i] == ALPHANUM)
            nb_new_token = nb_new_token + nb_space(cont_token[i]);
    }
    new_tokenisation = (nb_new_token);
    i = 0;
    while (new_tokenisation[i])
    {
        if (!new_tokenisation[i])
            remplir_le_nouveau(new_tokenisation, i);
        i++;
    }
    return (good_tokennisation);
}*/

char    **ft_parsing(int *type_token, int *nb_token, char **cont_token)
{
	if (error_size_or_spe_redi(type_token, *nb_token, cont_token))
		return (cont_token);
	printf("^^^^^^^^^^^ no error redirection ^^^^^^^^^^^^^^^^^^\n");
	if (error_pipe(type_token, *nb_token, cont_token))
		return (cont_token);
	printf("^^^^^^^^^^^     no error pipe    ^^^^^^^^^^^^^^^^^^\n");
	if (error_quote(type_token, *nb_token, cont_token))
		return (cont_token);
	printf("\n*********    no error quote    ***************\n");
	expande(type_token, *nb_token, cont_token);
    //kick_quote(type_token, nb_token, cont_token);
    //cont_token =
	 join_inter_space(cont_token, type_token, *nb_token);
	//ft_info_token(cont_token , nb_token);
	//new_list_type_token(cont_token,type_token);
	 if (error_grammaticale(type_token, *nb_token))
	   return (cont_token);
	//printf("^^^^^^^^^^^ no error grammaticale ^^^^^^^^^^^^^^^^\n");

	return (0);
}

//execve("/usr/bin/jjy", {"echo", "$USER"}, argv)
