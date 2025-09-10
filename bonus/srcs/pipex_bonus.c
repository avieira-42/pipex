/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:31:31 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/11 00:18:58 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	close_pipe_fd(int *pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (-1);
}

void	first_child_process(char **argv, char **envp, int *pipe_fd, char **dirs)
{
	char	*path;
	char	**cmd_and_args;
	int		fd;

	path = NULL;
	fd = open(argv[1], O_RDONLY, 0777);
	if (fd == -1)
		clean_contents(dirs, pipe_fd, 1);
	cmd_and_args = ft_split(argv[2], ' ');
	if (!cmd_and_args)
		clean_contents(dirs, pipe_fd, 2);
	get_path(dirs, &path, cmd_and_args[0]);
	ft_free_matrix(dirs);
	if (!path)
		clean_contents(cmd_and_args, pipe_fd, 3);
	dup2(pipe_fd[1], STDOUT_FILENO);
	dup2(fd, STDIN_FILENO);
	close_pipe_fd(pipe_fd);
	close(fd);
	execve(path, cmd_and_args, envp);
	free(path);
	exit(4);
}

void	final_child_process(char **argv, char **envp, int *pipe_fd, char **dirs)
{
	char	*path;
	char	**cmd_and_args;
	int		fd;

	path = NULL;
	fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
		exit(1);
	cmd_and_args = ft_split(argv[3], ' ');
	if (!cmd_and_args)
		clean_contents(dirs, pipe_fd, 2);
	get_path(dirs, &path, cmd_and_args[0]);
	ft_free_matrix(dirs);
	if (!path)
		clean_contents(cmd_and_args, pipe_fd, 3);
	dup2(pipe_fd[0], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close_pipe_fd(pipe_fd);
	close(fd);
	execve(path, cmd_and_args, envp);
	free(path);
	exit(4);
}

int	*middle_child_process(char *argv, char **envp, int *pipe_fd, char **dirs)
{
	int		n;
	int		pipe_fd_last[2];
	char    *path;
	char    **cmd_and_args;

	path = NULL;
	cmd_and_args = ft_split(argv[2], ' ');
	if (!cmd_and_args)
		clean_contents(dirs, pipe_fd, 2);
	get_path(dirs, &path, cmd_and_args[0]);
	return (pipe_fd_last);
}

int	wait_child_process(pid_t child_pid)
{
	int		exit_code;
	t_wait	wait;

	exit_code = 0;
	wait.pid = waitpid(child_pid, &wait.status, 0);
	if (WIFEXITED(wait.status))
		exit_code = (WEXITSTATUS(wait.status));
	if (exit_code == 1)
		ft_putstr_fd("Failed to get cmd path\n", 2);
	else if (exit_code == 2)
		ft_putstr_fd("Failed to execute cmd\n", 2);
	else if (exit_code == 3)
		ft_putstr_fd("Cant find cmd\n", 2);
	else if (exit_code == 4)
		ft_putstr_fd("Failed to open infile\n", 2);
	return (exit_code);
}

void	pipe_commands(char **dirs, char **envp, char **argv, int argc)
{
	int		pipe_fd[2];
	int		*pipe_fd_last;
	pid_t	child_pid_1;
	pid_t	child_pid_last;

	if (pipe(pipe_fd) == -1)
		exit_error_message("Failed to create pipe", -1, dirs);
	child_pid1 = fork();
	if (child_pid_1 == -1)
		exit_error_message("Failed to fork", -1, dirs);
	if (child_pid_1 == 0)
		first_child_process(argv, envp, pipe_fd, dirs);
	if (wait_child_process(child_pid_1) != 0)
		exit_error_message("Failed to fork first process", -1, dirs);

	while ( n < argc - 5) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
						  
		pipe_fd_last = middle_child_process();
	child_pid_last = fork();
	if (child_pid_last == -1)
		exit_error_message("Failed to fork", -1, dirs);
	if (child_pid_last == 0)
		final_child_process(argv, envp, pipe_fd, dirs);
	close_pipe_fd(pipe_fd);
	if (wait_child_process(child_pid_last) != 0)
		exit_error_message("Failed to fork last process", -1, dirs);
}

void	parse_args(int argc, char **argv)
{
	if (argc < 5 || argc < 6 && is_here_doc == TRUE)
		exit_error_message("Usge: ./pipex <infile> <cmd1> ... <cmdn> <outfile>\n
				OR\n
				./pipex here_doc <limiter> <cmd1> <cmd2> <file>\n", NULL);
}

void	handle_here_doc()
{
	ft_putstr_fd("HANDLE HERE_DOC!!", 2);
}

int	main(int argc, char **argv, char **envp)
{
	int		n
		int		fd_pipe[2];
	char	**dirs;
	pid_t   pid;
	t_bool	here_doc;

	here_doc = ft_bool_strcmp("here_doc", argv[1]);
	parse_args(argc, argv, here_doc);
	dirs = get_dirs(envp);
	if (here_doc == TRUE)
		handle_here_doc();
	pipe_commands(dirs, envp, argv, argc);
	ft_free_matrix(dirs);
}
