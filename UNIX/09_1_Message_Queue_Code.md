# Message Queue Code

## t1.c
### Sender
```c
#include "../Myheader.h"

struct q_entry {
	long mtype;
	int mnum;
};

int main(void) {
	int qid, i, in;
	key_t key;
	struct q_entry msg;

	key=ftok("data1", 3);
	qid=msgget(key, 0600 | IPC_CREAT);
	for(i=0;i<3;i++){
		scanf("%d", &in);
		msg.mtype=1;
		msg.mnum=in;
		msgsnd(qid,&msg,sizeof(int),0);
	}

	exit(0);
}
```
### Receiver
```c
#include "../Myheader.h"

struct q_entry {
	long mtype;
	int mnum;
};

int main(void) {
	int qid, i, in;
	key_t key;
	struct q_entry msg;

	key=ftok("data1", 3);
	qid=msgget(key, 0600 | IPC_CREAT);
	for(i=0;i<3;i++){
		scanf("%d", &in);
		msg.mtype=1;
		msg.mnum=in;
		msgsnd(qid,&msg,sizeof(int),0);
	}

	exit(0);
}
```
## lab14-1
server process는 세 개의 client process들과 데이터를 주고받기 위해 message queue를 만듭니다. 각 client는 message queue를 이용하여, 표준 입력으로 입력된 정수를 server process에게 전송 합니 다. server process는 client process로부터 전송된 정수 값에 +8을 한 후, 해당 client에게 다시 보냅니 다. client process는 돌려받은 정수 값을 표준 출력으로 출력 합니다. client process는 정수 데이터의 입/출력 작업을 5회 반복 한 후 종료 합니다. 각 client process는 main() 함수의 arguments로 자신의 id를 입력 받습니다
<img src="https://lh3.googleusercontent.com/4D8yslPRyavnRhlcVtuVEXGs8ztfZSLTHm-p2oC1Y3IGIgtb9sJKVi6Lnjb_0GPmvdujjzfKSAk_kCI4Lqdtx4lhc7cW_pSlGnvhiwtV0VH7U5a7gTz4TKKy4D_5j47Kd8iwMTEfVo90ZjIV-glgEAaE9cQg9luYkNBPy2mTKx_EpIdL27PKLjPyexLZIZLAuJmdR9QymY2uoatoLCXMNw88BpYkYwRBCPhS3odnPFAfDcuZFMXZ2CevpagtupZW0q2uQ0rZnB4wtP-JzS2q-NwivzBvZdsJdFXtggJduGP9WkTyIVCvhMz-E-kmk2D7divUBY2Qsk1LNUjyE5ZC4IsGixtce_417dot3h3__fAi6U--9Dji2TuJqTuxVEQHJP2aVSEfM55k9r0yNt9gMptj7F2WuVR-14Jh90fhQTFKYLTVi1jVIdmYLmGn5AJ9GwgqWk_r1ZibBkcZPd3zEJ20rGltZ1S9ZOarfPimypzcEHruo1Rka4q4A0foI7biKpsjIsKyDxGP2iD4i2CcbY3trr3Syi9CYLVch0291-HGo9yGgnFHmi2mTPq8aA4Y44uIz8I_St4vEGgHpU7L6Q1mMqltDZQ8MNYaMyxfInlt4bhobntbD8pmHwVnzJit0JaW8mI800AVOgRaYKu0vDDVf0tuklSYUAYifGqv3WVx6UJGwJHYf2Wjj8CNcU7legCaeVslHwYpIrxFkcSLfDt8QmfeZzprxkwPBfscyLMdtdhF=w1236-h904-no" width=600px />
### Client
```c
#include "../Myheader.h"

struct q_entry{
	long mtype;
	int data;
};

int main(int argc, char** argv){
	int i, qid, in, id;
	key_t key;
	struct q_entry msg;

	id=atoi(argv[i]);

	key=ftok("key", 3);
	//message queue open
	qid = msgget(key, 0600 | IPC_CREAT);
	
	for(i=0;i<5; i++){
		scanf("%d", &in);
		// mtype 설정
		msg.mtype=id;
		// 데이터 복사
		msg.data=in;
		// message 보내기
		msgsnd(qid, &msg, sizeof(int), 0);
		// message 받기
		msgrcv(qid, &msg, sizeof(int), id+3, 0);
		printf("%d\n", msg.data);
	}
	
	return 0;
}
```
### Server
```c
#include "../Myheader.h"

struct q_entry {
	long mtype;
	int data;
};

int main(void){
	int i, qid;
	key_t key;
	struct q_entry msg;

	key=ftok("key", 3);
	// message queue 만들고 open
	qid=msgget(key, 0600 | IPC_CREAT);
	
	for(i=0;i<15;i++){
		// message 받기
		msgrcv(qid, &msg, sizeof(int), -3, 0);	// 3보다 작은 순서대로
		// mtype 조정
		msg.mtype += 3;
		msg.data += 8;
		// message 보내기
		msgsnd(qid, &msg, sizeof(int), 0);
	}

	return 0;
}
```
## lab14-2
네 개의 프로세스가 동기화를 하며 자신의 프로세스 id를 5회 출력하는 프로그램을 작성 합니다. 이 프로그램은 main() 함수의 arguments로 동기화에 참여하는 전체 프로세스 중 자신의 출력 순서를 입력 받습니다. 프로그램이 시작되면, 순서대로 자신의 프로세스 id를 출력 합니다. 동기화 작업은 message queue를 사용하여 수행 합니다. 
```c
#include "../Myheader.h"

struct q_entry {
	long mtype;
	int data;
};

int main(int argc, char** argv){
	int i, qid, pid, id;
	key_t key;
	struct q_entry msg;
	
	id=atoi(argv[1]);

	key=ftok("key", 3);
	qid=msgget(key, 0600|IPC_CREAT);
	
	if(id>1)
		msgrcv(qid, &msg, sizeof(int), id, 0);
	pid=getpid();
	msg.mtype =id+1;
	msg.data = id;
	for(i=0;i<5;i++){
		printf("%ld\n", pid);
		sleep(1);
	}
	if(id<4)
		msgsnd(qid, &msg, sizeof(int), 0);

	exit(0);
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTIxNzIwOTAzNF19
-->