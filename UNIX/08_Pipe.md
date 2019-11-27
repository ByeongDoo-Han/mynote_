# Pipe
<img src="https://lh3.googleusercontent.com/WeQVF5rLdHr7fH61Iy5l-4O4fw2Lc9K4Rrn46aEYbN4rqNX-bRhn_WtvSPA349WdH34FA1JkL2zq5L2qLjYCG4GGBhn3rM4w3tiTRZDl-LK-YPbHE_KmefxzL8VhELKtZ-Vq5s2SpYaJeYYvBrnMBh-d87oHG6sQgf99_yY7a7wGZFK98B5qWYb0NWqCMtYrrijpBlM_-8Sbcezi4u6odHxco-qaWAE3PghZtCWOqc2Tyldyn6CdXoUTaojvw6zAhza3bVMZRyXDVC-bUnDkSzJ_23z1TCIYeKtDXwPhplw9ROVF4RV8PaYLEVjQEGtz19LlZAMZScayM6Ax9hxpSqNIaS9cxQFmVhj8rj7qTJgSM_WoFbc7KeBQjSGm_BydpOFfHzKyK7D5cZP0YQxzQGvvs7BMLWOOtMVknHdLgXx-8FoCrv4lMLODn56SOEJjvlYI5kIYlBDQbuIBgtmTkvvZsPnEoolP33rdWPu5B9FkLxDH7spwzNtRQy6IHGhCe07Q6pt1TwI7V4-u1mOlyV7gZMe4aW_Lpe21Rau3CIwril9tgj--MdKq2OPO8bL6UEKSK9XWsNAEmMtfX6fWUz0w8KTEMCqQJS7haAL-IxeLEqFHQPh_6PllHlHEv5jguK7j5XqfLVLmtKE3okczB066J8Za6qNXUutGWrZlggmCwoWoldhM1lqw69wFvGUERZ9fmNlFfN_fgDBeSfrQfh2vVcF18rrOv_VT3hxkj-81Zgwe=w502-h943-no" hieght=700px />

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
<img src="https://lh3.googleusercontent.com/lzEzL9IFlLGCcYjY2ixDzx6fqQdfQtUw1cWO9XjCqRL4FB-8gRJxLHCLOrSeOrMaPa9wjiOpeSogWz9U9V8ZJkZCMCq_htySs4DcA5WTGswpZP659hyp32WIVD2dFx5NGofRiI0d8z3Xx6pP7JBLOzHTHQjc5DKWIxpkxuhsg-8eAScHuvvvwRT33Mtn2o1pxS1AjKVciKpWMTNMfn-Bn6yoW2Pzjo1ZfecZAA4z5rmhcIlG_aMBIycdEuPrqz-XpBHV2JY7JgjTjwWqw-Kfp8XgYxVH5R68vAJEe0Sh_eG3WP4_CHuumIYtaYcl_4LEFM_746PIzyiVuu37prBTbbdCTbKQBbDhmS9Xf_DKKXXflfy1Ad5tG5BbvLIiuPEuwoTj1p-jsDxUqmemfSHJx7I3PZ8nB8pXl_buNHMgOzJJ7VwjcTqhobTZ6eif6UB4_zDmKE9NQ5_yJrxqEJKHorUW4_26XPgfPpk0UwIrWsbNzNGL2rQbKSCFcClxCZgxjShQFWw2uihGQbWUS_NhB783HHWLnEg2WqyNeDWy62JCf0HWaaDSI6b3RrVho2EQQS5htH-SPmFhzP3wNw7qGMJVKAeQ3SrOD1a0tZfT69ckqH4QgyhfyHUMgd62SHtSweVryKV9zO1PROd_b8aCaD9W16qugQQZs5fjq5QpK8c3KnHSQg7JSpqAwNnyRl_WDe_22LTOW1qCTxagTJnofhMgMqFldqeIpJzgbFlysyBoI4M6=w356-h944-no"
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
- 읽기 전용 pipe 닫기 : 더 이상 reader가 없으면, wrtier들은 SIGPIPE signal을 받는다. SIgnal handling이 되지 않으면 process는 종료; signal handling이 되면, signal 처리 후 write는 -1을 return;
- 쓰기 전용 pipe 닫기 : 다른 write가 없는 경우, read를 위해 기다리던 process들에게 0을 return (EOF와 같은 효과)


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
### non-Blocking read / non-Blocking write => fcntl

