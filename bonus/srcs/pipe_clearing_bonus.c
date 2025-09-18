/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_clearing_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 14:35:40 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/18 20:14:43 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

int	close_pipe_fd(t_pipe *pipe_node)
{
	close(pipe_node->fd[0]);
	close(pipe_node->fd[1]);
	pipe_node->is_closed = TRUE;
	return (-1);
}

void	error_message(char *specifier)
{
	ft_putstr_fd("pipex_bonus: ", 2);
	ft_putstr_fd("file not found: ", 2);
	ft_putstr_fd(specifier, 2);
	ft_putstr_fd("\n", 2);
}

void    exit_error_message(char *message, int error_code,
		char **dirs, t_pipe *pipe_list)
{
	if (dirs)
		ft_free_matrix(dirs);
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	if (pipe_list)
		pipe_list_free(pipe_list);
	exit(error_code);
}

void	clean_contents(char **matrix, t_utils util, char **av, int	exit_code)
{
	if (util.here_doc == TRUE)
		ft_free_matrix(av);
	if (matrix)
		ft_free_matrix(matrix);
	if (util.pipe_node)
		pipe_list_free(util.pipe_node);
	if (util.fd != -1 && util.fd != 0)
		close(util.fd);
	exit(exit_code);
}

void	exit_last(t_utils utils, t_pipe *pipe_list, char **argv, int exit_code)
{
	pipe_list_free(pipe_list);
	ft_free_matrix(utils.dirs);
	if (utils.here_doc == TRUE)
	{
		unlink(argv[1]);
		ft_free_matrix(argv);
	}
	exit (exit_code);
}
