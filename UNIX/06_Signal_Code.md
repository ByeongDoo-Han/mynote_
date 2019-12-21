## Signal
### t1.c 

child는 1초간격으로 "... child ...\n"를 print, parent는 child 생성후 5초 후에 SIGINT를 보내서 child를 종료시킴
```c
// signal handling 안하는 code
int main(void) {
	int i, status;
	pid_t pid;

	pid = fork();
	if(pid==0){
		for(i=0;i<100;i++){
			printf("... child ...\n");
			sleep(1);
		}
		exit(0);
	}
	else {
		sleep(5);
		kill(pid, SIGINT);
	 }
	wait(&status);
	if (WIFEXITED(status))
		printf("exits ... %ld\n", WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("terminated ...%d\n", WTERMSIG(status));

	exit(0);
}
```

### t2.c 

child는 1초간격으로 "... child ...\n"를 print, parent는 child 생성후 5초 후에 SIGINT를 보내고 child는 SIGNAL을 받고 catchint 함수를 실행하고 종료.
```c
// signal handling 하는 code
void catchint(int);

int main(void) {
	int i, status;
	pid_t pid;
	static struct sigaction act;

	act.sa_handler=catchint;
	sigaction(SIGINT, &act, NULL);

	pid=fork();
	if(pid==0){
		for(i=0;i<15;i++){
			printf(" ... child ... \n");
			sleep(1);
		}
		exit(0);
	}
	else {
		sleep(5);
		kill(pid, SIGINT);
	}
	wait(&status);
	if(WIFEXITED(status))
		printf("exits ... %ld\n", WEXITSTATUS(status));
	else if(WIFSIGNALED(status))
		printf("terminated ... %d\n", WTERMSIG(status));

	exit(0);
}

void catchint(int signo){
	printf("\n CATCHINT : signo=%d\n", signo);
}
```

### p1.c
parent는 1 sec 간격으로 child에게 SIGUSR1보냄, 이에 SIGNAL받은 child는 자신의 pid를 3번 print하고 종료

> lab9-1

```c
void printpid(int signo){
	int i;
	for(i=0;i<3;i++)
		printf("pid=%ld\n", getpid());
}

void do_child(int i){
	// SIGUSR1 signal 처리가 가능하도록 설정	
	static struct sigaction act;
	act.sa_handler = printpid;

	sigaction(SIGUSR1, &act, NULL);
	printf("%d-th child is creadted...\n", i);
	pause();

	// signal을 받으면, process id 세번출력하도록 설정

	exit(i);
}

int main(void) {
	int i, k , status;
	pid_t pid[3];

	for(i=0;i<3;i++){
		pid[i]=fork();
		if(pid[i]==0){
			do_child(i);
		}
	}

	// 1초씩 sleep()하면서, child들에게 SIGUSR1 signal 보내기
	for(i=0; i<3; i++){
		sleep(1);
		kill(pid[i], SIGUSR1);
	}
	
	for(i=0;i<3;i++){
		k=wait(&status);
		printf("child id=%d, exit status=%d\n", k, WEXITSTATUS(status));
	}

	exit(0);
}
```

### p2.c
child가 생성의 역순으로 pid를 출력하는 동기화작업

> lab9-2

```c
void printpid(){
	int i;
	for(i=0;i<5;i++){
		printf("child %d .... \n", getpid());
		sleep(1);
	}
}


void do_child(int i, int *cid) {
	// SIGUSR1 signal 처리가 가능하도록 설정
	static struct sigaction act;
	
	act.sa_handler = printpid;
	// 여기서 SIG_DFL 하면 마지막 child만하고 정상종료
	// SIG_IGN하면 lastchild를 제외한 child는 orpan이 되버림.
	sigaction(SIGUSR1, &act, NULL);

	// ...
	if(i!=4)
		pause();
	
	// ...
	kill(cid[i-1], SIGUSR1);
	
	exit(0);
}


int main(void) {
	int i, status;
	pid_t pid[5];

	for(i=0;i<5;i++){
		pid[i] = fork();
		if(pid[i]==0) {
			do_child(i, pid);
		}
	}

	for(i=0; i<5; i++){
		wait(&status);
	}

	exit(0);
}
```

