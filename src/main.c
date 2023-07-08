/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:36:06 by ncharii           #+#    #+#             */
/*   Updated: 2023/07/08 21:59:08 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"
#include <string.h> // a supprimer
#include <readline/readline.h> // pour faire fonctionner readline sur mac
#include <readline/history.h> // pour faire fonctionner readline sur mac


#define MAX_ARGS 64
#define MAX_ARG_LENGTH 256

int g_code_exit = 0;

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

int ft_check_full_cmd(char *cmd)
{
    char *invalid_char;

    invalid_char = ";(){}<>|&.";
    if(only_misuse(cmd, invalid_char))
    {
        printf("syntax error near unexpected token `%c%c'\n", cmd[0], cmd[1]);
        g_code_exit = MISUSE;
        return 1;
    }
    else if (ft_strcmp(cmd, "!") == 0)
    {
        g_code_exit = ERROR;
        return 1;
    }

    return 0;
}

void ft_check_error_exec(char **cmd)
{
    struct stat path_stat;
    char *full_cmd;
    int i;

    full_cmd = ft_strdup(cmd[0]);
    i = 1;
    while (cmd[i] != NULL)
    {
        full_cmd = ft_strjoin(full_cmd, cmd[i]);
        i++;
    }
    printf("full_cmd = %s\n", full_cmd);
    if (ft_check_full_cmd(full_cmd))
    {
        printf("full_cmd = %s\n", full_cmd);
        free(full_cmd);
        return ;
    }
    if (lstat(cmd[0], &path_stat) == 0) {
        if (path_stat.st_mode & S_IFDIR) {
            printf("bash: %s: is a directory: \n", cmd[0]);
            g_code_exit = CANTEXEC;
        } else {
            printf("bash: %s: Permission denied\n", cmd[0]);
            g_code_exit = CANTEXEC;
        }
    } else {
        printf("bash: %s: command not found\n", cmd[0]);
        g_code_exit = NOTFOUND;
    }
    free(full_cmd);
}

void gestion_pipe2(char ***env, t_global_parsing **g_parsing, int *nb_args) {
    if (ft_strcmp((*g_parsing)->args[0], "echo") == 0)
    {
        g_code_exit = ERROR_PIPE;
        return ;
    }
    char *additional_input = readline(">");
    char *new_line = ft_strjoin((*g_parsing)->line, additional_input);
    (*g_parsing)->line = new_line;
    (*g_parsing)->args = ft_lexeur((*g_parsing)->line);
    (*g_parsing)->info_args = ft_get_info_args((*g_parsing)->args, nb_args);
    (*g_parsing)->args = ft_parsing(nb_args, g_parsing, env);
    free(additional_input);
}

void gestion_unclosed_quote(char ***env, t_global_parsing **g_parsing, int *nb_args) {
    char *additional_input = readline(">");
    char *new_line = ft_strjoin((*g_parsing)->line, additional_input);
    if (g_code_exit == ERROR_QUOTE_D)
    {
        if (ft_strchr(additional_input, '"') != NULL)
            g_code_exit = SUCCESS;
    }
    else if (g_code_exit == ERROR_QUOTE_S)
    {
        if (ft_strchr(additional_input, '\'') != NULL)
            g_code_exit = SUCCESS;
    }
    (*g_parsing)->line = new_line;
    (*g_parsing)->args = ft_lexeur((*g_parsing)->line);
    (*g_parsing)->info_args = ft_get_info_args((*g_parsing)->args, nb_args);
    (*g_parsing)->args = ft_parsing(nb_args, g_parsing, env);
    free(additional_input);
}

int ft_custom_error(char **args)
{
    if (g_code_exit == ERROR_PIPE)
    {
        printf("bash: syntax error near unexpected token '%c'\n", '|');
        return 1;
    }
    if (g_code_exit == ERROR_REDIRECT)
    {
        if (ft_db_arr_len(args) == 1 && ft_strlen(args[0]) <= 2)
            printf("bash: syntax error near unexpected token 'newline'\n");
        else if (args[0][0] == '>' || args[0][0] == '<')
            printf("bash: syntax error near unexpected token '%c%c'\n", args[0][0], args[0][1]);
        else
            printf("bash: syntax error near unexpected token '<'\n");
        return 1;
    }
    return 0;

}

void minishell_loop(char ***env, t_global_exec *g_exec)
{
	t_global_parsing *g_parsing;

	// char *line_cpy;
	t_token* head;
//    t_commande *head_cmd;
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
		g_parsing->args = ft_parsing(&nb_args, &g_parsing, env);
        printf("\033[1;31m G_CODE_EXIT = %d \033[0m\n", g_code_exit);
        while ((g_code_exit == ERROR_PIPE2 || g_code_exit == ERROR_BACKSLASH))
            gestion_pipe2(env, &g_parsing, &nb_args );
        while (g_code_exit == ERROR_QUOTE_D || g_code_exit == ERROR_QUOTE_S)
            gestion_unclosed_quote(env, &g_parsing, &nb_args );
        if (g_code_exit == NOTFOUND)
        {
            printf("bash: : command not found\n");
            free_db_array(g_parsing->args);
            continue;
        }
        if (nb_args == 0) // a supprimer ?
        {
            g_code_exit = SUCCESS;
            ft_free_g_parsing(g_parsing);
            continue;
        }
		if (g_code_exit != SUCCESS)
		{
			printf("\n@@@@@@@@@@@@@ ERROR DETECT !!!! @@@@@@@@@@@@@@@\n");
            printf("\033[1;34m G_CODE_EXIT = %d \033[0m\n", g_code_exit);
            if (!ft_custom_error(g_parsing->args))
                printf("bash: syntax error near unexpected token '%c%c'\n", g_parsing->args[0][0], g_parsing->args[0][1]);
            g_code_exit = MISUSE;
			printf("\033[1;36mexit code2 = %d\n\033[0m", g_code_exit);
			free_db_array(g_parsing->args);
			continue;
		}
		else
			printf("############# validation ###############\n");
        g_code_exit = SUCCESS;
		if (!g_parsing->args)
            continue;
		g_parsing->tokens = ft_get_tokens_with_infos(g_parsing->args, nb_args);
        head = g_parsing->tokens;
		 while (g_parsing->tokens)
		  {
            //  if (g_parsing->tokens->info->type == 1 && !ft_is_bash_command(g_parsing->tokens->value, *env))
            //  {
            //      printf("bash: %s: command not found\n", g_parsing->tokens->value);
            //  }
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
//        ft_check_error(g_parsing->commande, env);
//        if (g_code_exit != SUCCESS)
//        {
//            ft_free_g_parsing(g_parsing);
//            continue;
//        }
		ft_set_index_for_exec(&g_parsing->tokens);
        exec(g_parsing->tokens, g_parsing->commande, env, &g_parsing);
    //    if (g_parsing->commande->cmd)
	// 	   ft_exec_bultins(g_parsing->commande->cmd, env, &g_parsing, &g_exec);
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

char *ft_add_quotes_export(char *str)
{
    int i = 0;
    int j = 0;
    int equal_found = 0;
    int len;
    char *new_str;

    len = ft_strlen(str);
    new_str = malloc(sizeof(char) * (len + 3));
    if (new_str == NULL)
        return NULL;
    while (str[i] != '\0')
    {
        if (str[i] == '=' && !equal_found)
        {
            new_str[j++] = str[i++];
            new_str[j++] = '"';
            equal_found = 1;
            continue;
        }
        new_str[j++] = str[i++];
    }
    if (equal_found)
        new_str[j++] = '"';
    new_str[j] = '\0';
    free(str);
    return new_str;
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
        export[j] = ft_add_quotes_export(export[j]); // TODO verifier avec valgrind
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




