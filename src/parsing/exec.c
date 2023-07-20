/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:21:17 by ncharii           #+#    #+#             */
/*   Updated: 2023/07/20 01:34:35 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

typedef struct exec
{
	char *infile;
	char *outfile;
	int fd_infile;
	int fd_outfile;
	int fd_in_last_pipe;
	int fd_heredoc;
	char *limiteur;
	int pos;
	int nb_cmd;
	int i;
	int path_input;
	char **path;
	char *path_cmd;
	t_global_parsing *g_parsing;
} t_exec;

void init_exec(t_exec *exec)
{
	exec->infile = NULL;
	exec->outfile = NULL;
	exec->limiteur = NULL;
	exec->path_cmd = NULL;
	exec->fd_infile = 0;
	exec->fd_outfile = 1;
	exec->fd_in_last_pipe = -1;
	exec->fd_heredoc = -1;
	exec->path_input = 0;
	exec->pos = FIRST;
}

int nb_token_whis_index(t_token *tokens, int index)
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

t_token *creat_info_token_list(t_token *tokens, int nb_node)
{
	int i;
	t_token *tmp;
	t_token *for_creat;
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
	return (head);
}

void copy_cont_token(t_token *dest, t_token *src)
{
	dest->token_index = src->token_index;
	dest->value = ft_strdup(src->value);
	dest->info = (t_token_info *)malloc(sizeof(t_token_info));
	dest->info->type = src->info->type;
	dest->prev = NULL;
	// TODO gestion d errerur a faire et
	// il faudrait meme reflechir  aune autre facon de faire
}

t_token *dup_info(t_token *info_token, t_token *tokens, int index)
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

// ########################################################################
bool is_token_redi_in(t_token *token)
{
	if (token->info->type == REDIRECT_IN || token->info->type == REDIRECT_D_IN)
		return (true);
	return (false);
}

void new_infile(t_exec *exec, t_token *token)
{
	if (exec->infile)
	{
		close(exec->fd_infile);
		free(exec->infile);
		exec->infile = NULL;
	}
	if (exec->limiteur)
	{
		close(exec->fd_infile);
		free(exec->limiteur);
		unlink("/tmp/here_doc_minishell");
		exec->limiteur = NULL;
	}
	exec->infile = ft_strdup(token->value);
	if (!exec->infile)
		return;
	exec->fd_infile = open(exec->infile, O_RDONLY);
	if (exec->fd_infile == -1)
		perror(exec->infile);
}

void start_heredoc(t_exec *exec, int exit_code)
{
	char *line;
	char ***env;

	env = exec->g_parsing->env;
	g_code_exit = CHILD;
	line = NULL;
	exec->fd_infile = open("/tmp/here_doc_minishell",
						   O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (exec->fd_infile == -1)
	{
		printf(" error heredoc\n");
		perror("open");
		return;
	}
	g_code_exit = g_code_exit + exec->fd_infile;
	while (1)
	{
		line = readline("> ");
		if (!line || g_code_exit == CSIGINT)
		{
			ft_fprintf(2, "bash: warning: here-document delimited by end-of-file (wanted `%s')", exec->limiteur);
			g_code_exit = SUCCESS;
			break;
		}
		if (!ft_strcmp(line, exec->limiteur))
		{
			g_code_exit = SUCCESS;
			break;
		}
		else if (ft_strncmp(line, "$?", 2) == 0)
			line = exit_code_expande(line, exit_code);
		else
			line = importe_expande(line, *env);
		write(exec->fd_infile, line, ft_strlen(line));
		write(exec->fd_infile, "\n", 1);
		free(line);
	}
	free(line);
	close(exec->fd_infile);
	ft_fprintf(2, " limiteur = %s \n", exec->limiteur);
	free(exec->limiteur);
	free_db_array(exec->path);
	exit(exec->fd_infile);
}

int new_heredoc(t_exec *exec, t_token *token)
{
	int info;
	int tmp_error;
	pid_t pid;

	info = 0;
	tmp_error = g_code_exit;
	if (exec->infile)
	{
		close(exec->fd_infile);
		free(exec->infile);
		exec->infile = NULL;
	}
	if (exec->limiteur)
	{
		close(exec->fd_infile);
		free(exec->limiteur);
		unlink("/tmp/here_doc_minishell");
		exec->limiteur = NULL;
	}
	exec->limiteur = ft_strdup(token->value);
	if (!exec->limiteur)
		return (-1);
	else
	{
		g_code_exit = FORK;
		pid = fork();
		if (pid == -1)
			return (perror("error fork"), -1);
		if (pid == 0)
			start_heredoc(exec, tmp_error);
		printf("pid fils = %d\n", pid);
		while (waitpid(pid, &info, 0) == -1 && info != 32718)
			;
		printf(" le fils a terminer\n");
		g_code_exit = tmp_error;
		if (info > 255)
			info = info / 256;
		printf("%d\n", info);
		if (info == 0)
			return (g_code_exit = 130, -1);
		exec->fd_infile = open("/tmp/here_doc_minishell", O_RDONLY);
		if (exec->fd_infile == -1)
			perror("error open heredoc");
	}
	return (0);
}

int set_new_infile(t_exec *exec, t_token *tokens)
{
	t_token *token;

	token = tokens;
	if (token->info->type == REDIRECT_IN)
		new_infile(exec, token->next);
	else
	{
		if (new_heredoc(exec, token->next) == -1)
			return (-1);
	}
	return (0);
}

int gestion_infile(t_token *tokens, t_exec *exec)
{
	t_token *seach_tok_in;
	int info;

	info = 0;
	seach_tok_in = tokens;
	while (seach_tok_in)
	{
		if (exec->fd_infile == -1)
			break;
		if (is_token_redi_in(seach_tok_in))
			info = set_new_infile(exec, seach_tok_in);
		if (info == -1)
			return (-1);
		seach_tok_in = seach_tok_in->next;
	}
	return (0);
}

void set_new_outfile(t_exec *exec, t_token *token)
{
	if (exec->outfile)
	{
		close(exec->fd_outfile);
		free(exec->outfile);
	}
	if (token->next)
		exec->outfile = ft_strdup(token->next->value);
	if (token->info->type == REDIRECT_OUT)
	{
		exec->fd_outfile = open(exec->outfile,
								O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (exec->fd_outfile == -1)
			perror(exec->outfile);
	}
	else if (token->info->type == REDIRECT_D_OUT)
	{
		exec->fd_outfile = open(exec->outfile,
								O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (exec->fd_outfile == -1)
			perror(exec->outfile);
	}
}

bool is_token_redi_out(t_token *token)
{
	if (token->info->type == REDIRECT_OUT || token->info->type == REDIRECT_D_OUT)
		return (true);
	return (false);
}

void gestion_outfile(t_token *tokens, t_exec *exec)
{
	t_token *seach_tok_out;

	seach_tok_out = tokens;
	while (seach_tok_out)
	{
		if (exec->fd_outfile == -1)
			break;
		if (is_token_redi_out(seach_tok_out))
			set_new_outfile(exec, seach_tok_out);
		seach_tok_out = seach_tok_out->next;
	}
}

char *for_test(char *test, char *argv)
{
	char *str;
	int i;
	int y;

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
	while (argv[y])
	{
		str[i] = argv[y];
		i++;
		y++;
	}
	str[i] = '\0';
	return (str);
}

int find_path(char **path, char *cmd, t_exec *info)
{
	char *test;

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

int add_one(int *is_bultin)
{
	*is_bultin = 1;
	return (1);
}

int ft_bultins_fork(char **cmd, char ***env, t_exec *info)
{
	int is_bultin;
	t_global_parsing **info_parsing;
	t_global_exec **g_exec;

	info_parsing = &(info->g_parsing);
	g_exec = &(info->g_parsing->exec);
	is_bultin = 0;
	g_exec = &(info->g_parsing->exec);
	if ((ft_strcmp(cmd[0], "cd") == 0) && add_one(&is_bultin))
		g_code_exit = builtin_cd(cmd, env);
	if (ft_strcmp(cmd[0], "echo") == 0 && add_one(&is_bultin))
		builtin_echo(cmd);
	else if (ft_strcmp(cmd[0], "pwd") == 0 && add_one(&is_bultin))
		builtin_pwd(cmd);
	else if (ft_strcmp(cmd[0], "exit") == 0 && add_one(&is_bultin))
		builtin_exit(cmd, info_parsing);
	else if (ft_strcmp(cmd[0], "env") == 0 && add_one(&is_bultin))
		builtin_env(cmd, *env);
	else if (ft_strcmp(cmd[0], "export") == 0 && add_one(&is_bultin))
		builtin_export(cmd, env, g_exec);
	return (is_bultin);
}

int add_zero(int *info_return)
{
	*info_return = 0;
	return (1);
}

int is_bultins_not_fork(char **cmd, char ***env, t_exec *info, int pos)
{
	t_global_parsing **info_parsing;
	t_global_exec **g_exec;
	int info_return;

	info_return = 1;
	info_parsing = &(info->g_parsing);
	g_exec = &(info->g_parsing->exec);
	if ((ft_strcmp(cmd[0], "cd") == 0) && (pos == DERNIER) && add_zero(&info_return))
		g_code_exit = builtin_cd(cmd, env);
	else if (ft_strcmp(cmd[0], "unset") == 0 && add_zero(&info_return))
		builtin_unset(cmd, env, g_exec);
	else if (ft_strcmp(cmd[0], "exit") == 0 && add_zero(&info_return) && (pos == DERNIER))
		builtin_exit(cmd, info_parsing);
	else if ((ft_strcmp(cmd[0], "export") == 0) && (cmd[1] != NULL) && add_zero(&info_return) && (pos == DERNIER))
		builtin_export(cmd, env, g_exec);
	return (info_return);
}
void null_cmd(t_exec *info)
{
	ft_fprintf(2, "bash: : command not found\n");
	g_code_exit = NOTFOUND;
	free(info->path_cmd);
	ft_free_g_parsing_total(info->g_parsing);
	printf("errno = %d\n", errno);
	return (exit(g_code_exit));
}

int exec_cmd(t_exec *info, char ***env, char **cmd)
{
	int exec_bultins;
	g_code_exit = CSIGINT;
	char *path;

	path = ft_getenv("PATH", *env);
	if (info->fd_infile != 0)
	{
		dup2(info->fd_infile, 0);
		close(info->fd_infile);
	}
	exec_bultins = ft_bultins_fork(cmd, env, info);
	if (exec_bultins != 0)
	{
		free(info->path_cmd);
		ft_free_g_parsing_total(info->g_parsing);
		printf("errno = %d\n", errno);
		printf("\033[1;35mERRNO BUILTINS = %d\n\033[0m", errno);
		return (exit(g_code_exit), -1);
	}
	if (cmd[0] == NULL)
		null_cmd(info);
	//printf ("cmd[0] before execve == %s \n",info->path_cmd);
	if (execve(info->path_cmd, cmd, *env) == -1)
	{
		free(info->path_cmd);
		if (path == NULL)
		{
			ft_fprintf(2, "bash: %s: No such file or directory\n", cmd[0]);
			g_code_exit = NOTFOUND;
		}
		else
			ft_check_error_exec(cmd);
		printf("\033[1;33mERRNO EXEC = %d\n\033[0m", errno);
		ft_free_g_parsing_total(info->g_parsing);
		free(path);
		return (exit(g_code_exit), -1);
	}
	printf("in exec_cmd !! \n");
	g_code_exit = SUCCESS;
	return (1);
}

void close_for_first(int *pipefd, t_exec *info)
{
	close(pipefd[1]);
	if ((info->infile || info->limiteur) && info->fd_infile != 0)
	{
		printf("close infile first= %d\n", info->fd_infile);
		close(info->fd_infile);
	}
	if (info->outfile && info->fd_outfile != 1)
	{
		printf("close outfile first= %d\n", info->fd_outfile);
		close(info->fd_outfile);
	}
	info->fd_in_last_pipe = pipefd[0];
}

int creat_pipe_and_file(t_exec *info, int *pipefd)
{
	int error;

	error = 1;
	if (pipe(pipefd) == -1)
	{
		perror("error pipe");
		error = -1;
	}
	if (info->fd_infile == -1 || info->fd_outfile == -1)
	{
		g_code_exit = 1;
		error = -1;
	}
	if ((!info->infile && !info->limiteur) || info->fd_infile == -1)
	{
		info->fd_infile = 0;
	}
	if (!info->outfile || info->fd_outfile == -1)
		info->fd_outfile = pipefd[1];
	if (error == -1)
		close_for_first(pipefd, info);
	return (error);
}

int dup_in_fork_first_inter(t_exec *info, int *pipefd)
{
	if (dup2(info->fd_outfile, 1) == -1)
		return (perror("error dup first"), -1);
	if (info->outfile)
		close(info->fd_outfile);
	close(pipefd[0]);
	close(pipefd[1]);
	return (0);
}

int first(char **cmd, t_exec *info, char ***env)
{
	int pipefd[2];
	pid_t pid;

	if (creat_pipe_and_file(info, pipefd) < 0)
		return (-1);
	if (cmd[0] != NULL)
	{
		find_path(info->path, cmd[0], info);
		if (is_bultins_not_fork(cmd, env, info, FIRST))
		{
			g_code_exit = FORK;
			pid = fork();
			if (pid == -1)
				return (perror("error fork"), -1);
			if (pid == 0)
			{
				if (dup_in_fork_first_inter(info, pipefd) == -1)
					return (-1);
				if (exec_cmd(info, env, cmd) == 1)
					return (1);
			}
		}
	}
	close_for_first(pipefd, info);
	return (0);
}

void close_inter(int *pipefd, t_exec *info)
{
	if ((info->infile || info->limiteur) && (info->fd_infile != info->fd_in_last_pipe) && (info->fd_infile != 0))
		close(info->fd_infile);
	if (info->outfile && info->fd_outfile != pipefd[1])
		close(info->fd_outfile);
	close(pipefd[1]);
	if (info->fd_in_last_pipe)
		close(info->fd_in_last_pipe);
	info->fd_in_last_pipe = pipefd[0];
}

int gestion_file_inter(t_exec *info, int *pipefd)
{
	int error;

	error = 1;
	if (pipe(pipefd) == -1)
		perror("error pipe");
	if (info->fd_infile == -1 || info->fd_outfile == -1)
	{
		g_code_exit = 1;
		error = -1;
	}
	if ((!info->infile && !info->limiteur) || info->fd_infile == -1)
		info->fd_infile = info->fd_in_last_pipe;
	if (!info->outfile || info->fd_outfile == -1)
		info->fd_outfile = pipefd[1];
	if (error == -1)
		close_inter(pipefd, info);
	return (error);
}

int inter(char **cmd, t_exec *info, char ***env)
{
	int pipefd[2];
	pid_t pid;

	if (gestion_file_inter(info, pipefd) < 0)
		return (-1);
	if (cmd[0] != NULL)
	{
		find_path(info->path, cmd[0], info);
		if (is_bultins_not_fork(cmd, env, info, INTER))
		{
			g_code_exit = FORK;
			pid = fork();
			if (pid == -1)
				return (perror("error fork"), -1);
			if (pid == 0)
			{
				if (dup_in_fork_first_inter(info, pipefd) == -1)
					return (-1);
				if (exec_cmd(info, env, cmd) == -1)
					return (1);
			}
		}
	}
	close_inter(pipefd, info);
	return (0);
}

int gestion_file_last(t_exec *info)
{
	int error;

	error = 1;
	if (info->fd_infile == -1 || info->fd_outfile == -1)
	{
		g_code_exit = 1;
		error = -1;
	}
	if ((!info->infile && !info->limiteur) || info->fd_infile == -1)
	{
		info->fd_infile = info->fd_in_last_pipe;
	}
	if (!info->outfile || info->fd_outfile == -1)
		info->fd_outfile = 1;
	return (error);
}

int close_last(t_exec *info)
{
	if ((info->infile || info->limiteur) && info->fd_infile != 0)
		close(info->fd_infile);
	if (info->outfile && info->fd_outfile != 1)
		close(info->fd_outfile);
	if (info->fd_in_last_pipe)
		close(info->fd_in_last_pipe);
	while (waitpid(-1, &g_code_exit, 0) != -1)
		;
	printf(" g_code_exit sortie de wait = %d", g_code_exit);
	if (g_code_exit == 2)
		g_code_exit = CSIGINT; // ! TODO ERROR code d'erreur modifie builtins
	if (g_code_exit > 255)
		g_code_exit = g_code_exit / 256;
	return (1);
}

int last(char **cmd, t_exec *info, char ***env)
{
	pid_t pid;

	if (gestion_file_last(info) < 0 && close_last(info))
		return (-1);
	if (cmd[0] != NULL)
	{
		find_path(info->path, cmd[0], info);
		if (is_bultins_not_fork(cmd, env, info, INTER))
		{
			g_code_exit = FORK;
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
	}
	close_last(info);
	return (0);
}

int start_exec(char **cdm, t_exec *info, char ***env)
{
	if (info->pos == FIRST)
		first(cdm, info, env);
	else if (info->nb_cmd == DERNIER)
		last(cdm, info, env);
	else if (info->pos == INTER)
		inter(cdm, info, env);
	info->pos = INTER;
	info->nb_cmd--;
	return (1);
}

void close_for_solo_and_free(t_exec *info)
{
	while (waitpid(-1, &g_code_exit, 0) != -1 || g_code_exit == 355)
		;
	printf(" g_code_exit sortie de wait = %d", g_code_exit);
	if (g_code_exit == 2) // ! TODO ERROR code d'erreur modifie builtins
		g_code_exit = CSIGINT;
	if (g_code_exit > 255)
		g_code_exit = g_code_exit / 256;
	if ((info->infile || info->limiteur) && info->fd_infile > 0)
		close(info->fd_infile);
	if (info->outfile && info->fd_outfile != 1)
		close(info->fd_outfile);
	if (info->path_cmd)
		free(info->path_cmd);
}

int file_solo(t_exec *info)
{
	int error;

	error = 1;
	if (info->fd_infile == -1 || info->fd_outfile == -1)
	{
		g_code_exit = 1;
		error = -1;
	}
	if ((!info->infile && !info->limiteur) || info->fd_infile == -1)
		info->fd_infile = 0;
	if (!info->outfile || info->fd_outfile == -1)
		info->fd_outfile = 1;
	return (error);
}

int solo_exec(char **cmd, t_exec *info, char ***env)
{
	pid_t pid;

	if (file_solo(info) < 0)
	{
		close_for_solo_and_free(info);
		return (-1);
	}
	find_path(info->path, cmd[0], info);
	if (is_bultins_not_fork(cmd, env, info, DERNIER))
	{
		g_code_exit = FORK;
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
				return (-1);
		}
	}
	close_for_solo_and_free(info);
	return (0);
}

int start_exec_one(t_token *tokens, char **cmd, t_exec *exec, char ***env)
{
	int info;

	info = 0;
	info = gestion_infile(tokens, exec);
	if (info == -1)
		return (-1);
	gestion_outfile(tokens, exec);
	// if (cmd[0] == NULL)
	// {
	// 	file_solo(exec);
	// 	close_for_solo_and_free(exec);
	// 	return (0);
	// }
	if (solo_exec(cmd, exec, env) == -1)
		return (-2);
	return (info);
}

int start_exec_mult(t_token *tokens, char **cmd, t_exec *exec, char ***env)
{
	int info;

	info = 0;
	info = gestion_infile(tokens, exec);
	if (info == -1)
		return (-1);
	gestion_outfile(tokens, exec);
	if (start_exec(cmd, exec, env) == -1)
		return (-2);
	return (info);
}

int get_path(t_exec *exec, char **envp)
{
	int i;
	char *skip_path;

	i = 0;
	if (envp == NULL)
	{
		exec->path = 0; // debug
	}
	while (envp[i])
	{
		if (!ft_strncmp("PATH", envp[i], 4))
			break;
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

void free_name_file(t_exec *exec)
{
	if (exec->infile)
	{
		free(exec->infile);
		exec->infile = NULL;
	}
	if (exec->outfile)
	{
		free(exec->outfile);
		exec->outfile = NULL;
	}
	if (exec->limiteur)
	{
		free(exec->limiteur);
		exec->limiteur = NULL;
	}
}
void ft_free_g_parsing_total(t_global_parsing *g_parsing)
{
	//    rl_clear_history();
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
	if (g_parsing->exec)
	{
		if (g_parsing->exec->export)
			free_db_array(g_parsing->exec->export);
		free(g_parsing->exec);
	}
	if (*(g_parsing->env))
		free_db_array(*(g_parsing->env));
}
void exec(t_token *tokens, t_commande *cmd, char ***env, t_global_parsing **g_pars)
{
	t_token *info_token;
	t_commande *commande;
	t_exec exec;
	int i;

	i = 0;
	commande = cmd;
	get_path(&exec, *env);
	init_exec(&exec);
	exec.g_parsing = *g_pars;
	exec.nb_cmd = nb_pipe(tokens) + 1;
	printf("nb_exec = %d\n", exec.nb_cmd);
	if (exec.nb_cmd == 1)
	{
		info_token = get_info_token(tokens, i);
		if (!info_token)
			return;
		if (start_exec_one(info_token, commande->cmd, &exec, env) == -1)
		{
			free_list_tokens(info_token);
			free_name_file(&exec);
			free_db_array(exec.path);
			ft_free_g_parsing_total(*g_pars);
			exit(NOTFOUND);
			return;
		}
		commande = commande->next;
		free_list_tokens(info_token);
		free_name_file(&exec);
	}
	else
	{
		while (commande)
		{
			info_token = get_info_token(tokens, i);
			if (!info_token)
				return;
			i++;
			if (start_exec_mult(info_token, commande->cmd, &exec, env) == -1)
			{
				free_list_tokens(info_token);
				return;
			}
			commande = commande->next;
			free_list_tokens(info_token);
			if (exec.path_cmd)
			{
				free(exec.path_cmd);
				exec.path_cmd = NULL;
			}
			free_name_file(&exec);
		}
	}
	free_db_array(exec.path);
}
