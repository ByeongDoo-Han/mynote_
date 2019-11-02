




# Process
- **process** : 프로그램 코드, 변수, 스택 등에 저장된 값, PCB 내용 등을 포함

- 계층구조 : parent process -> child process(UNIX system의 모든 process는 init의 descendent process)

- Shell 상에서 process 목록 확인 -> ```$ps```
- 실행 중인 프로세스 종료 시키기 -> ```$kill -9 프로세스 번호```

### process identifier

=> 음이 아닌 정수
- 0 : swapper
- 1 : init

```c
#include <unistd.h>
pid_t getpid(void);	// process id
pid_t getppid(void);	// parent process id
```

### process Group

process group
: 프로세스 들을 하나로 묶어서 하나의 group을 만듦
: 같은 group에 속한 process들에게 동시에 signal을 보낼 수 있음
: 초기에 **fork**나 **exec**에 의해 group id 계승

group leader
: 자신의 pid가 group id이면, group의 leader

- group id 검색 System call
	```c
	#include <sys/types.h>
	#include <unistd.h>
	pid_t getpgrp(void);
	pid_t getpgid(pid_t pid);
	```
	=> getpgid의 인자가 **0**이면 호출 프로세스 자신의 group id를 검색 

- process group 변경
	```c
	#include <sys/types.h>
	#include <unistd.h>
	int setpgid(pid_t pid, pid_t pgid);	// 두개의 인자가 동일(group 변경시 leader group으로만 변경가능
	```
	=> pid인 프로세스의 group id를 pgid로 변경

# Session
- 한 session은 한 단말기를 사용하는 foreground process group과 background process group의 집합체(**하나의 창**)
- 각 process group은 하나의 session에 속함

### getsid() System call
:  session id를 획득
```c
#include <sys/types.h>
#include <unistd.h>
pid_t getsid(pid_t pid);
```
### setsid() System call
 : session이 종료되어도 죽지 않는 process를 만듦
```c
#include <sys/types.h>
#include <unistd.h>
pid_t setsid(void);
```

- 제어 단말기를 갖지 않는 새로운 session과 group 생성;
- 호출 프로세스의 id가 session과 group의 id가 된다.
- 만약, 호출 process가 현재 group의 leader이면 -1를 return
```c
int main() {
  pid_t pid;
  int p[2];
  char c='?';

  if (pipe(p) != 0)
    perror("pipe() error");
  else
    if ((pid = fork()) == 0) {
      printf("child's process group id is %d, process id is %d\n", (int) getpgrp(), (int)getpid());
      write(p[1], &c, 1);
      setsid();
      printf("child's process group id is now %d, process id is now %d\n", (int) getpgrp(), (int)getpid());
      exit(0);
    }
    else {
      printf("parent's process group id is %d, process id is %d\n", (int) getpgrp(), (int) getpid());
      read(p[0], &c, 1);
      sleep(5);
    }
}

**********OUTPUT*************
parent's process group id is 25373, process id is 25373
child's process group id is 25373, process id is 25374
child's process group id is now 25374, process id is now 25374
```
<img src="https://lh3.googleusercontent.com/eNGHn1_qZAQuXU2EmZc4wMvNNDl10IKriF1uojz1Ou-VtbAAmwKe08wZLgYWMQbzHtI4uwXoVoN8VCRnfHpO8crsiiD4TnHyjjT8buKHt7g4c2YbsLTJU7lexrw5xxB4NQM1hr4s7AO9a2_BMAKmSnXUd9gLbNt8mk_WMhkFAfYqAhPZSfUd3fL1usy-cuLmy9Ej9qBssp6o13TYCqx0c3tz27D8DKQMtJFKQEUIbTe4bRCMOIMYg9Jj8XiSb22QlRyJVPuJdfM2rFsU2hG6A8FxJLSFtnMQKI6gHU1cz4tghGZEJkAN49pJICRfdQwDKt6Gkzg06035Ig8M0QeOXDAGrg2SgIHtuzkPuD8-Rt8SkcF1-gWuduvV8ywf-lBA5QXi5GkNrB-An3aygM8svrLL2uSsq1gbqLrhyInoxqf29gCQybpN_hnVk9vswus93kg8CtzwyYD5cLnVK8iug5PStOJghKavPzpCLPGIYFMG8x1DYiajI0D7EPXEzE-FEsVPNd9pA6MgVyThuBx408n1llEQZdqEa1RsV_yqdibBA32dn2rhIpauJlSn72cyX92IX8dD1K7I9Pj9zLh8XXEKYg6TNIQaQlN6whSYkQP9ZqKeRFmU7Gkw6j1zlYVbNlQB1T0YkJY4HjnJkJDuOD9TAwrdRke0X8m3vQeG3HD-uFWe-IR_zHQ=w977-h943-no" width=500px />

> #### process와 program의 관계
>  1대 1의 관계가 아니다!
>  program 내의 여러 개의 process 생성 가능.

### main() 함수 인자 사용
```c
int main(int argc, char **argv){
	int i;
	printf("%d\n", argc);
	i = 0;
	while(argv[i]!=NULL){
		printf("%s\n", argv[i]);
		i++;
	}
	return 0;
}
```

`$ ./a.out abc def ghi [enter key]` 인 경우

**argc**(문자열 몇개? 실행시킨 프로그램 포함) = 4;
**argv**(포인트 배열) = {"a.out", "abc", "def", "ghi};

사용목적? child process로 parent process가 인자 전달이 필요한 경우

## Process 생성
### fork System call
: 수행되던 process(parent)의 복사본 process(child) 생성
```c
#include <sys/types.h>
#include <unistd.h>
pid_t fork(void);
```

- stack, PCB까지 똑같이 생성
- fork() return 받은 다음 문장부터 동시에 실행
<img src="https://lh3.googleusercontent.com/FT-_X8vciOh9mZJAeANP45ZJ0lWmWb8p6pkM_TkwHaSEGFMGhgeCRtUGmCSpshAHB_yUhzzgVfhISc81qrr2UOib79yV8fglsiZogisJ-3qncD6Vk8O1-f_cYVD_6BzOHJmk5GCTCkvC9YDm7-TWOK_KF5vzzABI96fOHXsJBRZu63CjXH0w2LUNEHgprtKwu3SXcss85srn345bq91MtFxNtCUBM1f_Ez3GGY3jxdm_l7RdjcMyFdP9YXaT3W508tLzisIMErhsHZCJaUC2cviBc8yiPS83dIyIGoA0BQrSIsGenM7NpkAmmI4yI5Bf5cspePsGCUpyo0jcpu_mCOEGQYwBQ6zAHkhiXiP-X3vyrc00ov3Li3pHzcj2KVXhxMOzZmsBsGjA3JORGZtL_a-o0ZMUyohj29FBi0Rdw2SjBbvRDbs0DL-6T4YIBJp6DvjPJqOFI0sf9XIjkJC6_rA74FJ0YTcLwTJiyIsf0_rqYTw1QVtFbvHwjpgCagpxrZflFBz6ROMt7e8PSRdqqJkF0rPu1OtIrRrFz5oey0lsz1t8v1lPvEP1z3axmaA2BeG-QVBmhoNohAqqpWbR7yCqGvCRVyJRchcPhNtdSZMcNHkcV7emueyyjNr5I7MorY9ifMLrVsONtGWTQ7D5tfMIQdHI1cVEgiSFpDvVuVSinV1B373RbPz4DgBS7NhCnIXNLpcEc66lIFN05iWpcGGgRXF0pZXgFAHMqaZ7At4bHmAC=w875-h652-no" width=600px/>
- 두 process의 차이점
	- pid와 ppid가 다르다.
	- fork()의 return 값이 다르다.
		- parent process의 return -> child process의 pid(0보다 큰 정수)
		- child process의 return -> 0
- fork 실패시 -1 return
	- 실패 원인 :
		- 시스템 전체 process의 수 제한
		- 한 process가 생성할 수 있는 process 수 제한

- 예제 code)
	```c
	int main(void){
		pid_t pid;

		printf("Calling fork..\n");
		pid=fork();

		if(pid==0)
			printf("I am the child\n");
		else if(pid>0)
			printf("I am the parent, child has pid %d\n", pid);
		else
			printf("Fork returned error code\n");
		return 0;
	}
	```

