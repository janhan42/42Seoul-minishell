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
. 하나만 넣을때 No chiled Process에러 발생
bash: .: filename argument required
.: usage: . filename [arguments]
..도 동일
bash-3.2$ ..
bash: ..: command not found

cat 으로 입력을 받는중에
asdasd 후에
Ctrl+C Ctrl+\을 했을때
minishell$ cat
asdasdminishell$ asdasd 처럼 이상하게 나옴

```
### fix
```c
ft_cd_biltin.c
PWD 세그폴트
ft_change_pwd(){
current = info->mini_ev.front_node;
while (current && ft_strncmp(current->content, "PWD=", 4))
	current = current->next_node;
if (current == NULL)
	return ;
 // PWD가 없을때 Bash에서도 PWD는 재생성하지 않음
}
```
```c
. 하나만 넣을때 No chiled Process에러 발생
bash: .: filename argument required
.: usage: . filename [arguments]
..도 동일
int	ft_cmd_error_sup(t_exec_info *exec_info)
{
	if (exec_info->cmd_path[0] == '.'
		&& exec_info->cmd_path[1] == '.'
		&& exec_info->cmd_path[2] == '\0')
	{
		return (FAILURE);
	}
	else if (exec_info->cmd_path[0] == '.'
		&& exec_info->cmd_path[1] == '\0')
	{
		ft_printf_err("minishell: .: filename argument required\n\
.: usage: . filename [arguments]\n");
		exit(2);
	}
	return (SUCCESS);
}
함수 추가로 ft_exec_cmd에서 호출
에러 메세지 출력
```

```c
/
./
../
이 들와어왔을때
if ((cmd_path[0] == '.' && cmd_path[1] == '/') || cmd_path[0] == '/')
	{
		ft_cmd_is_directory(cmd_path);
		ft_cmd_path_error_handle(exec_info, cmd_path);
		if (access(cmd_path, X_OK) == SUCCESS) // env전체 unset 후 /bin/ls 무한 해결중 여기서 일단 SUCCESS로 나감
			return (SUCCESS);
	}
기존 방식으로 if문의 조건을 if (ft_strchr(cmd_path, '/') != 0)로
/가 하나라도 있으면 들어가게 변경
```
