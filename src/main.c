/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:36:06 by ncharii           #+#    #+#             */
/*   Updated: 2023/07/01 19:15:26 by bghandri         ###   ########.fr       */
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
    printf("free g_parsing->args ok\n");
	if (g_parsing->info_args)
		free(g_parsing->info_args);
	if (g_parsing->tokens)
		free_list_tokens(g_parsing->tokens);
	if (g_parsing->commande)
		free_list_commande(g_parsing->commande);
	free(g_parsing);
}

void ft_set_index_for_exec(t_token **tokens)
{
	t_token *head;
	int i;
	i = 0;
	head = *tokens;
	while (*tokens)
	{
		while ((*tokens)->info->type != 2)
		{
			(*tokens)->token_index = i;
			*tokens = (*tokens)->next;
			if (!*tokens)
				break;
		}
		if (!*tokens)
			break;
		(*tokens)->token_index = i;
		i++;
		*tokens = (*tokens)->next;
	}
	*tokens = head;
}

int only_misuse(char *str, char *invalid_char)
{
    int i;
    int j;

    i = 0;
    while (str[i] != '\0')
    {
        j = 0;
        while (invalid_char[j] != '\0')
        {
            if (str[i] == invalid_char[j])
                break;
            j++;
        }
        if (invalid_char[j] == '\0')
            return 0;
        i++;
    }
    return 1;
}

int ft_is_bash_command(char *args, char **env)
{
    char *path;
    char **path_tab;
    char *path_cmd;
    char *builtin_cmd;
    int i;
    int result = 0;

    builtin_cmd = "echo cd pwd export unset env exit";
    if (ft_strstr(builtin_cmd, args) != NULL)
        return 1;

    path = ft_getenv("PATH", env);
    path_tab = ft_split(path, ':');
    i = 0;
    while (path_tab[i] != NULL) {
        path_cmd = ft_strjoin(ft_strdup(path_tab[i]), "/");
        path_cmd = ft_strjoin(path_cmd, args);

        if (access(path_cmd, F_OK) == 0) {
            result = 1;
            break;
        }

        i++;
        free(path_cmd);
    }
    free_db_array(path_tab);
    return result;
}



void ft_set_exit_code(t_global_parsing *g_parsing, char ***env)
{
    char *invalid_char;

    invalid_char = ";(){}<>|&.";
    if(only_misuse(g_parsing->args[0], invalid_char))
    {
        printf("syntax error near unexpected token `%s'\n", g_parsing->args[0]);
        g_code_exit = MISUSE;
        return ;
    }
    else if (ft_is_bash_command(g_parsing->args[0], *env))
        g_code_exit = CMD_FOUND;
    else if (access(g_parsing->args[0], F_OK) == -1
    || ft_strncmp(g_parsing->args[0], "makefile", 8) == 0
    || ft_strncmp(g_parsing->args[0], "Makefile", 8) == 0)
    {
        printf("bash: %s: command not found\n", g_parsing->args[0]);
        g_code_exit = NOTFOUND;
    }
    else if (ft_strcmp(g_parsing->args[0], "!") == 0)
        g_code_exit = ERROR;
    else if (access(g_parsing->args[0], F_OK) == 0)
    {
        printf("bash: %s: is a directory\n", g_parsing->args[0]);
        g_code_exit = CANTEXEC;
    }
}

