/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 03:32:42 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/12 03:37:03 by a-soeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	parameter_init(int argc, char **argv, char **envp, t_cmdlinearg *param)
{
	parameter->argc = argc;
	parameter->argv = argv;
	parameter->envp = envp;
}
