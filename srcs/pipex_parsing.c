/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:17:02 by avieira-          #+#    #+#             */
/*   Updated: 2025/09/20 15:21:43 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	parse_files(char **argv, int argc)
{
	if (access(argv[1], F_OK) == 0
		&& access(argv[1], R_OK) == -1)
	{
		ft_putstr_fd("pipex_bonus: permission denied: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd("\n", 2);
	}
	if (access(argv[1], F_OK) == -1)
	{
		ft_putstr_fd("pipex_bonus: no such file or directory: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd("\n", 2);
	}
	if (access(argv[argc - 1], F_OK) == 0
		&& access(argv[argc - 1], W_OK) == -1)
	{
		ft_putstr_fd("pipex_bonus: permission denied: ", 2);
		ft_putstr_fd(argv[argc - 1], 2);
		ft_putstr_fd("\n", 2);
	}
}

void	parse_args(char **argv, int argc)
{
	if (argc != 5)
	{
		exit_error_message("Usage: ./pipex <infile> <cmd1> <cmd2> <outfile>",
			2, NULL);
	}
	parse_files(argv, argc);
}
