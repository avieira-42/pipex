/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:57:47 by avieira-          #+#    #+#             */
/*   Updated: 2025/09/02 09:25:31 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	**cmd_dirs_get(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH", 4) == 0)
		{
			return (ft_split(envp[i] + 5, ':'));
		}
		i++;
	}
	return (NULL);
}

char	*path_get(char *cmd, char **dirs)
{
	int		i;
	char	*tmp;
	char	*path;
	
	if (!cmd)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		if (!tmp)
			return (NULL);
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!path)
			return (NULL);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

void	process_parent(char **argv, char **envp, int *fd_pipe, char **cmd_dirs)
{
	int		fd;
	char	*path;
	char	**cmd_and_args;

	fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
	{
		ft_free_matrix(cmd_dirs);
		exit(1);
	}
	cmd_and_args = ft_split(argv[3], ' ');
	if (!cmd_and_args)
		exit(2);
	path = path_get(cmd_and_args[0], cmd_dirs);
	ft_free_matrix(cmd_dirs);
	if (!path)
	{
		ft_free_matrix(cmd_and_args);
		exit(3);
	}
	dup2(fd_pipe[0], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd_pipe[1]);
	execve(path, cmd_and_args, envp);
	ft_free_matrix(cmd_and_args);
	free(path);
}

void	process_child(char **argv, char **envp, int *fd_pipe, char **cmd_dirs)
{
	int		fd;
	char	*path;
	char	**cmd_and_args;

	fd = open(argv[1], O_RDONLY, 0777);
	if (fd == -1)
	{
		ft_free_matrix(cmd_dirs);
		exit(1);
	}
	cmd_and_args = ft_split(argv[2], ' ');
	if (!cmd_and_args)
	{
		exit(2);
		ft_free_matrix(cmd_dirs);
	}
	path = path_get(cmd_and_args[0], cmd_dirs);
	ft_free_matrix(cmd_dirs);
	if (!path)
	{
		ft_free_matrix(cmd_and_args);
		exit(3);
	}
	dup2(fd_pipe[1], STDOUT_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd_pipe[0]);
	execve(path, cmd_and_args, envp);
	ft_free_matrix(cmd_and_args);
	free(path);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd_pipe[2];
	char	**cmd_dirs;
	pid_t	pid;

	if (argc < 5 || !(argc % 2))
		return (1);
	if (pipe(fd_pipe) == -1)
		return (2);
	cmd_dirs = cmd_dirs_get(envp);
	if (!cmd_dirs)
		return (3);
	pid = fork();
	if (pid == -1)
	{
		ft_free_matrix(cmd_dirs);
		return (4);
	}
	if (pid == 0)
		process_child(argv, envp, fd_pipe, cmd_dirs);
	waitpid(pid, NULL, 0);
	process_parent(argv, envp, fd_pipe, cmd_dirs);
}
