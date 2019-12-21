# Pipe code

## lab12-1
> parent process는 세 개의 child process를 만들고, parent process에서 child process로의 단방향 통신이 가능 한 pipe를 만듭니다. parent process는 외부 입력으로 정수를 12개 입력 받아, 세 child process에게 순서대로 보냅니다. child process는 자신이 받은 정수를 자신의 프로세스 id와 함께 출력 합니다. parent process는 모든 정수를 전달 한 후 정수 -1을 전달하고, -1을 전달 받으면 child process는 종료 합니다. 모든 child process의 종료를 확인 한 후 parent process는 종료 합니다.

<img src="https://lh3.googleusercontent.com/UrTGH2h2FYW6L0QpTscaD7_p2Cl6TnnL34cc8-B9CerN5z1_RGbxKy-JRmC0fcwZDvN0D6Ne7LyB6tVhiuKt2vltHBJ291elDO6KcQRh9LNC1D0HYt5FIQTGTWPCYJiNmsQE75oaO8tYhEgzJcrS9iqK6KVgPD73eUSLdkyR1fHSytzaOXFKrRN6muUYxn0_H4Kld_av6uaXzbmqbvSAZe9I1ruPHi8nLXNY10fGtVvWhgfXXOL7Kv1C0I-ioTq9vvimvucqnh66h8QBpsFOy0V0Yyx66LzP2_o3FTtoA1av5P3yKJfOe7rP9onSmYWMmx8ItCGRs2EjE4ULSTwoVI9NlK8_mQ5eLXctqhazjUgZoqjoqjE4ZkMx4CI0vABgIgr5ZAwplnEJneY3p7bdbxlQPq12sWxYtPwJACk2PZs411ywvDGavRBnaFPf6WKpaQ5Malwy6_mSCeW3l45WRpBVB0d68tDlVsFfvDW95CWwFjUDMs8K0N1VpXmV-VlmcfKDgnZ0yL97dZbUkJeGwwfxyZAq511-Z1m3hmBBGGgCPZxNMaYNbLQm2lT-0GdAJy-8UqVfvZraEH3TijjKbjzkBt09TP5T4Einy1BSD9VBeKGodgVvyGqrBimVnCg9_mybyYjRy68gATTtN61IHguQ79WsT6d8P68GziN0fXhoP-5LEXE7-wGsVjM2oo9F4O3-kpeUm5jmhzzC0OHLQxwCGeMKhpAnK-MK7_WG24eTA0dj=w1396-h834-no" width=600px/>

```c
// lab12-1

void do_child(int p[3][2], int id){
        int in, i, pid;

        for(i=0;i<3;i++){
                close(p[i][1]);
                if(i!=id)
                        close(p[i][0]);
        }

        while(1){
                read(p[id][0], &in, sizeof(int));

                if(in == -1)
                        exit(0);
                else
                        printf("pid=%d, number=%d\n", getpid(), in);
        }
}

int main(){
        pid_t pid;
        int in, p[3][2];
        int i;

        for(i=0;i<3;i++)
                pipe(p[i]);

        for(i=0;i<3;i++){
                if(fork()==0)
                        do_child(p, i);
        }

        for(i=0;i<3; i++)
                close(p[i][0]);

        for(i =0;i<12; i++){
                scanf("%d", &in);
                write(p[i%3][1], &in, sizeof(int));
        }

        in=-1;
        for(i=0;i<3;i++)
                write(p[i][1], &in, sizeof(int));

        for(i=0;i<3;i++)
                wait(0);

        exit(0);
}
```

## lab12-2
> parent process는 세 개의 child process를 만들고, 모든 child process가 종료 한 후 종료 합니다. 각 child process는 자신의 순서가 될 때까지 대기 하였다가, 1초씩 쉬면서 (sleep (1); 사용) 자신의 process id(getpid(); 사용)를 5회 출력하는 작업을 한 후 종료 합니다. child process의 id 츨력 순서는 생성 순서의 역순이며, 이와 같은 순서 동기화 작업은 pipe를 이용하여 진행 합니다.

