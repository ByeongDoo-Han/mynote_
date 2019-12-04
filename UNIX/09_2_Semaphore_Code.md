# Semaphore Code
## t1.c

```c
union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
};

int main(void) {
        int semid, n;
        key_t key;
        union semun arg;

        key=ftok("key", 1);
        semid=semget(key, 3, 0600 | IPC_CREAT | IPC_EXCL);
        arg.val=3;
        semctl(semid, 0, SETVAL, arg);

        n=semctl(semid, 0, GETVAL, arg);
        printf("%d\n", n);
        n=semctl(semid, 0, GETPID, arg);
        printf("%d vs. %d\n", n, getpid());
        n=semctl(semid, 0, GETNCNT, arg);
        printf("%d\n", n);
        n=semctl(semid, 0, GETZCNT, arg);
        printf("%d\n", n);

        return 0;
}

******OUTPUT******
3
27500 vs. 27500
0
0
```

## t2.c

```c
union semun{
        int val;
        struct semid_ds *buf;
        unsigned short *array;
};

int main(void){
        int semid, i;
        key_t key;
        union semun arg;
        ushort buf[3];

        key=ftok("key", 1);
        semid=semget(key, 3, 0600 | IPC_CREAT);
        for(i=0;i<3;i++)
                buf[i]=i+1;
        arg.array=buf;
        semctl(semid,0,SETALL, arg);

        semctl(semid,0,GETALL, arg);
        for(i=0;i<3;i++)
                printf("%d\n", arg.array[i]);
        return 0;
}

***OUTPUT***
1
2
3
```

## Lab15-1
메모리 매핑을 이용하여 통신하는 두 개의 프로그램을 작성 합니다. 두 프로그램 모두 “data1" 파일을 메 모리 매핑 합니다. 한 프로그램은 외부 입력을 읽어서 매핑된 메모리에 저장 하는 작업을 3회 실행 합니다. 다른 프로그램은 매핑된 메모리의 내용을 출력하는 작업을 3회 실행 합니다. 두 프로그램 사이의 읽기-쓰기 동기화를 위해 semaphore를 사용 합니다.

<img src="https://lh3.googleusercontent.com/hDuEHYtElEkJlRpn3VfOh6tjaXsGNamoYJJjmb6I1jv7UeBPMbtcg5Z8d3-Vv0LtE_jdVH6FWbYI_H5RPrwAbnqGz_PKQ5MV0EFgBn90ohh7P1DWufnZyaD7xZ5E5dPjKxJesGg7BDwQKrob0xXQewikM5ooEb3vgBj7pPONSczgDJXdy4Dq5CRYrchFVB9rh9d1Pupzpyhb_--prOp88zAIF-IuUS87zXPu6662NPLCN4s8QvLsL4YSpOtfAfGPq6WkoCKNcovQcW2F4Uf7VaPDvLLDqZX6_j7XxcDMQj6XP5i1ZnnU3femJ2IcH2pSTZl8sUvyyd9dpY4M2BE__ROuEp3Gmvy0Rygsps97TwLuaq2MbKpZZtRudzpt222FnhS4eaRxoX6Q_ZcTO9HSGM5AE4sC25-0zrS0ruZQBGlsJOdvnV-G98cbnEfghhaU6f_q8gW3xKzDIp6lYsyWlTVngpiMg_Jv23E31rC7dZBtN1Q67NVUjDaGURE7wfh0C946eQt4Y3k0KGG6vc-UTFBDE97Wr1EYX2RNeihVpFqQF2Lv87zLeIliFYlt225PLIqKdiwsgVW1rkkhg9rOXmGPNheHTAovaYA016SlFJ7Tp2OTJLpv_bY9p8xwnxOyJq3-De4PPe5ph9ZQjHDMnJH5NWb9XHnEJTvX5msk-35R_Z_g-5YqwbxLkJ60SdFVqWp3G9CfCmFC_lXy3QIyPMHyn3JrrQEWdKY9g1oaO5ZJPk-z=w1365-h943-no" width=600px/>

### Writer

```c
union semun{
        int val;
        struct semid_ds *buf;
        unsigned short *array;
};

int main(void) {
        int fd, i , n=0, semid;
        key_t key;
        char *addr;
        union semun arg;
        struct sembuf p_buf;

        key = ftok("key", 3);
        semid = semget(key, 1, 0600 | IPC_CREAT | IPC_EXCL);
        if(semid == -1){
                semid=semget(key, 1, 0600);
        }
        else {
                arg.val = 0;
                semctl(semid, 0, SETVAL, arg);
        }

        fd=open("data1", O_RDWR | O_CREAT, 0600);
        addr = mmap(NULL,512, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        ftruncate(fd, 512);

        for(i=0;i<3;i++){
                n = n+read(0, addr+n, 512);
                n++;
                *(addr+n)='\0';
                p_buf.sem_num=0;
                p_buf.sem_op=1;
                p_buf.sem_flg=0;
                semop(semid, &p_buf, 1);
        }

        exit(0);
}
```

### Reader