- Parent가 여러 Child로 부터 data를 받는 상황
<img src="https://lh3.googleusercontent.com/Y93d4QBMf-SPIckcpd1fRUsWDtK_Lxh6Gs915ciyBayHxo_3FHCxqtQZhRLHJmANdLPWPOAHxBp5Iivy5PoO9Pw-bSDF1P1cBSKX0WkDn6rLXl4c23vSVO-fjDsuZFodOrJDtqEL-cTEz3nbZCuEWdz0j9yHGzWb-Kf2zzYUiH-naF7DqYFTlm4ODgsTfuWreP6m9bGHedxhgGTwDscHt9Idxnyi6JFwqd8WC6RwVK9TI1641JKqEG9sDzMU9rogkoVq4jMLVDIftIXPHPnVdxH1S60nTpkfFjg986saRdBMZolkARejmjYxkPWXFCSODzkBdolRKeRrTi0h5rZ2jUOZqyeuHEKyxme-nBDBoPX83ISy5HjUTFtwR-CzW_okTwe7mK6Fx2qllwyOWX_5MXbm61Abtbla-XJbcG9AtEyEcP86l0zqnL5mVSdEQ4BhVRO3IBdflS-T75-SvdFms2nRckpdlQbCyp4_GmFme09nviQqioT_X68dlH9J9GXp5A4__yped7Bc3UYkP0tqqPU01sPeh3g-MhGIY-F2MEu5H7gnq1PR3mTgy1wDRzfVHXyR7942UMXYS0uM2OO58hc_uSH9ztiVac1yxY3PVCFqESz0xyjhKJM0aXIgZ9XdfDalyKw0aZQ-Yl098JAnwMvfZz4ETerAWohD1MF_FfNIOBqs_7b9kWgur2ZZjJCpJ0vhj6LpqSGE4li9e1fmtyU07rA7Nx067lp5tL7Nw4RrX3pM=w968-h849-no" width=500px/>

=>여러 pipe를 차례로 polling 하는 경우;

```c
#include <fcntl.h>
fcntl(filedes, F_SETFL, O_NONBLOCK);
```
<인자>
- filedes
	- 가 읽기전용이고 pipe가 empty면 즉시 return -1
	- 가 쓰기전용이고 pipe가 full이면 즉시 return -1
	- 이 경우, errno는 **EAGAIN**
=> fcntl은 pipe가 empty한지 full한지를 계속 확인한다. (**CPU 계속 소모**)

### pipe를 이용한 Client-Server
- Client는 하나의 pipe로 request를 write
- Server는 여러 개의 pipe로 request를 read
- 3 Client - 1 Server 가정
	- 3 C 다 안보냄 -> Server는 blocking
	- 1 C만 보냄 -> 누구인지 찾아서 read
	- 2 C보냄 -> 순서대로 찾아서 read (in the order)

### Select System call
- 지정된 file descriptor 집합 중 어느 것이 읽기/쓰기가 가능한지 표시
- select가 읽기, 쓰기를 해주는 것은 아님!! ->**읽기, 쓰기가 가능한 pipe가 어디있는지 알려줌**
- 읽기, 쓰기가 가능한 file descriptor이 없으면 blocking
- 영구적 blocking을 막기위한 -> timeout

```c
#include <sys/time.h>
int select (int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout);
```

<인자>
- nfds : server가 관심이 있는 file descriptor의 최대 번호
- readdfs : 읽기 가능한 file descriptor
- writefds : 쓰기 가능한 file descriptor
- errorfds : 오류 발생한 file descriptor를 bit pattern으로 표현
- timeout : timeout 값 설정

<return 값>
- -1 -> 오류 발생 시
- 0 -> timeout 발생 시
- 0보다 큰 정수 -> 읽기/쓰기 가능한 file descriptor의 수

> **!!주의 사항 : return시 mask를 지우고, 재설정**

