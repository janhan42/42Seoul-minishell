/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 13:01:16 by janhan            #+#    #+#             */
/*   Updated: 2024/04/09 20:31:42 by sangshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_access_path(t_exec *exec, t_exec_info *exec_info)
{
	size_t	i;
	char	*cmd_path;

	cmd_path = exec_info->cmd_path;
	i = 0;
	while (exec->path_ev[i])
	{
		cmd_path = ft_make_path(exec_info, exec->path_ev[i]);
		if (access(cmd_path, X_OK) == SUCCESS)
		{
			exec_info->cmd_path = cmd_path;
			return (SUCCESS);
		}
		free(cmd_path);
		i++;
	}
	return (FAILURE);
}

static int	ft_find_cmd(t_exec *exec, t_exec_info *exec_info, t_parse *parse)
{
	char	*cmd_path;

	cmd_path = exec_info->cmd_path;
	if (parse->dquote_flag == TRUE && cmd_path[0] == 0)
		return (FAILURE);
	if (cmd_path == NULL)
		return (SUCCESS);
	if ((cmd_path[0] == '.' && cmd_path[1] == '/') || cmd_path[0] == '/')
	{
		ft_cmd_is_directory(cmd_path);
		ft_cmd_path_error_handle(exec_info, cmd_path);
		if (access(cmd_path, X_OK) == SUCCESS)
			return (SUCCESS);
	}
	if ((exec->path_ev[0] == NULL && !ft_is_builtin(exec_info)))
	{
		ft_printf_err("minishell: %s: No such file or directory\n",
			exec_info->cmd[0]);
		ft_free_all(parse, exec);
		exit(127);
	}
	if (ft_access_path(exec, exec_info) == SUCCESS)
		return (SUCCESS);
	return (FAILURE);
}

static void	ft_set_fd(t_exec *exec, t_exec_info *exec_info)
{
	ft_set_pipe_fd(exec, exec_info);
	ft_set_redirect_fd(exec_info);
	if (exec_info->infile_fd != NONE)
	{
		if (dup2(exec_info->infile_fd, STDIN_FILENO) == FAILURE)
			exit(ft_perror(errno));
		if (close(exec_info->infile_fd) == FAILURE)
			exit(ft_perror(errno));
	}
	if (exec_info->outfile_fd != NONE)
	{
		if (dup2(exec_info->outfile_fd, STDOUT_FILENO) == FAILURE)
			exit(ft_perror(errno));
		if (close(exec_info->outfile_fd) == FAILURE)
			exit(ft_perror(errno));
	}
}

static char	**ft_make_envp(t_list *mini_envp)
{
	char	**envp;
	size_t	count;
	t_node	*node;

	count = 0;
	node = mini_envp->front_node;
	while (node->next_node != NULL)
	{
		count++;
		node = node->next_node;
	}
	envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (envp == NULL)
		exit(ft_error("envp malloc failed\n", EXIT_FAILURE));
	count = 0;
	node = mini_envp->front_node;
	while (node->next_node != NULL)
	{
		envp[count] = (char *)node->content;
		node = node->next_node;
		count++;
	}
	envp[count] = NULL;
	return (envp);
}

void	ft_exec_cmd(t_info *info, t_parse *parse,
	t_exec *exec, t_exec_info *exec_info)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	ft_set_fd(exec, exec_info);
	/*
	if (ft_strchr(exec_info->cmd[0], ' ') != 0)
		if (ft_space_handle(exec_info) == FAILURE)
			exit(1);
	*/
	//FIX: $empty 처럼 빈 문자열이 여기까지 오면 저 어딘가에서 문제가 생기는지
	//exit code 가 139로 나옴
	//일단 주석처리하니까 문제가 없어짐ㅋㅋ
	if (ft_find_cmd(exec, exec_info, parse) == FAILURE
		&& ft_is_builtin(exec_info) == FALSE)
	{
		ft_printf_err("minishell: %s: command not found\n", exec_info->cmd[0]);
		ft_free_all(parse, exec);
		exit(127);
	}
	else
	{
		ft_cmd_null_handle(exec_info);
		if (exec_info->cmd_path == NULL)
			exit(EXIT_SUCCESS);
		if (ft_is_builtin(exec_info) == TRUE)
			ft_exec_builtin(info, parse, exec, exec_info);
		else
			execve(exec_info->cmd_path, exec_info->cmd,
				ft_make_envp(&info->mini_ev));
	}
}
