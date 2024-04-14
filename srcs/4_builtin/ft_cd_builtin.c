/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 09:43:03 by janhan            #+#    #+#             */
/*   Updated: 2024/04/15 08:27:37 by janhan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>

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
	if (!result)
	{
		free(oldpwd);
		exit (ft_error("ft_strjoin faild", FAILURE));
	}
	free(current->content);
	current->content = ft_strdup(result);
	free(result);
	free(oldpwd);
}

static void	ft_change_pwd(t_info *info)
{
	char	path[PATH_MAX];
	char	*pwd;
	char	*result = NULL;
	t_node	*current;

	pwd = ft_strdup("PWD=");
	if (!pwd)
		exit(ft_error("PWD malloc failed", FAILURE));
	current = info->mini_ev.front_node;
	printf("current->content : %s\n", (char *)current->content);
	while (current && ft_strncmp(current->content, "PWD=", 4))
		current = current->next_node;
	getcwd(path, sizeof(path));
	result = ft_strjoin(pwd, path);
	if (!result)
	{
		free(pwd);
		exit(ft_error("ft_strjoin failed", FAILURE));
	}
	free(current->content);
	current->content = ft_strdup(result);
	if (!current->content)
	{
		free(pwd);
		free(result);
		exit(ft_error("ft_strdup failed", FAILURE));
	}
	free(pwd);
	free(result);
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

static int	ft_cd_err(t_exec_info *exec_info, t_parse *parse, char *path)
{
	if (ft_strncmp(parse->token->original, "$HOME", 5) == 0
		&& exec_info->builtin_parent == TRUE)
	{
		ft_printf_err("minishell: cd: HOME not set\n");
		return (NOT_HOME);
	}
	else if (exec_info->builtin_parent == TRUE)
	{
		ft_printf_err("minishell: cd: %s: ", path);
		return (ft_perror(SUCCESS));
	}
	else
		exit(EXIT_SUCCESS);
}

int	ft_cd_builtin(t_info *info, t_exec_info *exec_info, t_parse *parse)
{
	char	*path;

	path = exec_info->cmd[1];
	if (path == NULL)
		path = ft_find_path(info);
	ft_change_oldpwd(info);
	if (chdir(path) == FAILURE)
	{
		g_child_exit_code = 1;
		ft_cd_err(exec_info, parse, path);
	}
	ft_change_pwd(info);
	if (exec_info->builtin_parent == TRUE)
		return (SUCCESS);
	else
		exit(EXIT_SUCCESS);
}
