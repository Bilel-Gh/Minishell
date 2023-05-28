#include "../../includes/minishell.h"

bool error_grammaticale(int *type_token, int nb_token)
{
    int i;
    i = 0;
    printf("*********erreur grammaticale ???***************\n");
    if (nb_token == 1)
    {
        if ((type_token[i] != 0 ) && (type_token[i] != 1))
            return(true);
    }
    while (i < nb_token - 1)
    {
        //printf("type_token %d i = %d\n",type_token[i], i);
        if (i == 0)
        {
            if ((type_token[i] != 0 ) && (type_token[i] != 1))
                return (true);
       }
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
            if (type_token[i] == 4 && type_token[i + 1] != 0)
                return (true);
            if (type_token[i] == 1 && type_token[i + 1] != 0)
                return (true);
        }
        i++;
    }
 //   printf("*********ft_parsing***************\n");
    return (false);
}

bool    redirection_error_size(char *redirection)
{
        int i;
        
        i = 0;
        while (redirection[i])
            i++;
        if (i > 2)
            return(true);
        return (false);
}
   

bool    error_size_and_spe_redirection(int *type_token, int nb_token, char **cont_token)
{
    int i;
    i = 0;
    printf("\n*********erreur parsing redirection ???***************\n");
    while (i < nb_token)
    {
        if (type_token[i] == 1)
        {
            if(redirection_error_size(cont_token[i]))
                return (true);
            if(!only_type((cont_token[i])))
                return (true);
            give_sp_token((cont_token[i], type_token));
        }
        i++;
    }

}

int ft_parsing(int *type_token, int nb_token, char **cont_token)
{

    if (error_grammaticale(type_token, nb_token))
        return(1);
    printf("\n********* no error grammaticale ***************\n");
    if (error_size_and_spe_redirection(type_token, nb_token, cont_token))
        return(1);
    printf("\n********* no error redirection ***************\n");
    if (error_quote(type_token, nb_token, cont_token))
        return (1);
    printf("\n*********    no error quote    ***************\n");



    return (0);
}