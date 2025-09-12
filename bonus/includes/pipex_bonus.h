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

typedef struct s_pipe
{
	int				fd[2];
	int				return_value;
	struct s_pipe	*next;
}	t_pipe;

typedef struct s_cmdlinearg
{
	int		argc;
	char	**argv;
	char	**envp;
}	t_cmd_line_arg;

char	*join_command(char *dir, char *cmd);
char	**get_dirs(char **envp);
void	get_path(char **dirs, char **path, char *cmd);
void	clean_contents(char **matrix, int *pipe_fd, int exit_code);
void	exit_error_message(char *message, int error_code, char **dirs);
int		return_error_message(char *message, int error_code, char **dirs);

#endif