<img src="https://lh3.googleusercontent.com/gf71LCxUfUojUS5rnRUdyaxMyxp31kYG6xhYINIj9X2rMZS25zsbA2VHN9Pf2-rJjQOsNARlQrzQZYWZkjxvtc8LjA5jdF2dbuYKvF2m7F8WT6CzOWkwzppDHmsYGxDFsU1wLmUQ_J6O1D9qytC0PVYsI4qAgQtPYBgEA6jsB5YcpJ7kcF-0d8lwKbJu5iuYVdmi44Z2VCF44vW-SfYPX8-ceDLjE5euy1dT2AjTf1tel5kSvXujw-61VOz_MGIMeTWUOqGiSQ0zFTY31EBnu6r2vAzEs18iOdY0qMBtud7gCghaKUYABKHYgp2_eliWripTPfV6cGmF5qRkTolyhhYx-cJooI2deaa-Kp1Bsvpw9eb0txI0V3C9Os1GnBI6gOkk-435QGhU_EpBkvc7MKg1mip0h8OcHYhXTJt-U-gUT6h-Uy1seip3Hrpkt_Qm-76R_ik2RRTvJWd4UnFthAsK_ntCmHhQ1ql0fjgENvOIGrk_K0udcftYKla8ln1USvqwiJW3KzVPBzYe-tN2Z25YjuykCmYyN8LWIoUhCmGDvVdDG9xLcR8g8uM6IdG6fbk44DhbUNaWfzg6gEJrPByLwRsqSbQuBoKEsctyp0zx_f1O4SqYS31UtzFGKsXJOYyBv_sVN7A79Fo1LdfYVXef67zaXgptzTkAtvYWvxc3Z4P-JtZA039CA0tUqEndKwv0NUKddyz9o4Wf2enPcoBhHmJkUM4ECRlSr3CiL3xH52oy=w968-h745-no" width=500px />

### fd_set 관련 매크로
- void FD_ZERO(fd_set *fdset);
	- fdset 초기화;
- void FD_SET(int fd, fd_set *fdset);
	- fdset의 fd bit를 1로 설정;
- int FD_ISSET(int fd, fd_set *fset);
	- fdset의 fd bit가 1인지 검사;
- void FD_CLR(int fd, fd_set *fset);
	- fdset의 fdbit를 0으로 설정;

<img src="https://lh3.googleusercontent.com/CSh1JvDt_7CEop_gXd3liM0BNTGWzdPNO5-6TftIXp3DByvutbxnoOkBK3Q5xXCOTmB9I1G62Ey5dbm7At3DRZz_6i4pVYLEkRSYugKvOuTL6tOJ6inZgZr9h544ylcA04KII86RdCZjtiQUYHN3iABGWtzlUdf1wN5vbPeHzevtQ8sZqb9XvirBc0w5_izRco2SqwcAm2CKsBKEg-O4tRHKRNXkWdOp0X_gpotqJtevj6rlM7UoVN-ez1JBLHPxd2aaLR2-HXpj8g-vOSKacKHq3LvlGXvSFvwJZjt6SYi5qIg-KP_ZQ_APLhqP61GlvzL6RCjSzn0p6wuMTttkxJOUdxmJsarYQOAggr_1ZiaZ-TY0HpZNYD2UmmGoTQgbPNZbgmNCxfMardRGAI1EfiAIz2FN-2ccnKP-lVDS8lGE-oBJSmqwtXUHPDRj6Ly8Ehlx2bPPC9m1RGZoupYuUS16GmOgmt59oxrkNYp3EUnYZipjEKGbLXP1yJOrfJmQMjg7051YKdjH-SfL0x6X9WVHHVPECFKQiwm3gqgb8b12hrmGzQsCwi4GbWjy02NrYUzHrA1lnyK7tHRScTURV-06_FGg1Fqy_I6wXGiqfbUyI7NEH6e5NI9zRELgdPrhwfTolUJl0u7knpxnTfjuHm884iYczNrOli2aLg94xuTxqPOIbJQzY5zSyUanA0u6XnfobNQoK1xgSO1FWxbENo6gH39FiEU-WyyPtmOSWzU2LQvx=w968-h896-no" width=500px />

### timeval의 구조
```c
struct timeval {
	long tv_sec;
	long tv_usec;
};
```
timeout이
- NULL -> 해당 event가 발생 시까지 blocking
- 0 -> non-blocking
- 0이 아닌 값 -> read/write가 없는 경우 정해진 시간 후에 return

#### select system call 예제 code
> 전체적인 select문을 이해할 수 있는 code !!