#### fork : 파일과 자료
 - child process는 parent의 복제
     - 모든 변수 값이 그대로 복제된다.
     - fork()후에 변경된 값은 복제 되지 않는다.
     - **file descriptor**도 복제 된다.
         -> parent process가 open한 file pointer도 child process도 똑같이 복제(parent : A file 5번째 3글자, child : A file 5번째 3글자)
     - parent와 child가 file을 공통으로 사용가능
 <img src="https://lh3.googleusercontent.com/kUuL2GuxZYSnNEMYEy7j30ionMoL2dmhchfKKCQS6ykCRU_QMnoY3E_NxzoTq2_JxO3gv5XWPQ4_mykaEKqtLj4p99mlMg6vwEe_S7zBLB4SoKBGWXv3kBuOqRGO5h7Ze3siq8XRWqWq-rNV-DJNO-GU3oFySMU6t-DPrdYn2YggPTN3QMbdZ82Uckz7UUMyUS_5QEaTKciO9oKSPjlO_y8_v5BqrkLQsMyLPkk0xXpModEuTcCTIrWwhhP8XiDMFIMgVbRgzaFXfWsuPjUqgr2HkwLGdOmfKlZXWdF432iDX4S7VwXjts-dAeB0LtTdDNU2iCKtrdAMXN4X5y67tvsimh7w5PYZSpwGw46_u48vfuypGddHooOXvMnw89PInxdULpE1Lg7PGs4ZDQR2yDHjkwhlf6IkuqqF5AXVkMlYuY2P5MxuaZRETvEp1Xh6fM3W8ehZ95h11DEjLYGplISVXDZKOr7dwDsng7JKHKu3azixop-L9P2klvPkYLzcprr9ci8NGx4B-E45lzxrsDHG7KoSAnkxZX6A-ZhckHLSWVgV1FN50hVgMxz0UbK7gK_8aaz9LeDj0WReuVY_1HIOI-n5rmfaonhLHk4i7kjyd3NGy0OsQApIVGbhvfjaTpRlxgPAl-wH-YmLWETFtbA60B167wetUwWHhCh-KwjqNwF19DpbUfqdB3VaG3yg-uZNgHBvKS4RYoOKCD457iMCEzur-QJHzrEhOTQWVXR8wDgp=w1023-h653-no" width=500px/>
 - 예제 code)
	```c
	 #include <unistd.h>
	 #include <fcntl.h>
	 main(void) {
	     int fd;
	     pid_t pid;
	     char buf[10];
	      
	     fd=open("data", O_RDONLY);
	     read(fd, buf, 10);
	     printf("before fork:%ld\n", lseek(fd, (off_t)0, SEEK_SET);
	      
	     switch(pid=fork()){
	         case -1: 
	                perror("fork failed\n");
	                exit(1);
	                break;
	         case 0:
	                printf("child before read: %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
	                read(fd, buf, 10);
	                printf("child after read: %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
	                break;
	         default:
	                wait((int*) 0);
	                printf("parent after wait: %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
	            }
	 }
	  실행 결과:
		  before fork:	0
		  child befor read:	0
		  child after read:	10
		  parent after wait:	10
	```

> #### parent code에 wait()가 없을 시
>  <img src="https://lh3.googleusercontent.com/nqrsRXd6gGcI7C7wm64bA8d6WC930anAd57L4Pjy0ICL13SPgamOdjOTkm0E4bDT4oZioX8VD-YKiGtx2B7C-wYWsoyQ2kb6vVBPaGTT2I23szP-xcs11IUhnD_feO616d4VhBrg0GBmu1t5xSuNX0pbbW1HSrv0h3A7zBn3cQMrZo1qsyDoae3WLzY2huFxylqqCeyO7R0sYtUYXZhH1Jnp8VkTYfXUxHmWXVGQaPxVF8snvjUZYgUjjIPJHb8_95M69czh6AMC2sBv7CyDmjfz6Bx3V_VhnLfPHGr0eg_LV2Ruli0KIIMcihZUS3ScxwUnMwMEjk5VpN8rqMB8sX8PrPXDcNfo7NAd1bTOMf7LX_fNYzijH13ecy--QLfg4c902oaIkarWpuTtAHOqKXZwejO1Kd0jpaLnv_IeauGhqrdnYIGR-KSGy4ItqKp4t_acTpnpeqwb40Y-3g0mInriQEl6MD1j8IWIP02bPhvSdfqZNxzAyqlTRGwDpeHmySizbU8r179YkJB2pXrCt2eIS7k3uGcy5q0KB5z-OxVXvA2ZqLntHtDAD-pOFp5RQJpBFraK_0QdIS1gDmxhlBkD1tfDW8BBK10pvTk3FJb1_0nbdYCl5kFOUdR-Otvsh44hINcelCxSMg4aNOUvCY7XapIJrZjqiUGfx7sDVzclnDfR8Lap8k17LXTiyp_HX_ISccEHwiNuZUllr5F47MRqK30HJ0_1iTGFRLUXkzckd6BJ=w1003-h637-no" width=700px/>

> #### child의 ppid가 1인 경우?
> -> 실행도중 parent process가 없어짐
> -> 고아 process를 가져다가 pid=1인 process(init)의 child로 만듦

> #### 잘못된 반복문으로 자식 프로세스 생성시
>  ```c
>  int main(int argc, char** argv){
>	int i, N;
>	pid_t pid;
>
>	N=atoi(argv[1]);
>	printf("pid=%ld ... ppid=%ld\n", getpid(), getpgrp());
>	for(i=0;i<N;i++){
>		pid=fork();
>		if(pid==0){
>			printf("pid=%ld ... ppid=%ld\n", getpid(), getpgrp());
>		}
>	}
>	for(i=0;i<N;i++){
>		if(pid>0) wait(0);
>	}
>	return 0;
>}
>```
> 3개의 자식 프로세스를 생성하고 싶지만 7개의 자식 프로세스가 생기는 이유? 
>
>이렇게 코드를 짜면 다음과 같이 생성
> <img src="https://lh3.googleusercontent.com/ERRhNiNdrN0VARMFpkILk0XsWXf8egQZ1KPBgYKX3o_PvQSsUykFlAFIyp4wopK6Dfr4mYrVpM0iEBtuBwBbc5UKXTD78cvNxtR8VNHh270_JnmUpiepRnLZBPAw89f4zLwI14DPYOQH9U0N7hqIFEsGGYROpwolkUUiCzds68dhJABnBeJmusPA_ZoeAheQs0HyZxuSHOyKpwkoC-j5GE5KucK0vm3eCQl40EgNF0R8oQXHIypvqiQTrkOqKIXPK23_a9r1sjGMBGLCW98YCr78v3z2bPDAPjC57t0PJpgfL_lBsS36iNG0IyzYEfIdESHQtZzsqNSiza5chw_AvPSZQXEcFyz4aqNlmg6xfpxTEnW77Vz-Q4lckeshxLnAUrdV9NyummZSkDsmEqMqSttMdCE7O8L5Sm6DRusj0BqUIx-eXBBqTGx6dfIx-9iaWn7qCta5u3C6a5NjFng-pppzb4nI5L5PE425gH1KV_a8yF8Zn17ag571eTAQusARxVonvx6vscgHNvh7TfyWe7Q7UetaMWktehYAg3UOrpPxHB-MZ2jgnnKL9ZS2iglQpbhhTHLbyM55Rb6CaNrvX3cxsObhF4p8hwt9R4xiGhJGHOsTMZnlQAn5Gfxsksz8ekc52pzaOimBgFmuBcz_po4gOc1MEgQ5xElqbMwkxctVyQ7UbDoWpoQXdAkhqhFrCNSbRkFI-LW1W49SXtTPbGxuU8pUkAaiQxW0SWx-3wCO3gqK=w385-h346-no" width=400px />

