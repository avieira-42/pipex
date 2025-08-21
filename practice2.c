/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   practice2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:31:31 by a-soeiro          #+#    #+#             *//*   Updated: 2025/08/21 17:17:35 by a-soeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "libs/libft/include/libft.h"

int	error_message(char *message, int error_code, char ***dirs)
{
	if (*dirs)
		ft_free_matrix(*dirs);
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	return (error_code);
}

char	*join_command(char *dir, char *cmd)
{
	char *path;
	char *tmp;

	path = ft_strjoin(dir, "/");
	if (!path)
		return (NULL);
	tmp = path;
	path = ft_strjoin(path, cmd);
	free(tmp);
	return (path);
}

void	get_path(char **dirs, char **path, char *cmd)
{
	int		i;
	int		fd;
	char	*tmp;

	*path = NULL;
	tmp = NULL;
	i = 0;
	while (dirs[i])
	{
		tmp = join_command(dirs[i++], cmd);
		if (!tmp)
			break ;
		if (access(tmp, F_OK) == 0)
		{
			*path = tmp;
			break ;
		}
		free(tmp);
	}
}

void	get_dirs(char **envp, char ***dirs)
{
	int		i;
	char	*path;

	i = 0;
	path = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH", 4) == 0)
			path = envp[i] + 5;
		i++;
	}
	if (path)
		*dirs = ft_split(path, ':');
}

void	child_process(char **argv, char **envp, int *pipe_fd, char **dirs)
{
	char	*path;
	char	**cmd_and_args;
	int		wait_status;
	int		fd;

	fd = open(argv[1], O_RDONLY);
	if (fd = -1)
		return ;
	cmd_and_args = ft_split(argv[2], ' ');
	if (!cmd_and_args)
	{
		ft_free_matrix(dirs);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		exit(1);
	}
	printf ("%s\n", cmd_and_args[0]);
	get_path(dirs, &path, cmd_and_args[0]);
	ft_free_matrix(dirs);
	if (!path)
	{
		ft_free_matrix(cmd_and_args);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		exit(2);
	}
	printf("%s\n", path);
	dup2(pipe_fd[1], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(pipe_fd[0]);
	execve(path, cmd_and_args, envp);
	free(path);
	exit(3);
}

void	parent_process(char **argv, char **envp, int *pipe_fd, char **dirs)
{
	char	*path;
	char	**cmd_and_args;
	int		wait_status;
	int		fd;

	fd = open(
	cmd_and_args = ft_split(argv[3], ' ');
	if (cmd_and_args)
	{
		ft_free_matrix(dirs);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		exit(1);
	}
	printf ("%s\n", cmd_and_args[0]);
	get_path(dirs, &path, cmd_and_args[0]);
	ft_free_matrix(dirs);
	if (!path)
	{
		ft_free_matrix(cmd_and_args);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		exit(2);
	}
	printf("%s\n", path);
	close(pipe_fd[0]);
	dup2(pipe_fd[0], STDOUT_FILENO);
	close(pipe_fd[1]);
	execve(path, cmd_and_args, envp);
	free(path);
	exit(3);
}

int	main(int argc, char **argv, char **envp)
{
	int		exit_code;
	int		pipe_fd[2];
	int		wait_status;
	char	**dirs;
	pid_t	child_pid;
	pid_t	wait_pid;

	dirs = NULL;
	if (argc != 4)
		return (error_message(
			"Usage: ./pipex \"infile\" \"cmd1\" \"cmd2\" \"outfile\"", 1,
			&dirs));
	exit_code = 0;
	get_dirs(envp, &dirs);
	if (!dirs)
		return (error_message("Failed to get path directories", 2, &dirs));
	if (pipe(pipe_fd) == -1)
		return (error_message("Failed to create pipe", 3, &dirs));
	child_pid = fork();
	if (child_pid == -1)
		return (error_message("Failed to fork", 4, &dirs));
	if (child_pid == 0)
		child_process(argv, envp, pipe_fd, dirs);
	parent_process(argv, envp, pipe_fd, dirs);
	wait_pid = waitpid(child_pid, &wait_status, 0);
	if (WIFEXITED(wait_status));
	exit_code = WEXITSTATUS(wait_status);
	if (exit_code == 1)
		return (error_message("Failed to get cmd1 path", 5, &dirs));
	else if (exit_code == 2)
		return (error_message("Failed to execute cmd1", 6, &dirs));
	ft_free_matrix(dirs);
	ft_printf("Success");
}
