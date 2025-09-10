/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:31:31 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/10 23:39:39 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	close_pipe_fd(int *pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (-1);
}

void	child_process(char **argv, char **envp, int *pipe_fd, char **dirs)
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

void	scnd_child_process(char **argv, char **envp, int *pipe_fd, char **dirs)
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

void	pipe_commands(char **dirs, char **envp, char **argv)
{
	int		pipe_fd[2];
	pid_t	child_pid1;
	pid_t	child_pid2;

	if (pipe(pipe_fd) == -1)
		exit_error_message("Failed to create pipe", -1, dirs);
	child_pid1 = fork();
	if (child_pid1 == -1)
		exit_error_message("Failed to fork", -1, dirs);
	if (child_pid1 == 0)
		child_process(argv, envp, pipe_fd, dirs);
	if (wait_child_process(child_pid1) != 0)
		exit_error_message("Failed to fork first process", -1, dirs);
	child_pid2 = fork();
	if (child_pid2 == -1)
		exit_error_message("Failed to fork", -1, dirs);
	if (child_pid2 == 0)
		scnd_child_process(argv, envp, pipe_fd, dirs);
	close_pipe_fd(pipe_fd);
	if (wait_child_process(child_pid2) != 0)
		exit_error_message("Failed to fork second process", -1, dirs);
}

int	main(int argc, char **argv, char **envp)
{
	char	**dirs;

	if (argc != 5)
		exit_error_message("Usage: ./pipex infile cmd1 cmd2 outfile", -1, NULL);
	dirs = get_dirs(envp);
	pipe_commands(dirs, envp, argv);
	ft_free_matrix(dirs);
}
