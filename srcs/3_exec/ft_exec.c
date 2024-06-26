/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 19:25:46 by janhan            #+#    #+#             */
/*   Updated: 2024/04/20 22:17:57 by sangshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_parent(t_exec *exec, t_exec_info *exec_info)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (exec->prev_pipe_fd != NONE)
	{
		if (close(exec->prev_pipe_fd) == FAILURE)
			return (ft_perror(FAILURE));
	}
	if (exec_info->use_pipe == TRUE)
	{
		if (close(exec_info->pipe_fd[OUT]) == FAILURE)
			return (ft_perror(FAILURE));
		exec->prev_pipe_fd = exec_info->pipe_fd[IN];
	}
	else
		exec->prev_pipe_fd = NONE;
	return (SUCCESS);
}

static int	ft_make_child(t_info *info, t_parse *parse,
	t_exec *exec, t_exec_info *exec_info)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	exec_info->pid = fork();
	if (exec_info->pid == FAILURE)
	{
		ft_free_all(parse, exec);
		return (ft_perror(FAILURE));
	}
	else if (exec_info->pid == CHILD_PID)
		ft_exec_cmd(info, parse, exec, exec_info);
	else
	{
		if (ft_parent(exec, exec_info) == FAILURE)
		{
			ft_free_all(parse, exec);
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

static int	ft_wait_child(t_exec *exec)
{
	int		child_status;
	pid_t	last_child_pid;

	last_child_pid = exec->exec_arr[exec->exec_arr_size - 1].pid;
	exec->exec_arr_index = 0;
	while (exec->exec_arr_index < exec->exec_arr_size)
	{
		exec->current_child_pid = waitpid(-1, &child_status, 0);
		if (exec->current_child_pid == FAILURE)
			return (ft_perror(FAILURE));
		if (exec->current_child_pid == last_child_pid)
		{
			if (WIFSIGNALED(child_status) == TRUE)
				g_child_exit_code = 128 + WTERMSIG(child_status);
			else
				g_child_exit_code = WEXITSTATUS(child_status);
		}
		exec->exec_arr_index++;
	}
	if (g_child_exit_code == 130)
		ft_putstr_fd("\n", STDERR_FILENO);
	else if (g_child_exit_code == 131)
		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	unlink("/tmp/whine");
	return (SUCCESS);
}

static int	ft_here_doc(t_parse *parse, t_exec *exec)
{
	if (ft_here_doc_loop_check(exec) == FAILURE)
		return (SUCCESS);
	if (ft_check_here_doc(exec) == FAILURE)
	{
		ft_free_all(parse, exec);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	ft_exec(t_info *info, t_parse *parse, t_exec *exec)
{
	t_exec_info	*exec_info;

	if (ft_here_doc(parse, exec) == FAILURE)
		return (FAILURE);
	exec->exec_arr_index = 0;
	while (exec->exec_arr_index < exec->exec_arr_size)
	{
		exec_info = &exec->exec_arr[exec->exec_arr_index];
		if (ft_is_builtin_parent(exec, exec_info) == TRUE)
			return (ft_exec_builtin_parent(info, parse, exec, exec_info));
		if (exec_info->use_pipe == TRUE)
		{
			if (pipe(exec_info->pipe_fd) == FAILURE)
			{
				ft_free_all(parse, exec);
				return (ft_perror(FAILURE));
			}
		}
		if (ft_make_child(info, parse, exec, exec_info) == FAILURE)
			return (FAILURE);
		exec->exec_arr_index++;
	}
	ft_wait_child(exec);
	return (SUCCESS);
}
