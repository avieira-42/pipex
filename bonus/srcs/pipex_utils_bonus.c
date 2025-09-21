/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:31:31 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/21 14:46:37 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

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
		exit_error_message("Failed to get path directories", -1, dirs, NULL);
	return (dirs);
}

void	execute_program(char **envp, char **cmd_and_args, char *path,
		t_utils utils)
{
	execve(path, cmd_and_args, envp);
	free(path);
	ft_free_matrix(cmd_and_args);
	if (utils.here_doc == TRUE)
		ft_free_matrix(utils.argv);
	exit(1);
}
