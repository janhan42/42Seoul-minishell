/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_clear.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 14:05:15 by janhan            #+#    #+#             */
/*   Updated: 2024/03/25 22:47:45 by sangshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
void	ft_list_clear1(t_list *list) // 연결리스트 전체 삭제
{
	list->current_node = list->front_node;
	while (list->current_node)
		ft_list_del_node(list, list->current_node);
}
*/

void	ft_list_clear(t_list *list)
{
	t_node	*tmp;
	t_node	*next;

	tmp = list->front_node;
	while (tmp)
	{
		next = tmp->next_node;
		free(tmp->content);
		free(tmp);
		tmp = next;
	}
}

void	*list_content_finder(t_node *head, void *content)
{
	t_node	*cur;

	cur = head;
	while (cur)
	{
		if (ft_strncmp(cur->content, content, ft_strlen(content)) == 0)
			return (cur->content);
		cur = cur->next_node;
	}
	return (NULL);
}
