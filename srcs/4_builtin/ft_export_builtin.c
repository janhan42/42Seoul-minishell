/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:45:56 by janhan            #+#    #+#             */
/*   Updated: 2024/04/17 19:01:17 by janhan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_node	*ft_find_node(char *to_find2, t_list *list)
{
	t_node	*cur;
	size_t	to_find_len;
	char	**temp;
	char	*to_find;

	temp = ft_split(to_find2, '=');
	to_find = temp[0];
	free(temp[1]);
	free(temp);
	cur = list->front_node;
	to_find_len = ft_strlen(to_find);
	while (cur)
	{
		if ((ft_strncmp(to_find, cur->content, to_find_len + 1) == 0)
			|| (ft_strncmp(to_find, cur->content, to_find_len + 1) == -61))
		{
			free(to_find);
			return (cur);
		}
		cur = cur->next_node;
	}
	free(to_find);
	return (NULL);
}

static int	ft_export_builtin_arg_check(char *str)
{
	size_t	i;

	i = 0;
	if (ft_isalpha(str[0]) == FALSE && str[0] != '_')
	{
		g_child_exit_code = 1;
		return (FAILURE);
	}
	while (str[i])
	{
		if (str[i] == '=')
			return (SUCCESS);
		if (ft_isalnum(str[i]) == FALSE && str[i] != '_')
		{
			g_child_exit_code = 1;
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

static void	ft_export_builtin_arg(t_info *info, t_exec_info *exec_info)
{
	size_t	i;
	t_node	*to_find;

	i = 1;
	while (exec_info->cmd[i])
	{
		if (ft_export_builtin_arg_check(exec_info->cmd[i]) == FAILURE)
		{
			ft_printf_err("export: `%s': not a valid identifier\n",
				exec_info->cmd[i++]);
			continue ;
		}
		to_find = ft_find_node(exec_info->cmd[i], &info->mini_ev);
		if (to_find)
		{
			if (ft_strchr(exec_info->cmd[i], '='))
			{
				free(to_find->content);
				to_find->content = ft_strdup(exec_info->cmd[i]);
			}
		}
		else
			ft_list_push_back(ft_strdup(exec_info->cmd[i]), &(info->mini_ev));
		i++;
	}
}

static void	ft_export_builtin_no_arg(t_list *list)
{
	t_node	*cur;

	cur = list->front_node;
	while (cur)
	{
		printf("declare -x %s\n", (char *)(cur->content));
		cur = cur->next_node;
	}
}

int	ft_export_builtin(t_info *info, t_exec_info *exec_info)
{
	if (exec_info->cmd[1] == NULL)
		ft_export_builtin_no_arg(&(info->mini_ev));
	else
		ft_export_builtin_arg(info, exec_info);
	if (exec_info->builtin_parent == TRUE)
		return (SUCCESS);
	else
		exit(SUCCESS);
}