```c
#define MSGSIZE 6

char *msg1 = "hello";
char *msg2 = "bye!!";
void parent(int[][2]);
int child(int[]);

int main() {
        int pip[3][2];
        int i;

        for(i=0;i<3;i++){               // 3개의 child 생성
                pipe(pip[i]);           // 3개의 pipe
                if(fork()==0)
                        child(pip[i]);
        }
        parent(pip);
        for(i=0;i<3;i++){
                wait(0);
        }
        exit(0);
}

void parent(int p[3][2]) {
        char buf[MSGSIZE], ch;
        fd_set set, master;
        int i, j, k;

        for(i=0;i<3;i++)
                close(p[i][1]);         // 읽기 pipe 닫기

        FD_ZERO(&master);               // fd_set을 0으로 초기화

        for(i=0;i<3;i++)
                FD_SET(p[i][0], &master);       // fd_set의 쓰기용 pipe를 1로 변경

        // 현재까지의 ->  fd_set은 master

		while(set=master, select(p[2][0] + 1, &set, NULL, NULL, NULL) >0 ) {
        // set=master하는 이유는 select의 return시 mask를 재설정 하는 이유때문
        //
        //              전의 set = 관심있는 pipe를 가진 set fd(3, 5, 7) = {1, 1, 1}
        // select 호출 
        //              후의 set = pipe를 체크한 후     set fd(3, 5 ,7) = {1, 0, 1}
        //
        // => (fd=5의 1 -> 0의 변경은 들어온 data가 없음을 의미)
        // => 여기서 select의 return은 data가 들어왔는지OR 안들어왔는지만 알수 있음
        //      즉, 어떤 pipe에서 데이터가 들어왔는지 확인하기 위해서는FD_ISSET으로 확인
        //      select의 return값이 3인 것은?
        //      1. 3명의 child가 모두 종료한 상황
        //      2. 3 child가 모두 data를 보내는 상황

                for(i=0;i<3;i++){
                        if(FD_ISSET(p[i][0], &set)){    // 각 pipe를 index로 접근하여 데이터가 들어왔는지 유무를 확인
                                if(read(p[i][0], buf, MSGSIZE) > 0)     // read >0 하는 이유는? => child가 종료되면서 write가 없으면 read의 return이 0이 상태
                                        printf("MSG from %d=%s\n", i, buf);
                        }
                }

                if(waitpid(-1, NULL, WNOHANG) == -1)    // pid=-1은 child중 아무나, waitpid == -1이면 child가 전부 종료인 상태
                        return;
        }
}

int child(int p[2]){
        int count;

        close(p[0]);

        for(count=0;count<2;count++){
                write(p[1], msg1, MSGSIZE);
                sleep(getpid()%4);
        }

        write(p[1], msg2, MSGSIZE);
        exit(0);
}
```

### FIFO 
- pipe와 FIFo
	- **pipe** -> parent - child 즉, 동일 ancestor를 갖는 프로세스간의 연결
	- **FIFO** -> 모든 프로세스들을 연결 가능

- UNIX의 file 이름을 부여 받음
	- (소유자, 크기, 연관 Permission을 가짐)
	- 일반 file처럼, open, close, read, write, remove가 가능

<사용법>
1. FIFO 만들기
2. FIFO open -> (O_RDONLY | O_WRONLY), mode (실행 option은 의미없음)
> O_RDWR 쓰는경우?  write 종료시 blocking된 채로 기다리기 위해, 아니면 무한 0 return
<img src="https://lh3.googleusercontent.com/VgqOQRFkxE0RyCd5Mz_LdfgsRiNBETsjhSfS2SZENReNDZ-8iFy_2noklWZKp6a_6s_ruCdqoerNvzk1vWib88LKIas2XmE2NWVC3axXZ9qn1xs6w9DrhNQmVV1bsR8o7Sz4wmnDPvxYoRDeU8qmj6CGyHg5MJzfh-1fUY_lPeSTqDrlv_SVglVYHZBmzut-haGuj0cue9tE_smfDNlAMSy9cyB30Fp_WxmRkKnoqT2qPbqmL7IA6Cx_SxRTPG3bu7uFXtQ6gPhB8b_MEd6lZUFdPpGUa1aSeERD8MRXVtKRbGIQ6kTbsDJIkDH5_ImXsMFVNtZh-MEW9mYPEMztN_jr9afRAAcrnTrsYAXyFtdCgMKW9vewR4A_6xGrNJVG81fJUOl-dcbvBGdWaLKAynzgD6itjBVYdX8lcRLxhey_o9ZoX6jUzAxRPJBZGLXP8Y0yAx0VnrZw_OEmeQfL1l-KJQF8_qHBo0-IaxaGUcRiIoPrq_Thy9vzl7qqBhjFpzRkEIeUcy0J3e7oJWQofSgqeUIOG79XZr5UcMKUcctT38YV_Y1ddfvHYJmG-BR5wkI-D3KAm2HxZ9Awqpz26fRLdwByBczSktw9awI9DkQHuKEAEVaTJ5bhsYCGTWaNHb05dOEZkuWgnMIEIdxWrDDzZAhAVTYOucA_PF8tuuG8F_0pWb2ObvG0SX3WgdM28NmbOd1nsq5nFKWTxLDxogar5f0WUZEgsrDLcFFToPhsoQYd=w968-h527-no" />
3. file에 read 또는 write

