/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_list_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 02:51:49 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/14 19:28:01 by a-soeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

t_pipe	*pipe_list_node_new(void)
{
	t_pipe	*pipe_node;

	pipe_node = malloc (sizeof(t_pipe));
	if (pipe_node == NULL)
		return (NULL);
	pipe_node->return_value = pipe(pipe_node->fd);
	pipe_node->next = NULL;
	pipe_node->is_closed = FALSE;
	return (pipe_node);
}

void	pipe_list_free(t_pipe *list)
{
	t_pipe	*tmp;
	t_pipe	*last;

	tmp = list;
	last = pipe_list_last(list);
	while (list != last)
	{
		tmp = list->next;
		if (list->is_closed == FALSE)
			close_pipe_fd(list->fd);
		free(list);
		list = tmp;
	}
}

t_pipe	*pipe_list_last(t_pipe *pipe_list)
{
	t_pipe	*firs;
	t_pipe	*last;

	first = pipe_list;
	last = pipe_list;
	pipe_list = pipe_list->next;
	while (pipe_list != first)
	{
		last = pipe_list;
		pipe_list = pipe_list->next;
	}
	return last;
}

void	pipe_list_add_back(t_pipe *pipe_list, t_pipe *pipe_new)
{
	pipe_list = pipe_list_last(pipe_list);
	pipe_new->next = pipe_list;
	pipe_list->next = pipe_new;
}

t_pipe	*pipe_list_create(int argc)
{
	int		n;
	t_pipe	*pipe_list;
	t_pipe	*pipe_node_new;
	t_pipe	*pipe_iterator;

	pipe_list = NULL;
	n = 0;
	while (n < argc - 4)
	{
		pipe_node_new = pipe_list_node_new();
		if (pipe_node_new->return_value == -1)
			pipe_list_free(pipe_list);
		if (pipe_list == NULL)
			pipe_list = pipe_node_new;
		else
			pipe_list_add_back(pipe_list, pipe_node_new);
		n++;
	}
	return (pipe_list);
}