### exit System call
: exit: process 정지-> open된 file 닫기 -> clean-up-action
```c
#include <stdlib.h>
void exit(int status);
```
- status의 값?
프로세스 종료후, `$ echo $?`명령에 의해 알아낼 수 있다.

- clean-up-action 지정
	```c
	#include <stdlib.h>
	int atexit(void (*func) (void));
	```
	- 지정된 순서의 역순으로 실행;
	- ex) 종료할 때 공유자원에 대한 사용 해제

> **Parent process는 child의 exit을 확인할 의무가 있다.** -> 안할 시 **좀비 process**

### exec System call
: 지금 실행하고 있는 process가 현재 process를 버리고 새로운 process를 실행
```c
#include <unistd.h>

int execl(const char *path, const char *arg0, ..., const char *argn, (char*) 0);

int execlp(const char *file, const char *arg0, ..., const char *argn, (char *)0);

int execv(const char *path, char *const argv[]);

int execvp(const char *file, char *const argv[]);
```
- 공통점:
	- 호출 프로세스의 주기억장치 공간에 새로운 프로그램을 적재;
	-> 호출 프로세스는 **no longer exists**;
	-> 새 process는 **처음부터 실행**;
	-> 새 process는 호출 프로세스의 id로 실행(**pid 그대로**)
	- 실패 시 -1 return; 성공시 return 없음;
	- fork와의 차이점 : 기존 프로세스와 병령 수행이 아님
	<img src="https://lh3.googleusercontent.com/kggN3J3q3cUgHcj0OBWAJZcPn8zXXgtEaDNmpEtea9IGmP6FUVSmHJ5ZXi7JrcsdJr7y1YKWJ4cguANl_6S6tc5uPDECTU0h4BGVpVdViAGr1uZKqzQbGV0tnPUU7QE9K24e1L5R1cJHM0Mx4CneuryhEc3mIV2Tx3ivmXGuv_6el44tAccFiaPXKj8yuBtxv62pxtoH5uN3G6P4wW_QDK1HUOQ0ThRYLWQnrfCkgnsU36CK96wqtusixuUekBzY1UUZQ_F91kPLOo4GamUwd-rxwLv5AgqkFom-m5RgVfvs9pQiaaKr1AqhNV6bxSSMbMvvkGrumHxmQWYr2HJa-kI6UX_tOywN8hmBG7vxlFwbtKKd3psYpmxNP_VPO6Z1yv9SacUFUUpjtxPGPqhZ3qUoy9Kyyz60J8OkKZBy8tg8zmW2vpzcvVqDr5wOErCm46nLZmONxAgsHgxsiIJsxTgNLTV_XM95c8dEEKrH0YmcPiJRNeEHkyZYOe16wVEpjayiKb1a3j3zz3oPEWRnAWo8wcWo4637NUP1OsNvw4XlXNWfRlFbQR9l0r9hyMTIH033NgYTyQafoFW_q9cLFJpbCv10ck55oIssk79YPOFCY1XcEG5r3_HZq__Q3qz18nGYmhWrqpM4BfpfYXCJu0zQoiaYtePDZmtIRs269HOoyTebGaTWdYtWzCkLkaWXE2qFC5oLsXp-1IPud8LaB3BHfmWUl2G5Rj8xp6egzy9K_mv-=w475-h356-no" width=500px />
- 차이점;
	-  path: 파일 경로 이름 포함 vs. file : 파일 이름;
	- 인수:
		- arg0 : 프로그램 이름(열로 이름 빼고); and arg1, ..., argn: 프로그램에 입력으로 사용될 인수들, 마지막엔 null ptr;
		- argv[] : 배열로 받기;

- file 이름을 쓰는 경우는?
환경 변수에 의해 설정된 path안의 file; (`$echo $PATH`)

- 예
#### 1	
```c
	#include <unistd.h>
	main() {
			printf("executing a.out\n");
			execl("./a.out", "a.out", "3", (char *)0);
			printf("execl failed to run a.out");
			exit(1);
	}
```
#### 2
```c
	#include <unistd.h>
	main() {
		char *const av[] = {"a.out", "3", (char*)0);
		printf("executing a.out \n");
		execv("./a.out", av);
		printf("execv failed to run a.out");
		exit(1);
```

- exec와 fork를 함께 사용
<여기 이미지 추가>

> **Q.  parent process가 직접 실행하지 않고 exec 하는 이유?**


```c
	#include <unistd.h>
	main() {
		pid_t pid;
		switch(pid=fork()){
		case -1: 	perror("fork failed");
					exit(1);
					break;
		case 0:		execl("/bin/ls", "ls", "-l", (char*)0);
					perror("exec failed");
					exit(1);
					break;
		default:	wait((int*) 0);
					printf("ls completed\n");
					exit(0);
		}
	}
```
### 프로세스의 동기화

### wait System call
```c
#include <sys/wait.h>
#include <sys/types.h>
pid_t wait (int* status);
```
<img src="https://lh3.googleusercontent.com/hIpOk5r0hRyMleKdz7vlOhAB3-zi0Pa-IFn3jophPPwf65gF8SEUokrR-fkm7RfOinBcXu-5tmuaNBrFX5jYwn1-L_FWEcN3yu4dWfp1GB-Qr4X3eFJoYJqltjTJLvQBCNi2-mbhCEMsaZ3tLZw4AnXCsawyeXqmT-zGbw3PK0ITpRFtBW-5NU_dnW5Chkjq9iy5uP4OqxEI5Lqnef16Hmc3GVhWRCgJ9ex5ZNMdScgHTfUGm_hkBK806VyyEhcJ9dHY4Kg_ONEwKMLfoH1-ShTC45uFeLS1qUmIhirH4_RNbQKXk4HrlmEAJ7S8m61Psh9Osw3QZA-8KBAOT7b0w_sXgDRpI5X08FIt0qCWCTefsPctjqPH5sCLJOJlFW-WTzi6Vx2mnimK4g_pEsLCGCKzieI4eAsBioFQahuCZYgVLTrhKKTowlu361gPrdLn34RcrFuTFdld3Dy5CqBolm51cF1azUyxuzR8les07Fgs6lfXH7UjFLhyIcYBNYhMT86n7JJWp58dOEkBEiNo1PNPzJmDVPoGDyf9w1VzSXxDK4v_xn_AP__DX8migGLw5vsK1G7AToVvOEpDYfqKgisicsXcbSlxnYr1LRW_s6NcX2pSGbjETg6i2BDPovu4I5VTGEbXvAiQs6X3YsJs0yNxXJ8d50lSzMXp1NjCH8h3d0oWVPgGjOZNVZLB5O92zzOeQeXiGy8nNQzbV-cdNLxLaDHFh6VMbPCJsOcYzjIJx--O=w958-h857-no" width=500px />

