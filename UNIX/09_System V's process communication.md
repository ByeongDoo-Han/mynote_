# 시스템 V의 프로세스간 통신
## IPC(InterProcess Communication) 설비
> file은 owner이 있음 -> owner만 보통 접근
> IPC는 OS에서 관리

- KEY
	- message_queue, semaphore, shared memory segment에 대한 identifier (file 이름에 해당)
	- 서로 다른 process들도 동일 IPC 객체는 같은 key 값으로 접근!!
	- 시스템에서 unique한 key값을 사용하여야함.

- KEY값 생성

	> 짜여진 Program이 작동이 안될 경우 Program상 오류도 있지만,
	> key값상 오류(다른 사람과 겹침)도 있을 수 있음 
	> -> 시스템 전체로 unique한 key값을 사용 (ftok)

	```c
	#include<sys/ipc.h>
	key_t ftok(const char *path, int id);
	```
	- 해당 파일의 st_dev, st_ino와 id로 key값 생성

## IPC 객체 상태 구조
```c
struct ipc_perm {
	uid_t cuid; // 생성자의 uid;
	gid_t cgid; // 생성장의 gid;
	uid_t uid; // 소유자 uid;
	gid_t gid; // 소유자 gid;
	mode_t mode; // permission readOnly = 4, writeOnly = 2(execution은 의미 없음);
}
```

## IPC 정보 검색 및 삭제
- ipc 관련 정보 검색
	- $ipcs
- ipc 삭제
	> ipc는 사용후 꼭 지워야한다!(System 전체에 사용가능한 Message queue의 제한)
	- $ipcrm
		- -m shmid : shared memory id
		-  -q msqid : message queue id
		-  -s semid : semaphore id

## FIFO vs Message queue
<img src="https://lh3.googleusercontent.com/CW186bXkjbsjCtZ6l_56wZfTLWLx03WE703O-pXeGeD8qVI1Y4dNQAErgCSpqkLYELYNJdYud00I_LieMuEOUrZY3UvD8zsh_6gs2MFAtSNqMmVyv1GSBwhyKx0O4V8Fdj3lpvG7GVPtYDbWfpIIhvyLPPggPyKbz7G2T_JVxalONkge5JAyrrdm1VZqJgkZUR41BUP4gF4_cPGYrPoStS7grO3EzN_Jd339oiz_6xhjJ31uhNCuXp8N9uGKVNLmk3ag8VwXK-z1f9O1dvMkIKXxL75GTYbmMmHlXMgT6C22T9jfasNPayBZaigaY1tq2c56NcAeiBZNs_nJ4gkt1_TX_t1fYaXe55-aSigIFkiq1T6ho51UBRC9VcyJR-ma4SSFyoYzPp4tCWnbPqedZcn9DZ4BdXmw_k-cAr5NN0QEWHyL9pjueoU7xKHp3-89PjD7aqTFVfgyPcaJ7mBmu51giBo2nxYMclUG5X02ZjLE51ON7e8nFIfirqr3vUjtDprh0QebzbP9-TaCwuNJsH-KbMxvgojOvvzsAcpHSdfh2WwkFfUvrqZuUQglqs8JOS8F16WMflGyXVyfooK0af3O7l-avO8ze_MT61WppECMeHjv94XI-UlBYcDIyQ5ZZDszOEgIU5YNE7EuqkDJFp7jds36wi1TFplZemCFyATv0vYeAqO-q7TKeb_-Wxq5nfG2Zmt_LAPanxJmiJYPdr4gXXyOlPW49w0PLR_ObuBAQ7Sb=w968-h434-no" width=600px/>

1. **Message queue**는 1개의 queue만 있어도 되고 각 child process는 고유 msqid를 가짐,  
**FIFO**는 여러개의 FIFO와 dest id와 src id를 통해 출, 목적지에 대한 확인이 가능

2.  <Input 5개 중 3개만 읽고 종료하는 경우>
**message queue** -> 남아있다.
**FIFO** -> 남은것은 flush된다.
## message passing
- message queue를 통한 message 전달
	- msgget : queue 생성
	- msgsnd : message 보내기
	- msgrcv : mssage 받기

