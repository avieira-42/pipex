/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_parsing_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:27:45 by avieira-          #+#    #+#             */
/*   Updated: 2025/09/22 00:32:39 by a-soeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	parse_files(char **argv, int argc, t_bool here_doc)
{
	if (access(argv[1], F_OK) == 0 && here_doc == FALSE
		&& access(argv[1], R_OK) == -1)
	{
		ft_putstr_fd("pipex_bonus: permission denied: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd("\n", 2);
	}
	if (access(argv[1], F_OK) == -1 && here_doc == FALSE)
	{
		ft_putstr_fd("pipex_bonus: no such file or directory: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd("\n", 2);
	}
	if ((access(argv[argc -1], F_OK) == 0)
		&& access(argv[argc - 1], W_OK) == -1)
	{
		ft_putstr_fd("pipex_bonus: permission denied: ", 2);
		ft_putstr_fd(argv[argc - 1], 2);
		ft_putstr_fd("\n", 2);
	}
}

void	parse_args(char **argv, int argc, t_bool *here_doc)
{
	if (argc > 4)
		*here_doc = ft_bool_strcmp("here_doc", argv[1]);
	if (argc < 5 || (argc < 6 && *here_doc == TRUE))
	{
		ft_putstr_fd("Usage: ./pipex <infile> <cmd1> ... <cmdn> <outfile>", 2);
		ft_putstr_fd("\nOR\n", 2);
		ft_putstr_fd("./pipex here_doc <limiter> <cmd1> ... <cmdn> <file>", 2);
		ft_putstr_fd("\n", 2);
		exit(1);
	}
	parse_files(argv, argc, *here_doc);
}
