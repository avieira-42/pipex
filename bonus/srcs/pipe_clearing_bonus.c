/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_clearing_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 14:35:40 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/14 23:07:05 by a-soeiro         ###   ########.fr       */
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

void	exit_error_message(char *message, int error_code,
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

int	return_error_message(char *message, int error_code,
		char **dirs, t_pipe *pipe_list)
{
	if (dirs)
		ft_free_matrix(dirs);
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	if (pipe_list)
		pipe_list_free(pipe_list);
	return (error_code);
}

void	clean_contents(char **matrix, t_pipe *pipe_list, int exit_code)
{
	if (matrix)
		ft_free_matrix(matrix);
	if (pipe_list)
		pipe_list_free(pipe_list);
	exit(exit_code);
}
