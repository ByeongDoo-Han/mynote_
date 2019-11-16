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
```

### IPC 정보 검색 및 삭제
- ipc 관련 정보 검색
	- $ipcs
- ipc 삭제
	> ipc는 사용후 꼭 지워야한다!(System 전체에 사용가능한 Message queue의 제한)
	- $ipcrm
		- -m shmid : shared memory id
		-  -q msqid : message queue id
		-  -s semid : semaphore id

### FIFO vs Message queue
<img src="https://lh3.googleusercontent.com/CW186bXkjbsjCtZ6l_56wZfTLWLx03WE703O-pXeGeD8qVI1Y4dNQAErgCSpqkLYELYNJdYud00I_LieMuEOUrZY3UvD8zsh_6gs2MFAtSNqMmVyv1GSBwhyKx0O4V8Fdj3lpvG7GVPtYDbWfpIIhvyLPPggPyKbz7G2T_JVxalONkge5JAyrrdm1VZqJgkZUR41BUP4gF4_cPGYrPoStS7grO3EzN_Jd339oiz_6xhjJ31uhNCuXp8N9uGKVNLmk3ag8VwXK-z1f9O1dvMkIKXxL75GTYbmMmHlXMgT6C22T9jfasNPayBZaigaY1tq2c56NcAeiBZNs_nJ4gkt1_TX_t1fYaXe55-aSigIFkiq1T6ho51UBRC9VcyJR-ma4SSFyoYzPp4tCWnbPqedZcn9DZ4BdXmw_k-cAr5NN0QEWHyL9pjueoU7xKHp3-89PjD7aqTFVfgyPcaJ7mBmu51giBo2nxYMclUG5X02ZjLE51ON7e8nFIfirqr3vUjtDprh0QebzbP9-TaCwuNJsH-KbMxvgojOvvzsAcpHSdfh2WwkFfUvrqZuUQglqs8JOS8F16WMflGyXVyfooK0af3O7l-avO8ze_MT61WppECMeHjv94XI-UlBYcDIyQ5ZZDszOEgIU5YNE7EuqkDJFp7jds36wi1TFplZemCFyATv0vYeAqO-q7TKeb_-Wxq5nfG2Zmt_LAPanxJmiJYPdr4gXXyOlPW49w0PLR_ObuBAQ7Sb=w968-h434-no" width=600px/>

1. Message queue는 1개의 queue만 있어도 되고 각 child process는 고유 msqid를 가짐, 		  FIFO는 여러개의 FIFO와 dest id와 src id를 통해 출, 목적지에 대한 확인이 가능

2.  <Input 5개 중 3개만 읽고 종료하는 경우>
message queue -> 남아있다.
FIFO -> 남은것은 flush된다.
### message passing
- message queue를 통한 message 전달
	- msgget : queue 생성
	- msgsnd : message 보내기
	- msgrcv : mssage 받기

### msgget System call
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
	> message queue 초기화시 4명 중 queue 생성에 성공한 사람을 확인하기 위해 => 처음 만든 사람, 실패한 사람 -> -1 return
- return 값 : 음수가 아닌 queue identifier

### msgsnd System call
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

#### <message의 구조>
- long type의 정수 값을 갖는 mtype과 임의의 message의 내용으로 구성.
- message의 size는 message 내용의 크기만.

- message 구조체의 예;
	```c
	struct mymsg{
		long mtype;		// message type (양의 정수)
		char mtext[OMEVALUE];	// message 내용
	}
	```
	- **mtype은 즉, message id (보내는 사람 id)를 의미**
### msgrcv System call
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
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTk2MjI2Nzc0NF19
-->