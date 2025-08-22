/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:31:31 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/08/22 03:10:44 by a-soeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	child_process(char **argv, char **envp, int *pipe_fd, char **dirs)
{
	char	*path;
	char	**cmd_and_args;
	int		fd;

	path = NULL;
	fd = open(argv[1], O_RDONLY, 0777);
	if (fd == -1)
		exit(1);
	cmd_and_args = ft_split(argv[2], ' ');
	if (!cmd_and_args)
		clean_contents(&dirs, pipe_fd, 2);
	get_path(dirs, &path, cmd_and_args[0]);
	ft_free_matrix(dirs);
	if (!path)
		clean_contents(&cmd_and_args, pipe_fd, 3);
	dup2(pipe_fd[1], STDOUT_FILENO);
	dup2(fd, STDIN_FILENO);
	close(pipe_fd[0]);
	execve(path, cmd_and_args, envp);
	free(path);
	exit(4);
}

void	parent_process(char **argv, char **envp, int *pipe_fd, char **dirs)
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
		clean_contents(&dirs, pipe_fd, 2);
	get_path(dirs, &path, cmd_and_args[0]);
	ft_free_matrix(dirs);
	if (!path)
		clean_contents(&cmd_and_args, pipe_fd, 3);
	dup2(pipe_fd[0], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(pipe_fd[1]);
	execve(path, cmd_and_args, envp);
	free(path);
	exit(4);
}

int	wait_child_process(pid_t child_pid, t_wait *wait, char ***dirs)
{
	int	exit_code;

	exit_code = 0;
	wait->pid = waitpid(child_pid, &wait->status, 0);
	if (WIFEXITED(wait->status))
		exit_code = WEXITSTATUS(wait->status);
	if (exit_code == 1)
		return (error_message("Failed to get cmd1 path", -1, dirs));
	else if (exit_code == 2)
		return (error_message("Failed to execute cmd1", -1, dirs));
	else if (exit_code == 3)
		return (error_message("Cant find cmd1", -1, dirs));
	else if (exit_code == 4)
		return (error_message("Failed to open infile", -1, dirs));
}

int	main(int argc, char **argv, char **envp)
{
	int		pipe_fd[2];
	char	**dirs;
	pid_t	child_pid;
	t_wait	wait;

	dirs = NULL;
	if (argc != 5)
		return (error_message("Usage: ./pipex infile cmd1 cmd2 outfile",
				-1, &dirs));
	get_dirs(envp, &dirs);
	if (!dirs)
		return (error_message("Failed to get path directories", -1, &dirs));
	if (pipe(pipe_fd) == -1)
		return (error_message("Failed to create pipe", -1, &dirs));
	child_pid = fork();
	if (child_pid == -1)
		return (error_message("Failed to fork", -1, &dirs));
	if (child_pid == 0)
		child_process(argv, envp, pipe_fd, dirs);
	if (wait_child_process(child_pid, &wait, &dirs) == -1)
		return (-1);
	parent_process(argv, envp, pipe_fd, dirs);
	ft_free_matrix(dirs);
	ft_printf("Success");
}