## msgget System call
```c
#include <sys/msg.h>
int msgget(key_t key, int permflags)
```
#### <인자>
- key : message queue의 key 값
- permflags (= queue에 대한 access permission)
	- = 0 : 기존 그대로 사용
	- | IPC_CRAET
		- 해당 queue가 (없으면->생성한 후 return;, 있으면-> return)
		- 이 flag가 설정 되지 않은 경우에는, queue가 존재하는 경우에만 return;
	- | IPC_EXCL
		- 해당 queue가 존재하지 않는 경우만 성공, 아니면 -1 return
	> message queue 초기화시 4명 중 queue 생성에 성공한 사람을 확인하기 위해 => 처음 만든 사람
	> 실패한 사람(처음 만든사람이 아닌경우) -> -1 return

- return 값 : 음수가 아닌 queue identifier

## msgsnd System call
```c
#include <sys/msg.h>
int msgsnd(int mqid, const void* message, size_t size, int flags);
```

#### <인자>
- mqid : message queue identifier
- message의 주소 : 보낼 message가 저장된 주소
- size : message의 크기
- flags (= IPC_NOWAIT)
	- send가 불가능하면 즉시 return (queue가 가득 찬 경우)
	- flag가 설정 되지 않으면 (즉, 0이면), 성공시 까지 blocking
- return 값은 0 or -1

### <message의 구조>
- long type의 정수 값을 갖는 mtype과 임의의 message의 내용으로 구성.
- message의 size는 message 내용의 크기만.

- message 구조체의 예;
	```c
	struct mymsg{
		long mtype;		// message type (양의 정수)
		char mtext[SOMEVALUE];	// message 내용
	}
	```
	- **=> mtype은 즉, message id (보내는 사람 id)를 의미**
## msgrcv System call
```c
#include <sys/msg.h>
int msgrcv(int mqid, void* message, size_t size, long msg_type, int flags);
```

#### <인자>
- mqid : message queue identifier
- message 주소 : 받는 message를 저장할 저장 장소의 주소
- size : (받는 사람이) 준비된 저장 장소의 크기
- msg_type
	- = 0 :  queue의 첫 message (inorder : 1->2->3)
	- \> 0 : 해당 값을 갖는 첫 message
	- < 0 : mtype값이 절대값 보다 작거나 같은 것 중 **최소값**을 갖는 첫 message
		- ex) -10 : 10보다 작은 message 중에서 순서대로 (방향을 구분해주기 위해)
- flags
	- = IPC_NOWAIT
		- receive가 불가능하면 즉시 return(queue에 해당 msg가 없는 경우)
		- return 값은 -1; (errno = EAGAIN)
		- flag가 설정 되지 않으면 (값이 0이면), 성공 시까지 blocking
	- = MSGNOERROR( Sender가 아닌 Receiver에 필요)
		- message가 size보다 길면 초과분을 자른다.
		- flag가 설정 되지 않으면 size 초과 시 error
- return 값
	- receive 성공 시 :받은 message의 길이
	- 실패시 return -1
	- access permission 때문에 실패한 경우 errono = EACCESS

### message 송수신의 예
```c
struct q_entry{
	long mtype;
	int mnum;
}
struct q_entry msg;
qid=msgget(0111, 0600 | IPC_CREAT);		// <-> open과 차이점 (open할때 RW설정)
while(msgrcv(qid, &msg, sizeof(int), 1, 0) > 0) {
	msg.mtype=2;
	msg.mnum=msg.mnum + 8;
	msgsnd(qid, &msg, sizeof(int), 0);
}
```

> 0600 -> 나만 읽고 쓰기 가능
> 0644 -> 상대방 읽기 가능

## msgctl 시스템 호출
- msgctl 호출
	- message queue에 대한 정보 획득
	- message queue 제거

- 사용법
```c
#include <sys/msg.h>
int msgctl(int mqid, int command, struct msqid_ds *msg_stat);
```

#### <인자>
- mqid : message queue identifier
- command =
	- IPC_STAT : msg queue의 상태 정보 확인
	- IPC_RMID : msg_queue 삭제 ( 3번째 인자값 = 0)
