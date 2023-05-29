#include "../../includes/minishell.h"

bool error_grammaticale(int *type_token, int nb_token)
{
    int i;
    i = 0;
    printf("*********check grammaticale error ???***************\n");
    if (nb_token == 1)
    {
        if ((type_token[i] != 0 ))
            return(true);
    }
    while (i < nb_token - 1)
    {
        //printf("type_token %d i = %d\n",type_token[i], i);
        if (type_token[i] == 5)
            i++;

        if (i == 0)
        {
            if ((type_token[i] != 0 ) && (type_token[i] != 1))
                return (true);
            if (type_token[i] == 1 && type_token[i + 1] != 0)
                return (true);       }
        if (i != 0)
        {
        //   printf("parsing 2 = %d nb = %d\n", type_token[i], i);
            if (type_token[i] == 0)
            {
              //  printf("i parsing.c = %d\n",i); 
                if (type_token[i + 1] != 4 && type_token[i + 1] != 0 && type_token[i + 1] != 1)
                    return (true);
            }
            //!!!!!!! ATTENTION ls | > outfile !!!!!!!!
            if (type_token[i] == 4 && type_token[i + 1] != 0 && type_token[i + 1] != 1)
                return (true);
            if (type_token[i] == 1 && type_token[i + 1] != 0)
                return (true);
        }
        i++;
    }
 //   printf("*********ft_parsing***************\n");
    return (false);
}

bool    error_size(char *cont_token, int size_max)
{
        int i;
        
        i = 0;
        while (cont_token[i])
            i++;
        if (i > size_max)
            return(true);
        return (false);
}

bool    error_no_only_type(char *str)
{
    char style;
    int i;

    i = 0;
    style = str[0];
    while (str[i])
    {
        if (str[i] != style)
            return(true);
        i++;
    }
    return (false);
}
   
void    give_sp_token_redi(char *token, int *type_token, int pos)
{
    int size_token;

    size_token = strlen(token);
    if (size_token == 1 && token[0] == '<')
        type_token[pos] = 2;
    else if (size_token == 2 && token[0] == '<')
        type_token[pos] = 6;
    else if (size_token == 1 && token[0] == '>')
        type_token[pos] = 3;
    else if (size_token == 2 && token[0] == '>')
        type_token[pos] = 7;
    return;
}

bool    error_size_and_spe_redirection(int *type_token, int nb_token, char **cont_token)
{
    int i;
    i = 0;
    printf("\n*********check parsing redirection ???***************\n");
    while (i < nb_token)
    {
  //      printf("sp redirection i = %d", i);
        if (type_token[i] == 1)
        {
            if(error_size(cont_token[i], 2))
                return (true);
            if(error_no_only_type((cont_token[i])))
                return (true);

            give_sp_token_redi(cont_token[i], type_token, i);
        }
        i++;
    }
    return (false);
}

bool error_pipe(int *type_token, int nb_token, char **cont_token)
{
    int i;
    i = 0;
    printf("\n*********check parsing pipe ???***************\n");
    while (i < nb_token)
    {
  //      printf("sp redirection i = %d", i);
        if (type_token[i] == 4)
        {
            if(error_size(cont_token[i], 1))
                return (true);
            if(error_no_only_type((cont_token[i])))
                return (true);
        }
        i++;
    }
    return (false);
}

bool quote_is_open(char *token)
{
    int state;
    int i;

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

bool error_quote(int *type_token, int nb_token, char **cont_token)
{
    int i;
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

int ft_size_of_expende(char *expande)
{
    int size;

    size = 1;
    while(expande[size] != ' ' && expande[size] != 0 && expande[size] != '$' &&
    expande[size] != 39 && expande[size] != 34)
        size++;
    return (size);
}

char *get_expende_detect(int size_of_expende, char *expande)
{
    char *txt_expande;
    int i;
    
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

char *give_env_expand(char *expande_search, int size)
{
    int i;

    i = 0;
    while (environ[i])
	{
		if (!strncmp(expande_search, environ[i], size - 1))
			break ;
		i++;
	}
    if (environ[i] == NULL)
        return (NULL);
    return (&environ[i][size]);
}

void traslate_expand(char *expande, char *extract_expend, int size_extract)
{
    char *get_expande;
    (void)expande;

    get_expande = give_env_expand(extract_expend, size_extract);
    if (get_expande)
        printf("\n my extratc = %s",get_expande);
    printf("\n my extratc no");
}

void replace_expande(char *cont_token, int i)
{   
    //char *new_cont_token;
    char *expend_recherche;
    int size_of_expende;

    size_of_expende = ft_size_of_expende(&cont_token[i]);
    if (size_of_expende < 2)
        return;
    printf("size = %d\n \n ", size_of_expende);
    expend_recherche = get_expende_detect(size_of_expende, &cont_token[i]);
    printf("expend_recherche == %s\n \n", expend_recherche);
    traslate_expand(cont_token, expend_recherche, size_of_expende);
}

void importe_expande(char *cont_token)
{   
    int i;

    i = 0; 
    while (cont_token[i])
    {
        if (cont_token[i] == '$')
            replace_expande(cont_token, i);
        i++;
    }
}

bool    have_expande(char *cont_token)
{
    int i;

    i = 0;
    while (cont_token[i])
    {
        if(cont_token[i] == '$')
            return (true);
        i++;
    }
    return (false);
}

void expande(int *type_token, int nb_token, char **cont_token)
{
    int i;
    i = 0;
    while (i < nb_token)
    {
        if (type_token[i] == ALPHANUM || type_token[i] == QUOTE_D)
        {
            if (have_expande(cont_token[i]))
            {
                printf("\n \n PRESANCE EXPANDE\n");
                 importe_expande(cont_token[i]);
            }

        }
        i++;
    }
}


int ft_parsing(int *type_token, int nb_token, char **cont_token)
{
    if (error_size_and_spe_redirection(type_token, nb_token, cont_token))
        return(1);
    printf("^^^^^^^^^^^ no error redirection ^^^^^^^^^^^^^^^^^^\n");
    if (error_pipe(type_token, nb_token, cont_token))
        return(1);
    printf("^^^^^^^^^^^     no error pipe    ^^^^^^^^^^^^^^^^^^\n");
    if (error_quote(type_token, nb_token, cont_token))
        return (1);
    printf("\n*********    no error quote    ***************\n");

    //trad_quote(type_token, nb_token, cont_token);
    expande(type_token, nb_token, cont_token);
    // if (error_grammaticale(type_token, nb_token))
     //   return(1);
    //printf("^^^^^^^^^^^ no error grammaticale ^^^^^^^^^^^^^^^^\n");



    return (0);
}

//execve("/usr/bin/jjy", {"echo", "$USER"}, argv)