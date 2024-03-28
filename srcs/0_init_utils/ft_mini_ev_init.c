/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mini_ev_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 00:46:59 by janhan            #+#    #+#             */
/*   Updated: 2024/03/21 15:26:36 by sangshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief
 * 환경변수 전체를 긇어서 리스트화 하는 함수.
 * @param ev 환경변수
 * @param info ft_init 에서 초기화한 info구조체
 */
void	ft_mini_ev_init(char **ev, t_info *info)
{
	size_t	i;

	i = 0;
	info->mini_ev = ft_list_init();
	while (ev[i])
	{
		ft_list_push_back(ft_strdup(ev[i]), &(info->mini_ev));
		i++;
	}
}
