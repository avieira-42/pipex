/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:31:31 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/08/22 03:45:06 by a-soeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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

void	clean_contents(char ***matrix, int *pipe_fd, int exit_code)
{
	ft_free_matrix(*matrix);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	exit(exit_code);
}
