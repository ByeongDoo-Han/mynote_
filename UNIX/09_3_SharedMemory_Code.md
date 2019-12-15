# SharedMemory
## t1

### reader

```c
#include "../Myheader.h"

struct databuf{
        int d_nread;
        char d_buf[512];
};

int main(void) {
        char *buf1;
        int i, shmid1, shmid2, shmid3, *buf2;
        struct databuf *buf3;
        key_t key;

        key = ftok("key", 1);
        shmid1=shmget(key, 512, 0600|IPC_CREAT);
        shmid2=shmget(key, 10*sizeof(int), 0600|IPC_CREAT);
        shmid3=shmget(key, 5*sizeof(struct databuf), 0600|IPC_CREAT);

        buf1=(char *)shmat(shmid1,0,0);
        buf2=(int *)shmat(shmid2,0,0);
        buf3=(struct databuf *)shmat(shmid3,0,0);

        read(0, buf1, 512);
        for(i=0;i<10;i++)
                scanf("%d", buf2+i);
        for(i=0;i<5;i++)
                scanf("%s", (buf3+i)->d_buf);

        exit(0);
}
```

### writer

```c
#include "../Myheader.h"

struct databuf {
        int d_nread;
        char d_buf[512];
};

int main(void) {
        char* buf1;
        int i, shmid1, shmid2, shmid3, *buf2;
        struct databuf *buf3;
        key_t key;

        key = ftok("key",1);
        shmid1=shmget(key, 512, 0600|IPC_CREAT);
        shmid2=shmget(key, 10*sizeof(int), 0600|IPC_CREAT);
        shmid3=shmget(key, 5*sizeof(struct databuf), 0600|IPC_CREAT);

        buf1=(char *)shmat(shmid1,0,0);
        buf2=(int *)shmat(shmid2,0,0);
        buf3=(struct databuf *)shmat(shmid3,0,0);

        write(1, buf1, 512);
        for(i=0;i<10;i++)
                printf("%d\n", *(buf2+i));
        for(i=0;i<5;i++)
                printf("%s\n", (buf3+i)->d_buf);

        exit(0);
}
```

## lab 16-1

<img src="https://lh3.googleusercontent.com/Nkm20Jn9Wfe8H2cmY-sKd98gHcJExH0VF1IheL8p06T2arxOFW_JW-BVK2Q9zVUkH0BDR48ahgcHspnhzVAEOH_ux3QZkm9_pV3J3mMB30kXYqG_0OnvKrDMcCSwqOdUdvwwyKBMLSJhskNx8QdMq5kc5-pDk2wChf-iJSPcQFppal8FsTxZaUJxjhTOOonzFjghrXYJPr4peU2jyIUgy363jb6VY3XlH1BK1SV4olOzvL5fzQdSRUsFQHuhkhBlJPMhKeWYhcdbI74MyyRdypBflRrGIQcwvzAvPEf8X1K2G2TLOVxJDWEDYqcX7s99Fqk0yjiPN0YjYyHp5HCAHGTyoujZXNwemHpKuw84j-R2BDqDVVrIf_NYlFWRNuEUsa721WtFeAyMoJejPjyHCRICg4lTmQST6JJ2coiSMEuEyo0NxECqnoQS-tJQQIFcfnSkCOjyfkxOgWoQaw9RwkQEN77Ck1HmmhtJ69utR64TZTun6_zMImAZ1AlF-GqOSwGIHmR7Uaq2G0wo9lY5-j-DcsccBnTe5nUkOQsvrlgEuFTOLPj44T7vjecNIAgscp766GU8g-sVSjoe7-YD8x-RTpPpUbcgGSM0vznyW_2HEnvDFSoVwT-jcmtXL3_2Aq7JOSMj5_XS2hjESLy-gyJi3yR_uryBaO4ffzOprHJyohNt3voBMmuUs3ObT2-4cNef3jnM2cKjlJ1QFZu9pwbavIlATVCgF_xhn75sSNQ4vNuW=w1134-h943-no" width=600px/>

공유 메모리를 이용하는 두 개의 프로그램을 작성 합니다. 프로그램 A는 scanf() 명령으로 10개의 정수를 입력 받아 공유 메모리에 저장 하는 작업을 10회 반복 실행 합니다. 프로그램 B는 공유 메모리 에 저장된 내용을 printf() 명령으로 출력하는 작업을 10회 반복 실행 합니다. 이때, 프로그램 B는 프로 그램 A가 정수를 쓴 후 읽어야 합니다. 이러한 동기화 작업은 semaphore를 사용합니다.