#### FIFO 형식
```c
#include <sys/types.h>
#include <sys/stat.h>
int mkfifo(const char *pathname, mode_t mode);
```

#### FIFO 예시
```c
mkfifo("/tmp/fifo", 0666);
fd = open(/tmp/fifo", O_WRONLY); 
// 또는 fd=open("/tmp/fifo", O_WRONLY | O_NONBLOCK);
```
- O_NONBLOCK -> read/write에 대한 non_block이 아닌 **open에 대한 non_block**
- 일반 open 호출은 다른 프로세스가 읽기 또는 쓰기를 위해 open될 때까지 blocking
- Non-blocking open의 경우, 상대 프로세스가 준비되지 않으면, **-1 return (errno=ENXIO)**

#### FIFO blocking
<img src="https://lh3.googleusercontent.com/Y53Et9-B7eHZHIa9qEM8xqzhadJGa1F5jhLbLrTGCoA-2ydqXHfrl8Lzdas_iT8-CgBnC_F8_FanCBAkMEpU_LuPeInrvRc8KCU6Zp0ef9vGbmjQh8iQAqaJUUjgyeBIe6rl2CGscN_XTkzm_vaVOasha4ONXBMrGZBTXsmkNbYJSjKl6MuSHk_H4KwbWXhEHg3QT6ur_H5IGszxRQOgHhInLe0bamhGwYOij-KBvYZ-ehyycbLEDNeP05JCnKcZSVn_UAqs13_bBYirMggcx3f8U3C6gToxa1TR16stXzuNOvGaOs_G93lEm9vvLyJen5WptkWsnVENuHNZkV5sHjcrnP0qvCSdmYazTxBZbqyYx7WPMsmiLmVHZI2Crvgz7dFEvekzqCJ_8cAsdEHYu_hBtNGTaTmi0yRDaRjI_VV46ZWR8rkT9t6Y25niuKeaeR0G_mEpHLJOEkfGGbhSec9BQYPQL-m-Hzh3ND1d_y9J2UC5BQki6p2Xw8BrgJQjeA7QqdYLn2l29I8WnERgp7Ag2IUmTIT0Mt9nL2yidX-GRMpVtgnF8XGH6lIhJq3XZzkMW4Ek1Wey2YkIqRYPRGHCAfY0KBraQ9wmO-chC2gcB2dpxxbzKWtDW4ifBPdea-XifYShU0kZVbmApaQMJX1IUKp636E0RZIOksvEhNAcnzXfvHCOZnkqFXvH4-AvStt-6b4b9vd12yjt3gt2cVeSKEkXR-WtGZ2ij19F4u4Wmhqp=w575-h923-no" height=700px />


<!--stackedit_data:
eyJoaXN0b3J5IjpbMTc1NTEyNTg3MCwxNzEyNzczMzMzLC00Nj
Y2NzE2OTEsNzI3NDI3MTA3LC0xNjY0MzEzNDkzLDEyNzc3OTU2
NTUsMTk4NzkyNzA2OSwtMTMzODUwMTk0MywxNzUwNzU4Mzg1LD
E3OTIzNzE1MzYsNDg0MzM4Mjc2LDIwOTkzNTc4NjJdfQ==
-->