<img src="https://lh3.googleusercontent.com/CVn2Zopos-ql0h2HEiA17AuL6Lul-gRG00d0_sd-o1SBIKs1tVr5BHx_HiN7yMhLJoesPFWsfwMdy_bdaX8TltiNJY6bYQwG5KuP-h5a63F917Rx2ia0FKsJF9VruzAzFO-XiCkPEt9ZNssTN7yDWMGfWciHxphbYW8fYIJ7YI36YpfdxjCE0fitdrb-ZahDgfRSJ5XsAV7H4QVdI8a5jNfVgLtBGdbGrIlUVcdZzV_BD331CKcJnf4r2AZpcCVPBTTJdbg0hOLtYF53Rm8R6wFq7oZ1e0p8j8tD9ZXR9wAK3QpKq3Ym9pkBijOi_NkDp7zzZlEkt-5TlutSwEuBUhVvY1p0nbcqrHoMhl1N8W2THrvSkfVKWX6Q1OKBGNB7TqJj0LX5ygBqUEd2Hj7JrRqqRze05ik3JiQs6d6qVZUZlUxpGYqlijMcLRoEaDdHSPx6_o0IR8QaEXwNQXrkQgRVtWn1XUc6lpOXlr5GxfVvci-dAdjqHFhWjlVdOX8JKUZ1YpvnSD6iJ8YtoDJUR4UMyBveqaZCAuvSI8oJdvKsh_GrUTWF8XKFGOLBLqOVi_A_ZmihEN04IIAuIVK15a9zGoUDKkB5dKJWCOYTDa6caJpV84bSb9u1vg43Pu8uzNzoHPEflaI2meF9Q4vEzzF6Qz9Igd1nzGtpZGccWDOBko_zb8T-jb3OaZXZ9fU6NcIkN2rV8hJmTyjH34sZtSYf_P7PHzuGSTU_zBqoNuD7TZSI=w1222-h732-no" width=600px/>

```c
// lab12-2

void do_child(int p[2][2], int id){
        char buf='a';
        int i, pid = getpid();

        if(id<2)
                read(p[id][0], &buf, 1);

        for(i=0;i<5;i++){
                sleep(1);
                printf("pid=%ld\n", pid);
        }

        if(id>0)
                write(p[id-1][1], &buf, 1);
        exit(0);
}

int main(void){
        pid_t pid;
        int i, p[2][2];

        for(i=0;i<2;i++){
                pipe(p[i]);
        }

        for(i=0;i<3;i++){
                if(fork()==0)
                        do_child(p, i);
        }

        for(i=0;i<3;i++)
                wait(0);

        exit(0);
}
```

## select.c
```c
#include "../Myheader.h"

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

```

