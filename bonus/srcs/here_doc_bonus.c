/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 23:21:19 by avieira-          #+#    #+#             */
/*   Updated: 2025/09/20 16:15:35 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

char	*get_user_infile_valid_name(void)
{
	long	n;
	char	*copy_number;
	char	*valid_infile;
	char	*invalid_infile;

	valid_infile = ft_strdup(INFILE);
	n = 0;
	while (access(valid_infile, F_OK) == 0)
	{
		n++;
		copy_number = ft_itoa(n);
		invalid_infile = valid_infile;
		valid_infile = ft_strjoin(INFILE, copy_number);
		free(invalid_infile);
		free(copy_number);
	}
	return (valid_infile);
}

void	argv_recreate(int *argc, char ***argv, char *user_infile_name)
{
	int		n;
	char	**argv_new;

	(*argc)--;
	argv_new = malloc(sizeof(char *) * ((*argc + 1)));
	n = 0;
	while (n < *argc)
	{
		if (n == 1)
			argv_new[n] = user_infile_name;
		else
			argv_new[n] = ft_strdup((*argv)[n + 1]);
		if (argv_new[n] == NULL)
			exit_error_message("Failed to recreate argv", 1, argv_new, NULL);
		n++;
	}
	argv_new[n] = NULL;
	*argv = argv_new;
}

void	user_input_read(char *limiter, char *user_infile_name)
{
	int		user_input_fd;
	char	*line;

	user_input_fd = open(user_infile_name, O_CREAT | O_WRONLY | O_APPEND, 0777);
	if (user_input_fd == -1)
		exit_error_message("Failed to create user input file", 3, NULL, NULL);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			break ;
		if (*line == '\0' || ft_bool_strcmp(line, limiter) == TRUE)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, user_input_fd);
		free(line);
	}
	free(limiter);
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
	char	*user_infile_name;

	user_infile_name = get_user_infile_valid_name();
	limiter = limiter_add_new_line((*argv)[2]);
	user_input_read(limiter, user_infile_name);
	argv_recreate(argc, argv, user_infile_name);
}
