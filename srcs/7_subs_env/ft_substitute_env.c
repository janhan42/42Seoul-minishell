/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substitute_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangshin <zxcv1867@naver.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 20:36:16 by sangshin          #+#    #+#             */
/*   Updated: 2024/04/09 20:51:58 by sangshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_is_in_single_quote(char *line, int i)
{
	int	count;
	int	double_quote_flag;

	count = 0;
	double_quote_flag = FALSE;
	while (i >= 0)
	{
		if (line[i] == '"')
			double_quote_flag = !double_quote_flag;
		if (line[i] == '\'' && double_quote_flag == FALSE)
			count++;
		i--;
	}
	if (count % 2 == 0 || double_quote_flag == TRUE)
		return (FALSE);
	else
		return (TRUE);
}

static void	ft_replace_str(char **line, char *env_name, char *env_value)
{
	char	*new_line;
	char	*start;
	char	*end;
	char	*env_name_with_dallor;
	int		i;

	env_name_with_dallor = ft_strjoin("$", env_name);
	start = ft_strnstr(*line, env_name_with_dallor, ft_strlen(*line));
	end = start + ft_strlen(env_name) + 1;
	new_line = ft_calloc(ft_strlen(*line) + ft_strlen(env_value)
			- ft_strlen(env_name) + 1, sizeof(char));
	i = 0;
	while (*line != start)
		new_line[i++] = *(*line)++;
	*line -= i;
	while (*env_value)
		new_line[i++] = *env_value++;
	while (*end)
		new_line[i++] = *end++;
	free(*line);
	*line = new_line;
}

int	ft_substitute_env(t_info *info, t_parse *parse)
{
	int		dallor_count;
	char	*env_name;
	char	*env_value;

	dallor_count = ft_count_dallor(parse->line);
	while (dallor_count)
	{
		env_name = ft_get_env_name(parse->line);
		env_value = ft_get_env_value(info, env_name);
		ft_replace_str(&parse->line, env_name, env_value);
		free(env_name);
		dallor_count--;
	}
	return (SUCCESS);
}
