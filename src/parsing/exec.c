/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharii <ncharii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:21:17 by ncharii           #+#    #+#             */
/*   Updated: 2023/07/04 12:28:32 by ncharii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

typedef struct		exec
{
	char *infile;
	char *outfile;
	int	fd_infile;
	int	fd_outfile;
	int	fd_in_last_pipe;
	int	fd_heredoc;
	char *limiteur;
	int	pos;
	int	nb_cmd;
	int i;
	int path_input;
	char	**path;
	char	*path_cmd;
	t_global_parsing	*g_parsing;
}					t_exec;

void	init_exec(t_exec *exec)
{
	exec->infile = NULL;
	exec->outfile = NULL;
	exec->limiteur = NULL;
	exec->path_cmd = NULL;
	exec->fd_infile = -1;
	exec->fd_outfile = -1;
	exec->fd_in_last_pipe = -1;
	exec->fd_heredoc = -1;
	exec->path_input = 0;
	exec->pos = FIRST;
}

int	nb_token_whis_index(t_token *tokens, int index)
{
	int i;
	t_token *seach;

	i = 0;
	seach = tokens;
	while (seach)
	{
		if (seach->token_index == index)
			i++;
		seach = seach->next;
	}
	return (i);
}

t_token	*creat_info_token_list(t_token *tokens, int nb_node)
{
	int			i;

	t_token	*tmp;
	t_token	*for_creat;
	t_token *head;

	for_creat = tokens;
	head = tokens;

	i = 1;
	while (i < nb_node)
	{
		tmp = malloc(sizeof(t_token));
		if (!tmp)
		{
			free_list_tokens(tokens);
			return (NULL);
		}
		tmp->next = 0;
		for_creat->next = tmp;
		for_creat = for_creat->next;
		i++;
	}
	//for_creat = NULL;
	return (head);
}

void	copy_cont_token(t_token *dest, t_token *src)
{
	dest->token_index = src->token_index;
	dest->value = ft_strdup(src->value);
	dest->info = (t_token_info *)malloc(sizeof(t_token_info));
	dest->info->type = src->info->type;
	dest->prev = NULL;
	// TODO gestion d errerur a faire et il faudrait meme reflechir  aune autre facon de faire
}

t_token	*dup_info(t_token *info_token, t_token *tokens, int index)
{
	t_token *tokens_value_index;
	t_token *all;

	all = tokens;
	tokens_value_index = info_token;
	while (all)
	{
		if (all->token_index == index)
		{
			copy_cont_token(tokens_value_index, all);
			tokens_value_index = tokens_value_index->next;
		}
		all = all->next;
	}
	return (info_token);
}

t_token *get_info_token(t_token *tokens, int index)
{
	t_token *info_token;
	int nb_token;

	nb_token = nb_token_whis_index(tokens, index);
	info_token = NULL;
	if (nb_token == 0)
		return (info_token);
	info_token = malloc(sizeof(t_token));
	info_token->next = NULL;
	if (!info_token)
		return (NULL);
	info_token = creat_info_token_list(info_token, nb_token);
	if (!info_token)
		return (NULL);
	info_token = dup_info(info_token, tokens, index);
	return (info_token);
}

//########################################################################
bool	is_token_redi_in(t_token *token)
{
	if (token->info->type == REDIRECT_IN || token->info->type == REDIRECT_D_IN)
		return (true);
	return (false);

}

void	new_infile(t_exec *exec, t_token *token)
{
	if (exec->infile)
	{
		close(exec->fd_infile);
		free(exec->infile);
		exec->infile = NULL;
	}
	if (exec->limiteur)
	{
		close (exec->fd_infile);
		free(exec->limiteur);
		unlink("/tmp/here_doc_minishell");
		exec->limiteur = NULL;
	}
	exec->infile = ft_strdup(token->value);
	if (!exec->infile)
		return ;
	exec->fd_infile = open(exec->infile, O_RDONLY);
}

void	start_heredoc(t_exec *exec)
{
	char *line;
	line = NULL;
	//	unsigned int size_limiteur;
	//
	//	size_limiteur = ft_strlen(exec->limiteur);
	exec->fd_infile = open("/tmp/here_doc_minishell", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (exec->fd_infile == -1)
		perror("open");
	// TODO security
	while (1)
	{
		line = readline("> ");
		if(!ft_strcmp(line, exec->limiteur))
			break;
		write(exec->fd_infile, line, ft_strlen(line));
		write(exec->fd_infile, "\n", 1);
		free(line);
	}
	free(line);
	close(exec->fd_infile);
	exec->fd_infile = open("/tmp/here_doc_minishell", O_RDONLY);
	printf("\033[1;34mexec->fd_infile2 = %d\n\033[0m", exec->fd_infile);
	if (exec->fd_infile == -1)
		perror("open");
	// TODO security
}


void	new_heredoc(t_exec *exec, t_token *token)
{
	if (exec->infile)
	{
		close(exec->fd_infile);
		free(exec->infile);
		exec->infile = NULL;
	}
	if (exec->limiteur)
	{
		close (exec->fd_infile);
		free(exec->limiteur);
		unlink("/tmp/here_doc_minishell");
		exec->limiteur = NULL;
	}
	exec->limiteur = ft_strdup(token->value);
	if (!exec->limiteur)
		return ;
	start_heredoc(exec);
}

void	set_new_infile(t_exec *exec, t_token *tokens)
{
	t_token *token;

	token = tokens;
	if (token->info->type == REDIRECT_IN)
		new_infile(exec, token->next);
	else
		new_heredoc(exec, token->next);
}

void	gestion_infile(t_token *tokens, t_exec *exec)
{
	t_token *seach_tok_in;

	seach_tok_in = tokens;
	while (seach_tok_in)
	{
		if (is_token_redi_in(seach_tok_in))
			set_new_infile(exec, seach_tok_in);
		seach_tok_in = seach_tok_in->next;
	}
}
//----------------------------------------------------------
void set_new_outfile(t_exec *exec, t_token *token)
{
	if (exec->outfile)
	{
		close(exec->fd_outfile);
		free(exec->outfile);
	}
	exec->outfile = ft_strdup(token->next->value);
	// TODO protection malloc
	if (token->info->type == REDIRECT_OUT)
	{
		exec->fd_outfile = open(exec->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (exec->fd_outfile == -1)
			perror("open");
	}
	else if (token->info->type == REDIRECT_D_OUT)
	{
		exec->fd_outfile = open(exec->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (exec->fd_outfile == -1)
			perror("open");
	}
}

bool	is_token_redi_out(t_token *token)
{
	if (token->info->type == REDIRECT_OUT || token->info->type == REDIRECT_D_OUT)
		return (true);
	return (false);
}

void    gestion_outfile(t_token *tokens, t_exec *exec)
{
	t_token *seach_tok_out;

	seach_tok_out = tokens;
	while (seach_tok_out)
	{
		if (is_token_redi_out(seach_tok_out))
			set_new_outfile(exec, seach_tok_out);
		seach_tok_out = seach_tok_out->next;
	}
}
//############################################################################
char	*for_test(char *test, char *argv)
{
	char	*str;
	int		i;
	int		y;

	i = 0;
	y = 0;
	str = malloc(ft_strlen(test) + ft_strlen(argv) + 2);
	while (test[i])
	{
		str[i] = test[i];
		i++;
	}
	str[i] = '/';
	i++;
	while (argv[y] != ' ' && argv[y])
	{
		str[i] = argv[y];
		i++;
		y++;
	}
	str[i] = '\0';
	return (str);
}

int	find_path(char **path, char *cmd, t_exec *info)
{
	char	*test;

	info->i = 0;
	if (cmd == 0)
		return (0);
	test = ft_strchr(cmd, 47);
	if (test != 0 || path == NULL)
	{
		info->path_cmd = ft_strdup(cmd);
		info->path_input = 1;
		return (1);
	}
	if (path == NULL)
		return (0);
	while (path[info->i] && path[0] != NULL)
	{
		test = for_test(path[info->i], cmd);
		if (access(test, F_OK) == 0)
			return (info->path_cmd = test, 1);
		info->i++;
		if (path[info->i])
			free(test);
	}
	info->path_cmd = test;
	return (1);
}

int	creat_pipe_and_file(t_exec *info, int *pipefd)
{
	if (pipe(pipefd) == -1)
		return (perror("error pipe"), -1);
	if (!info->infile && !info->limiteur)
		info->fd_infile = 0;
	if (!info->outfile)
		info->fd_outfile = pipefd[1];
	return (1);
}

int ft_bultins_fork(char **cmd, char **env)
{
	int is_bultin;

	is_bultin = 0;
	if (ft_strcmp(cmd[0], "echo") == 0)
	{
		builtin_echo(cmd);
		is_bultin = 1;
	}
	else if (ft_strcmp(cmd[0], "pwd") == 0)
	{
		builtin_pwd(cmd);
		is_bultin = 1;
	}
	else if (ft_strcmp(cmd[0], "env") == 0)
	{
		builtin_env(cmd, env);
		is_bultin = 1;
	}
	return (is_bultin);
}



int	exec_cmd(t_exec *info, char **env, char **cmd)
{
	int exec_bultins;

	if (info->fd_infile != 0)
	{
		dup2(info->fd_infile, 0);
		close(info->fd_infile);
	}
	exec_bultins = ft_bultins_fork(cmd, env);//env ; pwd; echo;
	if (exec_bultins != 0)
	{
		free(info->path_cmd);
		return (exit(g_code_exit), -1);
	}
	if (execve(info->path_cmd, cmd, 0) == -1)
	{
		free(info->path_cmd);
		return (perror("cdm"), exit(127), -1);
	}
	return (1);
}

void	close_for_first(int *pipefd, t_exec *info)
{
	close(pipefd[1]);
	if (info->infile || info->limiteur)
		close(info->fd_infile);
	if (info->outfile)
		close(info->fd_outfile);
	info->fd_in_last_pipe = pipefd[0];
}

int	first(char **cmd, t_exec *info, char **env)
{
	int		pipefd[2];
	pid_t	pid;

	if (creat_pipe_and_file(info, pipefd) < 0)
	{
		close_for_first(pipefd, info);
		return (-1);
	}
	write(1,"ff", 2);
	//	if (cmd[0] != NULL)
	{
		//		printf("dd");
		find_path(info->path, cmd[0], info);
		pid = fork();
		if (pid == -1)
			return (perror("error fork"), -1);
		if (pid == 0)
		{
			if (dup2(info->fd_outfile, 1) == -1)
				return (perror("error dup first"), -1);
			if (info->outfile)
				close(info->fd_outfile);
			close(pipefd[0]);
			close(pipefd[1]);
			if (exec_cmd(info, env, cmd) == 1)
				return (1);
		}
	}
	close_for_first(pipefd, info);
	return (0);
}

void gestion_file_inter(t_exec *info, int *pipefd)
{
	if (!info->infile && !info->limiteur)
		info->fd_infile = pipefd[0];
	if (!info->outfile)
		info->fd_outfile = pipefd[1];
	return ;
}

int	inter(char **cmd, t_exec *info, char **env)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (perror("error pipe"), -1);
	gestion_file_inter(info, pipefd);
	if (cmd[0] != NULL)
	{


		find_path(info->path, cmd[0], info);
		pid = fork();
		if (pid == -1)
			return (perror("error fork"), -1);
		if (pid == 0)
		{
			if (dup2(pipefd[1], 1) == -1)
				return (perror("error dup"), -1);
			close(pipefd[0]);
			close(pipefd[1]);
			if (exec_cmd(info, env, cmd) == -1)
				return (1);
		}
	}
	close(pipefd[1]);
	if (info->infile || info->limiteur)
		close(info->fd_infile);
	if (info->outfile)
		close(info->fd_outfile);
	if (info->fd_in_last_pipe)
		close(info->fd_in_last_pipe);
	info->fd_in_last_pipe = pipefd[0];
	return (0);
}

void gestion_file_last(t_exec *info)
{
	if (!info->infile && !info->limiteur)
		info->fd_infile = info->fd_in_last_pipe;
	if (!info->outfile)
		info->fd_outfile = 1;
	return ;
}

int	last(char **cmd, t_exec *info, char **env)
{
	pid_t	pid;

	gestion_file_last(info);
	if (cmd[0] != NULL)
	{
		find_path(info->path, cmd[0], info);
		pid = fork();
		if (pid == -1)
			return (perror("error fork"), -1);
		if (pid == 0)
		{
			if (dup2(info->fd_outfile, 1) == -1)
				return (perror("error dup first"), -1);
			if (info->outfile)
				close(info->fd_outfile);
			if (exec_cmd(info, env, cmd) == 1)
				return (1);
		}
	}
	if (info->infile || info->limiteur)
		close(info->fd_infile);
	if (info->outfile)
		close(info->fd_outfile);
	if (info->fd_in_last_pipe)
		close(info->fd_in_last_pipe);
	while (waitpid(-1, &g_code_exit, 0) != -1)
		;
	g_code_exit = g_code_exit / 256;
	return (0);
}

int	start_exec(char **cdm, t_exec *info, char **env)
{
	printf("cdm [0] ======= %s\n", cdm[0]);
	if (info->pos == FIRST)
	{
		printf("first");
		first(cdm, info, env);
	}
	else if(info->nb_cmd == DERNIER)
	{
		printf("last");
		last(cdm, info, env);
	}
	else if(info->pos == INTER)
	{
		printf("inter");
		inter(cdm, info, env);
	}
	info->pos = INTER;
	info->nb_cmd--;
	return (1);
}
//###################################################################
void	close_for_solo_and_free(t_exec *info)
{
	if (info->infile || info->limiteur)
		close(info->fd_infile);
	if (info->outfile)
		close(info->fd_outfile);
	if (info->path_cmd)
		free(info->path_cmd);
}

int	file_solo(t_exec *info)
{
	if (!info->infile && !info->limiteur)
		info->fd_infile = 0;
	if (!info->outfile)
		info->fd_outfile = 1;
	return (1);
}

int is_bultins_not_fork(char **cmd, char ***env, t_exec *info)
{
	t_global_parsing **info_parsing;
	t_global_exec **g_exec;

	//	g_exec = NULL;
	info_parsing = &(info->g_parsing);
	g_exec = &(info->g_parsing->exec);

	// ft_exec_bultins(cmd, env, &(info->g_parsing),  &(info->g_parsing->exec));
	if (ft_strcmp(cmd[0], "cd") == 0)
	{
		builtin_cd(cmd, env, (*g_exec)->export);
		return(0);
	}
	else if (ft_strcmp(cmd[0], "unset") == 0)
	{
		builtin_unset(cmd, env, g_exec);
		return(0);
	}
	else if (ft_strcmp(cmd[0], "exit") == 0)
	{
		builtin_exit(cmd, info_parsing);
		return(0);
	}
	else if (ft_strcmp(cmd[0], "export") == 0)
	{
		builtin_export(cmd, env, g_exec);
		return(0);
	}
	return (1);
}

int	solo_exec(char **cmd, t_exec *info, char ***env)
{
	pid_t	pid;

	if (file_solo(info) > 0)
	{
		find_path(info->path, cmd[0], info);
		{
			if (is_bultins_not_fork(cmd, env, info))
			{
				pid = fork();
				if (pid == -1)
					return (perror("error fork"), -1);
				if (pid == 0)
				{
					if (dup2(info->fd_outfile, 1) == -1)
						return (perror("error dup first"), -1);
					if (info->outfile)
						close(info->fd_outfile);
					if (exec_cmd(info, *env, cmd) == 1)
						return (1);
				}
			}
		}
	}
	while (waitpid(-1, &g_code_exit, 0) != -1)
		;
	g_code_exit = g_code_exit / 256;
	close_for_solo_and_free(info);
	return (0);
}

void	set_exec_and_start_exec_one(t_token *tokens, char **cmd, t_exec *exec, char ***env)
{
	gestion_infile(tokens, exec);
	(void)cmd;
	gestion_outfile(tokens, exec);
	if (cmd[0] == NULL)
	{
		close_for_solo_and_free(exec);
		return ;
	}
	// printf("exec->outfile = %s\n", exec->outfile);
	// printf("exec->fd_outfile = %d\n", exec->fd_outfile);
	if (solo_exec(cmd, exec, env) == -1)
		return ; // faire le destruction en cascade ou autre
}
//#######################################################################################

void	set_exec_and_start_exec(t_token *tokens, char **cmd, t_exec *exec, char **env)
{
	gestion_infile(tokens, exec);
	(void)cmd;
	gestion_outfile(tokens, exec);
	// printf("exec->outfile = %s\n", exec->outfile);
	// printf("exec->fd_outfile = %d\n", exec->fd_outfile);
	if (start_exec(cmd, exec, env) == -1)
		return ; // faire le destruction en cascade ou autre
}

int	get_path(t_exec *exec, char **envp)
{
	int	i;
	char *skip_path;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp("PATH", envp[i], 4))
			break ;
		i++;
	}
	if (envp[0] == NULL || envp[i] == NULL)
		exec->path = 0;
	else
	{
		skip_path = &envp[i][5];
		exec->path = ft_split(skip_path, ':');
	}
	return (1);
}

void	exec(t_token *tokens, t_commande *cmd, char ***env, t_global_parsing **g_pars)
{
	t_token *info_token;
	//t_token *for_print;
	t_commande *commande;
	t_exec exec;
	int i;

	i = 0;
	commande = cmd;
	get_path(&exec, *env);
	init_exec(&exec);
	exec.g_parsing = *g_pars;
	exec.nb_cmd = nb_pipe(tokens) + 1;
	if (exec.nb_cmd == 1)
	{
		info_token = get_info_token(tokens, i);
		if (!info_token)
			return ;
		set_exec_and_start_exec_one(info_token, commande->cmd, &exec, env);
		commande = commande->next;
		free_list_tokens(info_token);
	}
	while (commande)
	{
		printf("hh");
		info_token = get_info_token(tokens, i);
		if (!info_token)
			return ;
		i++;
		set_exec_and_start_exec(info_token, commande->cmd, &exec, *env);
		commande = commande->next;
		//for_print = info_token;
		/*	while (for_print)
			{
			printf("\033[1;31mtoken value = %s\n\033[0m", for_print->value);
			printf("\033[1;33mtoken type = %d\n\033[0m", for_print->info->type);
			printf("\033[1;34mtoken index = %d\n\033[0m", for_print->token_index);
			printf("\n\n");
			for_print	if (info->infile || info->limiteur)
			close(info->fd_infile);
			if (info->outfile)
			close(info->fd_outfile);
			= for_print->next;
			}
			printf("next \n\n");*/

		free_list_tokens(info_token);
		free(exec.path_cmd);
	}
	free_db_array(exec.path);
}
