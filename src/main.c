/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 20:12:28 by ncharii           #+#    #+#             */
/*   Updated: 2023/06/13 23:52:41 by ncharii          ###   ########.fr       */
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

t_global_parsing* ft_init_global_parsing(void)
{
	struct s_global_parsing *g_parsing;
	g_parsing = malloc(sizeof(struct s_global_parsing));
	if (!g_parsing)
		g_parsing = NULL;
	g_parsing->args = NULL;
	g_parsing->info_args = NULL;
	g_parsing->tokens = NULL;
	g_parsing->commande = NULL;
	return (g_parsing);
}

void ft_free_g_parsing(t_global_parsing *g_parsing)
{
	if (g_parsing->line)
		free(g_parsing->line);
	if (g_parsing->args)
		free_db_array(g_parsing->args);
	if (g_parsing->info_args)
		free(g_parsing->info_args);
	if (g_parsing->tokens)
		free_list_tokens(g_parsing->tokens);
	if (g_parsing->commande)
		free_list_commande(g_parsing->commande);
	free(g_parsing);
}

void minishell_loop(char **env)
{
	struct s_global_parsing *g_parsing;

	// char *line_cpy;
	t_token* head;
	while (1)
	{
		g_parsing = ft_init_global_parsing();
		if (!g_parsing)
			return ;
		g_parsing->line = readline("\033[1;32mminishell >\033[0m"); // TODO : BUG affichage quand on ecrit plein de caracteres
		if (g_parsing->line == NULL)
			exit(0);        
		if (strlen(g_parsing->line) == 0)
			continue;

		//		args = ft_split_line_to_character(line);
		int nb_args;
		g_parsing->args = ft_lexeur(g_parsing->line);
		g_parsing->info_args = ft_get_info_args(g_parsing->args, &nb_args); // anciennement info_token
		int error;
		error = 0;
		//char **for_test = 0;
		g_parsing->args = ft_parsing(&nb_args, g_parsing->args, &error, env);
		if (error == 0)
		{
			printf("\n@@@@@@@@@@@@@ ERROR DETECT !!!! @@@@@@@@@@@@@@@\n");
			//free(info_args);
			free_db_array(g_parsing->args);
			continue;
		}
		else
			printf("############# validation ###############\n");
		if (!g_parsing->args)
			continue;
		g_parsing->tokens = ft_get_tokens_with_infos(g_parsing->args, nb_args);
		g_parsing->commande = cmd_complete(g_parsing->tokens);
		head = g_parsing->tokens;
		while (g_parsing->tokens)
		{
			printf("\033[1;31mtoken value = %s\n\033[0m", g_parsing->tokens->value);
			printf("\033[1;33mtoken type = %d\n\033[0m", g_parsing->tokens->info->type);
			printf("\033[1;34mtoken index = %d\n\033[0m", g_parsing->tokens->token_index);
			if (g_parsing->tokens->prev)
				printf("\033[1;35mtoken prev value = %s\n\033[0m", g_parsing->tokens->prev->value);
			printf("\n\n");
			g_parsing->tokens = g_parsing->tokens->next;
		}
		g_parsing->tokens = head;
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
		ft_free_g_parsing(g_parsing);
	}
}

char **ft_db_array_dup(char **db_array)
{
	int i;
	char **db_array_cpy;
	i = 0;
	while (db_array[i])
		i++;
	db_array_cpy = malloc(sizeof(char*) * (i + 1));
	if (!db_array_cpy)
		exit(0) ;
	i = 0;
	while (db_array[i])
	{
		db_array_cpy[i] = ft_strdup(db_array[i]);
		i++;
	}
	db_array_cpy[i] = NULL;
	return (db_array_cpy);
}

int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	char **env_cpy;
	env_cpy = ft_db_array_dup(env);
	struct sigaction s_sigaction;

	s_sigaction.sa_handler = int_handler; // Nom de la fonction de gestionnaire
	sigaction(SIGINT, &s_sigaction, NULL);// Gestionnaire de signal 
	minishell_loop(env_cpy);
	if (env_cpy)
		free_db_array(env_cpy);
	return 0;
}




