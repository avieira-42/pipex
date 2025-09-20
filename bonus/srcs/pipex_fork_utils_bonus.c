/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_fork_utils_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:32:40 by avieira-          #+#    #+#             */
/*   Updated: 2025/09/20 15:58:18 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

int	wait_child_process(pid_t child_pid, char *specifier)
{
	int		exit_code;
	t_wait	wait;

	exit_code = 0;
	wait.pid = waitpid(child_pid, &wait.status, 0);
	exit_code = (WEXITSTATUS(wait.status));
	if (exit_code == 127)
	{
		ft_putstr_fd("pipex_bonus: ", 2);
		ft_putstr_fd("command not found: ", 2);
		ft_putstr_fd(specifier, 2);
		ft_putstr_fd("\n", 2);
	}
	return (exit_code);
}

void	mid_chld_pro(t_args param, t_pipe *pipe_lst, t_utils utils, int n)
{
	char	*path;
	char	**cmd_and_args;

	path = NULL;
	utils.pipe_node = pipe_lst;
	cmd_and_args = ft_split(param.argv[n], ' ');
	if (!cmd_and_args)
		clean_contents(utils.dirs, utils, param.argv, -2);
	get_path(utils.dirs, &path, cmd_and_args[0]);
	ft_free_matrix(utils.dirs);
	if (!path)
		clean_contents(cmd_and_args, utils, param.argv, 127);
	dup2(pipe_lst->fd[0], STDIN_FILENO);
	if (dup2(pipe_lst->next->fd[1], STDOUT_FILENO) == -1)
		clean_contents(cmd_and_args, utils, param.argv, -2);
	pipe_list_free(pipe_lst);
	execve(path, cmd_and_args, param.envp);
}

t_pipe	*loop_middle_child(t_args param, t_pipe *pipe_list, t_utils util)
{
	int		n;
	pid_t	child_pid_middle;

	n = -1;
	while (++n < param.argc - 5)
	{
		child_pid_middle = fork();
		if (child_pid_middle == -1)
			ft_putstr_fd("Need to handle loop middle_child fork failure", 2);
		if (child_pid_middle == 0)
			mid_chld_pro(param, pipe_list, util, n + 3);
		close_pipe_fd(pipe_list);
		pipe_list = pipe_list->next;
	}
	return (pipe_list);
}

void	loop_wait_pid_middle(t_pipe *pipe_lst, t_args args)
{
	int	n;

	n = 0;
	while (++n < args.argc - 5)
	{
		wait_child_process(pipe_lst->wait.pid, args.argv[n + 3]);
		pipe_lst = pipe_lst->next;
	}
}
