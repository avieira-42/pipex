/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   practice2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:31:31 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/08/10 03:48:14 by a-soeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "libs/libft/include/libft.h"

int	error_message(char *message, int error_code)
{
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

	*path = NULL;
	i = 0;
	while (dirs[i])
	{
		*path = join_command(dirs[i++], cmd);
		if (access(*path, F_OK) == 0)
			return ;
	}
}

void	get_dirs(char **envp, char ***dirs)
{
	int		i;
	char	*path;

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

void	child_process(char **argv, char **envp, int *pipe_fd, int argc, char **dirs)
{
	char	*path;
	char	**cmd_and_args;
	int		wait_status;

	cmd_and_args = ft_split(argv[1], ' ');
	printf ("%s\n", cmd_and_args[0]);
	get_path(dirs, &path, cmd_and_args[0]);
	if (!path)
		exit(1);
	printf("%s\n", path);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	execve(path, cmd_and_args, envp);
	exit(2);
}

int	main(int argc, char **argv, char **envp)
{
	int		exit_code;
	int		pipe_fd[2];
	int		wait_status;
	char	**dirs;
	pid_t	child_pid;
	pid_t	wait_pid;

	//if (argc != 4)
	//return (error_message("Usage: ./pipex infile cmd1 cm2 outfile", 1));
	get_dirs(envp, &dirs);
	if (!dirs)
		return (error_message("Failed to get path directories", 2));
	if (pipe(pipe_fd) == -1)
	{
		ft_free_matrix(dirs);
		return (error_message("Failed to create pipe", 3));
	}
	child_pid = fork();
	if (child_pid == -1)
		return (error_message("Failed to fork", 4));
	if (child_pid == 0)
		child_process(argv, envp, pipe_fd, argc, dirs);
	wait_pid = waitpid(child_pid, &wait_status, 0);
	if (WIFEXITED(wait_status));
		exit_code = WEXITSTATUS(wait_status);
	if (exit_code == 1)
		return (error_message("Failed to get cmd1 path", 5));
	else if (exit_code == 2)
		return (error_message("Failed to execute cmd1", 6));
	else
		ft_printf("Success");
	return (0);
}
