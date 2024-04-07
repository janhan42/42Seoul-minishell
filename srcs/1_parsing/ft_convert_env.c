/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janhan <janhan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 23:59:19 by janhan            #+#    #+#             */
/*   Updated: 2024/04/08 00:59:03 by sangshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_set_quote_flag(t_parse *parse)
{
	if (parse->token->str[parse->str_index] == '"'
		&& parse->dquote_flag == FALSE)
		parse->dquote_flag = TRUE;
	else if (parse->token->str[parse->str_index] == '"'
		&& parse->dquote_flag == TRUE)
		parse->dquote_flag = FALSE;
	if (parse->token->str[parse->str_index] == '\''
		&& parse->dquote_flag == FALSE
		&& parse->squote_flag == FALSE)
		parse->squote_flag = TRUE;
	else if (parse->token->str[parse->str_index] == '\''
		&& parse->dquote_flag == FALSE
		&& parse->squote_flag == TRUE)
		parse->squote_flag = FALSE;
}

static int	ft_check_env(t_info *info, t_parse *parse)
{
	parse->start_index = parse->str_index;
	while (ft_isalnum(parse->token->str[parse->str_index + 1])
		|| parse->token->str[parse->str_index + 1] == '-'
		|| parse->token->str[parse->str_index + 1] == '_')
		parse->str_index++;
	parse->temp_str_len = parse->str_index - parse->start_index + 1;
	parse->temp_str = (char *)ft_calloc(parse->temp_str_len + 1, sizeof(char));
	if (parse->temp_str == NULL)
		return (ft_error("Failed malloc temp_str", FAILURE));
	ft_strlcpy(parse->temp_str, &parse->token->str[parse->start_index + 1],
		parse->temp_str_len + 0);
	parse->target_ev = ft_strjoin(parse->temp_str, "=");
	if (parse->target_ev == NULL)
	{
		free(parse->temp_str);
		return (ft_error("Failed ft_strjoin target_ev", FAILURE));
	}
	free(parse->temp_str);
	if (ft_is_env(info, parse) == TRUE)
		return (TRUE);
	free(parse->target_ev);
	return (FALSE);
}

static int	ft_add_env(t_parse *parse)
{
	parse->token->str[parse->start_index] = '\0';
	parse->old_str = ft_strjoin(parse->token->str, parse->ev_val);
	if (parse->old_str == NULL)
	{
		free(parse->target_ev);
		free(parse->ev_val);
		return (ft_error("Failed ft_strjoin old_str", FAILURE));
	}
	parse->new_str
		= ft_strjoin(parse->old_str, &parse->token->str[parse->str_index + 1]);
	if (parse->new_str == NULL)
	{
		free(parse->target_ev);
		free(parse->ev_val);
		free(parse->old_str);
		return (ft_error("Failed ft_strjoin new_str", FAILURE));
	}
	free(parse->target_ev);
	free(parse->ev_val);
	free(parse->old_str);
	free(parse->token->str);
	parse->token->str = parse->new_str;
	return (SUCCESS);
}

static int	ft_find_env(t_info *info, t_parse *parse)
{
	ft_set_quote_flag(parse);
	if (parse->squote_flag == FALSE && parse->token->str[parse->str_index]
		== '$' && (ft_isalpha(parse->token->str[parse->str_index + 1])
			|| parse->token->str[parse->str_index + 1] == '_'
			|| parse->token->str[parse->str_index + 1] == '-'
			|| parse->token->str[parse->str_index + 1] == '?'))
	{
		parse->token->env_flag = TRUE;
		if (ft_is_child_exit_code(parse) == TRUE)
			return (ft_convert_child_exit_code(parse));
		else if (ft_check_env(info, parse) == TRUE)
		{
			if (ft_add_env(parse) == FAILURE)
				return (FAILURE);
			parse->str_index = parse->start_index + parse->ev_val_len - 1;
		}
		else
		{
			ft_strlcpy(&parse->token->str[parse->start_index],
				&parse->token->str[parse->str_index + 1],
				ft_strlen(parse->token->str) + 1);
			parse->str_index -= parse->temp_str_len + 1;
		}
	}
	return (SUCCESS);
}

void ft_remake_line(t_parse *parse)
{
	char	temp[10000];
	int i;

	int j;
	int double_quote;
	int blank_flag;

	i = 0;
	j = 0;
	double_quote = 0;
	blank_flag = 0;
	parse->str_index = 0;
	parse->token_index = 0;
	printf("parse->line : %ld\n", ft_strlen(parse->line));
	while (parse->line[parse->str_index])
	{
		if (parse->line[parse->str_index] == '$')
		{
			parse->str_index = 0;
			break ;
		}
		parse->str_index++;
	}
	if (parse->line[parse->str_index] == '\0')
		return ;
	while (parse->line[parse->str_index])
	{
		temp[j] = parse->line[parse->str_index]; // parse->line젤뒤에도 쓰레기값 들어옴zzz
		printf("parse->line[parse->str_index] : %c\n", parse->line[parse->str_index]);
		printf("str_index : %ld\n", parse->str_index);
		printf("j : %d\n", j);
		if (parse->line[parse->str_index] == ' ' && blank_flag == 0)
		{
			parse->token_index++;
			blank_flag = 1;
		}
		else
			blank_flag = 0;
		if (parse->line[parse->str_index] == '$')
		{
			if (parse->str_index > 0 && parse->line[parse->str_index - 1] == '\"')
				double_quote = 1;
			if (parse->tokens[parse->token_index].env_flag == TRUE)
			{
				i = 0;
				while (parse->tokens[parse->token_index].str[i])
				// 지금 토큰 제일 마지막에 쓰레기값 들어가있음
				{
					temp[j + i] = parse->tokens[parse->token_index].str[i];
					i++;  // i 글자만큼 복사했음
				}
				parse->tokens[parse->token_index].env_flag = FALSE;
				parse->token_index++;
				temp[j + i] = ' ';
				if (double_quote == 1)
				{
					while(parse->line[parse->str_index] && parse->line[parse->str_index] != '\"')
					{
						parse->str_index++;
					}
					//parse->start_index--;
				}
				else
				{
					while(parse->line[parse->str_index] && parse->line[parse->str_index] != ' ')
					{
						parse->str_index++;
					}
					if (parse->line[parse->str_index] == 0)
					{
						// 아무리 생각해도 line이 이상하게 들어올때가 있음 ㅠㅠ
						parse->start_index-= 2;
					}
					//parse->start_index--;
				}
			}
		}
		j += i + 1;
		i = 0;
		parse->str_index++;
	}
	temp[j - 1] = '\0';
	printf("last line : %s\n", temp);
	/*
	printf("temp[0] : %c\n", temp[0]);
	printf("temp[1] : %c\n", temp[1]);
	printf("temp[2] : %c\n", temp[2]);
	printf("temp[3] : %c\n", temp[3]);
	printf("temp[4] : %c\n", temp[4]);
	printf("temp[5] : %c\n", temp[5]);
	printf("temp[6] : %c\n", temp[6]);
	printf("temp[7] : %c\n", temp[7]);
	printf("temp[8] : %c\n", temp[8]);
	printf("temp[9] : %d\n", temp[9]);
	printf("temp[10] : %d\n", temp[10]);
	printf("temp[11] : %d\n", temp[11]);
	*/
	free(parse->line);
	parse->line = ft_strdup(temp);
	//ft_free_tokens(parse, parse->token_count);
	ft_parse_init(parse);
	ft_count_token(parse);
	ft_ready_tokenization(parse);
	ft_tokenization(parse); //
	 // <- 여기 0으로 넣으면 프리 안되낭 거기 원래 parse->token_count 넣는거 이전 parse 프리하고 다시 만들거야? ㅇ트
	// 아예 첨부터 싹 만들어야함 ㄱㄱ rcrcㄱㅍㄱㅊ 헤더에 다 넣고왔다 ㄹㅇ ㅋㅋ
	//  아니 형 근데 여기 들어오지도 않는데?ㅋㅋㅋ 씨발 이젠 되는게 없노 ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ 안됨
	// t는 왜들어감? 나 화장실좀 어디가 터지는거지 ㄲ  ㄱㅅㄱㅅ make 때려?
	// 그냥 터져버리는데 우캄ㄴㄴ ㄱㄷ 잘봐 ㅇㅋ 전설을 써달라고 ㅋㅋ 어어 헤더에 없는애들인데 그거 ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ static 으로 선언되어있음
	// (null) 왜나옴 저거? 우리 어디 출력문 걸둔거 있나?
	// 전부다고장났는데 그냥 ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ 아 왜 안되지 ㄹㅇ 에반데
	// 왜안돼지 exit도 안됨 ㄹㅈㄷ
	// 지금 조금 됨 오 조금 되고있음 근데 왜 ls 어디감 ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ
	// 갑자기 터미널 존나늘어나느데
	// 29개? ㅇㅋㅇㅋ
	// ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ내가 만들었음 예아 야식 타이밍인데 깃 푸시해두고 갈까 아님 걍 이거 계속 쓸까
	// 근데 이거 꽤나 괜찮은데? 익스텐션? 과제 봐줄때 좋을듯 푸시 한번 해조 ㄷㄷ ㅇㅋ
	// 내가 무조건 완성시키고 만다
	//                                                       *************************************************
	//														 *                                               *
	//														 *                                               *
	//														 *                                               *
	//														 *                                               *
	//														 *                  메이크 해줘!!!!!                *
	//														 *             plese make this file              *
	//														 *                                               *
	//														 *                                               *
	//														 *어디가 문제고!!!!!!!!!!!!!!!!!!                   *
	//                                                       *************************************************
}
// echo         $test       $test2
// echo    /  ls     -l   / $test2
/*

*/
int	ft_convert_env(t_info *info, t_parse *parse)
{
	parse->token_index = 0;
	while (parse->token_index < parse->token_count)
	{
		parse->token = &parse->tokens[parse->token_index];
		parse->str_index = 0;
		while (parse->token->str[parse->str_index])
		{
			if (ft_is_heredoc(parse->token->str[parse->str_index],
					parse->token->str[parse->str_index + 1]) == TRUE)
			{
				parse->token_index++;
				break ;
			}
			if (ft_find_env(info, parse) == FAILURE)
				return (FAILURE);
			if (parse->str_index < 0)
				break ;
			parse->str_index++;
		}
		parse->token_index++;
	}
	ft_remake_line(parse);

	return (SUCCESS);
}
