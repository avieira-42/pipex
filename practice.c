/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   practice.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 02:44:21 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/08/05 16:08:55 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/wait.h>
#include "libs/libft/include/libft.h"
#include "fcntl.h"

typedef struct s_cmd
{
	int		i;
	int		j;
	char	**env;
	char	**dirs;
	char	**args;
	char	*path;
	pid_t	pid;
}	t_cmd;

// CHANGE TO GET ENV (BROADER FUNCTION FOR FUTURE USE)
char	*get_path_env(char **envp)
{
	int i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH", 4) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}
// TO MAKE SPRINTF to libft
char *join_command(char *dir, char *cmd) {
	char *path;
	char *tmp;

	path = ft_strjoin(dir, "/");
	if (!path)
		return (NULL);
	tmp = path;
	path = ft_strjoin(path, cmd);
	free(tmp);
	if (!path)
		return(path);
	return (path);
}

char *get_path(char **dirs, char *cmd)
{
	int i;
	int	fd;
	char *path;

	i = 0;
	while (dirs[i])
	{
		path = join_command(dirs[i++], cmd);
		fd = open(path, O_RDONLY);
		if (fd > 2)
			return (close(fd), path);
		free(path);
	}
	return (NULL);
}

int	ft_matrix_size(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		i++;
	return (i);
}

void	ft_realloc_matrix(char ***split, int size)
{
	ft_free_matrix
	*split = malloc((sizeof(char *) * (size + 1)));
}

void	join_argv(char ***split)
{
	int		i;
	int		size;
	char	**split_ptr;

	i = 0;
	size = ft_matrix_size(*split);
	split_ptr = ft_matrixdup(*split);
	ft_realloc_matrix(split);
	while (i < size)
	{
	}
	ft_free_matrix(split_ptr);
}

int	pipex_init(t_cmd *cmd1, t_cmd *cmd2, char **argv, char **env)
{
	cmd1->i = 0;
	cmd1->j = 1;
	cmd2->i = 1;
	cmd2->j = 0;
	cmd1->args = ft_split(argv[1], ' ');
	cmd2->args = ft_split(argv[4], ' ');
	if (!cmd1->args || !cmd2->args)
		return (0);
	cmd1->env = env;
	cmd2->env = env;
	cmd1->path = get_path_env(env);
	cmd2->path = cmd1->path;
	cmd1->dirs = ft_split(cmd1->path, ':');
	if (!cmd1->dirs)
		return (0);
	cmd2->dirs = cmd1->dirs;
	cmd1->path = get_path(cmd1->dirs, argv[1]);
	if (!cmd1->path)
		return (ft_free_matrix(cmd1->dirs), 0);
	cmd2->path = get_path(cmd2->dirs, argv[2]);
	if (!cmd2->path)
		return (free(cmd1->path), ft_free_matrix(cmd2->dirs), 0);
	return (1);
}

int	get_pid(int *fd, t_cmd *cmd)
{
	cmd->pid = fork();
	if (cmd->pid == -1)
		return (0);
	if (cmd->pid == 0)
	{
		close (fd[cmd->i]);
		if (cmd->j == 1)
			dup2(fd[cmd->j], STDOUT_FILENO);
		else if (cmd->j == 0)
			dup2(fd[cmd->j], STDIN_FILENO);
		close(fd[cmd->j]);
		execve(cmd->args, cmd->path, cmd->env);
		perror("execve failed");
		exit(1);
	}
	return (1);
}

void	free_cmd_structs(t_cmd cmd1, t_cmd cmd2)
{
	ft_free_matrix(cmd1.dirs);
	free(cmd1.path);
	free(cmd2.path);
	ft_free_matrix(cmd1.args);
	ft_free_matrix(cmd2.args);
}

int main(int argc, char **argv, char **env)
{
	int		error_ctrl;
	int		fd[2];
	t_cmd	cmd1;
	t_cmd	cmd2;

	if (argc != 5)
		return (1);
	error_ctrl = pipex_init(&cmd1, &cmd2, argv, env);
	if (!error_ctrl)
		return (2);
	error_ctrl = pipe(fd);
	if (error_ctrl == -1)
		return (3);
	if (!get_pid(fd, &cmd1) || !get_pid(fd, &cmd2))
		return (4);
	close(fd[0]);
	close(fd[1]);
	waitpid(cmd1.pid, NULL, 0);
	waitpid(cmd2.pid, NULL, 0);
	free_cmd_structs(cmd1, cmd2);
}
