/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */ /*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 03:08:38 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/10 16:58:42 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "../libs/libft/include/libft.h"

typedef struct s_wait
{
	int		status;
	int		exit_code;
	pid_t	pid;
}	t_wait;

char	*join_command(char *dir, char *cmd);
char	**get_dirs(char **envp);
void	get_path(char **dirs, char **path, char *cmd);
void	clean_contents(char **matrix, int *pipe_fd, int exit_code);
void	exit_error_message(char *message, int error_code, char **dirs);
int		return_error_message(char *message, int error_code, char **dirs);

#endif
