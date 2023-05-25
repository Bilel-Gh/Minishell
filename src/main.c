/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 20:12:28 by ncharii           #+#    #+#             */
/*   Updated: 2023/05/25 04:32:09 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "builtins/builtins.c"

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

void minishell_loop(void)
{
    char *line;
    while (1)
    {
        line = readline("minishell >");
        if (line == NULL)
            exit(0);
        // args = split_line(line);
        if (strcmp(line, "exit") == 0)
            builtin_exit();
        else if (strcmp(line, "pwd") == 0)
            builtin_pwd(0);
        else if (strcmp(line, "env") == 0)
            builtin_env();
        else
            printf("Commande introuvable\n");
        // Libérez la mémoire allouée par readline
        free(line);
    }
}

int main(void)
{
    char *line;
    char **args;
    struct sigaction s_sigaction;

    s_sigaction.sa_handler = int_handler; // Nom de la fonction de gestionnaire
    sigaction(SIGINT, &s_sigaction, NULL); // Gestionnaire de signal SIGINT

    minishell_loop();
    return 0;
}




