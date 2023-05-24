/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 20:12:28 by ncharii           #+#    #+#             */
/*   Updated: 2023/05/24 22:36:08 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

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

int main(void)
{
    struct sigaction s_sigaction;

    s_sigaction.sa_handler = int_handler; // Nom de la fonction de gestionnaire
    sigaction(SIGINT, &s_sigaction, NULL); // Gestionnaire de signal SIGINT

    char *line;
    while (1)
    {
        line = readline("minishell >:");
        if (line == NULL)
            exit(0);

        // Traitez la ligne de commande ici (par exemple, exécutez des commandes)
		//printf("%s\n", line);
        // Libérez la mémoire allouée par readline
        free(line);
    }
    return 0;
}




