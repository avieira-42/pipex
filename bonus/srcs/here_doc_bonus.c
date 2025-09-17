/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 23:21:19 by avieira-          #+#    #+#             */
/*   Updated: 2025/09/17 13:35:58 by a-soeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	argv_recreate(int *argc, char ***argv)
{
	int		n;
	char	**argv_new;

	(*argc)--;
	argv_new = malloc(sizeof(char *) * ((*argc) + 1));
	n = 0;

	while (n < *argc)
	{
		if (n == 1)
			argv_new[n] = ft_strdup(USER_INFILE_NAME);
		else
		{
			argv_new[n] = ft_strdup((*argv)[n]);
			if (argv_new[n] == NULL)
				ft_free_matrix(argv_new);
		}
		n++;
	}
	*argv = argv_new;
}

void	user_input_read(char *limiter)
{
	int		user_input_fd;
	char	*line;

	user_input_fd = open(USER_INFILE_NAME, O_CREAT | O_APPEND | 0777);
	if (user_input_fd == -1
		exit_error_message("Failed to create user input file", 3, NULL, NULL);
	line = ft_strdup(FIRST_LINE);
	if (line == NULL)
	{
		close(user_input_fd);
		free(limiter);
		exit_error_message("Failed to create FIRST_LINE", 4, NULL, NULL);
	}
	while(line && ft_bool_strcmp(line, limiter) == FALSE)
	{
		free(line);
		line = get_next_line(0);
		ft_putstr_fd(line, user_input_fd);
	}
	free(line);
	close(user_input_fd);
}

char	*limiter_add_new_line(char *limiter)
{
	char	*limiter_new;

	limiter_new = ft_strjoin(limiter, NEW_LINE);
	if (limiter_new == NULL)
		exit_error_message("Failed to create limiter_new", 2, NULL, NULL);
	return (limiter_new);
}

void	here_doc_setup(int *argc, char ***argv)
{
	char	*limiter;

	limiter = limiter_add_new_line((*argv)[2]);
	user_input_read(limiter);
	argv_recreate(argc, argv);
}