# FIFO
### Reader
```c
#include "../Myheader.h"

int main(void) {
        int fd, n;
        char buf[512];

        mkfifo("fifo", 0600);

        fd=open("fifo", O_RDWR);

        for(;;){
                n=read(fd, buf, 512);
                write(1, buf, n);
        }
}
```
### Writer
```c
#include "../Myheader.h"

int main(void) {
        int fd, j , nread;
        char buf[512];

        if((fd=open("fifo", O_WRONLY | O_NONBLOCK) < 0)) {
                printf("fifo open failed");
                exit(1);
        }

        for(j=0;j<3;j++){
                nread=read(0,buf,512);
                write(fd,buf,nread);
        }

        exit(0);
}
```
## lab13-1
> FIFO를 이용하여 통신하는 두 프로그램. 프로그램 A는 외부 입력으로 정수를 입력 받아 프로그램 B에게 전달하고, 프로그램 B는 전달받은 정수에 +8을 한 뒤 프로그램 A에 돌려줍니다.(-1 입력시 두프로그램 종료)
### Reader
```c
#include "../Myheader.h"

// lab13-1
// fifo-r

int main(void){
        char f[2][3]={"f1", "f2"};
        int i, in, fd[2];

        // "f1"과 "f2" open
        fd[0]=open(f[0], O_RDONLY);
        fd[1]=open(f[1], O_WRONLY);

        for(;;){
                // "f1"에서 정수 읽기
                read(fd[0], &in, sizeof(int));
                printf("%d\n", in);
                if(in==-1)
                        exit(0);
                in=in+8;
                // "f2"로 정수 보내기
                write(fd[1], &in, sizeof(int));
        }

        return 0;
}
```
### Writer
```c
#include "../Myheader.h"

// lab13
// p1-w

int main(void){
        char f[2][3] = {"f1", "f2"};
        int i, in, fd[2];

        // FIFO 2개 만들기
        for(i=0;i<2;i++)
                mkfifo(f[i], 0600);

        // "f1"과 "f2" open
        fd[0]=open(f[0], O_WRONLY);     // w->r
        fd[1]=open(f[1], O_RDONLY);     // w<-r

        for(;;){
                scanf("%d", &in);

                // "f1"으로 정수 보내기
                write(fd[0], &in, sizeof(int));

                if(in==-1)
                        exit(0);

                // "f2"에서 정수 읽기
                read(fd[1], &in, sizeof(int));
                printf("%d\n", in);
        }

        return 0;
}
```
## lab13-2
> Server는 세개의 client들과 데이터를 주고받기 위한 FIFO를 만들고, 각 client는 미리 정해진 이름의 FIFO로 접속하여, 표준 입력으로 입력된 정수를 Server에게 전송합니다. +8을한뒤 client에게 다시 돌려보내고 client는 돌려받은 정수를 표준 출력으로 출력. 입출력 반복은 5회 후 종료 client로 부터 입력을 blocking으로 기다리기 위해 select문장을 사용

<img src="https://lh3.googleusercontent.com/vXIVn5WKpfcLZ4ChsEj4Vy0ug2eylYTpIcSbDAeFNEB05i2ELIREx478HjRpKVUcj1t47DqGiAJ1jWOSu1a-NgNgNcrN3e1IcVjEZwZYI8Z9SJ5AJfHD6rW4esrmRfbDlfIHomHhpfqTHOBZRk8ZAEM4-z3nYI98BCY44xcQrxm-_Q3UmR5cqJPzNvtoyL_ywhiHYXAjKvHRjkhS0IILOIBIOWFsJLyDJfulFJ46wzvcgTMc7cNWAqsPutpg4n1SFinb1tS2pMjaUO-r-EXghBlXk4bi2ISPgU1Sw3uHG5uhLyme03TGNcGIr65g2Zw82Je94A1D6ECzMRJWgyMrOXRErg9LcCvNxHZSIzYPAI0Sy1aFX5weZcNjJaSShuHxi58LMCRaIvKMy-C1fzOwvytceCApYqQoIJS35TgmULK4Nw2LVoECFnhfIj4AcxSJQqH0v0j9WbqxOjwBAWv1Y2O4eT7bX3ojLq9wfW4MVc8lnleTFAAZduSNVSSjsadQayji4GNFnLZXoigRtHPuIFJkK0aAUQzHC4MsVY8_c6iKD_FablGW2nYd2frkUghMKlqtZuTv2H8BEEVMDz7P4tSRZZxorLBEYKpoB3Oie0yOHWm8tP7vxwq_qQKpsvd7-rkBK0adFEuCPccJQd8xD122FEmVIN0dzzaXRvJm36niCu1ANaV-fSZ1395sbd-JZ_hfe1Tot6QjqCK19ZTmFliIeYF7aEg-aEEUM59sRg6n_S1-=w992-h904-no" width=600px />

