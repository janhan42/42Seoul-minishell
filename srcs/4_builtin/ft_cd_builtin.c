/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 09:43:03 by janhan            #+#    #+#             */
/*   Updated: 2024/03/27 20:19:23 by janhan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_change_oldpwd(t_info *info)
{
	char	path[PATH_MAX];
	char	*oldpwd;
	char	*result;
	t_node	*current;

	oldpwd = ft_strdup("OLDPWD=");
	if (!oldpwd)
		exit(ft_error("OLDPWD malloc failed", FAILURE));
	current = info->mini_ev.front_node;
	while (current && ft_strncmp(current->content, "OLDPWD=", 7))
		current = current->next_node;
	if (current == NULL)
	{
		free(oldpwd);
		return ;
	}
	getcwd(path, sizeof(path));
	result = ft_strjoin(oldpwd, path);
	free(oldpwd);
	free(current->content);
	current->content = result;
}

static void	ft_change_pwd(t_info *info)
{
	char	path[PATH_MAX];
	char	*oldpwd;
	char	*result;
	t_node	*current;

	oldpwd = ft_strdup("PWD=");
	if (!oldpwd)
		exit(ft_error("PWD malloc failed", FAILURE));
	current = info->mini_ev.front_node;
	while (current && ft_strncmp(current->content, "PWD=", 4))
		current = current->next_node;
	getcwd(path, sizeof(path));
	result = ft_strjoin(oldpwd, path);
	free(oldpwd);
	free(current->content);
	current->content = result;
}

static char	*ft_find_path(t_info *info)
{
	t_node	*current;
	char	*path;

	current = info->mini_ev.front_node;
	while (current && ft_strncmp(current->content, "HOME=", 5))
		current = current->next_node;
	path = current->content + 5;
	return (path);
}

int	ft_cd_builtin(t_info *info, t_exec_info *exec_info)
{
	char	*path;

	path = exec_info->cmd[1];
	if (path == NULL)
		path = ft_find_path(info);
	ft_change_oldpwd(info);
	if (chdir(path) == FAILURE)
	{
		g_child_exit_code = 1;
		if (exec_info->builtin_parent == TRUE)
		{
			ft_printf_err("cd: %s: ", path);
			return (ft_perror(SUCCESS));
		}
		else
			exit(EXIT_SUCCESS);
	}
	ft_change_pwd(info);
	if (exec_info->builtin_parent == TRUE)
		return (SUCCESS);
	else
		exit(EXIT_SUCCESS);
}
