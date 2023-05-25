/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 20:12:28 by ncharii           #+#    #+#             */
/*   Updated: 2023/05/25 05:16:12 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "builtins/builtins.c"
#include "builtins/b_export.c"

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

void minishell_loop(void)
{
    char *line;
    // char *line_cpy;
    char **args;
    while (1)
    {
        line = readline("minishell >");
        if (line == NULL)
            exit(0);
        if (strlen(line) == 0)
            continue;
        args = ft_split(line);
        // printf("args[0] = %s\n", args[0]);
        if (strcmp(args[0], "exit") == 0)
            builtin_exit();
        else if (strcmp(args[0], "pwd") == 0)
            builtin_pwd(0);
        else if (strcmp(args[0], "env") == 0)
            builtin_env();
        else if (strcmp(args[0], "cd") == 0)
            builtin_cd(args);
        else if (strcmp(args[0], "echo") == 0)
            builtin_echo(args);
        else if (strcmp(args[0], "export") == 0)
            builtin_export(args);
        else if (strcmp(args[0], "unset") == 0)
            builtin_unset(args);
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




