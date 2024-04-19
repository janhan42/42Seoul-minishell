/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sig_init_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 20:27:08 by janhan            #+#    #+#             */
/*   Updated: 2024/04/19 20:54:58 by janhan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_sig_for_child(int sig)
{
	if (sig == SIGINT)
	{
		printf("^C\n");
	}
	if (sig == SIGQUIT)
	{
		printf("^\\Quit: %d\n", sig);
	}
}
