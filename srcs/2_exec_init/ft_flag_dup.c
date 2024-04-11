/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flag_dup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:20:33 by janhan            #+#    #+#             */
/*   Updated: 2024/04/11 20:25:06 by janhan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	*ft_dup_dqoute_flag(t_parse *parse)
{
	int	*result;
	size_t	index;

	index = 0;
	result = (int *)ft_calloc(parse->token_count, sizeof(int));
	if (!result)
		exit(ft_error("qoute_flag malloc failed", 1));
	while (index < parse->token_count)
	{
		result[index] = parse->tokens[index].dqoute_flag;
		index++;
	}
	return (result);
}
int	*ft_dup_sqoute_flag(t_parse *parse)
{
	int	*result;
	size_t	index;

	index = 0;
	result = (int *)ft_calloc(parse->token_count, sizeof(int));
	if (!result)
		exit(ft_error("qoute_flag malloc failed", 1));
	while (index < parse->token_count)
	{
		result[index] = parse->tokens[index].sqoute_flag;
		index++;
	}
	return (result);
}
