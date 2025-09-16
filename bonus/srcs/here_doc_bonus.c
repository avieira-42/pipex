/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 23:21:19 by avieira-          #+#    #+#             */
/*   Updated: 2025/09/17 00:14:00 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	cmd_line_args_recreate(int *argc, char ***argv, char *user_input)
{
	int		n;
	char	**cmd_line_args;

	(*argc)--;
	cmd_line_args = malloc(sizeof(char *) * *argc);
	n = 0;

	// alloc for 0 to be argv[0]
	// alloc for 1 to be user_input file
	// alloc for rest on while loop maybe 
	while (n < *argc)
	{
		if (n = 0
		cmd_line_args[n] = malloc(sizeof(char) * ft_strlen(argv[n + 3]);

		n++;
	}

	*argv = cmd_line_args;
}

void	user_input_read(

void	here_doc_setup(int *argc, char ***argv)
{
	user_input_read(
	cmd_line_args_recreate(argc, argv);
}
