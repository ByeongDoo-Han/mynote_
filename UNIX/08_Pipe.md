# Pipe
<img src="https://lh3.googleusercontent.com/WeQVF5rLdHr7fH61Iy5l-4O4fw2Lc9K4Rrn46aEYbN4rqNX-bRhn_WtvSPA349WdH34FA1JkL2zq5L2qLjYCG4GGBhn3rM4w3tiTRZDl-LK-YPbHE_KmefxzL8VhELKtZ-Vq5s2SpYaJeYYvBrnMBh-d87oHG6sQgf99_yY7a7wGZFK98B5qWYb0NWqCMtYrrijpBlM_-8Sbcezi4u6odHxco-qaWAE3PghZtCWOqc2Tyldyn6CdXoUTaojvw6zAhza3bVMZRyXDVC-bUnDkSzJ_23z1TCIYeKtDXwPhplw9ROVF4RV8PaYLEVjQEGtz19LlZAMZScayM6Ax9hxpSqNIaS9cxQFmVhj8rj7qTJgSM_WoFbc7KeBQjSGm_BydpOFfHzKyK7D5cZP0YQxzQGvvs7BMLWOOtMVknHdLgXx-8FoCrv4lMLODn56SOEJjvlYI5kIYlBDQbuIBgtmTkvvZsPnEoolP33rdWPu5B9FkLxDH7spwzNtRQy6IHGhCe07Q6pt1TwI7V4-u1mOlyV7gZMe4aW_Lpe21Rau3CIwril9tgj--MdKq2OPO8bL6UEKSK9XWsNAEmMtfX6fWUz0w8KTEMCqQJS7haAL-IxeLEqFHQPh_6PllHlHEv5jguK7j5XqfLVLmtKE3okczB066J8Za6qNXUutGWrZlggmCwoWoldhM1lqw69wFvGUERZ9fmNlFfN_fgDBeSfrQfh2vVcF18rrOv_VT3hxkj-81Zgwe=w502-h943-no" />

## Pipe
- 한 프로세스에서 다른 프로세스로의 단방향 통신 채널
- file은 아니나 system에서는 file 취급
-> write, read로 통신가능
> file descriptor과 달리 pipe는 읽기, 쓰기 file des가 각각 다르다.

```c
#include <unistd.h>
int pipe(int filedes[2]);
```
<인자>
- filedes[0] : 읽기용
- filedes[1] : 쓰기용
- return
	- 성공 -> 0
	- 실패 -> -1

- process당 open file수, 시스템 내의 file수 제한

### pipe의 특성
- FIFO 처리
- lseek는 작동하지 않음 (why? 읽은 데이터가 pipe에서 사라짐)
- pipe는 fork()에 의해 상속 가능
- 외부에서 access는 불가

### pipe를 이용한 단방향 통신 (부모 -> 자식)
1. pipe create
2. fork()에 의해 자식 생성 & pipe 복사
3. 부모는 읽기용, 자식은 쓰기용
4. pipe close

### 예제 code
```c
int main() {
	char ch[10];
	int pid. p[2];
	
	if(pipe(p) == -1){
		perror("pipe call");
		exit(1);
	}

	pid=fork();
	if(pid==0){
		close(p[1]);
		read(p[0], ch, 10);
		printf("%s\n", ch);
		exit(0);
	}

	close(p[0]);
	scanf("%s", ch);
	write(p[1], ch, 10);
	wait(0);
	exit(0);
}
```

### pipe를 이용한 양방향 통신
d<img src="https://lh3.googleusercontent.com/lzEzL9IFlLGCcYjY2ixDzx6fqQdfQtUw1cWO9XjCqRL4FB-8gRJxLHCLOrSeOrMaPa9wjiOpeSogWz9U9V8ZJkZCMCq_htySs4DcA5WTGswpZP659hyp32WIVD2dFx5NGofRiI0d8z3Xx6pP7JBLOzHTHQjc5DKWIxpkxuhsg-8eAScHuvvvwRT33Mtn2o1pxS1AjKVciKpWMTNMfn-Bn6yoW2Pzjo1ZfecZAA4z5rmhcIlG_aMBIycdEuPrqz-XpBHV2JY7JgjTjwWqw-Kfp8XgYxVH5R68vAJEe0Sh_eG3WP4_CHuumIYtaYcl_4LEFM_746PIzyiVuu37prBTbbdCTbKQBbDhmS9Xf_DKKXXflfy1Ad5tG5BbvLIiuPEuwoTj1p-jsDxUqmemfSHJx7I3PZ8nB8pXl_buNHMgOzJJ7VwjcTqhobTZ6eif6UB4_zDmKE9NQ5_yJrxqEJKHorUW4_26XPgfPpk0UwIrWsbNzNGL2rQbKSCFcClxCZgxjShQFWw2uihGQbWUS_NhB783HHWLnEg2WqyNeDWy62JCf0HWaaDSI6b3RrVho2EQQS5htH-SPmFhzP3wNw7qGMJVKAeQ3SrOD1a0tZfT69ckqH4QgyhfyHUMgd62SHtSweVryKV9zO1PROd_b8aCaD9W16qugQQZs5fjq5QpK8c3KnHSQg7JSpqAwNnyRl_WDe_22LTOW1qCTxagTJnofhMgMqFldqeIpJzgbFlysyBoI4M6=w356-h944-no"
/>

