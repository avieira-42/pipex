/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:39:35 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/20 17:36:59 by avieira-         ###   ########.fr       */
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
# define INFILE ".user_infile"
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
	t_wait			wait;
	struct s_pipe	*next;
}	t_pipe;

typedef struct s_args
{
	int		argc;
	char	**argv;
	char	**envp;
}	t_args;

typedef struct s_utils
{
	int		fd;
	char	**dirs;
	t_bool	here_doc;
	t_pipe	*pipe_node;
}	t_utils;

// pipe_clearing_bonus.c
int		close_pipe_fd(t_pipe *pipe_node);
void	error_message(char *specifier);
void	exit_last(t_utils utils, t_pipe *pipe_list, char **argv, int exit_code);
void	clean_contents(char **matrix, t_utils util, char **av, int exit_code);
void	exit_error_message(char *message, int error_code,
			char **dirs, t_pipe *pipe_list);

// pipe_init_bonus.c
void	utils_init(t_utils *utils, char **dirs, t_bool here_doc);
void	parameter_init(int argc, char **argv, char **envp, t_args *param);

// pipe-list_bonus.c
t_pipe	*pipe_list_node_new(void);
void	pipe_list_free(t_pipe *list);
void	pipe_list_add_back(t_pipe *pipe_list, t_pipe *pipe_new);
t_pipe	*pipe_list_create(int argc);
t_pipe	*pipe_list_last(t_pipe *pipe_list);

// pipex_fork_utils_bonus.c
int		wait_child_process(pid_t child_pid, char *specifier);
void	loop_wait_pid_middle(t_pipe *pipe_lst, t_args args);
t_pipe	*loop_middle_child(t_args param, t_pipe *pipe_list, t_utils util);

// pipex_parsing_bonus.c
void	parse_files(char **argv, int argc, t_bool here_doc);
void	parse_args(char **argv, int argc, t_bool *here_doc);

// pipex_utils_bonus.c
char	*join_command(char *dir, char *cmd);
void	get_path(char **dirs, char **path, char *cmd);
char	**get_dirs(char **envp);
void	execute_program(char **envp, char **cmd_and_args, char *path);

// here_doc_bonus.c
void	here_doc_setup(int *argc, char ***argv);

#endif
