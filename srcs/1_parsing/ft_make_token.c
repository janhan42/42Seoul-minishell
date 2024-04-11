/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_make_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 23:32:02 by janhan            #+#    #+#             */
/*   Updated: 2024/04/11 21:17:44 by sangshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_qoute_finder(t_parse *parse)
{
	size_t	index;
	char	end_quote = '\0';

	index = 0;
	// parse->tokens[parse->token_index].str = a="ls      -al";
	while (parse->tokens[parse->token_index].str[index])
	{
		if (ft_is_quote(parse->tokens[parse->token_index].str[index]) == TRUE)
		{
			end_quote = parse->tokens[parse->token_index].str[index];
			break ;
		}
		index++;
	}
	index++;
	/*
	printf("end_qoute : %c\n", end_quote);
	printf("%ld ", index);
	*/
	while (parse->tokens[parse->token_index].str[index]
		&& parse->tokens[parse->token_index].str[index] != end_quote)
		index++;
	//printf("str[%ld] : %c\n", index, parse->tokens[parse->token_index].str[index]);
	if (parse->tokens[parse->token_index].str[index] == end_quote)
	{
		if (end_quote == '"')
			parse->tokens[parse->token_index].dqoute_flag = TRUE;
		else if (end_quote == '\'')
			parse->tokens[parse->token_index].sqoute_flag = TRUE;
	}
	/*
	printf("%s\n", parse->tokens[parse->token_index].str);
	printf("parse->tokens->sqoute_flag : %d\n", parse->tokens[parse->token_index].sqoute_flag);
	printf("parse->tokens->dqoute_flag : %d\n", parse->tokens[parse->token_index].dqoute_flag);
	*/
}

int	ft_make_token(t_parse *parse, t_token_type type)
{
	parse->tokens[parse->token_index].type = type;
	parse->temp_str
		= (char *)ft_calloc(parse->line_index
			- parse->start_index + 2, sizeof(char));
	if (parse->temp_str == NULL)
		return (ft_error("Failed malloc temp_str", FAILURE));
	ft_strlcpy(parse->temp_str, &parse->line[parse->start_index],
		parse->line_index - parse->start_index + 2);
	parse->tokens[parse->token_index].str = parse->temp_str;
	parse->tokens[parse->token_index].original
		= ft_strdup(parse->tokens[parse->token_index].str);
	parse->tokens[parse->token_index].dqoute_flag = FALSE;
	parse->tokens[parse->token_index].sqoute_flag = FALSE;
	ft_qoute_finder(parse);
	parse->tokens[parse->token_index].env_flag = FALSE;
	parse->token_index++;
	return (SUCCESS);
}
