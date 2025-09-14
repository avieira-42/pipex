/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_init_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 03:32:42 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/14 23:20:19 by a-soeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	parameter_init(int argc, char **argv, char **envp, t_shellarg *param)
{
	param->argc = argc;
	param->argv = argv;
	param->envp = envp;
}
