/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_clearing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:19:10 by avieira-          #+#    #+#             */
/*   Updated: 2025/09/20 17:26:59 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	clean_contents(char **matrix, int *pipe_fd, int exit_code, int fd)
{
	if (matrix)
		ft_free_matrix(matrix);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	if (fd != -1)
		close(fd);
	exit(exit_code);
}

int	close_pipe_fd(int *pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (-1);
}
