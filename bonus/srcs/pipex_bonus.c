/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:31:31 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/18 13:33:58 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

int	wait_child_process(pid_t child_pid, char *child_phase, char *cmd)
{
	int		exit_code;
	t_wait	wait;

	exit_code = 0;
	wait.pid = waitpid(child_pid, &wait.status, 0);
	if (ft_bool_strcmp(child_phase, "last") == FALSE)
		return (exit_code);
	if (WIFEXITED(wait.status))
		exit_code = (WEXITSTATUS(wait.status));
	if (exit_code == 127)
	{
		ft_putstr_fd("pipex:", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	return (exit_code);
}

void	frst_chld_proc(t_args param, t_pipe *pipe_node, t_utils utils)
{
	char	*path;
	char	**cmd_and_args;

	path = NULL;
	utils.fd = open(param.argv[1], O_RDONLY, 0777);
	if (utils.fd == -1)
		clean_contents(utils.dirs, pipe_node, param.argv, utils);
	cmd_and_args = ft_split(param.argv[2], ' ');
	if (!cmd_and_args)
		clean_contents(NULL, pipe_node, param.argv, utils);
	get_path(utils.dirs, &path, cmd_and_args[0]);
	ft_free_matrix(utils.dirs);
	if (!path)
		clean_contents(cmd_and_args, pipe_node, param.argv, utils);
	dup2(pipe_node->fd[1], STDOUT_FILENO);
	dup2(utils.fd, STDIN_FILENO);
	pipe_list_free(pipe_node);
	close(utils.fd);
	execve(path, cmd_and_args, param.envp);
	free(path);
	exit(4);
}

void	mid_chld_pro(t_args param, t_pipe *pipe_lst, t_utils utils, int n)
{
	char    *path;
	char    **cmd_and_args;

	path = NULL;
	cmd_and_args = ft_split(param.argv[n], ' ');
	if (!cmd_and_args)
		clean_contents(utils.dirs, pipe_lst, param.argv, utils);
	get_path(utils.dirs, &path, cmd_and_args[0]);
	ft_free_matrix(utils.dirs);
	if (!path)
	{
		close_pipe_fd(pipe_lst);
		clean_contents(cmd_and_args, pipe_lst, param.argv, utils);
	}
	dup2(pipe_lst->fd[0], STDIN_FILENO);
	dup2(pipe_lst->next->fd[1], STDOUT_FILENO);
	pipe_list_free(pipe_lst);
	execve(path, cmd_and_args, param.envp);
	free(path);
}

void	fi_chld_pro(t_args arg, t_pipe *pipe_lst, t_utils util, int n)
{
	char	*path;
	char	**cmd_and_args;

	path = NULL;
	if (util.here_doc == TRUE)
		util.fd = open(arg.argv[n + 1], O_CREAT | O_RDWR | O_APPEND, 0777);
	else
		util.fd = open(arg.argv[n + 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (util.fd == -1)
		exit(1);
	cmd_and_args = ft_split(arg.argv[n], ' ');
	if (!cmd_and_args)
		clean_contents(util.dirs, pipe_lst, arg.argv, util);
	get_path(util.dirs, &path, cmd_and_args[0]);
	ft_free_matrix(util.dirs);
	if (!path)
		clean_contents(cmd_and_args, pipe_lst, arg.argv, util);
	dup2(pipe_lst->fd[0], STDIN_FILENO);
	dup2(util.fd, STDOUT_FILENO);
	pipe_list_free(pipe_lst);
	close(util.fd);
	execve(path, cmd_and_args, arg.envp);
	free(path);
	exit(4);
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
		wait_child_process(child_pid_middle, "middle", NULL);
		pipe_list = pipe_list->next;
	}
	return (pipe_list);
}

void	pip_cmd(t_args param, t_pipe *pipe_lst, t_utils utils)
{
	pid_t	child_pid_1;
	pid_t	child_pid_last;
	char	**dirs;

	dirs = utils.dirs;
	child_pid_1 = fork();
	if (child_pid_1 == -1)
		exit_error_message("Failed to create first child", -1, dirs, pipe_lst);
	if (child_pid_1 == 0)
		frst_chld_proc(param, pipe_lst, utils);
	wait_child_process(child_pid_1, "first", NULL);
	pipe_lst = loop_middle_child(param, pipe_lst, utils);
	child_pid_last = fork();
	if (child_pid_last == -1)
		exit_error_message("Failed to create last child", -1, dirs, pipe_lst);
	if (child_pid_last == 0)
		fi_chld_pro(param, pipe_lst, utils, param.argc - 2);
	close_pipe_fd(pipe_list_last(pipe_lst));
	wait_child_process(child_pid_last, "last", param.argv[param.argc - 1]);
}

void	parse_args(int argc, t_bool here_doc)
{
	if (argc < 5 || (argc < 6 && here_doc == TRUE))
	{
		ft_putstr_fd("Usage: ./pipex <infile> <cmd1> ... <cmdn> <outfile>", 2);
		ft_putstr_fd("\nOR\n", 2);
		exit_error_message("./pipex here_doc <limiter> <cmd1> <cmd2> <file>\n",
				-1, NULL, NULL);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char		**dirs;
	t_pipe		*pipe_list;
	t_bool		here_doc;
	t_args		parameter;
	t_utils		utils;

	here_doc = ft_bool_strcmp("here_doc", argv[1]);
	parse_args(argc, here_doc);
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
	pip_cmd(parameter, pipe_list, utils);
	pipe_list_free(pipe_list);
	ft_free_matrix(dirs);
	if (here_doc == TRUE)
		unlink(argv[1]);
	if (here_doc == TRUE)
		ft_free_matrix(argv);
}
