# Lock
## t1.c

```c
int main(void) {
        int fd, i, num;

        fd=open("data1", O_RDWR | O_CREAT, 0600);

        for(i=0;i<10;i++){
                read(fd, &num, sizeof(int));
                num = num+10;
                sleep(1);
                lseek(fd, -sizeof(int), SEEK_CUR);
                write(fd, &num, sizeof(int));
                printf("%d\n", num);
        }

        return 0;
}
``` 

## t2.c

```c
int main(void){
        int fd, i, num;
        struct flock lock;

        fd=open("data1", O_RDWR | O_CREAT, 0600);

        lock.l_whence = SEEK_CUR;
        lock.l_len=sizeof(int);

        for(i=0;i<10;i++){
                lock.l_type=F_WRLCK;
                lock.l_start=0;
                fcntl(fd, F_SETLKW, &lock);

                read(fd, &num, sizeof(int));
                num = num + 10;
                sleep(1);
                lseek(fd, -sizeof(int), SEEK_CUR);
                write(fd, &num, sizeof(int));


                lock.l_type = F_UNLCK;
                lock.l_start = -sizeof(int); // 쓴만큼 풀어야 해서
                fcntl(fd, F_SETLK, &lock);
                printf("%d\n", num);
        }

        return 0;
} 
```
## Lab17-1.c
Parent process는 표준 입력으로 정수를 하나 입력 받아, “data" 파일에 쓴 후, 세 개의 child process들을 만듭니다. 각 child process는 "data" 파일의 정수 값을 읽고 5초간 기다렸다 +10 한 값을 씁니다. 세 child의 덧셈이 정확히 되도록 file locking을 써서 동기화를 합니다.

```c
void do_child(int fd) {
        int in;
        struct flock lock;

        // file locking
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = sizeof(int);
        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, 0, SEEK_SET);
        read(fd, &in, sizeof(int));
        sleep(5);

        in=in+10;
        lseek(fd, 0, SEEK_SET);
        write(fd, &in, sizeof(int));

        // file unlocing
        lock.l_type=F_UNLCK;
        //lock.l_start = -sizeof(int); //반복문이 아니라서 필요없음
        fcntl(fd, F_SETLK, &lock);

        exit(0);
}

int main(void) {
        int i, in, fd;
        pid_t pid;

        fd = open("data1", O_RDWR | O_CREAT, 0600);
        scanf("%d", &in);
        write(fd, &in, sizeof(int));

        for(i=0;i<3;i++){
                pid=fork();
                if(pid==0)
                        do_child(fd);
        }

        for(i=0;i<3;i++)
                wait(0);

        lseek(fd, 0, SEEK_SET);
        read(fd, &in, sizeof(int));
        printf("%d\n", in);

        exit(0);
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE0OTU5NDg2MTRdfQ==
-->