- 하나 이상의 child process 수행 시 아무나 하나가 종료되면 return
- return값
	- 성공 -> 종료된 child의 id
	- 실패 -> -1 (살아 있는 child process가 없는 경우)
- wait ex code)
	```c
	void do_child(void) {
	        printf("%ld\n", getpid());
	        sleep(1);
	        exit(0);
	}

	int main(void){
	        int fd, i, status, n;
	        pid_t pid;

	        for(i = 0; i < 3; i++){
	                pid =fork();
	                if(pid == 0)
	                        do_child();
	        }

	        while((n = wait((int*)0)) != -1){
	                printf("%d\n", n);
	                sleep(1);
	        }

	        return 0;
	}
	*********OUTPUT*************
	32368
	32370
	32371
	32368
	32370
	32371
	*****************************
	```
- status : child의 종료 상태가 전달

<img src="https://lh3.googleusercontent.com/l7cZfh0D-XlXHWzC2qysETKTwaKsxUhwM7LHwSrklG9qJ6WY-hPWRmHTmFf_EZSPonQEWzMH3Lxf6hXQY2Nnydit8vel-S5aLAWAa6L2mWfRQ07Mv3ArZZWdwglO3z4VnboOtl9IRAW6dnukaqLxRjqUqp9fGMZTwioYaehUQnzubWEnwa1UKGwa0m3dRiglAUEfqGcHgt2Cl45z5KM7V2Gn4XQKY574ToACOIV61R21r15xiJmmZrxXrKbgT-Bb2mTZ1a_-tvgIwr02M1aYkR_9CmyCkFxdPbEid4vC1-SCc0b_o8MSTOLeJu1WhCv3327Piqu25ozVfQ8C3FNcMiXO9MQze1_F2zFMw55Fdab6Tq6HizJQUCb7mbFlYQWDRJ_sjyubcr2-xftMC6bbCVQSC1un90n6j1f2mH6zOs9iOMqledGQybuNrrgdYoeI2bYNbUJHF-yc1g7uEoJ6WzXDo-qCdVRxgGGR3dB9JpBIJd9hc8j1cFf6PSTERMJ-uZJVyF-IBF9ijxs9topzItXAGvDt-TacUVRmlHzf9Pw_QAwonMBEBg6jeBFSBPkCJRkfJZC8MJxKxV6MpeKclxgG1BCUn_ONQcQiZ7tbsbsjHH5lboAfGyyAZJafbhVZx6dVtznighHZq0cpB93RsBLOAiuoscx0yEL0I42h-GowcsISI9x8tKnG27wNCbQLFAK3CZVGf0KULCk1dhcmZ6WJDemjg5PubMUpR6gDI7us7mz4=w920-h932-no" width=500px />

- 예제 코드
	```c
	#include <sys/wait.h>
	#include <unistd.h>
	#include <stdlib.h>
	main() {
			pid_t pid;
			int status, exit_status;
			
			if((pid=fork()) <0) {
				perror("fork failed");
				exit(1);
			}
			
			if(pid==0) {
				sleep(4);
				exit(5);
			}

			if((pid=wait(&status)) == -1 ) {
				perror("wait failed");
				exit(2);
			}

			if(WIFEXITED(status)) {
				exit_status = WEXITSTATUS(status);
				printf("Exit status from %d was %d\n", pid, exit_status);
			}

			exit(0);
	}
	```

- #### &status
	<img src="https://lh3.googleusercontent.com/PmugU2l2cwW3qs5s8EJerEPLC8ltSUgaf7AON7lvdm2a485-qOhunkm-Dij3CuqNNGgL0Sxgh8Ov5Y8gbDGoL1NpODDS5moENVyX_axcWOqE8wbdJIcK3MSVaXEY5HXG3wcH2jZtukMNJVli7Suv9tSoWjec0k4o25gK3fBW4fthmZUghtGOf4DtGIoYyOsYEqtG5ViIt7Q1rxKyu_stllJFZ2RR_bw8v3PA6bn-hxA2IHNpSYOvagMPJlrzrdDC9oZzp6sTG-coH8Ixz36s9wL35dLTejnjEo83FyUhNGpIhg7cR5_M4t00xEbPfj1fDs0Vgf5QVJ1P5tcVS5goS_GkaNRSAnNVrgHSblV5C37BzmmOcHo-JjBO2q-D95o36c9wLF7c45IQmPBAkALxQ5pdAZrD_mAqr2ZIB9K8er4ClmL3kEOUlJyvy_QnK8Nr0jUBsx2tZKRc5NO6N-bqEn2AqKb5bYgwMk6AdB3L_CO0MQQ36W-FGLH_oD_EVVcH0NSrEKY226WvI4lJLLUB4MOPPSDjsH3pvVyJ2R6m6BTlr38hPautPipxioCGUzTn9w7LpMVVLNBEGoRNAmkHq9IkUfEOyZJxTfnJ6WrZtX3gvv29dJA1UIrs-BEr4vHBto3ThzidBRRK1DruX-3GZtBJSgvDgYtVwzmzusRYOkpf8ix3xVfPqgNeFQ1xybhl8OuNp3hnYhY6bKWR5d3iFf9znQRBNaAcDdVbVKcGPsOPb9RF=w958-h563-no" width=400px />
	
	- WIFEXITED(status) : status의 하위 8bit가 0인지 검사; 정상종료인지 검사;
	- WEXITSTATUS(status) : status의 상위 비트에 저장된 값을 return;
	- why? child가 parent에게 전달하는 값이 status의 상위 8bit에 전달됨
	> signal을 보내서 다른 process를 종료시킬 수 있다.

### Zombie와 너무 이른 퇴장

1. parent가 wait를 수행하지 않고 있는 상태에서 child가 퇴장 -> child는 **Zombie process**가 된다.

	<img src="https://lh3.googleusercontent.com/T_NDtt8d53jWfHP_0Lh8BN2aA_Gevj97JfOlGyvHzUZi8nMJtHvXS8bg6x0Kl1aI9ctHkOthOFcaqhcLgWa6rghtnXV1BOGPKLpptsYEtVHStPEKJsWyrisYbzpnOjBT3gjd1zeGz4GRd-C2Ht5raoto1NS5pfPUxM8Ojmt3o0P830ic7SIICCv5FeRroiSfK3oYEwgzWjJLUN-HOC6m63vKcfSwj2u6o8Qa8LZBCv47m4xh5uExke_qZ_7sxL4aANQe7Ic_EnsYrGjTq6EyFHVQdnacbNzeYQtnFlYxkJTvm4IJ0mok7hWAPpOX_hc5prxOaQcVwKmEK6bt8YfiNt6hdOcDNgi-4iVwWHv0EQA5DrE9FyAASbD-i6K4UGlTimjONjOfllKVswbyQ1ot47iMUCOQc2HD2PWmYAsFQBCDY5XNn28FgSQ4xUYOzlBPJcKEkuJwfD78NUEAmGdqGWurlCA1_GsrgvZTi0EewJ2aBDw0DEh0mYTMXuG1eYxXlfnFU5F_Ro6vxrut4vDi1KD7T1g52nEhZnyVFyw0kJOhdu02hGCju-eqWVLHvqJSc5emAyeaOZDgA7eDgDu9g8wgnNU515r91BzYP40EMq_Pb4EPt7XlNkBmQ55nzMfHKwbJ35AOuhQw2-J7cSiKgBX3bEThJDuKGbpwt0mF5La9uhxawmtRE50rczQBIyCc-D_uZ4Wd697bvxAYRfybZpdGffW5fpGzcD_g-6uXhl5KAFMi=w797-h943-no" width=400px />

