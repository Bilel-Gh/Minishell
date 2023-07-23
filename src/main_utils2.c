/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 00:22:15 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/24 00:22:24 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_custom_error(char **args)
{
	if (g_code_exit == ERROR_PIPE)
	{
		ft_fprintf(2, "bash: syntax error near unexpected token '%c'\n", '|');
		return (1);
	}
	if (g_code_exit == ERROR_REDIRECT)
	{
		ft_print_error_redirect(args);
		return (1);
	}
	if (g_code_exit == ERROR_QUOTE_D)
	{
		ft_fprintf(2, "bash: command not found (unclosed quote) : '\"'\n");
		return (1);
	}
	if (g_code_exit == ERROR_QUOTE_S)
	{
		ft_fprintf(2, "bash: command not found (unclosed quote) : '\''\n");
		return (1);
	}
	return (0);
}

void	ft_print_error_redirect(char **args)
{
	if (ft_db_arr_len(args) == 1 && ft_strlen(args[0]) <= 2)
		ft_fprintf(2, "bash: syntax error near unexpected token 'newline'\n");
	else if (args[0][0] == '>' || args[0][0] == '<')
		ft_fprintf(2, "bash: syntax error near unexpected token '%c%c'\n",
			args[0][0], args[0][1]);
	else
		ft_fprintf(2, "bash: syntax error near unexpected token '>'\n");
}

int	ft_general_error(t_global_parsing *g_parsing)
{
	if (ft_strlen(g_parsing->line) == 0 || ft_strcmp(g_parsing->line, ":") == 0
		|| ft_strcmp(g_parsing->line, ";") == 0
		|| is_only_space(g_parsing->line) == 1)
	{
		g_code_exit = SUCCESS;
		return (1);
	}
	return (0);
}

void	ft_do_exec(char ***env, t_global_parsing *g_parsing, int nb_args)
{
	g_parsing->tokens = ft_get_tokens_with_infos(g_parsing->args, nb_args);
	g_parsing->commande = cmd_complete(g_parsing->tokens);
	ft_set_index_for_exec(&g_parsing->tokens);
	// DEBUG
	t_token	*head;
	head = NULL;
	FT_PRINT_TOKEN_DEBUG(g_parsing, head);
	// DEBUG
	exec(g_parsing->tokens, g_parsing->commande, env, &g_parsing);
	rl_replace_line("", 0);
}

void	ft_do_parsing(char ***env, t_global_parsing *g_parsing, int *nb_args)
{
	g_parsing->args = ft_lexeur(g_parsing->line);
	g_parsing->info_args = ft_get_info_args(g_parsing->args, nb_args);
	g_parsing->args = ft_parsing(nb_args, &g_parsing, env);
}
