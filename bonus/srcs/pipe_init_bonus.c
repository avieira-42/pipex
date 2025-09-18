/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_init_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 03:32:42 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/18 12:45:31 by a-soeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	parameter_init(int argc, char **argv, char **envp, t_args *param)
{
	param->argc = argc;
	param->argv = argv;
	param->envp = envp;
}

void	utils_init(t_utils *utils, char **dirs, t_bool here_doc)
{
	utils->fd = 0;
	utils->here_doc = here_doc;
	utils->dirs = dirs;
}
