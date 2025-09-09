/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:31:31 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/10 00:16:52 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

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
		clean_contents(&dirs, pipe_fd, 1);
	cmd_and_args = ft_split(argv[2], ' ');
	if (!cmd_and_args)
		clean_contents(&dirs, pipe_fd, 2);
	get_path(dirs, &path, cmd_and_args[0]);
	ft_free_matrix(dirs);
	if (!path)
		clean_contents(&cmd_and_args, pipe_fd, 3);
	dup2(pipe_fd[1], STDOUT_FILENO);
	dup2(fd, STDIN_FILENO);
	close_pipe_fd(pipe_fd);
	close(fd);
	execve(path, cmd_and_args, envp);
	free(path);
	exit(4);
}

void	midle_child_process(char *argv, char **envp, int *pipe_fd, char **dirs)
{
	char	*path;
	char	**cmd_and_args;

	path = NULL;
	cmd_and_args = ft_split(argv[2], ' ');
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
	close_pipe_fd(pipe_fd);
	close(fd);
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
	return (0);
}

t_bool	ft_bool_strcmp(char *str1, char *str2)
{
	if (ft_strcmp(argv1, "here_doc") == 0)
		return TRUE;
	return FALSE;
}

void	parse_args(int argc, char **argv)
{
	if (argc < 5 || argc < 6 && is_here_doc == TRUE)
		return (error_message("Usage: ./pipex <infile> <cmd1> <cmd2> <outfile>\n
					OR\n ./pipex here_doc <limiter> <cmd1> <cmd2> <file>",
				   	-1, &dirs));
	exit(1);
}

void	pipe_array_generate()
{
}

void	pipe_commands()
{
}

int	main(int argc, char **argv, char **envp)
{
	int		n;
	int		**pipe_fd;
	char	**dirs;
	t_bool	here_doc;

	here_doc = ft_bool_strcmp("here_doc", argv[1]);
	parse_args(argc, argv, here_doc);
	get_dirs(envp, &dirs);
//
	// pipe_fd = pipe_array_generate(argc, here_doc);
	int		i;
	int		n;
	
	i = 0;
	if (here_doc == TRUE)
		n = argc - 4;
	else
		n = argc - 3;
	**pipe_fd = malloc(sizeof(int) * n);
	if (pipe_fd == NULL)
		// exit message
	while (i < n)
	{
		*pipe_fd = malloc(sizeof(int) * 2);
		pipe(pipe_fd[i]);
		i++;
	}
	return (pipe_fd);
	
//
	// pipe_commands();
	int			i;
	pid_t		child_pid;
	t_wait		wait;

	i = 1;
	while (i < argc - 2)
	{
		child_pid = fork();
		if (child_pid == -1)
			return (error_message("Failed to fork", -1, &dirs));
		if (child_pid == 0 && i > 1)
			midle_child_process(argv[i], envp, pipe_fd, dirs);
		else if (child_pid == 0 && i == 1)
			child_process(argv, envp, pipe_fd, dirs);
		i++;
	}
	child_pid = fork();
	if (wait_child_process(child_pid, &wait, &dirs) != 0)
		return (close_pipe_fd(pipe_fd));
	parent_process(argv, envp, pipe_fd, dirs);
	ft_free_matrix(dirs);
	ft_printf("Success");
}
