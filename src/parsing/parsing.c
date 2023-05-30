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
        if (token[i] == 34 && state == REDIRECT)
		    state = ALPHANUM;
		else if (token[i] == 34 && state == ALPHANUM)
			state = REDIRECT;
		else if (token[i] == 39 && state == RED_IN)
			state = ALPHANUM;
		else if (token[i] == 39 && state == ALPHANUM)
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
        if (cont_token[i][0] == 34 || cont_token[i][0] == 39) // 34 = " 39 = '
        {
            if (quote_is_open(cont_token[i]))
                return (true);

            if (cont_token[i][0] == 34)
                type_token[i] = QUOTE_D;
            else
                type_token[i] = QUOTE_S;
        }
        i++;
    }
    return (false);

}

int ft_parsing(int *type_token, int nb_token, char **cont_token)
{

    if (error_grammaticale(type_token, nb_token))
        return(1);
    printf("^^^^^^^^^^^ no error grammaticale ^^^^^^^^^^^^^^^^\n");
    if (error_size_and_spe_redirection(type_token, nb_token, cont_token))
        return(1);
    printf("^^^^^^^^^^^ no error redirection ^^^^^^^^^^^^^^^^^^\n");
    if (error_pipe(type_token, nb_token, cont_token))
        return(1);
    printf("^^^^^^^^^^^     no error pipe    ^^^^^^^^^^^^^^^^^^\n");
    if (error_quote(type_token, nb_token, cont_token))
        return (1);
    printf("\n*********    no error quote    ***************\n");



    return (0);
}

//execve("/usr/bin/jjy", {"echo", "$USER"}, argv)
