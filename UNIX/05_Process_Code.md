## process
### print pid, gid, sid
> lab6-2
```c
int main(void) {
	printf("pid=%ld\n", getpid());
	printf("parent pid=%ld\n", getppid());
	printf("group id=%ld\n", getpgid(0));
	printf("session id=%ld\n", getsid(getpid()));

	return 0;
}
```

### main (int argc, char** argv)
> lab6-3
```c
int main(int argc, char **argv){
	int i;

	printf("%d\n", argc);
	i = 0;
	while(argv[i] != NULL){
		printf("%s\n", argv[i]);
		i++;
	}
	return 0;
}
```
### compare child, parent pid, ppid, gid and before & after setpgid
```c
int main(void) {
	pid_t pid;

	printf("parent .. %ld : %ld\n", getpid(), getppid());
	printf("parent .. %ld vs. %ld\n", getpgrp(), getpgid(0));

	pid = fork();
	if(pid==0){
		printf("child before ... %ld : %ld\n", getpid(), getppid());
		printf("child before ... %ld vs. %ld\n", getpgrp(), getpgid(0));
	
		setpgid(getpid(), getpid());

		printf("child after ... %ld : %ld\n", getpid(), getppid());
		printf("child after ... %ld vs. %ld\n", getpgrp(), getpgid(0));
	}
	wait(0);
	return 0;
}
```

### after fork, compare file descriptor between parent and child process
```c
int main(void) {
	int fd;
	pid_t pid;
	char buf[10];

	fd = open("data", O_RDONLY);
	read(fd, buf, 10);
	printf("before fork : %ld\n", lseek(fd, (off_t)0, SEEK_SET));

	switch(pid=fork()){
		case -1:
			perror("fork failed");
			exit(1);
			break;
		case 0:
			printf("child before read : %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
			read(fd, buf, 10);
			printf("child after read : %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
			break;
		default:
			wait((int*)0);
			printf("parent after wait : %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
	}

	return 0;
}
```
### functionize child process separated from parent (Good version)
```c
// main문에는 child process의 실행 code를 두지 말것!!!!!!!! (child 할 일 함수화)
void do_child(void){
	printf("pid=%ld ... ppid=%ld\n", getpid(), getppid());
	exit(0); // 이게 중요!!!!!!!!!!!!
}

int main(int argc, char** argv){
	int i, N;
	pid_t pid;

	N=atoi(argv[1]);
	printf("pid=%ld ... ppid=%ld\n", getpid(), getppid());
	for(i=0;i<N;i++){
		pid=fork();
		if(pid==0)
			do_child();
	}
	for(i=0;i<N;i++)
		wait(0);
	return 0;
}
```
### number of "argv[2]" child process, "argv[1]" file open and write child pid;
```c
void do_child(char** argv) {
	int fd, n;
	char buf[5] = {0};
	fd=open(*argv, O_RDONLY);
	
	write(fd, buf, 2);
	
	printf("%s by %ld\n", buf, getpid());
	exit(0);
}

int main(int argc, char** argv){
	int i, N;
	pid_t pid;

	N = atoi(argv[2]);
	for(i=0;i<N;i++){
		pid=fork();
		if(pid==0)
			do_child(&argv[1]);
	}
	for(i=0;i<N;i++)
		wait(0);
	return 0;
}
```

### atexit
```c
void ABC(void){
	printf("ABC ...\n");
}
void DEF(void){
	printf("DEF ...\n");
}
void GHI(void){
	printf("GHI ...\n");
}

int main(void){
	atexit(ABC);
	atexit(DEF);
	atexit(GHI);
	exit(15);
}
```
### execl
```c
int main(void) {
	execl("./test1", "test1", "abc", "def", "ghi", (char*) 0);
	
	exit(0);
}
```
### execlp
```c
int main(void) {

	execlp("test2", "test2", "abc", "def", "ghi", (char*) 0);

	exit(0);
}
```
### execv
```c
int main(void) {
	char *av[] = {"test1", "abc", "def", "ghi", (char*)0};

	execv("./test1", av);

	exit(0);
}
```
### execvp
```c
int main(void) {
	char *av[] = {"test2", "abc", "def", "ghi", (char *) 0};

	execvp("test2", av);

	return 0;
}
```
### make N child process, "wait" child process's status and exit
```c
void do_child(void) {
	int i;

	for(i=0;i<5;i++){
		printf("child %ld running ...\n", getpid());
		sleep(1);
	}

	exit(0);
}

int main(void) {
	int i, status, N=3;
	pid_t pid;

	for(i=0;i<N;i++){
		pid=fork();
		if(pid==0)
			do_child();
	}
	
	// N개의 child가 있으면 N개의 wait도 중요!!!
	for(i=0;i<N;i++){
		pid=wait(&status);
		if(WIFEXITED(status)){
			printf("%ld ... %d\n", pid, WEXITSTATUS(status));
		}
	}

	exit(0);
}
```
### "waitpid"
```c
void do_child(void) {
	int i;

	for(i=0;i<5;i++){
		printf("child %ld running ...\n", getpid());
		sleep(1);
	}

	exit(0);
}

int main(void) {
	int i,n, status;
	pid_t pid;

	pid=fork();
	if(pid==0)
		do_child();
	
	while(1) {
		n=waitpid(pid, &status, WNOHANG);
		if(n>0 && WIFEXITED(status)){
			printf("%ld ... %d\n", n, WEXITSTATUS(status));
			exit(0);
		}
		printf("waiting...\n");
		sleep(1);
	}
}
```
### make 3 child process, execl {"p1_a", "p1_b", "p1_c"} and wait
> lab8-1
```c
int main(void){	
	int i, status;
	pid_t pid;
	
	for(i = 0; i < 3; i++){
		pid=fork();
		if(pid==0&&i==0){
			execl("./p1_a","p1_a", "1", "abcde", (char *)0);
		}
		else if(pid==0&&i==1){
			execl("./p1_b","p1_b", "2", (char *) 0);
		}
		else if (pid==0){
			execl("./p1_c", "p1_c", "3", (char *)0);
		}
	}
	
	for(i=0; i < 3; i++){
		pid = wait(&status);
		if(WIFEXITED(status)){
			printf("...... %d\n", WEXITSTATUS(status));
		}
	}
	
	exit(0);

}

```
### make 3 child process, execl, waitpid
> lab8-2
```c
int main(void){
	int i, status, n;
	pid_t pid[3];

	for(i =0; i < 3; i++){
		pid[i]=fork();
		if(pid[i]==0&&i==0){
			execl("./p1_a", "p1_a", "1", "abcde", (char *)0);
		}
		else if(pid[i]==0&&i==1){
			execl("./p1_b", "p1_b", "2", (char *) 0);
		}
		else if(pid[i]==0){
			execl("./p1_c", "p1_c", "3", (char *) 0);
		}
	}
	for(i=2;i>=0;i--){
		n = waitpid(pid[i], &status, 0);
		if(WIFEXITED(status)){
			printf("%d ... %d\n",n, WEXITSTATUS(status));
		}
	}
	exit(0);	
}
```


<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE4OTE0NjY3NThdfQ==
-->