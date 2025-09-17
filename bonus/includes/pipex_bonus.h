/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:39:35 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/17 14:46:25 by a-soeiro         ###   ########.fr       */
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

# define NEW_LINE "\n"
# define INFILE "user_infile"
# define FIRST_LINE	"An string to be allocated then freed right away"

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
	t_bool			is_closed;
	struct s_pipe	*next;
}	t_pipe;

typedef struct s_shellarg
{
	int		argc;
	char	**argv;
	char	**envp;
}	t_shellarg;

int		close_pipe_fd(t_pipe *pipe_node);
char	*join_command(char *dir, char *cmd);
char	**get_dirs(char **envp);
void	get_path(char **dirs, char **path, char *cmd);
void	clean_contents(char **matrix, t_pipe *pipe_list, int exit_code);
void	exit_error_message(char *message, int error_code,
		char **dirs, t_pipe *pipe_list);
int		return_error_message(char *message, int error_code,
		char **dirs, t_pipe *pipe_list);
void	parameter_init(int argc, char **argv, char **envp, t_shellarg *param);
t_pipe	*pipe_list_node_new(void);
void	pipe_list_free(t_pipe *list);
void	pipe_list_add_back(t_pipe *pipe_list, t_pipe *pipe_new);
t_pipe	*pipe_list_create(int argc);
t_pipe	*pipe_list_last(t_pipe *pipe_list);
void	here_doc_setup(int *argc, char ***argv);
void	user_input_read(char *limiter);
void	argv_recreate(int *argc, char ***argv);

#endif
