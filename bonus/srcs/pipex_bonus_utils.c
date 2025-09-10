/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:31:31 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/10 20:46:57 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	exit_error_message(char *message, int error_code, char **dirs)
{
	if (dirs)
		ft_free_matrix(dirs);
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	exit(error_code);
}

int	return_error_message(char *message, int error_code, char **dirs)
{
	if (dirs)
		ft_free_matrix(dirs);
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	return (error_code);
}

char	*join_command(char *dir, char *cmd)
{
	char	*path;
	char	*tmp;

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

char	**get_dirs(char **envp)
{
	int		i;
	char	*path;
	char	**dirs;

	i = 0;
	path = NULL;
	dirs = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH", 4) == 0)
			path = envp[i] + 5;
		i++;
	}
	dirs = ft_split(path, ':');
	if (dirs == NULL)
		exit_error_message("Failed to get path directories", -1, dirs);
	return (dirs);
}

void	clean_contents(char **matrix, int *pipe_fd, int exit_code)
{
	if (matrix)
		ft_free_matrix(matrix);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	exit(exit_code);
}
