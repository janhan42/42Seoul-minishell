/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 00:07:27 by janhan            #+#    #+#             */
/*   Updated: 2024/04/14 13:56:36 by janhan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_parse_execute(t_info *info, t_parse *parse, t_exec *exec)
{
	if (ft_parse(info, parse) == FAILURE)
		return ;
	if (ft_make_exec_info(info, parse, exec) == FAILURE)
		return ;
	if (ft_exec(info, parse, exec) == FAILURE)
		return ;
	ft_free_all(parse, exec);
}

void	check(void)
{
	system("leaks minishell");
}

int	main(int ac, char **av, char **ev)
{
	t_info		info;
	t_parse		parse;
	t_exec		exec;

	ft_init(ac, av, ev, &info);
	while (TRUE)
	{
		ft_sig_init(&info);
		parse.line = readline("\033[32mminishell$ ");
		if (parse.line == NULL)
		{
			ft_list_clear(&info.mini_ev);
			ft_putstr_fd("\x1b[1A\033[11Cexit\n", STDOUT_FILENO);
			return (EXIT_SUCCESS);
		}
		if (parse.line[0] == '\0')
		{
			free(parse.line);
			continue ;
		}
		add_history(parse.line);
		ft_parse_execute(&info, &parse, &exec);
		free(parse.line);
	}
	atexit(check);
	return (EXIT_SUCCESS);
}
