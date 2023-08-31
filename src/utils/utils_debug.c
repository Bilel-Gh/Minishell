/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 17:37:32 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/22 23:37:52 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;

	i = 0;
	while (str[i] != '\0')
	{
		j = 0;
		while (to_find[j] == str[i + j])
		{
			if (to_find[j + 1] == '\0')
			{
				return (str + i);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	ft_is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	verify_string(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		printf("%c", str[i]);
		i++;
	}
	printf("\n");
	if (str[i] == '\0')
	{
		printf("La chaîne se termine correctement avec \
le caractère nul '\\0' a l'index %d.\n", i);
	}
	else
	{
		printf("La chaîne ne se termine pas \
avec le caractère nul '\\0'.\n");
	}
}

void	ft_print_token_debug(t_global_parsing *g_parsing, t_token *head)
{
	head = g_parsing->tokens;
	while (g_parsing->tokens)
	{
		printf("\033[1;31mtoken value = %s\n\033[0m", g_parsing->tokens->value);
		printf("\033[1;33mtoken type = %d\n\033[0m",
			g_parsing->tokens->info->type);
		printf("\033[1;34mtoken index = %d\n\033[0m",
			g_parsing->tokens->token_index);
		if (g_parsing->tokens->prev)
			printf("\033[1;35mtoken prev value = %s\n\033[0m",
				g_parsing->tokens->prev->value);
		printf("\n\n");
		g_parsing->tokens = g_parsing->tokens->next;
	}
	g_parsing->tokens = head;
}

void	pwd_null(char ***env)
{
	char	cwd[SIZE_PATH];
	char	*home;

	if (!getcwd(cwd, SIZE_PATH))
	{
		home = ft_getenv("HOME", *env);
		if (chdir(home) != 0)
		{
			free(home);
			perror("chdir");
			g_code_exit = ERROR;
		}
	}
}
