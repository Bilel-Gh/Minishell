/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 23:07:43 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/23 23:08:02 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

bool	back_slach(char *args, int i)
{
	int	nb_back_s;

	nb_back_s = 0;
	if (i == 0)
		return (true);
	i = i - 1;
	while (args[i] == 92)
	{
		i--;
		nb_back_s++;
		if (i < 0)
			break ;
	}
	if (nb_back_s % 2 == 0)
		return (true);
	else
		return (false);
}