```c
union semun{
        int val;
        struct semid_ds *buf;
        unsigned short *array;
};

int main(void){
        int fd, i, n=0, semid;
        key_t key;
        char *addr;
        union semun arg;
        struct sembuf p_buf;

        key = ftok("key", 3);
        semid = semget(key, 1, 0600 | IPC_CREAT | IPC_EXCL);
        if(semid == -1){
                semid=semget(key, 1, 0600);
        }
        else{
                arg.val = 0;
                semctl(semid, 0, SETVAL, arg);
        }

        fd=open("data1", O_RDWR | O_CREAT, 0600);
        addr=mmap(NULL,512, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        ftruncate(fd, 512);

        for(i=0;i<3;i++){
                p_buf.sem_num = 0;
                p_buf.sem_op = -1;
                p_buf.sem_flg = 0;
                semop(semid, &p_buf, 1);

                write(1,addr+n,strlen(addr+n));
                n=n+strlen(addr+n)+1;
        }

        exit(0);
}
```

## Lab15-2
네 개의 프로세스가 동기화를 하며 자신의 프로세스 id를 5회 출력하는 프로그램을 작성 합니다. 이 프로 그램은 main() 함수의 arguments로 동기화에 참여하는 전체 프로세스 중 자신의 출력 순서를 입력 받습니다. 프로그램이 시작되면, 순서대로 자신의 프로세스 id를 출력 합니다. 동기화 작업은 semaphore를 사용하여 수 행 합니다. 

<img src="https://lh3.googleusercontent.com/WjXDxeg7DyGANgMHg1lTQTnGFrf7d8wCOWe8AmcrnTDlr4CCyPA3YOrKStLzFCOyy6IY0egjZDl-56dy0KGSPP1rTQZB_F7AGaHF_sxmKtnOfeu57GSzhcPFjce_TkQqYhwr7YQvIw11sHnHbPwyoB9t-Z818r6tZILaOtO2weZ_rDRhYlHZAk7NOSdOtC5-OOj_gcSJIrnu9fwjcvoRuDE6QlLt7iljvmqEsKaQdmlLj81LIm2vruV5qlTkXOp-1OfZSun5ljOQIYoMq0UUntp_BEz2LOcxymRw1_jigNvBiLjAMv31X_VqQF0ffpO1WFIZQaDw0Wq94mqDsZ-k-vmfOcYUJS8RTkLHm0KghxvemPPwHzvNbUV9Y0FyjowiAdQAP0nMDVaOByKm1idMPRfFESkBprnq07Zd1W_QarPKV-bz2rs9pmvE9R1ZbResKiPmwzCze0lx-Y9z2ipmKVawUhpg0zd92hDRbMiaVsTLha48s9mXxMQWkdQUuMmFUq-MrgFEL_rfIIkBIN6szdEVuOWSgd2Tw6NTXSl1WV8kB-FINc02G52ssgC0bJAnjh9xEsgxmuOgSHs_2UrhtMIh9xlGJDDsV4QLppOBzPlVDKy2eQ9fTqZREEOdGaKVbPXYEG1xKosMNOYsRxkOTfEWm_rFdA-uv92dgUnbbnqGdMrAZQ_-3HtiKdR6VmDFQv-hFfQrWZwxiyDRg1-Yoeqxuz8BMNylp2AnVhJEtcyGOE2J=w786-h943-no" width=600px />

### <첫번째 방법 : 여러개의 semaphore>

```c
union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
};

int main(int argc, char** argv) {
        ushort buf[3] = {0};
        int i, id, semid;
        union semun arg;
        struct sembuf p_buf;
        key_t key;

        id = atoi(argv[1]);
        key = ftok("key", 3);
        semid = semget(key, 3, 0600 | IPC_CREAT | IPC_EXCL);
        if(semid == -1){
                semid = semget(key, 3, 0);
        }
        else {
                arg.array = buf;
                semctl(semid, 1, SETALL, arg);
        }
        // wait
        if(id > 1){
                p_buf.sem_num=id-2;     // 0,1,2 semaphore에 대한 index
                p_buf.sem_op=-1;
                p_buf.sem_flg=0;        // 사용하지 않더라도 쓰레기값이 들어가서 잘못되는 경우가 있어 '0'으로 확실히 설정해 둘것!!!!
                semop(semid, &p_buf, 1);
        }
        for(i=0;i<5;i++){
                printf("%d\n", getpid());
                sleep(1);
        }
        // signal
        if(id < 4){
                p_buf.sem_num=id-1;
                p_buf.sem_op=1;
                p_buf.sem_flg=0;
                semop(semid, &p_buf, 1);
        }
        else {
                semctl(semid, 0, IPC_RMID, 0);
        }

        exit(0);
}
```

### <두번째 방법 : 하나의 semaphore>

```c
union semun {
        int val;
        struct semid_df *buf;
        ushort *array;
};

int main(int argc, char** argv){
        int i, id, semid, pid;
        union semun arg;
        struct sembuf p_buf;
        key_t key;

        id=atoi(argv[1]);
        key = ftok("key", 3);
        semid = semget(key, 1, 0600 | IPC_CREAT | IPC_EXCL);
        if(semid == -1){
                semid = semget(key, 1, 0);
        }
        else {
                arg.val = 0;
                semctl(semid, 0, SETVAL, arg);
        }

        if(id > 1){
                p_buf.sem_num = 0;
                p_buf.sem_op = -(id-1);
                p_buf.sem_flg = 0;
                semop(semid, &p_buf, 1);
        }
        pid=getpid();
        for(i=0;i<5;i++){
                sleep(1);
                printf("%d\n", pid);
        }
        if(id < 4){
                p_buf.sem_num = 0;
                p_buf.sem_op = id+1;
                p_buf.sem_flg = 0;
                semop(semid, &p_buf, 1);
        }

        exit(0);
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbODQ5ODYwMDhdfQ==
-->