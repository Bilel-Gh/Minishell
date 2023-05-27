#include "../../includes/minishell.h"

int ft_parsing(int *type_token, int nb_token)
{
    int i;
    i = 0;
    printf("*********erreur grammaticale ???***************\n");
    if (nb_token == 1)
    {
        if ((type_token[i] != 0 ) && (type_token[i] != 1))
            return(1);
    }
    while (i < nb_token - 1)
    {
        //printf("type_token %d i = %d\n",type_token[i], i);
        if (i == 0)
        {
            if ((type_token[i] != 0 ) && (type_token[i] != 1))
                return (1);
       }
        if (i != 0)
        {
        //   printf("parsing 2 = %d nb = %d\n", type_token[i], i);
            if (type_token[i] == 0)
            {
              //  printf("i parsing.c = %d\n",i); 
                if (type_token[i + 1] != 4 && type_token[i + 1] != 0 && type_token[i + 1] != 1)
                    return (2);
            }
            if (type_token[i] == 4 && type_token[i + 1] != 0)
                return (3);
            if (type_token[i] == 1 && type_token[i + 1] != 0)
                return (4);
        }
        i++;
    }
 //   printf("*********ft_parsing***************\n");
    return (0);
}