### t3.c -> blocking X
```c
void catchsig(int);

void do_child(){
	int i;
	static struct sigaction act;

	act.sa_handler=catchsig;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);

	for(i=0;i<5;i++){
		printf("child is running...\n");
		sleep(1);
	}

	exit(0);
}

int main(void){
	int i, k, status;
	pid_t pid;

	pid=fork();
	if(pid == 0){
		do_child();
	}

	sleep(2);
	kill(pid, SIGINT);
	sleep(1);
	kill(pid, SIGUSR1);
	sleep(1);
	kill(pid, SIGUSR2);

	wait(&status);
	printf("exit status=%d\n", WEXITSTATUS(status));
	
	exit(0);
}

void catchsig(int signo){
	int i, big;
	for(i = 0 ; i<5;i++){
		printf("handling sig=%d ... \n", signo);
		sleep(1);
	}
}
**********OUTPUT***********
child is running...
child is running...
handling sig=2 ... 
handling sig=10 ... 
handling sig=12 ... 
handling sig=12 ... 
handling sig=12 ... 
handling sig=12 ... 
handling sig=12 ... 
handling sig=10 ... 
handling sig=10 ... 
handling sig=10 ... 
handling sig=10 ... 
handling sig=2 ... 
handling sig=2 ... 
handling sig=2 ... 
handling sig=2 ... 
child is running...
child is running...
child is running...
exit status=0
```
### t4.c -> blocking O(with. sigemptyset)
```c
void catchsig(int);

void do_child(){
	int i;
	static struct sigaction act;
	
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGINT);
	sigaddset(&act.sa_mask, SIGUSR1);
	sigaddset(&act.sa_mask, SIGUSR2);

	act.sa_handler=catchsig;

	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
	sigaction(SIGINT, &act, NULL);

	for(i=0;i<5;i++){
		printf("child is running...\n");
		sleep(1);
	}

	exit(0);
}

int main(void){
	int i, k, status;
	pid_t pid;

	pid=fork();
	if(pid == 0){
		do_child();
	}

	sleep(2);
	kill(pid, SIGINT);
	sleep(1);
	kill(pid, SIGUSR1);
	sleep(1);
	kill(pid, SIGUSR2);

	wait(&status);
	printf("exit status=%d\n", WEXITSTATUS(status));
	
	exit(0);
}

void catchsig(int signo){
	int i, big;
	for(i = 0 ; i<5;i++){
		printf("handling sig=%d ... \n", signo);
		sleep(1);
	}
}
**********OUTPUT***********
child is running...
child is running...
handling sig=2 ... 
handling sig=2 ... 
handling sig=2 ... 
handling sig=2 ... 
handling sig=2 ... 
handling sig=10 ... 
handling sig=10 ... 
handling sig=10 ... 
handling sig=10 ... 
handling sig=10 ... 
handling sig=12 ... 
handling sig=12 ... 
handling sig=12 ... 
handling sig=12 ... 
handling sig=12 ... 
child is running...
child is running...
child is running...
exit status=0
```
### t5.c

flags = SA_SIGINFO로 더 많은 정보를 출력한다.

```c
void catchint(int, siginfo_t *, ucontext_t *);

int main(void) {
	int i, status;
	pid_t pid;
	static struct sigaction act;

	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = catchint;	//incompatible pointer type
	sigaction(SIGUSR1, &act, NULL);

	pid = fork();
	if(pid == 0){
		for(i = 0; i < 5; i++){
			printf("... child ... \n");
			sleep(1);
		}
		exit(0);
	}
	else{
		sleep(5);
		kill(pid, SIGUSR1);
	}
	wait(&status);
	if(WIFEXITED(status))
		printf("exits ... %d\n", WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("terminated ... %d\n", WTERMSIG(status));
	
	exit(0);
}

void catchint(int signo, siginfo_t *sf, ucontext_t *uc){
	printf("\n CATCHINT:signo=%d\n", signo);
	psiginfo(sf, "...");
	printf("\n%d\n", sf->si_code);
}
```
### alarm1.c -> alarm recursive in 3 sec.
```c
/*주기적으로 작업을 할려고하는 code*/
// alarm(3)
//  |
//  v
// 알람울림 다시 alarm(3)
//  |
//  v
// 알람울림 다시 alarm(3)
//  |
//  v
// 알람울림 다시 alarm(3)
void catchalarm(int);

int main(void) {
	int i, n;
	static struct sigaction act;

	act.sa_handler = catchalarm;
	sigaction(SIGALRM, &act, NULL);

	alarm(10);
	for(i = 0; i < 10; i++){
		printf("... child ... \n");
		if(i == 1){
			n = alarm(3);	// 여기서 alarm 3초
			printf("%d\n", n);
		}
		sleep(1);
	}

	exit(0);
}

void catchalarm(int signo){
	printf("\n CATCHALARM:signo=%d\n", signo);
	alarm(3);	// alarm을 재설정하거나 끄지 않으면 3초마다 울림
}
```

### alarm2.c

scanf를 받고 sum을 구한다. alarm초간 반응이 없을시 SIGNAL처리

```c
void catchalarm(int signo);

int main(void){
        int i, j, num, sum = 0;
        static struct sigaction act;

        act.sa_handler = catchalarm;
        sigaction(SIGALRM, &act, NULL);

        for(i = 0; i < 10; i++){
                do {
                        alarm(10);
                }while(scanf("%d", &num) <0);   // scanf가 성공할때 까지 alarm을 10>초로 계속설정
                alarm(0);       // scanf가 정상적으로 되었을때 다른작업을 위한 알람>을 끄기를 해야한다.
                sum += num;

                for(j = 0; j <= i; j++){
                        printf("sum=%d\n", sum);
                }
        }

        exit(0);
}

void catchalarm(int signo){
        printf("input !!! input !!! input !!!");
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbMjk0ODkyMzU3LDE5MzM2ODY0MjksODc5MT
M2Njg1XX0=
-->