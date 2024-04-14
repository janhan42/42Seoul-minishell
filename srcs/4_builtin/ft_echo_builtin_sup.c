/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_builtin_sup.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 08:50:49 by janhan            #+#    #+#             */
/*   Updated: 2024/04/14 08:51:00 by janhan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_remove_space(char *string)
{
	int		index;
	char	**list;
	char	*one_line;
	char	*tmp;

	index = 0;
	list = ft_split(string, ' ');
	one_line = ft_strdup("");
	while (list[index])
	{
		tmp = one_line;
		one_line = ft_strjoin(one_line, list[index]);
		free(tmp);
		if (list[index + 1] != 0)
		{
			tmp = one_line;
			one_line = ft_strjoin(one_line, " ");
			free(tmp);
		}
		free(list[index]);
		index++;
	}
	free(list);
	return (one_line);
}