void minishell_loop(char ***env, t_global_exec *g_exec)
{
	t_global_parsing *g_parsing;

	// char *line_cpy;
	t_token* head;
    t_commande *head_cmd;
	while (1)
	{
		g_parsing = ft_init_global_parsing();
		if (!g_parsing)
			return ;
        g_parsing->exec = g_exec;
		g_parsing->line = readline("\033[1;32mminishell >\033[0m"); // TODO : BUG affichage quand on ecrit plein de caracteres
		if (g_parsing->line == NULL)
			exit(0);
		if (strlen(g_parsing->line) == 0
        || ft_strcmp(g_parsing->line, ":") == 0
         || ft_strcmp(g_parsing->line, ";") == 0
         || is_only_space(g_parsing->line) == 1)
        {
            g_code_exit = SUCCESS;
            continue;
        }

		//		args = ft_split_line_to_character(line);
		int nb_args;
		g_parsing->args = ft_lexeur(g_parsing->line);
		g_parsing->info_args = ft_get_info_args(g_parsing->args, &nb_args);
		int error;
		error = 0;
		//char **for_test = 0;
		g_parsing->args = ft_parsing(&nb_args, &g_parsing, &error, env);
        g_code_exit = SUCCESS;
		if (error == 0)
		{
			printf("\n@@@@@@@@@@@@@ ERROR DETECT !!!! @@@@@@@@@@@@@@@\n");
			//free(info_args);
            g_code_exit = MISUSE;
			printf("\033[1;36mexit code2 = %d\n\033[0m", g_code_exit);
			free_db_array(g_parsing->args);
			continue;
		}
		else
			printf("############# validation ###############\n");
        if (ft_db_tablen(g_parsing->args) == 1)
        {
            ft_set_exit_code(g_parsing, env);
            printf("\033[1;36mexit code3 = %d\n\033[0m", g_code_exit);
            if (g_code_exit != CMD_FOUND && g_code_exit != SUCCESS)
            {
                ft_free_g_parsing(g_parsing);
                continue;
            }
        }
        g_code_exit = SUCCESS;
		if (!g_parsing->args)
            continue;
		g_parsing->tokens = ft_get_tokens_with_infos(g_parsing->args, nb_args);
        head = g_parsing->tokens;
		 while (g_parsing->tokens)
		  {
             if (g_parsing->tokens->info->type == 1 && !ft_is_bash_command(g_parsing->tokens->value, *env))
             {
                 printf("bash: %s: command not found\n", g_parsing->tokens->value);
             }
		  	printf("\033[1;31mtoken value = %s\n\033[0m", g_parsing->tokens->value);
		 	printf("\033[1;33mtoken type = %d\n\033[0m", g_parsing->tokens->info->type);
		 	printf("\033[1;34mtoken index = %d\n\033[0m", g_parsing->tokens->token_index);
		 	if (g_parsing->tokens->prev)
		 		printf("\033[1;35mtoken prev value = %s\n\033[0m", g_parsing->tokens->prev->value);
		 	printf("\n\n");
		 	g_parsing->tokens = g_parsing->tokens->next;
		 }
		 g_parsing->tokens = head;
		g_parsing->commande = cmd_complete(g_parsing->tokens);
        head_cmd = g_parsing->commande;
        while (g_parsing->commande)
        {
            if (!ft_is_bash_command(g_parsing->args[0], *env))
            {
                printf("bash: syntax error near unexpected token `newline'\n");
                g_code_exit = MISUSE;
            }
            g_parsing->commande = g_parsing->commande->next;
        }
        g_parsing->commande = head_cmd;
		ft_set_index_for_exec(&g_parsing->tokens);
         //exec(g_parsing->tokens, g_parsing->commande, *env);
       if (g_parsing->commande->cmd)
		   ft_exec_bultins(g_parsing->commande->cmd, env, &g_parsing, &g_exec);
		printf("\033[1;36mexit code = %d\n\033[0m", g_code_exit);
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
		return (NULL);// TODO !!!
	i = 0;
	while (db_array[i])
	{
		db_array_cpy[i] = ft_strdup(db_array[i]);
		i++;
	}
	db_array_cpy[i] = NULL;
	return (db_array_cpy);
}

char **ft_get_export(char **env)
{
	int i;
	int j;
	char **export;
	char *suffix;

	export = NULL;
	suffix = ft_strdup("export ");
	if (!suffix)
		return (NULL);
	i = 0;
	j = 0;
	while (env[i])
		i++;
	export = malloc(sizeof(char*) * (i + 1));
	export[i] = NULL;
	while (j < i)
	{
		export[j] = ft_strjoin(suffix, env[j]);
		suffix = ft_strdup("export ");
		if (!suffix)
			return (NULL);
		j++;
	}
	free(suffix);
	return (export);
}

int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
    g_code_exit = 0;
	char **env_cpy;
	env_cpy = ft_db_array_dup(env);
	t_global_exec *g_exec;
    g_exec = malloc(sizeof(t_global_exec));
    g_exec->export = ft_get_export(env_cpy);
	struct sigaction s_sigaction;
	unlink("/tmp/here_doc_minishell");

	s_sigaction.sa_handler = int_handler; // Nom de la fonction de gestionnaire
	sigaction(SIGINT, &s_sigaction, NULL);// Gestionnaire de signal
	minishell_loop(&env_cpy, g_exec);
	if (env_cpy)
		free_db_array(env_cpy);
	return 0;
}




