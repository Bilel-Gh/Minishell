/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 20:12:28 by ncharii           #+#    #+#             */
/*   Updated: 2023/05/26 05:21:25 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define MAX_ARGS 64
#define MAX_ARG_LENGTH 256

// Gestionnaire de signal SIGINT
void int_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("\n");
        rl_replace_line("", 0);  // Efface la ligne de commande actuelle
        rl_on_new_line();  // Place le curseur sur une nouvelle ligne
        rl_redisplay();  // Affiche le prompt
    }
}

void execute_command(char **splited_line)
{
    pid_t pid = fork(); // Création d'un nouveau processus

    if (pid < 0) // Si la création du processus a échoué
    {
        // Gestion d'erreur si la création du processus a échoué
        perror("fork");
        return;
    }
    else if (pid == 0) // Si le processus créé est le processus fils
    {
        // Code exécuté par le processus fils
        execvp(splited_line[0], splited_line); // !utiliser execve

        // En cas d'erreur lors de l'exécution de la commande
        perror("execvp");
        exit(1);
    }
    else
    {
        // Code exécuté par le processus parent
        int status;
        waitpid(pid, &status, 0); // Attente de la fin du processus fils

        // Vérification du statut de sortie de la commande exécutée
        if (status != 0)
            printf("Commande introuvable\n");
    }
}
/*char	**ft_split_line_to_token(char *line)
{
	char **args;
	int start;
	int end;
	int i;

	

	i = 0;
	while(line[i])
	{
		
	}



}*/

void minishell_loop(void)
{
    char *line;
    // char *line_cpy;
    char **args;
    int *info_token;
    while (1)
    {
        line = readline("minishell >");
        if (line == NULL)
            exit(0);
        if (strlen(line) == 0)
            continue;

//		args = ft_split_line_to_token(line);
    int nb_token;
    args = ft_lexeur(line);
    info_token = ft_info_token(args, &nb_token);
    int i = 0;
    i = ft_parsing(info_token, nb_token, args);
    if (i)
        printf("@@@@@@@@@@@@@ ERROR DETECT !!!! @@@@@@@@@@@@@@@\n");
    else
        printf("############# validation ###############\n");
     
     i = 0;

		while (i < nb_token)
		{
			printf("type = %d\n",info_token[i]);
			i++;
		}
		 i = 0;

		while (args[i])
		{
			printf("token nb %d = %s\n", i, args[i]);
			i++;
		}
//
            //execute_command(args);
        //else
           // printf("Commande introuvable\n");
        // Libérez la mémoire allouée par readline
        free(line);
    }
}

int main(void)
{
    struct sigaction s_sigaction;

    s_sigaction.sa_handler = int_handler; // Nom de la fonction de gestionnaire
    sigaction(SIGINT, &s_sigaction, NULL); // Gestionnaire de signal SIGINT

    minishell_loop();
    return 0;
}




