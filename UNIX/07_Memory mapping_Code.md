# Memory mapping
## t1.c
mapping "data1" file, off = 4096 ~ +5만큼 +5
```c
#include "../Myheader.h"

int main(void) {
		int fd, i;
		char *addr;

		fd = open("data1", O_RDWR);
		addr = mmap(NULL, 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096);
		for(i=0;i<5;i++){
				printf("%c ...", *(addr+i));
				*(addr+i)=*(addr+i)+5;
				printf("%c\n", *(addr+i));
		}
		munmap(addr,10);
}
********OUTPUT*********
2 ...7
2 ...7
2 ...7
2 ...7
2 ...7
```

## t2.c
MAP_SHARED를 통한 parent, chlid memory 공유
```c
#include "../Myheader.h"
// MAP_SHARED를 통해 부모프로세스의 변경내용이 자식프로세스에 영향을 주고있다.
int main(void){
        int fd, i;
        char *addr;
        pid_t pid;

        fd=open("data2", O_RDWR);
        if(fork()==0){
                addr=mmap(NULL, 50, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                printf("%s\n", addr);
        }
        else {
                addr=mmap(NULL, 50, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                for(i=0;i<5;i++){
                        *(addr+i) = *(addr+i) + 1;
                }
        }

        wait(0);
        exit(0);
}
```

## t3.c
MAP_PRIVATE로 parent는 배열처럼 메모리 공간을 복사해서 사용(child와 file에는 영향X)
```c
#include "../Myheader.h"
// MAP_PRIVATE로 부모프로세스는 배열처럼 메모리 공간을 복사해서 사용한다.
// 자식프로세스와 file에는 영향이 없다.
int main(void){
        int fd, i;
        char *addr;
        pid_t pid;

        fd=open("data2", O_RDWR);
        if(fork() == 0){
                addr=mmap(NULL, 50, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                printf("%s\n", addr);
        }
        else {
                addr=mmap(NULL, 50, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
                for(i=0;i<5;i++){
                        *(addr+i) = *(addr+i) + 1;
                }
                printf("......\n%s\n", addr);
        }
        wait(0);
        exit(0);
}
```

## t4.c
새로운 file open시, truncate(or ftruncate) 필요 (BUS 오류)
```c
#include "../Myheader.h"

int main(void) {
        int fd, i;
        int *addr;
        pid_t pid;

        // 현재 data3가 없는 상황
        fd=open("data3", O_RDWR | O_CREAT, 0600); // newfile 했을 당시 filesize = 0
        if(fork()==0){
                addr=mmap(NULL, 50, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                printf("%d\n", *(addr+4));
        }
        else {
                addr=mmap(NULL, 50, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                // filesize = 0인데 50만큼 mapping할려고 하는 상황-> "버스오류"
                for(i=0;i<5;i++){
                        *(addr+i)=i;
                }
        }
        wait(0);
        exit(0);
}
```

## t5.c
ftruncate 예시
```c
#include "../Myheader.h"

int main(void){
        int fd, i;
        int *addr;
        pid_t pid;

        fd=open("data3", O_RDWR | O_CREAT, 0600);
        if(fork()==0){
                addr=mmap(NULL, 50, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                printf("%d\n", *(addr+4));
        }
        else {
                addr=mmap(NULL, 50, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                ftruncate(fd, sizeof(int)*5);
                for(i=0;i<5;i++){
                        *(addr+i) = (int)i;
                }
        }
        wait(0);
        exit(0);
}
```
## lab11-1
### Reader
"temp" 파일에 대한 scanf
```c
#include "../Myheader.h"

int main(void){
        int fd, i;
        int* addr;

        fd = open("temp", O_RDWR | O_CREAT, 0600);
        addr = mmap(NULL, sizeof(int)*10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        ftruncate(fd, sizeof(int)*10);
        for(i = 0; i < 10; i++){
                scanf("%d", addr+i);
        }

        exit(0);
}
```
### Writer
"temp" 파일에 대한 sleep(5) -> printf(\~5) -> sleep(5) -> printf(6\~10)
```c
#include "../Myheader.h"

int main(void){
        int fd, i, num;
        int * addr;

        fd = open("temp", O_RDWR | O_CREAT, 0600);
        addr = mmap(NULL, sizeof(int)*10, PROT_READ, MAP_SHARED, fd, 0);
        sleep(5);
        for(i = 0; i < 5; i++){
                printf("%d\n", *(addr+i));
        }
                sleep(5);
        for(i=5; i < 10; i++){
                printf("%d\n", *(addr+i));
        }

        exit(0);
}
```
## lab11-2
### Reader
"temp" 파일에 대한 read로 데이터 읽기
```c
#include "../Myheader.h"

int main(void) {
        int fd, i, len = 0;
        char *addr;

        fd = open("temp", O_RDWR | O_CREAT, 0600);
        addr = mmap(NULL, 512, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        ftruncate(fd, 512);
        for(i=0;i<3;i++){
                len = len+read(0, addr+len, 512);
                if(len>512)
                        break;
        }
        exit(0);
}
```
### Writer
"temp" 파일에 대한 write로 표준출력
```c
#include "../Myheader.h"

int main(void) {
        int fd, i, len = 0;
        char* addr;

        fd=open("temp", O_RDWR | O_CREAT, 0600);
        addr=mmap(NULL, 512, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        for(i=0;i<3;i++){
                sleep(3);
                len = len + write(1, addr+len, 512);
                write(1, "-------\n", 8);
                if(len > 512)
                        break;
        }
        exit(0);
}
```
## lab13.c
"temp" 파일에 대한  child process를 3개 생성하여 memory mapping방법을 통한 동기화하기
```c
#include "../Myheader.h"

void do_child(int i, int *turn){  // memory mapping 방법을 통한 동기화 해결
        int j, pid;

        if(i<2){
                do{ }while(i!=*turn); // busy waiting(이것을 통해 memory mapping을 통한 동기화는 효율적이지 못함.
        }
        pid = getpid();
        for(j=0;j<5;j++){
                printf("child %d .... \n", pid);
                sleep(1);
        }

        if(i>0)
                *turn=i-1;
        exit(0);
}

int main(void){
        int fd, i , *addr;
        pid_t pid;

        fd=open("temp", O_RDWR | O_CREAT, O_TRUNC, 0600);
        addr=mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        ftruncate(fd, sizeof(int));
        *addr = 2;

        for(i=0;i<3;i++){
                pid=fork();
                if(pid==0){
                        do_child(i, addr);
                }
        }

        for(i=0;i<3;i++){
                wait(0);
        }
        exit(0);
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTU3NjcyNzgyMV19
-->