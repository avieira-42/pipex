/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:31:31 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/09/19 23:22:43 by avieira-         ###   ########.fr       */
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
	execve(path, cmd_and_args, param.envp);
}

void	mid_chld_pro(t_args param, t_pipe *pipe_lst, t_utils utils, int n)
{
	char    *path;
	char    **cmd_and_args;

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
	/*if (util.fd == -1)
		clean_contents(util.dirs, util, arg.argv, 2);*/
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
	execve(path, cmd_and_args, arg.envp);
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
		wait_child_process(child_pid_middle, param.argv[n + 3]);
		pipe_list = pipe_list->next;
	}
	return (pipe_list);
}

int	pip_cmd(t_args param, t_pipe *pipe_lst, t_utils utils)
{
	pid_t	child_pid_1;
	pid_t	child_pid_last;
	char	**dirs;

	dirs = utils.dirs;
	if (access(param.argv[1], F_OK) == 0)
	{
		child_pid_1 = fork();
		if (child_pid_1 == -1)
			exit_error_message("Failed to create first child", -1, dirs, pipe_lst);
		if (child_pid_1 == 0)
			frst_chld_proc(param, pipe_lst, utils);
	}
	pipe_lst = loop_middle_child(param, pipe_lst, utils);
	child_pid_last = fork();
	if (child_pid_last == -1)
		exit_error_message("Failed to create last child", -1, dirs, pipe_lst);
	if (child_pid_last == 0)
		fi_chld_pro(param, pipe_lst, utils, param.argc - 2);
	close_pipe_fd(pipe_lst);
	if (access(param.argv[1], F_OK) == 0)
		wait_child_process(child_pid_1, param.argv[1]);
	return (wait_child_process(child_pid_last, param.argv[param.argc - 2]));
}

void	parse_files_exit(char **argv, int argc)
{
	if ((access(argv[argc -1], F_OK) == 0
				&& access(argv[argc - 1], R_OK | W_OK) == -1))
		exit(1);
}

void	parse_files(char **argv, int argc, t_bool here_doc)
{
	if (access(argv[1], F_OK) == 0 && here_doc == FALSE
			&& access(argv[1], R_OK | W_OK) == -1)
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
			&& access(argv[argc - 1], R_OK | W_OK) == -1)
	{
		ft_putstr_fd("pipex_bonus: permission denied: ", 2);
		ft_putstr_fd(argv[argc - 1], 2);
		ft_putstr_fd("\n", 2);
	}
	//parse_files_exit(argv, argc);
}

void	parse_args(char **argv, int argc, t_bool *here_doc)
{
	if (argc > 4)
		*here_doc = ft_bool_strcmp("here_doc", argv[1]);
	if (argc < 5 || (argc < 6 && *here_doc == TRUE))
	{
		ft_putstr_fd("Usage: ./pipex <infile> <cmd1> ... <cmdn> <outfile>", 2);
		ft_putstr_fd("\nOR\n", 2);
		exit_error_message("./pipex here_doc <limiter> <cmd1> <cmd2> <file>\n",
				-1, NULL, NULL);
	}
	parse_files(argv, argc, *here_doc);
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
