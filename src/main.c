/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 20:12:28 by ncharii           #+#    #+#             */
/*   Updated: 2023/05/25 11:45:23 by bghandri         ###   ########.fr       */
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
        args = ft_lexeur(line);
        printf("args[0] = %s\n", args[0]);
        printf("args[1] = %s\n", args[1]);
        printf("args[2] = %s\n", args[2]);
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
        else if (line != NULL)
            execute_command(args);
        else
            printf("Commande introuvable\n");
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




