/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:31:31 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/19 23:03:28 by avieira-         ###   ########.fr       */
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
		error_message(argv[1]);
	cmd_and_args = ft_split(argv[2], ' ');
	if (!cmd_and_args)
		clean_contents(dirs, pipe_fd, 2);
	get_path(dirs, &path, cmd_and_args[0]);
	ft_free_matrix(dirs);
	if (!path)
		clean_contents(cmd_and_args, pipe_fd, 127);
	dup2(pipe_fd[1], STDOUT_FILENO);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		free(path);
		clean_contents(cmd_and_args, pipe_fd, -8);
	}
	close_pipe_fd(pipe_fd);
	close(fd);
	execve(path, cmd_and_args, envp);
}

void	scnd_child_process(char **argv, char **envp, int *pipe_fd, char **dirs)
{
	char	*path;
	char	**cmd_and_args;
	int		fd;

	path = NULL;
	fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
	{
		ft_free_matrix(dirs);
		exit(1);
	}
	cmd_and_args = ft_split(argv[3], ' ');
	if (!cmd_and_args)
		clean_contents(dirs, pipe_fd, 2);
	get_path(dirs, &path, cmd_and_args[0]);
	ft_free_matrix(dirs);
	if (!path)
		clean_contents(cmd_and_args, pipe_fd, 127);
	dup2(pipe_fd[0], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close_pipe_fd(pipe_fd);
	close(fd);
	execve(path, cmd_and_args, envp);
}

int	wait_child_process(pid_t child_pid, char *specifier)
{
	int		exit_code;
	t_wait	wait;

	exit_code = 0;
	wait.pid = waitpid(child_pid, &wait.status, 0);
	exit_code = (WEXITSTATUS(wait.status));
	if (exit_code == 127 || exit_code == -8)
	{
		ft_putstr_fd("pipex_bonus: ", 2);
		ft_putstr_fd("command not found:", 2);
		ft_putstr_fd(specifier, 2);
		ft_putstr_fd("\n", 2);
	}
	return (exit_code);
}

int	pipe_commands(char **dirs, char **envp, char **argv)
{
	int		pipe_fd[2];
	pid_t	child_pid1;
	pid_t	child_pid2;

	if (pipe(pipe_fd) == -1)
		exit_error_message("Failed to create pipe", -1, dirs);
	if (access(argv[1], F_OK ) == 0)
	{
		child_pid1 = fork();
		if (child_pid1 == -1)
			exit_error_message("Failed to fork", -1, dirs);
		if (child_pid1 == 0)
			child_process(argv, envp, pipe_fd, dirs);
	}
	child_pid2 = fork();
	if (child_pid2 == -1)
		exit_error_message("Failed to fork", -1, dirs);
	if (child_pid2 == 0)
		scnd_child_process(argv, envp, pipe_fd, dirs);
	close_pipe_fd(pipe_fd);
	if (access(argv[1], F_OK ) == 0)
		wait_child_process(child_pid1, argv[2]);
	return (wait_child_process(child_pid2, argv[3]));
}

void	parse_files(char **argv, int argc)
{
	if (access(argv[1], F_OK) == 0
			&& access(argv[1], R_OK | W_OK | X_OK) == -1)
	{
		ft_putstr_fd("pipex_bonus: permission denied: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd("\n", 2);
	}
	if (access(argv[1], F_OK) == -1)
	{
		ft_putstr_fd("pipex_bonus: no such file or directory: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd("\n", 2);
	}
	if (access(argv[argc -1], F_OK) == 0
			&& access(argv[argc - 1], R_OK | W_OK) == -1)
	{
		ft_putstr_fd("pipex_bonus: permission denied: ", 2);
		ft_putstr_fd(argv[argc - 1], 2);
		ft_putstr_fd("\n", 2);
	}
	if ((access(argv[argc -1], F_OK) == 0
				&& access(argv[argc - 1], R_OK) == -1))
		exit(1);
}

void	parse_args(char **argv, int argc)
{
	if (argc != 5)
	{
		exit_error_message("Usage: ./pipex <infile> <cmd1> <cmd2> <outfile>",
				2, NULL);
	}
	parse_files(argv, argc);
}


int	main(int argc, char **argv, char **envp)
{
	char	**dirs;
	int		exit_code;

	parse_args(argv, argc);
	dirs = get_dirs(envp);
	exit_code = pipe_commands(dirs, envp, argv);
	ft_free_matrix(dirs);
	return (exit_code);
}
