/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 19:22:26 by janhan            #+#    #+#             */
/*   Updated: 2024/04/14 14:35:28 by sangshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stddef.h>

void	ft_free_exec(t_exec *exec, size_t exec_arr_i)
{
	size_t	i;

	i = 0;
	while (i < exec_arr_i)
	{
		free(exec->exec_arr[i].cmd);
		free(exec->exec_arr[i].redirect);
		free(exec->exec_arr[i].env_flags);
		free(exec->exec_arr[i].dqoute_flags);
		free(exec->exec_arr[i].sqoute_flags);
		i++;
	}
	free(exec->exec_arr);
	i = 0;
	while (exec->path_ev[i])
		free(exec->path_ev[i++]);
	free(exec->path_ev);
}