> 현재 program A core dumped 나는 상황... 교수님 코드랑 똑같은데.. 문제가 있다.

### program A

```c
union semun {
        int val;
        struct semid_ds *buf;
        ushort *array;
};

int main(void) {
        int shmid, semid, i, n, *buf;
        union semun arg;
        struct sembuf p_buf;
        key_t key;

        // semaphre 생성 및 초기화
        key = ftok("key", 1);
        semid = semget(key, 1, 0600 | IPC_CREAT | IPC_EXCL);
        if(semid == -1){
                semid = semget(key, 1, 0);
        }
        else {
                arg.val = 0;
                semctl(semid, 0, SETVAL, arg);
        }

        // sharedmemory 생성 및 초기화
        shmid = shmget(key, 10*sizeof(int), 0600 | IPC_CREAT | IPC_EXCL);
        buf = (int *)shmat(shmid, 0,0);

        for(i=0;i<10;i++){
                // 공유메모리 입력
                scanf("%d", (buf+i));
                // semaphore signal
                p_buf.sem_num=0;
                p_buf.sem_op=1;
                p_buf.sem_flg=0;
                semop(semid, &p_buf, 1);
        }

        exit(0);
}
```
### program B

```c
union semun{
        int val;
        struct semid_ds *buf;
        ushort *array;
};

int main(void){
        int shmid, semid, i, n, *buf;
        union semun arg;
        struct sembuf p_buf;
        key_t key;
        // semaphore 생성 및 초기화
        key = ftok("key", 1);
        semid = semget(key, 1, 0600 | IPC_CREAT | IPC_EXCL);
        if(semid == -1){
                semid = semget(key, 1, 0);
        }
        else {
                arg.val = 0;
                semctl(semid, 0, SETVAL, arg);
        }
        // 공유메모리 생성 및 초기화
        shmid = shmget(key, sizeof(int)*10, 0600 | IPC_CREAT | IPC_EXCL);
        buf = (int*)shmat(shmid,0,0);
        for(i=0;i<10;i++){
                // semaphore wait
                p_buf.sem_num=0;
                p_buf.sem_op=-1;
                p_buf.sem_flg=0;
                semop(semid, &p_buf, 1);
                // 공유메모리 내용 출력
                printf("%d\n", *(buf+i));
        }

        semctl(shmid, IPC_RMID, 0);

        exit(0);
}
```

## lab16-2

<img src="https://lh3.googleusercontent.com/o5sHh9a5eitIbW1QP1FRFnH8Tx4GfOT66z686fnBzi956yjSOObuYe3BcVSj1DuW4sNb-ClZonErmYZQc8mgUSNKooXGpWAPDUdb26l5fXl3MIiFEXE-DzKQab99kHJAkgbHA29sKvOzxkiANaHWNDJVPFc1XdFouFZ1t7-qpVKoavdP48FgtkIRIxLGS4opwHcPiffpZJkCe7lea3kprgjc3iRPYih2SYcGiAN0AHpmbJ-IEYzwBnue44mXwllhgPbJI-vCOos7X5ae6L96VjTGS5g4plwOzoaqiema9wAsQ3rtgWk3EKGCMOZoD9wblrdvqPyXIlYhJPConE6JSbsjzOl1xG3A6Vi8ewVrNCbqYbwTXC9d4uXrknm9M1SlHYlX7hkZAZ6ZnUU1PjZuG3BtIHAZsEQCTLQTqRi6RwLszENVCnZfJ6j6C0vnSIvb50lhDGmqPY7Cxh6j2po9KHS1BbZoGtCDnuEysXGlIg4qIfxxgEEoymgdcjsxtP-2gDnUggVA0oBq0ijlCpsJlg5HeRYRAk8NJ-yV8z3nIjpMuzeRVA3AHeX06acu73wTDWGu5kbDjq-E-cPPHygojsVwJGrXksdRTyYKXvPfnAJQ9n53utYzszvHS6c_Pg5HDa1gnlTcbzaljArKHNLm4UT9_56xPxxO8ntDz9wzO6-P5CVPToqIQuNKp1BPnuf82bMdoydkH81gP8nKk3jC_Uqj0-Gc5lTiYUdmHScJjmPQRhkD=w1498-h608-no" width=500px/> 

lab16-1의 자체정보에 대한 동기화가 이루어지게 한다.


<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE2MjU3OTQ0NDZdfQ==
-->