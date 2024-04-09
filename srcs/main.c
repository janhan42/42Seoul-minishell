/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 00:07:27 by janhan            #+#    #+#             */
/*   Updated: 2024/04/09 19:17:43 by sangshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_is_special_dallor(char *line)
{
	int	i;
	i = 0;
	while (line[i] != '$')
		i++;
	i++;
	if (line[i] == '_' || line[i] == '-' || line[i] == '?'
		|| ft_isdigit(line[i]))
		return (TRUE);
	return (FALSE);
}

int	ft_count_dallor(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (line[i] == '$' && !ft_is_special_dallor(&line[i]))
			count++;
		i++;
	}
	return (count);
}

char	*ft_get_env_name(char *line)
{
	int		i;
	int		start;
	char	*env_name;

	i = 0;
	while (line[i] != '$')
		i++;
	i++;
	start = i;
	while (ft_isalnum(line[i]) || line[i] == '_')
		i++;
	env_name = ft_substr(line, start, i - start);
	return (env_name);
}

char	*ft_get_env_value(t_info *info, char *env_name)
{
	t_node	*node;
	char	*env_name_with_eq;

	env_name_with_eq = ft_strjoin(env_name, "=");
	node = info->mini_ev.front_node;
	while (node)
	{
		if (ft_strncmp(node->content, env_name_with_eq, ft_strlen(env_name_with_eq)) == 0)
		{
			free(env_name_with_eq);
			return (ft_strchr(node->content, '=') + 1);
		}
		node = node->next_node;
	}
	return ("");
}

void	ft_replace_str(char **line, char *env_name, char *env_value)
{
	char	*new_line;
	char	*start;
	char	*end;
	char	*env_name_with_dallor;
	int		i;

	// line = $abcdefg
	// ft_strlen(line) = 8
	// ft_strlen(env_name) = 7
	//
	// in this case, start should be 0, end should be 8
	//
	//export a="ls  -l"
	//line = cat $a
	//in this case, start should be 4, end should be 6
	env_name_with_dallor = ft_strjoin("$", env_name);
	start = ft_strnstr(*line, env_name_with_dallor, ft_strlen(*line));
	end = start + ft_strlen(env_name) + 1;
	new_line = ft_calloc(ft_strlen(*line) + ft_strlen(env_value) - ft_strlen(env_name) + 1, sizeof(char));
	i = 0;
	while (*line != start)
		new_line[i++] = *(*line)++;
	*line -= i;                      // line을 원래대로 돌려놓음
	while (*env_value)
		new_line[i++] = *env_value++;
	while (*end)
		new_line[i++] = *end++;
	free(*line);
	*line = new_line;
}

int	ft_substitue_env(t_info *info, t_parse *parse)
{
	//라인에 $표시가 있는지 체크
	//$표시 뒤에 특정 문자(_, -, ?, 숫자)가 있는지 체크 -> 얘들은 넘어감
	//없으면 $표시 뒤에 있는 문자열을 찾아서 환경변수에 있는지 체크
	//있으면 환경변수로 바꿔줌
	//없으면 그냥 넘어감

	// 달러가 몇개 있는지 변수에 저장
	// 해당 변수가 0이 될 때까지 반복
	int		dallor_count;
	char	*env_name;
	char	*env_value;

	dallor_count = ft_count_dallor(parse->line);
	while (dallor_count)
	{
		env_name = ft_get_env_name(parse->line);      // free 필요
		env_value = ft_get_env_value(info, env_name); // free 필요없음
		ft_replace_str(&parse->line, env_name, env_value);
		free(env_name);
		dallor_count--;
	}
	return (SUCCESS);
}

static void	ft_parse_execute(t_info *info, t_parse *parse, t_exec *exec)
{
	// TODO: 이곳에 환경변수를 싸그리 넣어주는 함수를 추가할 예정
	ft_substitue_env(info, parse);
	//printf("line: %s\n", parse->line);
	if (ft_parse(info, parse) == FAILURE)
		return ;
	if (ft_make_exec_info(info, parse, exec) == FAILURE)
		return ;
	if (ft_exec(info, parse, exec) == FAILURE)
		return ;
	ft_free_all(parse, exec);
}

int	main(int ac, char **av, char **ev)
{
	t_info		info;
	t_parse		parse;
	t_exec		exec;

	ft_init(ac, av, ev, &info);
	printf("이 브랜치는 공사중 --sangshin\nDo not touch this branch --sangshin\n");
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
	return (EXIT_SUCCESS);
}
