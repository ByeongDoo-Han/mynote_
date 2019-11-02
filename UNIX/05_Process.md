# Process
- **process** : 프로그램 코드, 변수, 스택 등에 저장된 값, PCB 내용 등을 포함

- 계층구조 : parent process -> child process(UNIX system의 모든 process는 init의 descendent process)

- Shell 상에서 process 목록 확인 -> ```$ps```
- 실행 중인 프로세스 종료 시키기 -> ```$kill -9 프로세스 번호```

## process identifier

=> 음이 아닌 정수
- 0 : swapper
- 1 : init

```c
#include <unistd.h>
pid_t getpid(void);	// process id
pid_t getppid(void);	// parent process id
```

## process Group

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

## Session
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

## main() 함수 인자 사용
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

#### parent code에 wait()가 없을 시
<img src="https://lh3.googleusercontent.com/nqrsRXd6gGcI7C7wm64bA8d6WC930anAd57L4Pjy0ICL13SPgamOdjOTkm0E4bDT4oZioX8VD-YKiGtx2B7C-wYWsoyQ2kb6vVBPaGTT2I23szP-xcs11IUhnD_feO616d4VhBrg0GBmu1t5xSuNX0pbbW1HSrv0h3A7zBn3cQMrZo1qsyDoae3WLzY2huFxylqqCeyO7R0sYtUYXZhH1Jnp8VkTYfXUxHmWXVGQaPxVF8snvjUZYgUjjIPJHb8_95M69czh6AMC2sBv7CyDmjfz6Bx3V_VhnLfPHGr0eg_LV2Ruli0KIIMcihZUS3ScxwUnMwMEjk5VpN8rqMB8sX8PrPXDcNfo7NAd1bTOMf7LX_fNYzijH13ecy--QLfg4c902oaIkarWpuTtAHOqKXZwejO1Kd0jpaLnv_IeauGhqrdnYIGR-KSGy4ItqKp4t_acTpnpeqwb40Y-3g0mInriQEl6MD1j8IWIP02bPhvSdfqZNxzAyqlTRGwDpeHmySizbU8r179YkJB2pXrCt2eIS7k3uGcy5q0KB5z-OxVXvA2ZqLntHtDAD-pOFp5RQJpBFraK_0QdIS1gDmxhlBkD1tfDW8BBK10pvTk3FJb1_0nbdYCl5kFOUdR-Otvsh44hINcelCxSMg4aNOUvCY7XapIJrZjqiUGfx7sDVzclnDfR8Lap8k17LXTiyp_HX_ISccEHwiNuZUllr5F47MRqK30HJ0_1iTGFRLUXkzckd6BJ=w1003-h637-no" width=700px/>

#### child의 ppid가 1인 경우?
-> 실행도중 parent process가 없어짐
-> 고아 process를 가져다가 pid=1인 process(init)의 child로 만듦

#### 잘못된 반복문으로 자식 프로세스 생성시
 ```c
int main(int argc, char** argv){
	int i, N;
	pid_t pid;

	N=atoi(argv[1]);
	printf("pid=%ld ... ppid=%ld\n", getpid(), getpgrp());
	for(i=0;i<N;i++){
		pid=fork();
		if(pid==0){
			printf("pid=%ld ... ppid=%ld\n", getpid(), getpgrp());
		}
	}
	for(i=0;i<N;i++){
		if(pid>0) wait(0);
	}
	return 0;
}
```

#### 3개의 자식 프로세스를 생성하고 싶지만 7개의 자식 프로세스가 생기는 이유? 

이렇게 코드를 짜면 다음과 같이 생성
 <img src="https://lh3.googleusercontent.com/ERRhNiNdrN0VARMFpkILk0XsWXf8egQZ1KPBgYKX3o_PvQSsUykFlAFIyp4wopK6Dfr4mYrVpM0iEBtuBwBbc5UKXTD78cvNxtR8VNHh270_JnmUpiepRnLZBPAw89f4zLwI14DPYOQH9U0N7hqIFEsGGYROpwolkUUiCzds68dhJABnBeJmusPA_ZoeAheQs0HyZxuSHOyKpwkoC-j5GE5KucK0vm3eCQl40EgNF0R8oQXHIypvqiQTrkOqKIXPK23_a9r1sjGMBGLCW98YCr78v3z2bPDAPjC57t0PJpgfL_lBsS36iNG0IyzYEfIdESHQtZzsqNSiza5chw_AvPSZQXEcFyz4aqNlmg6xfpxTEnW77Vz-Q4lckeshxLnAUrdV9NyummZSkDsmEqMqSttMdCE7O8L5Sm6DRusj0BqUIx-eXBBqTGx6dfIx-9iaWn7qCta5u3C6a5NjFng-pppzb4nI5L5PE425gH1KV_a8yF8Zn17ag571eTAQusARxVonvx6vscgHNvh7TfyWe7Q7UetaMWktehYAg3UOrpPxHB-MZ2jgnnKL9ZS2iglQpbhhTHLbyM55Rb6CaNrvX3cxsObhF4p8hwt9R4xiGhJGHOsTMZnlQAn5Gfxsksz8ekc52pzaOimBgFmuBcz_po4gOc1MEgQ5xElqbMwkxctVyQ7UbDoWpoQXdAkhqhFrCNSbRkFI-LW1W49SXtTPbGxuU8pUkAaiQxW0SWx-3wCO3gqK=w385-h346-no" width=400px />

## exit System call
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

## exec System call
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

#### Q.  parent process가 직접 실행하지 않고 exec 하는 이유?

사용자가 shell에 액세스하지 않고 특정 응용 프로그램을 실행하기를 원할때, /etc/passwd에서 로그인 프로그램을 변경할 수 있지만, 시작 파일에서 환경 설정을 사용하기를 원할 수도 있다. 만약 .profile에서 로그인 프로그램을 변경 후에는 program이 충돌하더라도 최종 사용자는 되돌아갈 shell이 없다.
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
## 프로세스의 동기화

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
- [wait] ex code)
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

## Zombie와 너무 이른 퇴장

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

<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE3MDM3ODYzMTJdfQ==
-->