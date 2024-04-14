/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 09:48:52 by janhan            #+#    #+#             */
/*   Updated: 2024/04/14 09:02:07 by janhan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stddef.h>

static char	**ft_echo_builtin_malloc_str(t_exec_info *exec_info, size_t *str_i,
int *n_flag)
{
	char	**str;
	size_t	str_size;

	str_size = 1;
	*str_i = 0;
	*n_flag = FALSE;
	while (exec_info->cmd[str_size])
		str_size++;
	str = (char **)ft_calloc(str_size, sizeof(char *));
	if (str == NULL)
		exit(EXIT_FAILURE);
	return (str);
}

static int	ft_echo_builtin_is_n(char *str)
{
	size_t	i;

	if (ft_strncmp(str, "-n", 2) != 0)
		return (FALSE);
	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

static size_t	ft_echo_builtin_find_cmd_i(char **cmd, int *n_flag)
{
	size_t	cmd_i;

	cmd_i = 1;
	while (cmd[cmd_i])
	{
		if (ft_echo_builtin_is_n(cmd[cmd_i]) == FALSE)
			return (cmd_i);
		*n_flag = TRUE;
		cmd_i++;
	}
	exit(EXIT_SUCCESS);
}

static void	ft_echo_builtin_print_str(char **str, int n_flag)
{
	size_t	i;

	i = 0;
	while (str[i + 1])
	{
		printf("%s ", str[i]);
		i++;
	}
	printf("%s", str[i]);
	if (n_flag == FALSE)
		printf("\n");
}

int	ft_echo_builtin(t_exec_info *exec_info)
{
	char	**str;
	size_t	str_i;
	size_t	cmd_i;
	int		n_flag;

	if (exec_info->cmd[1] == NULL)
	{
		printf("\n");
		exit(SUCCESS);
	}
	str = ft_echo_builtin_malloc_str(exec_info, &str_i, &n_flag);
	cmd_i = ft_echo_builtin_find_cmd_i(exec_info->cmd, &n_flag);
	while (exec_info->cmd[cmd_i])
	{
		if (exec_info->dqoute_flags[cmd_i] == 0
			&& exec_info->env_flags[cmd_i] == TRUE)
			str[str_i] = ft_remove_space(exec_info->cmd[cmd_i]);
		else
			str[str_i] = exec_info->cmd[cmd_i];
		str_i++;
		cmd_i++;
	}
	ft_echo_builtin_print_str(str, n_flag);
	exit(EXIT_SUCCESS);
}