- msqid_ds 구조 :
	- struct ipc_perm msg_perm;	// 소유권
	- msgqnum_t msg_qnum;			// msg 수
	- msglen_t msg_qbytes;			// bytes 수
	- pid_t msg_lspid;						// last sender
	- pid_t msg_lrpid;						// last receiver
	- time_t msg_stime;					// last sending time
	- time_t msg_rtime;					// last receipt time
	- time_t msg_ctime;					// last s/r time

## Semaphore

<img src="https://lh3.googleusercontent.com/ZIZf7fGX-UTxhPR0W_fXbKz16ewn_Fh24wa3-rl1PJFVodAHs6rmEauKcEKY9XMKjSqUKoM1ZaXKoFP9LawTerExK62RvLvm6YJf94JOEtsNGUDPhqE9Y7R1YWKgQSpayFkbI2Ioj6f5Xh6t_nVrxMS2cNwEBNyMpaVWEc1-Nlkh-C3hc_Sp0qiphb_l_OGn0ZculmB4-Z76iD1QgXGIUKqgzJhBi5_hDAgYzNG-4UfLUP3I9w64xb3odcnx404G2jfl3lg99iUhIsEh5bW1Z_KzYpQL_LQcYRlVMfxqo48cpBhvlvnQmy5OC-7hrp-gpQOaTHlyfsJSCR4ndvV7rVREIJr0N_aFatESPXsyVQSbbM30kvHfkQZX4u3T3CVMeNAg1mICYKr0TYuza2ircCgscjj4cGcRYiFYmi5Qb4LqErkdT-SLacILXuDMkfebZnkenthkmvFjALOnnDT8KTtpCs57BHUs9AniYeMn0NcEsKa2IxQLTB2FR-T52_mXhEklJ8PO2NkK_QqdG5oRW_zhzrHGn1e5jmsQesMQ7-wyRmhVh077_SnuHUzEg9gszgbyqEg1gok9YGEUa8wH8JzeLGRpOYwH0WgCNGPEj8Gx9A0T9sMenOXAXCv7uK5ucmGeKZ9xRkTOZCj2Gk4e6IeRgrxqIvZRGzhxk6m52svthMI9pq1j91wzb9WgkCGMXXNoPaBfrkGlW0nOX_-Sd5NUacJNDAt_tlgczxuliQXx-Uxw=w915-h904-no" width=600px />

- p(sem) or wait(sem)
	```c
	if(sem>0)	// 빼기전에 sem>0인지 확인 (UNIX의 semaphore 값은 음수가 없음!!)
		decrement sem by one;
	else {		// blocking (sem == 0)
		wait until sem becomes non-zero;
		then decrement;
	}
	```
- v(sem) or signal(sem)
	```
	increment sem by one;		// (원래) : -5 (queue에 기다리는 사람 5명)
	if(queue of waiting processes not empty)	// (unix)는 음수가 허용되지 않아 
		restart first process in wait queue;	// 확인할 수 있는 함수가 존재
	```

- 사용예 : 전형적인 critical section
	```
	p(sem);
	someting interesting;
	v(sem);
	```

## Semget System call
```c
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
int semget(key_t key, int nsems, int permflags);
```
#### <인자>
- key : semaphore 집합 이름 (semaphore는 여러개)
- nsems : semaphore 집합 내의 semaphre 수
- permflags : 0600, IPC_CREAT, IPC_EXCL, ...
	- (default : IPC_CREAT | IPC_EXCL -> return != -1)
	- **IPC_EXCL** : semaphore은 초기값이 매우 중요!!
		- -1 -> semaphore 이미
		- 정상값 -> 새로생성
		- 0 -> critical section에 못들어간
		- 10 -> critical section이 아님
		- **\*\*=> 1명만 만들고 + 초기화**
- return 값 : semaphore 집합 id
- 집합 내 각 semaphore와 연관된 값
	- semval : semaphore 값 (semaphore 값의 초기화 필요)
	- sempid : 최근 semaphore를 access한 process id
	- semncnt : semaphore 값이 증가하기를 기다리는 process 수
	- semzcnt : semaphore 값이 0이 되기를 기다리는 process 수

## Semctl System call
```c
#include <sys/sem.h>
int semctl(int semid, int sem_num, int command, union semun arg);
```

