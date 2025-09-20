/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:31:31 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/20 15:18:58 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	error_message(char *specifier)
{
	ft_putstr_fd("pipex_bonus: ", 2);
	ft_putstr_fd("file not found: ", 2);
	ft_putstr_fd(specifier, 2);
	ft_putstr_fd("\n", 2);
}

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
