/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 03:08:38 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/09 22:55:14 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "../../libs/libft/include/libft.h"

typedef struct s_wait
{
	int		status;
	int		exit_code;
	pid_t	pid;
}	t_wait;

int		error_message(char *message, int error_code, char ***dirs);
char	*join_command(char *dir, char *cmd);
void	get_path(char **dirs, char **path, char *cmd);
void	get_dirs(char **envp, char ***dirs);
void	clean_contents(char ***matrix, int *pipe_fd, int exit_code);

#endif