2. 하나 이상의 child process가 수행되고 있는 상태에서 parent가 퇴장할 때 -> child는 **init(pid = 1)**의 child로 남는다.

	<img src="https://lh3.googleusercontent.com/x5LqXBUTJdsV4IQec--kM_vc807BkEwTy0QiDOYBoWX51fT_8hTTQBEAfPHIcfbgdxOYBU1z8SgB7sre9-zwrBBzIyxY0LeCLgJlRyw39Dh0owZrcaReSd0BdpB8FDZ-RFXDj6b32NbzpOH5aCi5XKJXG__K2MY0uA8GpbOI9RZ9CwzySMK0q7vAz87RGFWGUJTCKSzU0ZHelI9-xChezCl5evUKE__fDbMym71GujNcyviZZH_nIIVj10vNADp8uMqwxqVK6UNlFZSzTu-UOwnP1DT9Eneoc-rGF9oJH2ZJEvOEv4C1TcSAlGmVKO-ASCm2m8lqEFuO2WsciC0EoJK41BvwbOX3wGIrBeCDF0gtH55gNLVW8CTuB3MH-bhkn6aHcgxEYhW1IQ7vJ47b0bv7LNR6wGCyUglm41UrJbTtyd8H91hTLePFTwSAYEcUCjuzjFBSmimU3kyC2Mj8VESbqnV5lS0BCPAoTxcwXVFUL2vsdV-Tznip-0-Q651MsVypXL1Q0zOPbLdoKN-V8ifD2y0h8JvaK7swRN9HYhjV1p4mMF-AtjE70Ne7t1H6PLwcKKFKkcAX1I1GwK3DCx481Wd20d5W0ogBu-L-nRQangja3STljgsQXCN4Qt7axzDuwLRzxeNowQLDmMVzMKbuiWIX0MzV3KMmylHTYsAxY3zsVdCgNSxXVblcVDUkmQeXnULPdGaDvWU3SVewJQb3-nMZxptbLb1EzwBOM7Efi9Qj=w962-h680-no" width=400px />

### waitpid system call
```c
#include <sys/types.h>
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *status, int options);
```

- 인자
	- pid : 기다리고 싶은 child의 pid
	- status : child의 종료 상태
	- options : WNOHANG(child가 종료하지 않았으면 0을 return;)

> **WNOHANG** -> child가 종료했는지 확인, 종료하지 않았으면 다른 할 일 하고 중간중간에 확인하겠다.

- return값
	- 0 -> child가 종료하지 않음
	- (> 0) -> child가 종료

	<img src="https://lh3.googleusercontent.com/mqTAikleiGYZyf7LRm3WjmczcYJG4RJxn1BG0HThtnbDnVzgR1QYUxQcHVuNYc3tAkexEDMqkbxo5tHA0Vm3CLloJrau1jd7-8WyH3UEUsehdnbc1sZSwhmU3RbqvHcDfm5oSdUEkPMiFdfB4bOUpvmDytBm4-h3rgDXEN4XyY50EmSibqvr9jvHdR9NjsW5j3mUDn9jXG-nCvv1PQzcZVVGW_sVp-S3vyoZoECEVkN1dYzXMi-FHIcvTs74a1Rf4Wm3IN4Ls8GpKIYmDjk_yLBUuc3yeJ0Sr1fXNuTb6gBQmjByvpkpf2SoxR8DMJ-A6Qnalts_LuIfYV4pew9-7hwKl2rSQ3L3SrriZd8Mb5tIGAUADj7GJ6hPiUNvrb7sdGEI3G28vuQHdKimiLxwnrC-b_2mM_TdqR3tA8oe4lW_aerSiLVZmYj-EHpmp9KfXpJukSp1nmHZqtf90JBijaRl8udwYymdlpqz5mhA8QTzJgdRZpcsQDzMcs9NrcVeXM4D6aju4kAvhPV8uQ-5s-swXglYIzaNfBOPlZaRwL6VvPZN-oM-8wtePOlgAiQosy2YV8mQK_cl6HlxitzPvQmngJjau74uro_mkXUV14E8rS0tEKidvMn8PIx2tRMR97WeKk0HQu1ZtbPRhbVsHUOJ9txAa4uJGmiDJDiKVqXCJwvb-bMiStPqPCUW9H-axfx_AuvxdtmoT86H15XSzkqbHNDvZ0J8Slu7HPIsLSTpdIPq=w734-h941-no" width=400px />

# Signal

### signal?

<img src="https://lh3.googleusercontent.com/qeR_xjunXWUTk_tHYb6CTGXDKA9fECLhV_FpHhrhupvjWB7IfvyGw0pMzxKG057fVYAzmWHxmGovZJwbHkMtg99LvodDD8H9xzr8uWMXsNWMmKyiCylSMXO5I_TP4Sp1OT3MOZtGjvMh2CcTswKWncZJ_HNezITSSc8hoSIdtMlejxfW0nF0ZatfT10GXhDbhTyk0VpTAJP2wMDmb0JEsqZ_2DeuI_qs-nW00WZKKcesSIXGR98294F2t4Aw-uSwant7u_0WpDDRDGjqYOIVIyZqCeHoQPq08Xb8Kn884n2ngFG2sTVeuXjDr8AJAg6kdPu9zi02vfZFCSRZzvLcyqFdubnSJjVligTX1IyPmN6CPUpJUM7n-0k4kUxdYN8GhABS0KAvd6rxuIOd3kpfRmvmqviYQy79EinkoNDvafpY0QA8Sp0Y9TXxtfkbSC5ZYSUvzYRH6Ltzc1oDRAi6U-e2qpm1St_jYykcCSlloFnZB0gnlYMWWhtnqX2tpJuk1pmsuazXQWfa2YSNXZNKzyvFZu4KxKXjOsS58gloequ53LwyjwfFe0IooM6C-YVox3FOwXvSxbkvefyaTMxR5wHbNv6nHoZeahORUHDZbg5aur-ItIqwhcuJYRYFLkzvJWtN7eHnR_VcbGL4CfnWzSeG-nAcYh8r3vJ7eVMOOUtbqjEefHTWPlv-5gwKRc9NQiZfzBeQEIENr1r3tMXcRlntetSmm4sWtUfg95uUbicMVO9_=w963-h554-no" width=500px/>

> **signal을 보낸다는 것**은 (B가 signal을 받는다)라는 의미보다 
>  OS에 B의 **signal table**에 mapping된 Action을 B에서 실행함을 의미
> ex) 무한루프 중 Ctrl + c -> OS가 SIGINT를 받음 -> 종료

