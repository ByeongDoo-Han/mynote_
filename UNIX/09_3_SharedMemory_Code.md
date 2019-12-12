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
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTY3NTczOTUzNV19
-->