/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 00:25:49 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/24 00:25:57 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_init_global_parsing(t_global_parsing *g_parsing)
{
	if (!g_parsing)
		g_parsing = NULL;
	g_parsing->args = NULL;
	g_parsing->info_args = NULL;
	g_parsing->tokens = NULL;
	g_parsing->commande = NULL;
}