- software interrupt
- kernel -> process
- process -> process
- 자료 전송보다는 [비정상적인 상황]을 알릴때 사용
- ex) program 수행중 Ctrl + c (interrupt key)
	-> kernel이 문자 감지, 해당 session에 있는 모든 process에게 "SIGINT"라는 signal을 보냄
	-> 모든 process 종료! **그러나, shell process는 무시**
- <signal.h>
### signal의 기본처리
1. 종료 
-> 내 program은 잘 작동하는데 OS의 사정에 의해 종료 (signal에 의한 정상 종료, default)
2. 코어덤프 후 종료 
-> 내 program 상 비정상적인 작동으로 OS가 중단시킴 (signal에 의한 비정상 종료)
	> core file (종료 직전의 momory의 상태)를 생성 후 종료 : debug
3. 중지 
-> 멈춘 상태로 계속 진행가능
4. 무시
<img src="https://lh3.googleusercontent.com/oqLhcStmYo6-q5qgfvOJfJA6KgdkVM-eSMzjTRyJD-Xxl1IBz8sZM4FHF0U_4fyU6RWLuVonao3Pfk39w8ouqCR871irXvXk2gKG6fJyouqIDWgs8f3bmX5DcdBXpuxXfxmDrV9y8s_LEcIjsM-aNG1D_N3x_C7Au3VWtUJVpnxEGnEd12Kbtvo8X_9NVMF-v9mZ3InLETBR0aQQvuW5QBQDTAqK7_P4iigAdI3vJfxy4GqL6T7_aBWTkzakZYJBsNDPVVIwL0S6a-VRZF29DF_FzqqYGBDWpMn5ww1wvypBOQk-RMVzY1q9YpH22S3mXpiByFIyB0WaY1h5_brdQ8vrQ8KqMDHuK0AAnYpZHxI82pW2suTlnBKCwnCH16a2o21qFWoG2K-CCgpMtUq2IaDaN28B4BNBqgrk_uA8yIrQCVol743sqWKXuCV1SY3bLa2DwN2t0z14qir2l-GEvWp0eaxNMlb-N2vW04qeDMVlp3CFBt_y8oxE03lTTZJ2Ad-9_3lOSQbtf0hXJNfYcKIhoAneCRV7GJD-0IE58v5zAa-VjZmWtJCxSlg44noLQLnFAC9Bx3PSSmAGZJvQDmVcjFurhF76dwvVtcuRssu8O_xgsTik5zuOM76h95-D0yS6tvd_BjSvtz_H3Dkz_r9tP5yTnJhvhiq_wqBXfwO6SoGoMZfHU9lwLClefCcHBNG19u2wYL6kg2MuXiKDPpzrlyEhGeWb_8PPs4NxXLT1kY7X=w717-h923-no" width=250px />


### 기본으로 알아두어야할 signal
- SIGUSR1, SIGUSR2
- 그 외의 signal은 process가 특정 상황에 OS가 사용하기 위한 signal

### child process의 종료 상태 확인
```c
pid=wait(&status);
if(WIFEXITED(status))	// 정상종료
	printf("%d\n", WEXITSTATUS(status));

if(WIFSIGNALED(status))	//signal을 받고 종료
	printf("%d\n", WTERMSIG(status));
```

### SIGNAL send
```c
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int sig);
```

- pid : signal을 받게될 process 지정;
- sig : 보낼 signal 지정;
- signal을 받을 process 또는 process group 지정
	- pid > 0 : **해당** id의 process에게 signal 전달
	- pid = 0 : sender와 **같은 process group**에 속하는 **모든** process에게 signal 전달. **(sender 자신 포함)**
	- pid = -1 : uid가 **sender의 euid**와 같은 모든 process에게 signal전달. **(sender 자신 포함)**
	- pid < 0 & pid != -1 : process의 group id가 pid의 절대값과 같은 모든 process에게 signal 전달
		> pid = 345 -> pid가 345인 process에게
		> pid = -345 -> group id가 345인 process에게
	- 만약에 다른 사용자의 process에게 signal을 보내면 ? -1 return

		<img src="https://lh3.googleusercontent.com/4_dDQccWPipzY5tet-ohmZgP4zJJDsuie2BWbdfIaqEEFli1OARiI_3cJ32JrTfAdFzbmSwkJTyGKWPbU7e3cqCM1_4o99HB9BoTqfNQdfkocciRHfDpgnTu1FEHgt55xu2OomhDWBPZgHJv5we-QNfCozqh6ZG5kzUg_Kw_8F614CelNqYp14XN9trGDdbZBZTzZ7NGN_4TGRX9ykqrOJLpvv6D6s6WzEiS-Pd76YMQmSNWbAPsRur099EDGXA_sRr5q2Qk6tKwMbhzf0PzscwNlmDrLX_ROhu5Ek5HCsv-pvtaSkmN33PQ1fVgVR72NYOM48dQQ9dHEI15Rgd6_PKUBMiqcDWaq0lM5rSukWvPbiWZ7bGhKfhVu2Gy92a6NlZTqjrrTTPfMfKJeKEOqIlfktqeZfo2MbAkiYYDo2XzD25U1qrHAM_X84rhYuVDPHcIVyiaMdL_el14UTFPITrvH05vaDuYWvYr6w_ljxsEvdfmZGspVavtz2p1TiIIDO2hmeaSxxsCyPWd6JXVWr8xH30BKDDREndY6IIAY0DgUxUVKiqaeUBEx8tV1uxlsySvPF7IFOQPFpWsh4RZUT5N9yk4dw3NDCTkS74ejPyYactkb-NmRdI9E32Qvyv_zfpg8oJ1y3OuWEdzbQhpd1Oar0G0Vh4mrewalk4vOjfAqQ-M4VkuspEnab1ZdOPZoCtx5CTIgwkC4SHAahg9ZUFVIwfhXhjyfhN5obJccOTK6qnB=w963-h889-no" width=400px />
		
		> 위와 같은 예제의 코드로 
		> 1. pid == -1로 실행한 결과 shell까지 튕김(sender포함)
		> 2. pid == 0로 실행한 결과 a.out2만 튕김
		

- 호출 process에게 sig를 보낸다.
	```c
	#include <signal.h>
	int raise(int sig);
	```
	(주기적으로 내가 무슨일을 반복할때 timing을 알려줄때 사용)

### signal Handling

- default action (프로세스 종료)
- 무시
- 정의된 action (sigaction)

### 1. sigaction
```c
#include <signal.h>
int sigaction(int signo, const struct sigaction *act, struct sigaction *oact);
```
<img src="https://lh3.googleusercontent.com/gCVygB0TPqFw_Zj8Ctdt91kSi8DlDCLs4KDXnqmd9k8OaVCGV8b-ypTJuFtCa0gULKYgOdD6bXmkLtYLSR8k1fbujQjW94iThR28xbYN39V9mC2VqMvJGdZ8wDrVMBAuO_YmRQhXU76IgIt-4CQThXaUqNsCfK6vcMf6Wy6qBMbKp7bmOgQgzBZ4Xg5EzDwNnc8O2dtAOB7adaF77ZcyIvn7aqIf2vBG8evJ5IlVMWEhw9IWjVwiyeHGIVL8iof7xJifAcuwZSMTy-SRBOdWAWqORfF1SYAhYK99-3ooWGK7gpmDBHCskh8GVcxuknQZ2QeezJpPLzs2aZsDsn-Rzv1rQLQ29R0r8oIFoyYNrOIPWpTKgmijjpnvCAZiEO7Q1XNy6YJ_ZiSUb4ZDJ950jW6R60f5m1oyll9ykFGpwWHkZ2cRprtD1nfFGt5Af4S6qzLWWfPvdyvL0uq__VjtuebbsPl1yirJoyb36rXZvPCevAWzHTXrNJQlk8KmJnkf5y9j1csZ_g00nWez4lkh6Dy92a6AHdCm0vQHFF-bRKDgskkyNxoorpdK4CYQPi67xXzMYOz_F22WkMW6BXcVHfu35pJhkeNoED2mx5R4MNijUYG74dJWT6FVIT2zt_iXFvY0sl1stiDRnhugp_BFBBssZzqbsCgS3ThVRWDbZNaF2DpXcun2VPH3m0rNQqYU1InUCwwc8bwLTwWPI4i7vpwOJL1quP87Jau_h1FiVAde6Oz-=w963-h528-no" width=500px />

