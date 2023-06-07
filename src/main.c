/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 20:12:28 by ncharii           #+#    #+#             */
/*   Updated: 2023/06/07 20:21:46 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <string.h> // a supprimer
#include <readline/readline.h> // pour faire fonctionner readline sur mac
#include <readline/history.h> // pour faire fonctionner readline sur mac


#define MAX_ARGS 64
#define MAX_ARG_LENGTH 256

// Gestionnaire de signal SIGINT //
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


void minishell_loop(void)
{
    char *line;
    // char *line_cpy;
    char **args;
    int *info_args;
    t_token* tokens;
    while (1)
    {
        line = readline("\033[1;32mminishell >\033[0m");
        if (line == NULL)
            exit(0);
        if (strlen(line) == 0)
            continue;

//		args = ft_split_line_to_character(line);
    int nb_args;
    args = ft_lexeur(line);
    info_args = ft_get_info_args(args, &nb_args); // anciennement info_token
    int i = 0;
    int error;
    //char **for_test = 0;
    args = ft_parsing(info_args, &nb_args, args, &error);
    if (error == 0)
        printf("\n@@@@@@@@@@@@@ ERROR DETECT !!!! @@@@@@@@@@@@@@@\n");
    else
        printf("############# validation ###############\n");
    tokens = ft_get_tokens_with_infos(args, info_args, nb_args);
    //cmd_complete(tokens);
    while (tokens)
    {
        printf("\033[1;31mtoken value = %s\n\033[0m", tokens->value);
        printf("\033[1;33mtoken type = %d\n\033[0m", tokens->info->type);
        tokens = tokens->next;
    }
     i = 0;
       info_args = ft_get_info_args(args, &nb_args);
		while (i < nb_args)
		{
			i++;
		}
		 i = 0;

//		while (args[i])
//		{
//			printf("token nb %d = %s avec len = %d\n", i, args[i], ft_strlen(args[i]));
//			i++;
//		}
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




