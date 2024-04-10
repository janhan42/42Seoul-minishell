# MINISHELL

#### 빌트인 목록
```
echo
-n 옵션 포함
cd
pwd
export
unset
env
exit
```
___

### Issues

```
exec_info를 만들때 ft_malloc_cmd로 사이즈 할당을 하는데

type == WORD면
ft_add_cmd에서 parse->tokens[parse->token_index].str을
exec_info->cmd[exec_info->cmd_index]에 넣는데

// 아직 처리 해야하나 말아야하나 모르겠지만
qoute_flag둘다도 배열을 하나 만들어서 들고 다녀야하나 모르겠음
//

이때 exec_info->env_flag[exec_info->cmd_index]로
각 커맨드가 환경변수인지를 넣어서

echo, cat쪽에서 env_flag를 확인후 처리하는 방안으로 가야 할것같음.
```