- signal 수신 시 원하는 행동을 취할 수 있도록 한다.
	> 예외) SIGSTOP(process의 일시중단), SIGKILL(process의 종료)의 경우 별도의 action을 취할 수 없다.

-  sigaction의 구조
	```c
	struct sigaction {
		void(*sa_handler)(int);
		sigset_t sa_mask;
		int sa_flags;
		void (*sa_sigaction) (int, siginfo_t *, void *);
	}
	```
1. void (*sa_handler)(int)

	- SIG_DEL (default 행동, 즉 종료 선택);
		
	- SIG_IGN (무시) ;

	- 정의된 함수 (signal을 받으면 함수로 제어 이동; 함수 실행 후, signal을 받기 직전의 처리 문장으로 return);

	
#### SIGNAL Handling 예시 code
- signal handling 안하는
<img src="https://lh3.googleusercontent.com/mRFf5H-y6RdVqxYQ7fY4F_mb9HT9YMlBWPolus1yztHhVRNAWbU6sd_ih67SIq3jF-Xi8WRh5VZgtGqmjpRiYWpjoqdsRE9h7lvzbioio8n0fjGOzLtSjvecz8oNxKT0xFK6UaWO0GtUvuBZQRT1KP8UnBdTmnBsGrcJUJaOjZmeiiIa6xoQnE0YJzdtz4hHvbWpWFGzk5q6BSS5dPEq4z5IeIkCIfQ3Y8WkuYy69sQXT5H-G8pi2wkPQmRCHAZQXYppPsZy63n1VVkRMsh7P82UsohZB28hpnCIk-EfdAijRX5CXWBV-6dA_mrLEoTx8E4oxXQ5EOld5MvvJytMXRoOAkFy-7kCraq_b-x8UWoJxqOA44vBEGk7-VGvaPi9IgrgfCa35nAw-s9MXRK3oXpwHZLNFB9kdv0SZ5Zx7znADThlpovl6i7djY7kqYhezKXv8iHOx6sbEUwiClMp6vEtYGSXxjzH4Sc879pC1pmyAy3o87tgT3rXx4SWSezfrauZ9s5Xk62Q4mI-2ZkPeCOT4e5M8aBlL0q_Zb90zXjPVxMjq-pYUF5oJMz8QCVK_BhT8yeTCyUBKfMITjgUEKmk5uN0XqP0XKtGkyiFrRPH-IFq2QaJsQ-ZS-jMCyHG3kP5N0g56IIYgxny6BF4-Mm6Uedvt814VC07EwJN59VMj-h0NkpLpAnv2977KBjbPHWIR4sKaQG-wz3Z6bt0Gz9q9cc3uUdFihOQ5QrlY6uu_sO5=w890-h693-no" width=700px />

- signal Handling 하는
<img src="https://lh3.googleusercontent.com/TOvBYw1QO9NTX-JxJ8QlABctO0s3lNxnIoCQ0tHIuWOcRgEy2F3gGXWfBZFUEMCgyXnJMr8vPzNrFwpvZlif7z1X7WTPcGC4dsK7djGxQa-MumUrXmPeTznHW-joXmuGrJfFa4grr2lA_EQ0TjNyOj2yNP-XCcokWmZzMhyrTKpTuWhDrkxuriN4NNNbKpBIMywabIUCQOWSGWxXYvjXT8vJPKQUWagKClBaQh3ElswVBLo_FK8aE8In23QsbbnZWeBPot1V8fsDdFtuV8EMqJOy3CY-L9TzZqceDuVbizgnDOUVq10sE6SOErA2W0IvKizPhqr7c1b5wqZFY9yn6cXMdDVaknvAZuh50baFKYE-WovDzOdwf_QVguKcUaktDV_ITZhpZ8xQ49SUUn1lAN5OXoiNg4SPkIKR-l2yw5SMWh9FMEJwKvn0OKtjaGt4UyLY-EvUfPXinhEyatLolY5V29Sp-3ewIPa_u4W_QoUIdqn4sJx1_O4CAn_h28GWDigDPdyZHOEpjCemeIiI5LTo_7y3BxSu7Qq6LAIG83n9LL6XemXK_ieqcnl68-n-QR7BoXxsmc7MBtGJIYUz820E6kfeBQYg9g3pDTBgEGbNRZ8FVPDKgq8Qf_B24ZHG4yfgAE8NHUm_iIn6o3nxbjQO_6nuig-UVnoT1vbkYR1M0_1Y039OtNY5vD-gOnQOKADoc1SZKVyr9zJPqxgDzqPK39hWWJBvFmixXippskt41H8p=w941-h931-no" width=700px />

2. sigset_t sa_mask;
	=> 여기 정의된 signal들은, sa_handler에 의해 지정된 함수가 수행되는 동안 blocking된다.
3.  int sa_flags;
	=> SA_RESETHAND : handler로부터 복귀 시 signal action을 SIG_DFL로 재설정;
	=> SA_SIGINFO : sa_handler 대신 sa_sigaction 사용

4. void (*sa_sigaction) (int, siginfo_t *, void *);
	-  sa_sigaction()에 의한 signal handling
	```c
	int main(void) {
		static struct sigaction act;
		act.sa_flags = SA_SIGINFO;
		act.sa_sigaction = handler;
		sigaction(SIGUSR1 &act, NULL);
		...
	}
	void handler(int signo, siginfo_t *sf,ucontext_t *uc){
		printf("%d\n", sf->si_code);
	}
	```
	=> sa_sigaction은 sa_handler와 비슷한 기능을 하지만 조금 더 많은 내용을 표기해 준다.

#### Signal 사용 예
1. SIGINT를 무시;
	```c
	act.sa_hander=SIG_DFL;
	siganction(SIGINT, &act, NULL);	
	```
2. SIGINT시 종료;
	```c
	act.sa_hander=SIG_IGN;
	sigaction(SIGINT, &act, NULL);
	```
3. 여러개의 signal을 무시하려면;(1개의 act.sa_handler + 다수의 sigaction)
	```c
	act.sa_handler=SIG_IGN;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
	```
> **한 process에서 무시되는 signal은 exec() 후에도 계속 무시된다.**(signal table까지 copy한다. 다만, **함수로 수행하는 경우**에는 함수 주호가 다르기때문에 **수행할 수 없다**.)

### signal 집합 지정
Q. signal 처리 중 다른 signal이 올 경우?

1) 다른 signal로 넘어간 후 다시 돌아와 남은 signal을 처리한다.
2) 현재 signal 처리 후 다른 signal을 처리한다.

