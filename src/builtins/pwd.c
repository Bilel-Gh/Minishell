/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 22:51:14 by bghandri          #+#    #+#             */
/*   Updated: 2023/06/17 22:51:15 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void builtin_pwd()
{
    char cwd[SIZE_PATH]; // Taille maximale du chemin d'accès = 4096 octets

    if (getcwd(cwd, SIZE_PATH))
        printf("%s\n", cwd);
    else
        perror("pwd");
}