### Client
```c
#include "../Myheader.h"

// lab13-2

int main(int argc, char **argv){
        char f[6][3] = {"f1", "f2", "f3", "f4", "f5", "f6"};
        int i, in, k, fd[2];

        k = atoi(argv[1]);
        // 필요한 FIFO open
        fd[0]=open(f[k], O_WRONLY);
        fd[1]=open(f[k+3], O_RDONLY);

        for(i=0;i<5;i++){
                scanf("%d", &in);
                //정수보내기
                write(fd[1], &in, sizeof(int));
                //정수받기
                read(fd[0], &in, sizeof(int));
                printf("%d\n", in);
        }

        exit(0);
}
```
### Server
```c
#include "../Myheader.h"

// write에 신경 X, Read에만 신경씀!!
// Child process는 shell을 3개 켜서 실행
int main(void){
        char f[6][3]={"f1", "f2", "f3", "f4", "f5", "f6"};
        fd_set set, master;
        int i, j, k, in, fd[6];
        struct timeval t;

        for(i=0;i<6;i++){
                // FIFO 만들기
                mkfifo(f[i], 0600);
        }

        for(i=0;i<3;i++){
                // FIFO open                            file descriptor 번호
                fd[i]=open(f[i], O_RDONLY);     //S<-C  3 5 7
                fd[i+3]=open(f[i+3], O_WRONLY); //S->C  4 6 8
        }

        // master에 적절한 file descriptor 설정
        FD_ZERO(&master);
        for(i=0;i<3;i++)
                FD_SET(fd[i], &master);

        // timer 설정
        t.tv_sec = 20;
        // timer가 이상하게 동작할것이니깐 -> 제대로 동작할 수 있는 방향생각!
        // 자신의 child가 아니기때문에 종료조건에 대해서 생각을 해보아야함!!
        // timeout되는 경우에는 select의 return 값이 0이된다.
        // 여기서 >= 0을 할경우 잘못된 반복
        while (set=master, select(fd[2]+1, &set, NULL, NULL, &t) > 0){

                for(i=0;i<3;i++){
                        if(FD_ISSET(fd[i], &set)){// message 도착 확인 FD_ISSET의 return 값이 0이 아니면 데이터가 온것!
                                if((j=read(fd[i], &in, sizeof(int))) > 0){      // read의 return 값이 0이면 client의 종료를 의미! 
                                                                                // 1. read >=0 인경우 어떤문제가 발생할까? (기말문제)
                                        printf("여기지남");
                                        in = in + 8;
                                        write(fd[i+3], &in, sizeof(int));       // read의return값이 0인데 write하면 SIG PIPE신호를 보내고 종료함!
                                }
                                if(j==0)
                                        FD_CLR(fd[i], &master);                 // 2. 만약에 이문장을 빼면 어떤일이 발생할까? (기말문제!!)
                        }
                }
                // message 보내기
                t.tv_sec=20;    // 3. 만약에 이 timer를 지우면 무슨일이 발생할까? (기말문제!!)
                                // 4. timer를 사용하지 않고 client의 종료를 확인할 수 있는 다른방법!
        }
                // 추가로 필요한 코드는 적절한 위치에 추가...
        // timer의 동작 확인
        exit(0);
}
```

## lab13-3.c
> 네개의 프로세스가 동기화 하며 자신의 pid를 5회 출력하는 프로그램 작성, main 함수의 arguments로 동기화에 참여하는 전체 프로세스 중 자신의 출력 순서를 입력받음. 순서대로 pid를 출력 (FIFO사용)
```c
#include "../Myheader.h"

int main(int argc, char** argv){
        char f[3][3] = {"f1", "f2", "f3"};
        int fd[2];
        int i, id, pid, in;

        id = atoi(argv[1]);
        pid = getpid();

        // make fifo
        for(i=0;i<3;i++)
                mkfifo(f[i], 0600);

        // open fifo
        if(id > 1)
                fd[0] = open(f[id-2], O_RDONLY);
        if(id < 4)
                fd[1] = open(f[id-1], O_WRONLY);


        if(id > 1)
                read(fd[0], &in, sizeof(int));

        for(i=0;i<5;i++){
                printf("%ld\n", pid);
                sleep(1);
        }

        if(id < 4)
                write(fd[1], &id, sizeof(int));

        exit(0);
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTM4NjQxMTU0MywxOTkxMDEwODkxLC0xMT
IwNTgxMzk4LC0xNTEzNTExODAyXX0=
-->