#### <인자>
- semid : semaphore identifier
- sem_num : 집합 내 특정 semaphore 지정
- command :
	- IPC_STAT : 상태 정보를 arg.stat에 저장
	- IPC_RMID : semaphore 집합 삭제
	- <단일 semaphore에 영향을 미치는 기능>
	- GETVAL : semval 값 return
	- SETVAL : seval 값을 arg.val 값으로 지정 (정확한 값을 정해주어야함)
	- GETPID : sempid 값을 return
	- GETNCNT : semncnt 값을 return
	- GETZCNT : semzcnt 값을 return
	- <semaphore 집합 전체에 영향을 미치는 기능>
	- GETALL : 모든 semval 값을 arg.array에 저장
	- SETALL : arg.array값으로 모든 semval 값을 지정
- semun arg : 명령에 필요한 정보
#### struct semun arg;
```c
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};
```

## semaphore 만들고 초기값 설정하기
// 1
```c
union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};
union semun arg;

semid = semget((key_t) 0123, 1, 0600 | IPC_CREAT | IPC_EXCL);
arg.val = 3;
semctl(semid, 0, SETVAL, arg);
```
// 2
```c
union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};
union semun arg;
ushort buf[3];

semid = semget((key_t) 0246, 3, 0600 | IPC_CREAT | IPC_EXCL);
for(i=0;i<3;i++)
	buf[i] = i+1;
arg.array=buf;
semctl(semid, 0, SETALL, arg);
```
> #### Union
> 공간은 하나인데 (인자가 하나인데) 안에 담긴 변수를 지정해서 사용가능
> ( 특정 경우에 따라 인자의 자료형이 달라짐)

## Semop System call
```c
#include <sys/sem.h>
int semop(int semid, struct sembuf *op_array, size_t num_ops);
```
#### <인자>
- semid : semaphore identifier
- oparray : 수행 할 연산 지정(수행할 연산 pointer)
- num_ops : op_array내의 sembuf의 수 (연산 몇개?, 여러 개의 semaphore에 대한 연산을 동시에 지정할 수 있음.)
	- 구조체 변수
	- 구조체 배열 -> **한꺼번에**

	> **구조체 배열을 사용**
	> producer - consumer
	> wait x 2, signal x2 -> 함께 요청하는게 가능

### struct sembuf
구조체 변수 or 구조체 배열
```c
struct sembuf {
	unsigned short sem_num;
	short sem_op;
	short sem_flg;
};
```
- sem_num : semaphore index
- sem_op : 수행 할 연산
	- 양의 정수 -> signal (+1)
	- 음의 정수 -> wait(-1)
	- 0 -> semval 변화 X, semval이 0이 될때까지 wait
- sem_flg
	- IPC_NOWAIT : signal 의미없음, wait 걸렸을때 sem-1하고 pass
	- SEM_UNDO : (지금사용X) 프로세스 종료시 semaphore을 초기값으로 복구하고 종료, process간 semaphore 동기화가 어려울 수 있다.
	- 그 외 -> **0**으로 설정할 것!!
### UNIX의 특별한 Semaphore
**sem_op의 음수값**
- 음수 : p() or wait() 연산
```
if(semval >= |sem_op|)
	set semval to semval - |sem_op|;	// 현재 semval에서 sem_op의 절대값 빼기
else
	wait until semval reaches or exceeds |sem_op|;	// 뺄수 있을때 까지 대기
	then set semval to semval - |sem_op|;
```

#### UNIX Semaphore는?
FIFO queue(순서대로 queue를 진입(-1), 진출(+1))가 아니라 **Non-FIFO queue**사용
=> 뺄 수 있는 첫번째 사람을 깨움
=> **영원히 blocking 될 수도 있다.**
=> **semaphore 하나로 해결할 수 있다.**(Semaphore를 최소로 사용하는 것을 생각해보기!!)
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTY1NzkxMjQ4MiwtMTMxMTgyOTEwNyw5OD
k3NjgwMzEsODgzNDQxOTk0LDEzMjE3NjYyOTUsLTEzNjE1Njkw
MTEsMTk5MjA5ODA3OSw3Mzc5NzE2OTIsNTk2MDgzNDA1LC0zND
k4NTk5MzMsLTEwNDE0NDM4OTEsMTk2MjI2Nzc0NF19
-->