1. pipe 2개 생성
2. fork()에 의해 자식 생성 & pipe 2개 복사
	- pipe1 : 부모는 읽기용, 자식은 쓰기용 pipe를 close
	- pipe2 : 부모는 쓰기용, 자식은 읽기용 pipe를 close

> close를 안해도 괜찮다. 하지만 매 pipe마다 writer과 reader를 확실히 정해줘야한다!

### blocking read / blocking write
- read가 blocking 되는 경우 : pipe가 비어 있는 경우
- write가 blocking 되는 경우 : pipe가 가득 찬 경우

**-> 알아서 blocking하는 점 순서를 자동으로 맞춰준다.**

### pipe 닫기
- 쓰기 전용 pipe 닫기 : 다른 write가 없는 경우, read를 위해 기다리던 process들에게 0을 return (EOF와 같은 효과)
- 읽기 전용 pipe 닫기 : 더 이상 reader가 없으면, wrtier들은 SIGPIPE signal을 받는다. SIgnal handling이 되지 않으면 process는 종료; signal handling이 되면, signal 처리 후 write는 -1을 return;

#### 양방향 예제 code
<img src="https://lh3.googleusercontent.com/wampVbl82XMCrMLDpIoK08yd6DIxBvjfpJM0KvAJz-fBvtNAqo5jJWN1sQRXqNKcmkOA_GKJLnD-Td_m8WQw-aStrDYSDK180ZB76IUnCVvwXNj5X8wbQx9rZXv96XyjnFA0QWieTbnAd8GhZJG5DPhpTLs4fLlcO2Jaw6fJqzFj7g4DDOHuxxW-80Cm3G6LCtqy0FKOlcrPfkuzbtCeBvoXWQ_VCbVtUu2F5yHAbovDXiJY761tL7gHheocq0Zhn-TCGfKqUHgOHMZUOgIEAOigUUlZbecC0-jGuqILn0tQa9Tx9dDkrBzcANUC-S5E-gL_qUHh42frRPSIHHvV2-PdOps5Vfufg6BLeHzmGVmxCUhoqIvwBSXnpwuDWGja90XSNTxawQEieYEjGIQro5eaGfmVopiib3ks8F24YhnHl2EOGMVLT8Q9zJBZ8or5KBqzuqxxvs6tUhVwxM8HMxykoKfId4PqQlQLosBgMd-Ca-PctrQVRx8sSrZ9Pv3rqKHxo0RnrMg3Dp5uQY1JB2Rpftda1Vi800gcqd_8TFn6TlEpajiKhS8dKqEZqTuwHbczjwmQcBdgAZF3h06R_sEzn2KBohcg7EBIFF8elrz_FMsxAWd1EMIxRvq96REID2AFxulgZywwBhDtntkhib9eZ1cjPZIqMGKn-s5_MhKoGXebWbd7MfBT-S0tzhuaoSCG4ThnxjBbJULaXrzCqEMArZA1m5AuZCypPQe82XkTbGIg=w1430-h920-no" width=500px />

```c
int main(void) {
	int i, in, pid, p[2][2];
	
	for(i=0;i<2;i++){
		pipe(p[i]);
	}
	if((pid=fork()) == 0){
		close(p[0][1]);
		close(p[1][0]);
		read(p[0][0], &in, sizeof(int));
		in++;
		write(p[1][1], &in, sizeof(int));
		exit(0);
	}
	close(p[0][0]);
	close(p[1][1]);
	scanf("%d", &in);
	write(p[0][1], &in, sizeof(int));
	read(p[1][0], &in, sizeof(int));
	printf("%d\n", in);
	wait(0);
	exit(0);
}
```





<!--stackedit_data:
eyJoaXN0b3J5IjpbMjA5OTM1Nzg2Ml19
-->