`sigset_t sa_mask` -> 나중에 처리, 즉시 처리하기 위한 blocking을 위한

> **blocking**(언젠가 반드시 처리)과 **무시**(pass)는 다른말이다.

- signal 집합 지정:
	- sigemptyset -> sigaddset (빈 sigset에 signal을 추가)
	- sigfillset -> sigdelset(모든 signal을 추가 후 예외 signal은 제외)

	```c
	#include <signal.h>
	int sigemptyset(sigset_t *set);
	int sigfillset(sigset_t *set);

	int sigaddset(sigset_t *set, int signo);
	int sigdelset(sigset *setm int signo);

	int sigismember(sigset_t *set, int signo);
	//-----------------------------------------------
	//ex)
	sigset_t maskt1, mask2;
	
	sigemptyset(&mask1);
	sigaddset(&mask1, SIGINT);
	sigaddset(&mask1, SIGQUIT);

	sigfiilset(&mask2);
	sigdelset(&mask2, SIGCHLD);
	```

### 이전의 설정 복원하기
```c
sigaction(SIGTERM, NULL, &oact); /*과거 설정 저장 */

act.sa_handler = SIG_IGN;
sigaction(SIGTERM, &act, NULL);
// do anything
sigaction(SIGTERM, &oact, NULL);
```

### alarm signal 설정
- timer 사용
```c
#include <signal.h>
unsigned int alarm(unsigned int secs);
```
- secs : 초 단위의 시간; 시간 종료 후 SIGALRM을 보낸다.
- alarm은 exec 후에도 계속 작동; but! fork 후에는 자식 process에 대한 alarm은 작동하지 않는다. (alarm은 program단위가 아니라 process 단위)
- alarm(0); ----> alarm 끄기;
- alarm은 누적되지 않는다. 2번 사용되면, 두 번째 alarm이 대체;
- 두 번째 alarm의 return 값이 첫 alarm의 잔여시간;

> <사용법>
> 1) 몇 초 후에 무슨일을 하겠다.
>2) 몇초 동안 무슨 일이 일어나지 않으면 무슨 일을 하겠다.

### signal blocking
```c
#include <signal.h>
int sigprocmask(int how, const sigset_t *set, sigset_t *oset)
```
- how : (= SIG_SETMASK), set에 있는 signal들을 지금부터 봉쇄
- oset은 봉쇄된 signal들의 현재 mask; 관심 없으면 NULL로 지정;
- how : (= SIG_UNBLOCK); 봉쇄 제거;

	```c
	void catchint(int);

	int main(void){
		int i, j, num[10], sum=0;
		static struct sigaction act;

		act.sa_handler=catchint;
		sigaction(SIGINT, &act, NULL);

		for(i=0;i<5;i++){
			scanf("%d", &num[i]);
			sum+=num[i];
			for(j=0;j<=i;j++){
				printf(" ... %d\n", num[j]);
				sleep(1);
			}
		}
		exit(0);
	}

	void catchint(int signo) {
		printf("DO NOT INTERRUPT .... \n");
	}

	********OUTPUT***********
	1
	... 1
	2
	... 1
	... 2
	3
	... 1
	... 2
	... 3
	4
	... 1
	^CDO NOT iNTERRUPT ....
	... 2
	^CDO NOT iNTERRUPT ....
	... 3
	... 4
	^CDO NOT iNTERRUPT ....
	... 1
	... 2
	... 3
	... 4
	... -12424645
	```
	<문제 상황 => 해결법>
	<img src="https://lh3.googleusercontent.com/BAAOmDcGXZHhkyXGYv_duVQ6yKQcyCEvINTHTWM56WyjRP276OVNJAn5gzLm6yOQWDB9RdFvt8p4nths8w6oKOsaSvF1UvuM3ClnSVzrlcwiNJgneq6b9TEuuxHDaPvBkJT8R9IimwUIiryHPsRq-w1MVFwZnOqnI6Uy31jikCLrNoeh3KepIfQd2wDrL_oE3FnpTp4Xrn3AGMonupuYrlm1lZxMwckOhgEowkFkMrRdELn_pxS2sX0SwI0eXrUNzZe5zfJ7LvsLlRS54EjZ7vgqE1nGOMY7Y6QxldLOVae0vmREIaMf2-TBp9D1-_OAraAmnGtJppsPNTAnTywXu_lOmFsSxn9plGBGUHhSi1oLJ6Tfp0LHnOOUa6NJaNOCHRiF84sMO55st9pyAmje22cM9cYo68bN3NDdhVS3-rebg_p1GXlZJr_1bH7Cc8hCATMWg-TGxGseht58d0Atc-J8OIxq2Hc2V3uHxW19D6W_2lhXVWNDguSnVbcFobeNIRIShgEDX6U_qbDhs4Wz4Ec1opZA2jzwQ8a57w8TcdJu1Zlbivq_KB59PqytPJo3MWMpAwLgCG0GQC7CrZcfwgtqYTBcKvA97jmuUSGrT8hGSA0As9dewrrAGd3KgFrjYHfdUkqwjnVusiVTCXAoStaOZCGOOP6O2S7ssc5pApMt22AR6jO0Nr9h0gKA6xnJ2Y9uVdUpD_ZWT4NYWbhgrrLmUmNJuFu1NRUbRo0AGAPgmoEz=w619-h943-no" width=500px/>
	<해결한 코드>
	```c
	void catchint(int);

	int main(void){
		int i, j, num[10], sum=0;
		sigset_t mask;
		static struct sigaction act;

		act.sa_handler=catchint;
		sigaction(SIGINT, &act, NULL);

		sigemptyset(&mask);
		sigaddset(&mask, SIGINT);

		for(i=0;i<5;i++){
			sigprocmask(SIG_SETMASK, &mask, NULL);
			scanf("%d", &num[i]);
			sigprocmask(ISG_UNBLOCK, &mask, NULL);
			sum+=num[i];
			for(j=0;j<=i;j++){
				printf(" ... %d\n", num[j]);
				sleep(1);
			}
		}
		exit(0);
	}

	void catchint(int signo) {
		printf("DO NOT INTERRUPT .... \n");
	}

	********OUTPUT***********
	1
	... 1
	2
	... 1
	... 2
	3
	... 1
	... 2
	... 3
	4
	... 1
	... 2
	... 3
	... 4
	^C5
	... 1
	... 2
	... 3
	... 4
	... 5
	```

### pause System call
```c
#include <unistd.h>
int pause(void);
```
- signal 도착까지 실행을 일시 중단(CPU 사용없이);
- signal이(어떤 signal 상관없이) 포착되면; 처리 routine 수행 & -1 return;

<!--stackedit_data:
eyJoaXN0b3J5IjpbLTg5NTU5NDYzLC0xMjMyNzIxMjA3LC00Nj
c5NTAyNjIsOTY0MjU5MjA3LDk1NTA0NTMwNywtMTkxNTA4NTUy
MCwtNjMzODU2ODk3LC0yMzE1MzE4MDIsMTEzMzQ3MDQ2MiwxND
EzMDI2MjQxLC0xMzU5Mzg0NzM0LDk3NjY3MzMxNCwtMjY2Nzg2
NzY5LC0xNTMzOTM5MDgxLDE0OTc5Nzg0MTMsLTEzOTM5MTkyMj
AsMTM3MTUzOTQ2NCwtMTE5MjM0MzQ5MiwtMTIxMDc5Mzc1Niwx
NDE2MTkxOTgyXX0=
-->