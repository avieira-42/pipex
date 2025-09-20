/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:31:31 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/20 17:42:28 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	frst_chld_proc(t_args param, t_pipe *pipe_node, t_utils utils)
{
	char	*path;
	char	**cmd_and_args;

	path = NULL;
	utils.pipe_node = pipe_node;
	utils.fd = open(param.argv[1], O_RDONLY, 0777);
	cmd_and_args = ft_split(param.argv[2], ' ');
	if (!cmd_and_args)
		clean_contents(NULL, utils, param.argv, -2);
	get_path(utils.dirs, &path, cmd_and_args[0]);
	ft_free_matrix(utils.dirs);
	if (!path)
		clean_contents(cmd_and_args, utils, param.argv, 127);
	dup2(pipe_node->fd[1], STDOUT_FILENO);
	if (dup2(utils.fd, STDIN_FILENO) == -1)
	{
		free(path);
		clean_contents(cmd_and_args, utils, param.argv, 1);
	}
	pipe_list_free(pipe_node);
	close(utils.fd);
	execute_program(param.envp, cmd_and_args, path);
}

void	fi_chld_pro(t_args arg, t_pipe *pipe_lst, t_utils util, int n)
{
	char	*path;
	char	**cmd_and_args;

	path = NULL;
	util.pipe_node = pipe_lst;
	if (util.here_doc == TRUE)
		util.fd = open(arg.argv[n + 1], O_CREAT | O_RDWR | O_APPEND, 0777);
	else
		util.fd = open(arg.argv[n + 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	cmd_and_args = ft_split(arg.argv[n], ' ');
	if (!cmd_and_args)
		clean_contents(util.dirs, util, arg.argv, -2);
	get_path(util.dirs, &path, cmd_and_args[0]);
	ft_free_matrix(util.dirs);
	if (!path)
		clean_contents(cmd_and_args, util, arg.argv, 127);
	dup2(pipe_lst->fd[0], STDIN_FILENO);
	if (dup2(util.fd, STDOUT_FILENO) == -1)
	{
		free(path);
		clean_contents(cmd_and_args, util, arg.argv, 1);
	}
	pipe_list_free(pipe_lst);
	close(util.fd);
	execute_program(arg.envp, cmd_and_args, path);
}

int	pip_cmd(t_args args, t_pipe *pipe_lst, t_utils utils)
{
	char		**dirs;
	pid_t		child_pid_1;
	t_pipe		*pipe_last;

	dirs = utils.dirs;
	if (access(args.argv[1], F_OK) == 0)
	{
		child_pid_1 = fork();
		if (child_pid_1 == -1)
			exit_error_message("Failed to create first child", -1,
				dirs, pipe_lst);
		if (child_pid_1 == 0)
			frst_chld_proc(args, pipe_lst, utils);
	}
	pipe_last = loop_middle_child(args, pipe_lst, utils);
	pipe_last->wait.pid = fork();
	if (pipe_last->wait.pid == -1)
		exit_error_message("Failed to create last child", -1, dirs, pipe_lst);
	if (pipe_last->wait.pid == 0)
		fi_chld_pro(args, pipe_last, utils, args.argc - 2);
	close_pipe_fd(pipe_last);
	if (access(args.argv[1], F_OK) == 0)
		wait_child_process(child_pid_1, args.argv[2]);
	loop_wait_pid_middle(pipe_lst, args);
	return (wait_child_process(pipe_last->wait.pid, args.argv[args.argc - 2]));
}

int	main(int argc, char **argv, char **envp)
{
	char		**dirs;
	t_pipe		*pipe_list;
	t_bool		here_doc;
	t_args		parameter;
	t_utils		utils;

	parse_args(argv, argc, &here_doc);
	if (here_doc == TRUE)
		here_doc_setup(&argc, &argv);
	if (argv == NULL)
		return (2);
	parameter_init(argc, argv, envp, &parameter);
	dirs = get_dirs(envp);
	if (dirs == NULL)
		return (3);
	utils_init(&utils, dirs, here_doc);
	pipe_list = pipe_list_create(argc);
	exit_last(utils, pipe_list, argv, pip_cmd(parameter, pipe_list, utils));
}
