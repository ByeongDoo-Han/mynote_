# Permission
#### rwx | rwx | rwx
User Group Others

ex) 0600 0700 0640

- **r**(4) : 디렉토리 내부를 읽을 수 있다.(ls 사용)
- **w**(2) : 다른 사용자가 내 디렉토리에 파일을 생성할 수 있다.
- **x**(1) : 접근. (cd 명령 사용)

### File IO
1. System call을 호출 했을 떄 User가 Os에 알려줘야 할 정보들(U -> O)
2. 작업을 끝나고 나서 OS가 User에 돌려주는 정보들(O -> U) ex) return값 = 주소
3. OS 내부에 어떤 변화? ex) 내부 자료구조 변화

- 고수준 파일 입출력
`fprintf`를 적다가 5번째에서 error가 발생했을 때 중간값을 확인해봐도 저장 내용이 없다 -> fprintf는 내부 buffer로 동작하기 때문에

- 저수준 파일 입출력
	**최소의 System call**을 사용할것 (Switching이 잦으면 비효율적)

> **고수준 저수준을 나누는 기준**은
>  HW에 얼마나 더 가까이 접근하는가로 나눈다.

#### file descriptor
- 현재 open된 file을 구분할 목적으로 UNIX가 붙여 놓은 번호
-  User마다 file table을 가지고 있다.
<img src="https://sites.cs.ucsb.edu/~rich/class/old.cs170/notes/FileSystem/filetable.rich.jpg" width="400px" height="400px" />

> **file table**
>  각 table은 index를 가지고 있음
>  0(표준 입력), 1(표준 출력), 2(표준 에러 출력)
>  3~> 사용가능

- 한 프로세스가 동시에 open할 수 있는 file의 개수에 제한이 있음. (Close를 사용하는 이유)

> ### !!! Header는 따로 외우는 것이 아니라 Header를 모아서 따로 저장해두자

### Open System call
```c
int open(const char *filename, int oflag, [mode_t mode])
```

=> 기존의 file을 open하거나, 새롭게 file을 생성하여 open하는 system call

인수
- filename : 파일이름
- oflag

| file을 access하는 방식 | file을 create하는 방식 |
|--|--|
| O_RDONLY | O_CREAT |
| O_WRONLY | O_EXCL  |
| O_RDWR | O_TRUNC |
| | O_APPEND |

> RDONLY + CREAT -> (X)
> WRONLY + CREAT -> (O)
 
##### * EXCL
 - file 존재(open fail)
 - 존재X (**CREAT와 같이 사용해서** open)
	```c
	fd = open("./test", O_WRONLY | O_CREAT | O_EXCL, 0644);
	```
##### * TRUNC 
- file 존재 (기존삭제=덮어씀 open)
- 존재X (새로 Create 후 open)

	> Open에 관해 잘정리된 사이트[http://forum.falinux.com/zbxe/?mid=C_LIB&page=5&document_srl=408448](http://forum.falinux.com/zbxe/?mid=C_LIB&page=5&document_srl=408448)


- mode: file을 새로 생성할때만 사용(Permission)
ex) 0600, 0664...

- return 값
	- 성공 -> file descriptor(>=3)(음이 아닌정수)
	- 실패 -> -1
	- 실패 이유)
		-  1. 너무 많은 open 
		-  2. CREAT + EXCL 
		-  3. Permission과 flag가 맞지 않음

### Creat System call
```c
int creat(const char *filename, mode_t mode);
```
=> file을 생성하여 open하거나, 기존 file을 open하는 system call

주의사항
1. WRONLY로 open한다.
2. file이 이미 존재하면, 두번째 인자 무시; 기존 file은 0으로 open한다(WRONLY + CREAT + TRUNC)

### Close System call
```c
int close(int filedes);
```

=> open된 file을 close할때 사용
process 종료 시 open된 file들은 자동으로 close But, 동시에 open할 수 있는 file 수 제한 때문에 close를 사용;

인수
- filedes : open된 file의 file descriptor
- return 값
	- 성공 -> 0
	- 실패 -> 1

### read System call
```c
ssize_t read(int filedes, void *buffer, size_t nbytes);
```

=> open된 file로부터 지정한 byte수 만큼의 data를 읽어 지정된 저장장소에 저장하는 명령
- file pointer(read-write pointer) : 읽혀질 다음 byte의 위치를 나타냄

> #### 현재의 file pointer에 각별히 주의하면서 사용!!

인수
- filedes : opened file descriptor
- *buffer : 읽은 data를 저장할 주소; data type은 상관 없음
- nbytes : 읽을 byte 수; data type과 상관 없이 지정된 byte 수만큼 읽음
- return 값
	- 성공 -> 실제 읽힌 byte 수
		- (return값 < nbytes) -> file에 남은 data가 nbytes보다 적음
		- (더이상 읽을게 없으면) -> 0
	- 실패 -> -1

```c
for(i=0;i<5;i++){
	n=read(0, buf, 512);
	write(fd, buf, n);
}
lseek(fd, 0, SEEK_SET);
n=read(fd, buf, 512);
write(1, buf, n);

******INPUT******
10
20
30
40
50
******OUTPUT******
10
20
30
40
50
```

### Write System call

=> 지정된 저장장소에서 지정한 byte수 만큼의 data를 읽어 open된 file에 쓰는 명령(file pointer | r/w pointer : 쓰여질 다음 byte의 위치)

```c
ssize_t write(int filedes, const void *buffer, size_t nbytes);
```

인수
- filedes : write할 file descriptor
- *buffer : write할 내용이 들어 있는 저장 장소의 주소;
- nbytes : write할 byte 수;
- return값
	-  성공 (보통)-> 쓰여진 byte수 (n)
	-  (return 값 < n) -> 쓰는 도중 media가 full
	- 실패(쓰기전에 꽉참)  -> -1

주의 사항
- 기존 file을 open system call로 open하고 바로 write를 하면?
	```c
	fd = open("data", O_WRONLY | O_APPEND);
	```
	-> open 하자마자, file pointer가 file의 끝으로 이동..

### Read/Write 효율성
- File을 copy하는 프로그램 실행 시간은
	-  BUF_SIZE가 **512(disk blocking factor)의 배수**일 때 효율적
	- System call의 수가 **적을수록** 효율적

1 Read -> 1 Write시)
1. [1문자씩 읽었을때]- block의 크기와 차이가 크면 빠르지만 block이랑 차이가 적으면 느림.
2. [1 block(512) 읽었을때] , 3. [1 block X (배수) 읽었을때] - 비슷

### lseek와 임의 접근
=> open된file내의 특정 위치로 file pointer를 이동하는 system call

```c
off_t lseek(int filedes, off_t offset, int whence);
```

인수
- filedes : open된 file의 file descriptor
- offset : whence에서 offset만큼 떨어진 위치로 이동(+/-로 방향)
- whence : whence에서 offset만큼 떨어진 위치로 이동
	- SEEK_SET : file 시작 지점
	- SEEK_CUR : 현재 file 위치
	- SEEK_END : file의 끝 지점
- return값
	- 성공 -> 이동한 file pointer
	- 실패 -> -1
		- 실패이유)
		1. filedes가 잘못됨
		2. whence를 잘못 명시
		3. 존재하지 않은 위치로 움직이라고 한 경우
			> <---- SEEK_SET -1로이동 :  음수값이면 움직이지 않고 -1 return
			> ----> SEEK_END +1이동 : file pointer를 움직이고 -1 return

### file의 제거
=> file 삭제(?) 명령
```c
#include <unistd.h>
int unlink(const char *filename);

#include <stdio.h>
int remove(const char *filename);
```
- return 값
	- 성공 -> 0
	- 실패 -> -1 (그런 이름의 file이 없을 경우)

#### unlink는 실제 file의 삭제보다는 file에 연결된 link 제거.

- **주의 사항**
	- include file이 다름
	- file descriptor이 아니라, **file이름**을 씀!!
	- **file이 open한 상태로 file 삭제 명령을 하면?**
		-> 읽기, 쓰기는 여전히 가능한 상태 but, file descriptor로 open한 상태여서 다쓰고 닫으면 file이 없음.

- **차이점**
unlink는 file만 삭제, remove는 file과 빈 directory도 삭제

### 표준 입력, 표준 출력, 표준 오류
- 표준 입력(keyboard) : fd=0
	-> 무조건 문자로 받아진다. (정수X, 실수X)
- 표준 출력(terminal) : fd=1
	-> 정수, 실수배열로 출력을 하면 binary data이기 때문에 정상출력이 되지않는다.
- 표준 오류(terminal) : fd=2
	-> console상 화면에 오류 표시

### Redirection
- `$ ./a.out < infile` : (infile이 fd=0이된다,)
- `$ ./a.out > outfile` : (outfile이 fd=1이된다.)
	outfile시 표준 오류는 저장하지 않음
- `$./a.out< infile > outfile`

### pipe
- `$ ./a.out1 | ./a.out2` : (a.out1의 표준 출력이 a.out2의 표준 입력으로)
- 두 프로그램이 동시 실행된다.
### 오류 메시지 출력
- `perror("error message...");` -> error message... : No such file or directory

# file
### 파일 정보의 획득
파일 관련 각종 정보를 알아볼 수 있는 System call
[https://downman.tistory.com/126](https://downman.tistory.com/126)
```c
int stat(const char* pathname, struct stat *buf);
int fstat(int filedes, struct stat *buf);
```
- 차이점
**stat** -> filename을 사용
**fstat** -> open된 file에 대해

- 공통점
file에 대한 정보를 저장(buf)

- buf에 채워지는 내용?
	- st_dev, st_ino : identifier (논리적 장치번호, inode번호)
	- st_mode : permission mode
	- st_nlink : link 수
	- st_uid, st_gid : user의 uid와 gid
	- st_rdev : file이 장치인 경우만 사용(UNIX는 file과 장치를 한꺼번에 사용한다.)
	- st_size : 논리적 크리
	- st_atime, st_mtime, st_ctime : file의 최근 access time, update time, stat 구조의 update time
	- st_blksize : I/O block 크기
	- st_blocks : 파일에 할당된 block의 수

### file mode
1. **numeric** : (0764)  read(4) + write(2) + execution(1) / owner:group:others
2. **symbol** : 0764 = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IROTH

> S_IRWXU = S_IRUSR | S_IWUSR | S_IXUSR로 묶을 수 있다.
### etc permission
[https://devanix.tistory.com/289](https://devanix.tistory.com/289)
- 04000 (S_ISUID) : 실행이 시작되면, 소유자의 uid가 euid가 된다.
- 02000 (S_ISGID) : 실행이 시작되면, 소유자의 gid가 egid가 된다.
	> uid, gid는 소유권자(파일생성자)와 관련
	> euid, egid는 사용자(파일실행자)와 관련있다.
- 다른사람이 내 file 실행한다는 조건
	- (711) uid = 나, euid = 다른사람
	- (4711) uid = 나, euid = 나
- 사용목적 
	-> data file에 중요한 data가 들어있어서 다른 사람이 cat, vi로 읽기, 쓰기 등을 막기 위해 data file에는 접근제한 하는 경우 ex) passwd file에 대한 permission
	
<img src ="https://lh3.googleusercontent.com/xiRgNbfzbl8qdNcNB_C--FNuo7Jtkl2GLzvroRMWppWhth0BfYAF8ZKXts016Il_U-Jf6QlaghKQV6D1v5gtbDthLOBhYMwsrfW7HX-PRcdwUEKhjTfw3UD_z4imA8v7c8d4nKwdoymDfh76uNQAgAwlXO8DafRp50-zRM0E30fAiG0qPQ5f07FOMQz3GBV-uYwCfDpiU1cJvz7aZ-wT78L4fCXbtH4IoSZXXxj3NtEFCungJmPdomZOKpEye15VeqCcvAU-ZffV35gjO0uYpJU3XvOoP1WGNL3UTbvLyXHVX5TNOy9GS9wEMUqLbn0cqnDcSuuE3onO4uysiL2Fc0HXPcxKRsRyNep4WOq1B7ktuaYTRJCvDQatxJawZO_MndUIVoE4WSauIlLUdpAemBaq6xRq_Ic1kre5AkPPt3uqBB_lF8gUM7uz6BLVFzOIu6drjpo7t0_RiZuKGIgaQMPmSwmtUSy7pkKVZoSCKnsxsX6iU71oErpCQJbkPdhC0m_PPtI76M8hBlOXzlLvozqXs2umJOgsKkf0tYAvTBGEn3CSWBFIuVGKw1mzMcxdK4_Lb16O7_pIoIfOfYUW8WTlrsu7lB8Y9bEi_KoWD08W4O2AQGJWcGxCaLnJ8LJrftLynULVGyolD1oID4NKkQiwcpUX-JUvpT_RT3OHjcAiTQlFKHy7C6mSMxBWyAqbAD74LVtp0w6XRiEhCcal7eiPzMChJWw0BDXduppllDgWwlhF=w699-h439-no" width="700px" heigh="500px"/>
	
### permission 확인
1. if(s.st_mode&s_IRUSR) -> printf("소유자 읽기 권한 설정!!!");
2. if(S_ISREG(s.st_mode)) -> printf("일반파일!!!");

### access System call
=> 특정 파일에 대한 읽기 / 쓰기 / 실행(+존재여부)이 가능한지 확인하는 System call

```c
#include <unistd.h>
int access(const char* pathname, int amode);
```

- 인수
	- amode : R_OK, W_OK, X_OK or F_OK
	- return
		- 성공 -> 0
		- 실패 -> -1
	- euid가 아니라 **uid**에 근거하여 process가 file에 접근 가능한지를 표현.

### chmod System call
=> 특정 파일의 access permission을 변경하는 System call

```c
#include <sys/types.h>
#include <sys/stat.h>
int chmod(const char* pathname, mode_t mode);
int fchmod(int fd, mode_t mode);
```

> **소유자만 사용가능하다.**

### link System call
=> 기존 파일에 새로운 이름을 부여하는 System call(hard link)
link count : link의 수

#### hardlink
```c
#include <unistd.h>
int link(const char* original_path, const char* new_path);
```

- return 값
	- 성공 -> 0
	- 실패 -> -1 (new path가 이미 존재하면)

- unlink의 재검토 
`(unlink("a.out");)`
	- 실제로는 link를 제거 -> link_count--;
	- 만약,  link_count가 0이되면 실제로 제거된다. (free block으로)

> file은 1개, 이름만 여러개

- link 확인법 : link 수, file 크기, 수정날짜

#### symbolic link
link의 제한점으로 사용
1. directory에 대한 link 생성불가
2. 다른 file system에 있는 file에 대해서는 link 생성 불가
-> (symbolic link자체가 file, 그 안에 다른 file에 대한 경로 수록)

```c
#include <unistd.h>
int symlink(const char* realname, const char* symname);
```

- return 값
	- 성공 -> 0
	- 실패 -> -1(symname이 이미 존재하면)

- 사용처
버전관리 어려움 -> symbolic link사용
1개의 main directory 생성과 다수의 branch directory로 운용

<img src="https://lh3.googleusercontent.com/qDQzoEZK495YTGJ2ZaMGcTs7RmIP11M8-LZPop3HwAP3QvFjyazDXoNll5ZusAVSzmWr_ibnGFjcQqLGvAY5kBrouVTOqlTVyg0fdWOjWULY3GGK9yKOHa2yrFTJWFdGpsPNkIwSRv5HHry39lov-qmUusxVyrMSke7mnagOoOY0a9nTGFO36_KT5_LcUImlmBOFbrIDtgBvdpLw_qTjVMfaYMcla7ijvUqtVDdaQzRWysqq88Rv6YwuRZ9Lh0m79_H95heVpRtAvrPjyqZeu90_n8iMCvNcMVS1j6IOEPechVR4hGxecoQfESeyS4VyxbcxyHgsawcrBY5ov67AxmP-RMRS8YF1Q0pJRqn4wXFO_D4KaDdgo9OqNHSu_snIjrfVzv1qYYgd3F65w_4OOdlZl1udlvTviN2XIZuXUs6rbucer3c56iVlDW0QWd4ZsLs2UtFQ2IjzWTZOS6Q__1XNQSTKjAy842yK1hR8cqquXHaVJL4E6Ov6dh37on67kVmqHw0gMrFO8lRbstu1ViJva84h9XO8vFkkxf_jqe0g1nJVSuuZt84ZtWTgqMB9okaLyaPRhvU0DwkAvcjvM2ECBoJKlfJILW-LmtxOOEfrPapilJ0QDc_9O5-HWkCWW216M9v-0miNltARri_WTWA8cHDTHmz8yBl-g9MikCk7HwYuSq2hXjE=w888-h660-no" width="600px" height="400px" />

> Q?
> open System call에 의해 open 가능?
> 가능하다면 어떤 file이 open 되는지?
> realname? or symname?

- symname안의 내용을 보고 싶으면 (즉, link중인 realname)
	```c
	#include <unistd.h>
	int readlink (const char* sympath, char* buffer, size_t buffsize)
	```

- symbolic link가 가리키는 파일이 아닌 symbolic link 자체의 파일정보 전달
	```c
	#include <sys/types.h>
	#include <sys/stat.h>
	int lstat (const char* linkname, struct stat *buf)
	```

> **mycopy 제출 feedback**
> #### 실행에 관해서
> 1. "a file"이 없는경우
> 2. "a"가 있는데 "b"가 없는경우
> 3. "a"가 있는데 "b"도 있는 경우
> 4. "a"에 0400으로 붙어있는경우 -> RDWR로 하면 안됨
> 5. "a"있고 "b"도 있는데 B가 0200 -> Write가 안되는 경우가 있음
> 6. 엄청나게 큰 file을 복사할려는 경우(Memory크기보다 더큰 file이면 동적할당도 무의미) -> RD WR를 단순 반복해야할것
> 7. 실행file을 copy 후 실행 해볼것(중간의 줄바꿈, NULL문자를 추가하는것 X)
> 8. 마지막으로 읽은 부분(Buf 크기보다 작음) -> 어떻게 처리해야할것인가? -> 정확히 A B file이 같기위해서는 남은 부분만 copy해야함.
> 9. O_CREAT option에 amode를 안해서 permission을 OS가 자동적으로 할당한 경우 -> 임의의 Permission때문에 RD WR가 되지 않는 경우가 생긴다.
> 
>  #### 효율성 문제
> 1. BUFFER SIZE 512, 1024로 하는 것을 권장
> 2. 불필요한 System Call 남용
> 3. O_CREAT option이 없는데도 amode를 사용하는경우 -> 불필요한 code

> **채점기준**
> 1. 실행이 되는가? -> 10개 이상의 case
> 

# directory
### directory 생성 및 제거
### directory create
=> directory file에 . 과 .\. 을 넣어서 생성;
```c
#include <sys/types.h>
#include <sys/stat.h>
int mkdir(const char* pathname, mode_t mode);
```

- **permission**

| P num | description |
|--|--|
| 4(READ) | 디렉토리를 읽을 수 있다. = 디렉토리에 담긴 정보를 확인 가능(ls) |
| 2(WRITE) | file 및 sub directory를 생성할 수 있다. |
| 1(ACCESS) | change directory, 해당 디렉토리로 이동할 수 있다. |

- **Read, Write는 Access가 선행되어야 한다.**

	1. 0600, 0400, 0200은 불가능 -> 접근이 전제가 되지 않아서
	2. 0500, 0300, 0100은 가능

<img src="https://lh3.googleusercontent.com/9YouVvUNkeN0-QIPGHSLsMJUnvWYYSs9pzb2l5SWXlAig8ZVYcGwFsRCVtZQg7vKVtLqWP26gCXsv88RLo0mWKcGvKvpKQPD4ecvOvRn2kIGJsNAoDuiZ2xSQGji-APLcij8BjiufkMYy1d0EJPPCCqRotrdYNH7v3IzF503fiyEY47HtKp1rUmYvtCWsZId9JiIlWpp3Sf1EQ3FMghbVft9-IugRtGCt9eNN9wvqF6kt13cfkNuipBK6NAGisxSNIfsrgAo_sH1sa1yUPsHFo2MP5Bnj9H02ABPW7ctbtBITlie4Al4QvJAM-CmozKX4Kbq_nAJDc0K-YfNO6mDXnVTj06lf5EMel5uDf1XRcPn1Qa8C-bHGOPXgAma2ridkbPxOFRZDzpfYOlDxMC42DULzF5ug-crF7ukrl8WyB2e992a3Qo2unM0D0pRpo5c-J_2RxzrvSZJ4iJRnArLDUmT7VWWkRguoj2ecwb2ltEi2tWbZPiRuzOJYoVtOZBXCj5h31eHXaHb58aM1wdVLVD6F-jss7S2ly0LgXMf3H_qFoK6hwA4ua_HD95Kjhp3HzhGsvgm6FBy5PbiS0kRDq7q00-7vzIBFTSDlD0DCzy9QFIFkQszvGuWX6FZrXWhKTXPKDQWCMmZwmHTFrt4OMxuPEGEW9HjCEGR8SVohfJJ4s0gaXNC94UKtKvtj-Jnhe_EfPaLr1c1IqXmXyALJYb9lKCIjZ_oNRB4jMJQxQcLH_1t=w467-h328-no" width =600px, height=400px />

### rename System call
```c
#include <stdio.h>
int rename (const char* oldpathname, const char* newpathname);
```
- file과 directory 둘다 rename 가능
- **주의** newpathname이 존재해도 -> -1 return하는게 아님! **기존 file을 제거하고 새 이름을 부여!!!**

### getcwd System call
```c
#include <unistd.h>
char* getcwd(char* name, size_t size);
```

- return값 
	- 성공 -> current working directory의 경로 이름
	- 실패 -> null pointer
- **size**는 실제 길이보다 **+1**이 커야한다.('\0' 포함)

### chdir System call
```c
#include <unistd.h>
int chdir(const char *path)
```

- ex)
	```c
	fd1=open("/usr/ben/abc", O_RDONLY);
	fd2=open("/usr/ben/xyz", O_RDWR);

	chdir("/usr/ben");
	fd1=open("abc", O_RDONLY);
	fd2=open("xyz", O_RDWR);
	```

- 주의 할 점
```
[username@sec LAB3]$./t6
/~~~~/LAB3/T1/T2/T3

[username@sec LAB3]$ shell상 현재 디렉터리 변경이 없음

=> Shell과 process(./t6)는 별개로 process를 만들어서 실행
```
### directory 열기와 닫기
### directory open
```c
#include <sys/types.h>
#include <dirent.h>
DIR* opendir(const char* dirname);
```

- return
	- 성공 -> DIR형의 data structure에 대한 pointer를 return;
	- 실패 -> null ptr

### directory close

```c
#include <dirent.h>
int closedir(DIR *dirptr);
```

#### inode와 dirptr
![](https://lh3.googleusercontent.com/s--fx8czGTv4mGKUdNQwTQwmpUvxWPtv7VQBLepzd4VjlrOwMsM5HT4HAuTA5L3l16aQ4ZGALTJq-Aa6QJam0iVVBk7ny2zfbfnuvg2u9wWZFDb7_a1vpnS9s2jrOdjhIe4OZJJ2R4SjCyX9suKpehsdZZzNwTSlmnWJ5kF2Y6C3wPEemOJ6bokPw3ZghtsmOP3CfEiPdWAMWz4c8nU5wKRoV1A_6lwkdl4daPDQryfqWjxGbH3fPCGd1cAnQbtJq5zpLoJvfa40Zjp9k-J3xbooQgYOdceIxNi-q7DRZDAgG6oHv-Sn6BT783WvivJWCQXxG6ApG7swKWt4zmlmT0otjrA_0cwg9PWCDFsAQT15hZVF_OvY9o6voSyymXio9g2YajzXaR_K42khdg4K7kneqenoA9nJRhtQcvhI17r8xWC2ZP5_osvs3pnc20boHPYZHay8KnS5BnQHVLCA-oFtE45MHODJVzeuIU0Q3rjGcwicVxjIEYovPRobKgcWylXuGFuyBOvLZrqDmNco6b0TUINPSZEYO78GrUNCqZpPAz7xcc-9fvniO4cuWALEd3fcf9pmS24IYPuUjxofcGoZdBEwqc-meYQmx7sb3ixDbo9NAMLASANNQojCFnPEliQNv4p28BhTZd2M6AHPkd9jEu6zKnZ2Rz8riV83TWmWP1cUEafdqgYBBrFuOhkUzRfFT0lMrhl2-qYtL4jgh0g_gLTjniZArzWvulJLHwpa4XWh=w660-h346-no)

- DIR* : 현재dir, 부모dir, file, subdir 들에 대한 inode를 가리킴
- inode table은 OS에 저장되어있다.

> opendir 후 `lseek`는 사용되지 않음!

### directory 읽기
### directory read

```c
#include <sys/types.h>
#include <dirent.h>
struct dirent* readdir(DIR *dirptr);
```

- return : dirptr이 가리키는 DIR 구조내의 한 항
- 	```c
	struct dirent {
		ino_t d_ino;	//inode number
		off_t d_off;	//offset
		unsigned short d_reclen;	//d_name 길이
		char d_name[NAME_MAX + 1];
	}
	```
- **pointer dirptr은 read 후 다음 항을 가리킨다.**
- directory의 끝에 도달하면 null pointer를 return

#### directory file pointer의 이동
```c
#include <sys/types.h>
#include <dirent.h>
void rewinddir(DIR *dirptr);
```
### dircectory tree의 산책
<img src="https://lh3.googleusercontent.com/Nki-qkJlhH2qkEM85X_TuAj_ky5DPOKF6Y9wVSiaDWuODcjncFzj5NaONlYJdlMk01utnbP14Ma_0K8BmhtyvYbNFKqF37x8Oy4tQNavCTinWDaqJCEEuypx11InvcK-u_0a8W0cAJmKgAPhFeJF6sKHaegXPuB5PFFBxGnX6g8m6KCei64un92Cc6wp5jOMSYRrz8Xfg_I2Y-9r7p2z9cR4wm2qAsgrvjMAxhCfTMso4QgkPDAzScttBR9-E0Ifdl1853dV5Vpt-8ixh_vY2RLHEDUzq6WoEAlim5lNdtJERYtYmK-A0v3mMgPbe5npAXgcXrwD-bux0vfSl5iAFbgkG9dp9RVYzTa5Ea1dgesSsFBMKcLSwEmshO4S4_dH9DB3G_eFJqJkJLVBbXS9mGajaVHw1-V_V2ZfuKxbo0u1133MjdFyfMNSRzunu-e-lqJg_R9ei7w83t2DDneXA5xvfHpPm3Pj9W0GnDbkKlj8NWYH_xDHw7qdManxWea7qLelDCoFNLMnQ9FtWqW-gEv3BnfPRn7sIZLoG1DMEIIs8Lxv4rGh1qGmSUTyuu4ZGMW475kafJFixUObwUh74yMdlIrhlAYCf-pcxMX5_pO6OZP0mIlf5KplCAtJFQQ5UqimZ7rsqi6iJY7KoxhqC087ZBVDHkc0RJLZlDifDb4vugmNRkaPNRHQh4sK0ISv2NHJ3pPHJiO42jd1Gw5FIpKGP_g8B7xfvOzrd9r5Ve-09d7u=w643-h547-no" width=600px/>

### ftw()사용법
```c
#include <ftw.h>
int ftw(const char *path, int(*func)(), int depth);
```

- 인자
	- path : 어디서 시작?
	- depth : 한번 open하는 file descriptor의 수
	- func : 무슨작업?

- path에서 시작해서 recursive하게 subdirectory와 file들에 func() 함수를 적용;

### func() 사용법
```c
int func(const char *name, const struct stat *sptr, int type){}
```

- 인자
	- name : target object의 이름
	- sptr : object에 대한 stat 명령어의 자료가 저장된 곳에 대한 pointer;
	- type
		- FTW_F -> object가 file
		- FTW_D -> object가 directory
		- FTW_DNR -> object가 읽을 수 없는 directory(read permission 없음)
		- FTW_NS -> stat을 적용하여 정보를 얻을 수 없는 object(sptr = NULL)
		> DNR과 NS는 오류를 막기위해 사용

- return값
정상적인 작동은 **return 0**
0이외의 값은 **중간에 종료**

<img src="https://lh3.googleusercontent.com/dg73O1U2enHyMtY5EY--qqYTGQcjOz2h0wzrHKnVd6da5KJb1p3--IL_28Ncc_wMN3lil-pcemCKzbpfHrTC_BfNIXlOsmDumZayOyntwdipuIShYVFdK0RwzKLppI9pgAFVsL55hXnDRZYzD8676M6BKiQvg2ThStq88n3WaLvbcNNOacgpcWrFetDkwn1Bp025sWbZ7n4g4nBz7cMAh9JaRZ-HhCc9a3P-RNisJKbVpVDITIdkvnUBO0fkZ2bou8pKk0wvm8NG7mV2-G4jIz7PAomi_dDKlUK22454SjW890SDVoMPRkH-h56seeNLYayGr2Fdonu2H4khlrZ5Gr_KGDGVy7ES6r1yI6kFk7dtv3TkOY89a5ZWX6SxTPlvWQaLF6dqvrPe3rNgpQRRJ_GqEUXNCuBDnJtOV8NLwrENQHVncTsXGHIteuquxjgLROb3XwXKHHoQV76mnLi9M7xfWq9T5RrpX3HUHg2qGTF48GJgX24ijNXeyFU_oyilUt3_yXHQouqOAVWlXWDH3jFWnpsBFNsJKr5XACKuiM7IT_1aWovodPsgwJug0GzidTqbiyrOQqwYIMYnYkVyAVrP_yRDnxDGA-g6B6aSMfYZS8yPooKv1Z02H0_BptZYXwEmL1lySbhvyW3gZLKxPgD-VLfz_3RrZ7PG4MX0oZcaC0I4jFrIZcaGw9VGVtQD2TH4rGlNWETqfJX3IhyiXiPyXtiFPiPz22Praj5qS26xFUHX=w1215-h865-no" />

ftw 예시 code
```c
#include <sys/stat.h>
#include <ftw.h>
int list(const char *name, const struct stat *status, int type) {
	if(type==FTW_NS)
		return 0;
	if(type==FTW_F)
		printf("%-30s\t0%3o\n", name, status->st_mode&0777);
	else
		printf("%-30s*\t0%3o\n", name, status->st_mode&0777);
	return 0;
}
int main() {
	ftw(".", list, 1);
	
	return 0;
}
```
# 시스템 정보
### uid 검색
```c
#include <sys/types.h>
#include <unistd.h>
uid_t getuid(void);
uid_t geteuid(void);
```

### guid 검색
```c
#include <sys/types.h>
#include <unistd.h>
gid_t getgid(void);
gid_t getegid(void);
```



# Process
- **process** : 프로그램 코드, 변수, 스택 등에 저장된 값, PCB 내용 등을 포함

- 계층구조 : parent process -> child process(UNIX system의 모든 process는 init의 descendent process)

- Shell 상에서 process 목록 확인 -> ```$ps```
- 실행 중인 프로세스 종료 시키기 -> ```$kill -9 프로세스 번호```

### process identifier

=> 음이 아닌 정수
- 0 : swapper
- 1 : init

```c
#include <unistd.h>
pid_t getpid(void);	// process id
pid_t getppid(void);	// parent process id
```

### process Group

process group
: 프로세스 들을 하나로 묶어서 하나의 group을 만듦
: 같은 group에 속한 process들에게 동시에 signal을 보낼 수 있음
: 초기에 **fork**나 **exec**에 의해 group id 계승

group leader
: 자신의 pid가 group id이면, group의 leader

- group id 검색 System call
	```c
	#include <sys/types.h>
	#include <unistd.h>
	pid_t getpgrp(void);
	pid_t getpgid(pid_t pid);
	```
	=> getpgid의 인자가 **0**이면 호출 프로세스 자신의 group id를 검색 

- process group 변경
	```c
	#include <sys/types.h>
	#include <unistd.h>
	int setpgid(pid_t pid, pid_t pgid);	// 두개의 인자가 동일(group 변경시 leader group으로만 변경가능
	```
	=> pid인 프로세스의 group id를 pgid로 변경

# Session
- 한 session은 한 단말기를 사용하는 foreground process group과 background process group의 집합체(**하나의 창**)
- 각 process group은 하나의 session에 속함

### getsid() System call
:  session id를 획득
```c
#include <sys/types.h>
#include <unistd.h>
pid_t getsid(pid_t pid);
```
### setsid() System call
 : session이 종료되어도 죽지 않는 process를 만듦
```c
#include <sys/types.h>
#include <unistd.h>
pid_t setsid(void);
```

- 제어 단말기를 갖지 않는 새로운 session과 group 생성;
- 호출 프로세스의 id가 session과 group의 id가 된다.
- 만약, 호출 process가 현재 group의 leader이면 -1를 return
```c
int main() {
  pid_t pid;
  int p[2];
  char c='?';

  if (pipe(p) != 0)
    perror("pipe() error");
  else
    if ((pid = fork()) == 0) {
      printf("child's process group id is %d, process id is %d\n", (int) getpgrp(), (int)getpid());
      write(p[1], &c, 1);
      setsid();
      printf("child's process group id is now %d, process id is now %d\n", (int) getpgrp(), (int)getpid());
      exit(0);
    }
    else {
      printf("parent's process group id is %d, process id is %d\n", (int) getpgrp(), (int) getpid());
      read(p[0], &c, 1);
      sleep(5);
    }
}

**********OUTPUT*************
parent's process group id is 25373, process id is 25373
child's process group id is 25373, process id is 25374
child's process group id is now 25374, process id is now 25374
```
<img src="https://lh3.googleusercontent.com/eNGHn1_qZAQuXU2EmZc4wMvNNDl10IKriF1uojz1Ou-VtbAAmwKe08wZLgYWMQbzHtI4uwXoVoN8VCRnfHpO8crsiiD4TnHyjjT8buKHt7g4c2YbsLTJU7lexrw5xxB4NQM1hr4s7AO9a2_BMAKmSnXUd9gLbNt8mk_WMhkFAfYqAhPZSfUd3fL1usy-cuLmy9Ej9qBssp6o13TYCqx0c3tz27D8DKQMtJFKQEUIbTe4bRCMOIMYg9Jj8XiSb22QlRyJVPuJdfM2rFsU2hG6A8FxJLSFtnMQKI6gHU1cz4tghGZEJkAN49pJICRfdQwDKt6Gkzg06035Ig8M0QeOXDAGrg2SgIHtuzkPuD8-Rt8SkcF1-gWuduvV8ywf-lBA5QXi5GkNrB-An3aygM8svrLL2uSsq1gbqLrhyInoxqf29gCQybpN_hnVk9vswus93kg8CtzwyYD5cLnVK8iug5PStOJghKavPzpCLPGIYFMG8x1DYiajI0D7EPXEzE-FEsVPNd9pA6MgVyThuBx408n1llEQZdqEa1RsV_yqdibBA32dn2rhIpauJlSn72cyX92IX8dD1K7I9Pj9zLh8XXEKYg6TNIQaQlN6whSYkQP9ZqKeRFmU7Gkw6j1zlYVbNlQB1T0YkJY4HjnJkJDuOD9TAwrdRke0X8m3vQeG3HD-uFWe-IR_zHQ=w977-h943-no" width=500px />

> #### process와 program의 관계
>  1대 1의 관계가 아니다!
>  program 내의 여러 개의 process 생성 가능.

### main() 함수 인자 사용
```c
int main(int argc, char **argv){
	int i;
	printf("%d\n", argc);
	i = 0;
	while(argv[i]!=NULL){
		printf("%s\n", argv[i]);
		i++;
	}
	return 0;
}
```

`$ ./a.out abc def ghi [enter key]` 인 경우

**argc**(문자열 몇개? 실행시킨 프로그램 포함) = 4;
**argv**(포인트 배열) = {"a.out", "abc", "def", "ghi};

사용목적? child process로 parent process가 인자 전달이 필요한 경우

## Process 생성
### fork System call
: 수행되던 process(parent)의 복사본 process(child) 생성
```c
#include <sys/types.h>
#include <unistd.h>
pid_t fork(void);
```

- stack, PCB까지 똑같이 생성
- fork() return 받은 다음 문장부터 동시에 실행
<img src="https://lh3.googleusercontent.com/FT-_X8vciOh9mZJAeANP45ZJ0lWmWb8p6pkM_TkwHaSEGFMGhgeCRtUGmCSpshAHB_yUhzzgVfhISc81qrr2UOib79yV8fglsiZogisJ-3qncD6Vk8O1-f_cYVD_6BzOHJmk5GCTCkvC9YDm7-TWOK_KF5vzzABI96fOHXsJBRZu63CjXH0w2LUNEHgprtKwu3SXcss85srn345bq91MtFxNtCUBM1f_Ez3GGY3jxdm_l7RdjcMyFdP9YXaT3W508tLzisIMErhsHZCJaUC2cviBc8yiPS83dIyIGoA0BQrSIsGenM7NpkAmmI4yI5Bf5cspePsGCUpyo0jcpu_mCOEGQYwBQ6zAHkhiXiP-X3vyrc00ov3Li3pHzcj2KVXhxMOzZmsBsGjA3JORGZtL_a-o0ZMUyohj29FBi0Rdw2SjBbvRDbs0DL-6T4YIBJp6DvjPJqOFI0sf9XIjkJC6_rA74FJ0YTcLwTJiyIsf0_rqYTw1QVtFbvHwjpgCagpxrZflFBz6ROMt7e8PSRdqqJkF0rPu1OtIrRrFz5oey0lsz1t8v1lPvEP1z3axmaA2BeG-QVBmhoNohAqqpWbR7yCqGvCRVyJRchcPhNtdSZMcNHkcV7emueyyjNr5I7MorY9ifMLrVsONtGWTQ7D5tfMIQdHI1cVEgiSFpDvVuVSinV1B373RbPz4DgBS7NhCnIXNLpcEc66lIFN05iWpcGGgRXF0pZXgFAHMqaZ7At4bHmAC=w875-h652-no" width=600px/>
- 두 process의 차이점
	- pid와 ppid가 다르다.
	- fork()의 return 값이 다르다.
		- parent process의 return -> child process의 pid(0보다 큰 정수)
		- child process의 return -> 0
- fork 실패시 -1 return
	- 실패 원인 :
		- 시스템 전체 process의 수 제한
		- 한 process가 생성할 수 있는 process 수 제한

- 예제 code)
	```c
	int main(void){
		pid_t pid;

		printf("Calling fork..\n");
		pid=fork();

		if(pid==0)
			printf("I am the child\n");
		else if(pid>0)
			printf("I am the parent, child has pid %d\n", pid);
		else
			printf("Fork returned error code\n");
		return 0;
	}
	```

#### fork : 파일과 자료
 - child process는 parent의 복제
     - 모든 변수 값이 그대로 복제된다.
     - fork()후에 변경된 값은 복제 되지 않는다.
     - **file descriptor**도 복제 된다.
         -> parent process가 open한 file pointer도 child process도 똑같이 복제(parent : A file 5번째 3글자, child : A file 5번째 3글자)
     - parent와 child가 file을 공통으로 사용가능
 <img src="https://lh3.googleusercontent.com/kUuL2GuxZYSnNEMYEy7j30ionMoL2dmhchfKKCQS6ykCRU_QMnoY3E_NxzoTq2_JxO3gv5XWPQ4_mykaEKqtLj4p99mlMg6vwEe_S7zBLB4SoKBGWXv3kBuOqRGO5h7Ze3siq8XRWqWq-rNV-DJNO-GU3oFySMU6t-DPrdYn2YggPTN3QMbdZ82Uckz7UUMyUS_5QEaTKciO9oKSPjlO_y8_v5BqrkLQsMyLPkk0xXpModEuTcCTIrWwhhP8XiDMFIMgVbRgzaFXfWsuPjUqgr2HkwLGdOmfKlZXWdF432iDX4S7VwXjts-dAeB0LtTdDNU2iCKtrdAMXN4X5y67tvsimh7w5PYZSpwGw46_u48vfuypGddHooOXvMnw89PInxdULpE1Lg7PGs4ZDQR2yDHjkwhlf6IkuqqF5AXVkMlYuY2P5MxuaZRETvEp1Xh6fM3W8ehZ95h11DEjLYGplISVXDZKOr7dwDsng7JKHKu3azixop-L9P2klvPkYLzcprr9ci8NGx4B-E45lzxrsDHG7KoSAnkxZX6A-ZhckHLSWVgV1FN50hVgMxz0UbK7gK_8aaz9LeDj0WReuVY_1HIOI-n5rmfaonhLHk4i7kjyd3NGy0OsQApIVGbhvfjaTpRlxgPAl-wH-YmLWETFtbA60B167wetUwWHhCh-KwjqNwF19DpbUfqdB3VaG3yg-uZNgHBvKS4RYoOKCD457iMCEzur-QJHzrEhOTQWVXR8wDgp=w1023-h653-no" width=500px/>
 - 예제 code)
	```c
	 #include <unistd.h>
	 #include <fcntl.h>
	 main(void) {
	     int fd;
	     pid_t pid;
	     char buf[10];
	      
	     fd=open("data", O_RDONLY);
	     read(fd, buf, 10);
	     printf("before fork:%ld\n", lseek(fd, (off_t)0, SEEK_SET);
	      
	     switch(pid=fork()){
	         case -1: 
	                perror("fork failed\n");
	                exit(1);
	                break;
	         case 0:
	                printf("child before read: %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
	                read(fd, buf, 10);
	                printf("child after read: %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
	                break;
	         default:
	                wait((int*) 0);
	                printf("parent after wait: %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
	            }
	 }
	  실행 결과:
		  before fork:	0
		  child befor read:	0
		  child after read:	10
		  parent after wait:	10
	```

> #### parent code에 wait()가 없을 시
>  <img src="https://lh3.googleusercontent.com/nqrsRXd6gGcI7C7wm64bA8d6WC930anAd57L4Pjy0ICL13SPgamOdjOTkm0E4bDT4oZioX8VD-YKiGtx2B7C-wYWsoyQ2kb6vVBPaGTT2I23szP-xcs11IUhnD_feO616d4VhBrg0GBmu1t5xSuNX0pbbW1HSrv0h3A7zBn3cQMrZo1qsyDoae3WLzY2huFxylqqCeyO7R0sYtUYXZhH1Jnp8VkTYfXUxHmWXVGQaPxVF8snvjUZYgUjjIPJHb8_95M69czh6AMC2sBv7CyDmjfz6Bx3V_VhnLfPHGr0eg_LV2Ruli0KIIMcihZUS3ScxwUnMwMEjk5VpN8rqMB8sX8PrPXDcNfo7NAd1bTOMf7LX_fNYzijH13ecy--QLfg4c902oaIkarWpuTtAHOqKXZwejO1Kd0jpaLnv_IeauGhqrdnYIGR-KSGy4ItqKp4t_acTpnpeqwb40Y-3g0mInriQEl6MD1j8IWIP02bPhvSdfqZNxzAyqlTRGwDpeHmySizbU8r179YkJB2pXrCt2eIS7k3uGcy5q0KB5z-OxVXvA2ZqLntHtDAD-pOFp5RQJpBFraK_0QdIS1gDmxhlBkD1tfDW8BBK10pvTk3FJb1_0nbdYCl5kFOUdR-Otvsh44hINcelCxSMg4aNOUvCY7XapIJrZjqiUGfx7sDVzclnDfR8Lap8k17LXTiyp_HX_ISccEHwiNuZUllr5F47MRqK30HJ0_1iTGFRLUXkzckd6BJ=w1003-h637-no" width=700px/>

> #### child의 ppid가 1인 경우?
> -> 실행도중 parent process가 없어짐
> -> 고아 process를 가져다가 pid=1인 process(init)의 child로 만듦

> #### 잘못된 반복문으로 자식 프로세스 생성시
>  ```c
>  int main(int argc, char** argv){
>	int i, N;
>	pid_t pid;
>
>	N=atoi(argv[1]);
>	printf("pid=%ld ... ppid=%ld\n", getpid(), getpgrp());
>	for(i=0;i<N;i++){
>		pid=fork();
>		if(pid==0){
>			printf("pid=%ld ... ppid=%ld\n", getpid(), getpgrp());
>		}
>	}
>	for(i=0;i<N;i++){
>		if(pid>0) wait(0);
>	}
>	return 0;
>}
>```
> 3개의 자식 프로세스를 생성하고 싶지만 7개의 자식 프로세스가 생기는 이유? 
>
>이렇게 코드를 짜면 다음과 같이 생성
> <img src="https://lh3.googleusercontent.com/ERRhNiNdrN0VARMFpkILk0XsWXf8egQZ1KPBgYKX3o_PvQSsUykFlAFIyp4wopK6Dfr4mYrVpM0iEBtuBwBbc5UKXTD78cvNxtR8VNHh270_JnmUpiepRnLZBPAw89f4zLwI14DPYOQH9U0N7hqIFEsGGYROpwolkUUiCzds68dhJABnBeJmusPA_ZoeAheQs0HyZxuSHOyKpwkoC-j5GE5KucK0vm3eCQl40EgNF0R8oQXHIypvqiQTrkOqKIXPK23_a9r1sjGMBGLCW98YCr78v3z2bPDAPjC57t0PJpgfL_lBsS36iNG0IyzYEfIdESHQtZzsqNSiza5chw_AvPSZQXEcFyz4aqNlmg6xfpxTEnW77Vz-Q4lckeshxLnAUrdV9NyummZSkDsmEqMqSttMdCE7O8L5Sm6DRusj0BqUIx-eXBBqTGx6dfIx-9iaWn7qCta5u3C6a5NjFng-pppzb4nI5L5PE425gH1KV_a8yF8Zn17ag571eTAQusARxVonvx6vscgHNvh7TfyWe7Q7UetaMWktehYAg3UOrpPxHB-MZ2jgnnKL9ZS2iglQpbhhTHLbyM55Rb6CaNrvX3cxsObhF4p8hwt9R4xiGhJGHOsTMZnlQAn5Gfxsksz8ekc52pzaOimBgFmuBcz_po4gOc1MEgQ5xElqbMwkxctVyQ7UbDoWpoQXdAkhqhFrCNSbRkFI-LW1W49SXtTPbGxuU8pUkAaiQxW0SWx-3wCO3gqK=w385-h346-no" width=400px />

### exit System call
: exit: process 정지-> open된 file 닫기 -> clean-up-action
```c
#include <stdlib.h>
void exit(int status);
```
- status의 값?
프로세스 종료후, `$ echo $?`명령에 의해 알아낼 수 있다.

- clean-up-action 지정
	```c
	#include <stdlib.h>
	int atexit(void (*func) (void));
	```
	- 지정된 순서의 역순으로 실행;
	- ex) 종료할 때 공유자원에 대한 사용 해제

> **Parent process는 child의 exit을 확인할 의무가 있다.** -> 안할 시 **좀비 process**

### exec System call
: 지금 실행하고 있는 process가 현재 process를 버리고 새로운 process를 실행
```c
#include <unistd.h>

int execl(const char *path, const char *arg0, ..., const char *argn, (char*) 0);

int execlp(const char *file, const char *arg0, ..., const char *argn, (char *)0);

int execv(const char *path, char *const argv[]);

int execvp(const char *file, char *const argv[]);
```
- 공통점:
	- 호출 프로세스의 주기억장치 공간에 새로운 프로그램을 적재;
	-> 호출 프로세스는 **no longer exists**;
	-> 새 process는 **처음부터 실행**;
	-> 새 process는 호출 프로세스의 id로 실행(**pid 그대로**)
	- 실패 시 -1 return; 성공시 return 없음;
	- fork와의 차이점 : 기존 프로세스와 병령 수행이 아님
	<img src="https://lh3.googleusercontent.com/kggN3J3q3cUgHcj0OBWAJZcPn8zXXgtEaDNmpEtea9IGmP6FUVSmHJ5ZXi7JrcsdJr7y1YKWJ4cguANl_6S6tc5uPDECTU0h4BGVpVdViAGr1uZKqzQbGV0tnPUU7QE9K24e1L5R1cJHM0Mx4CneuryhEc3mIV2Tx3ivmXGuv_6el44tAccFiaPXKj8yuBtxv62pxtoH5uN3G6P4wW_QDK1HUOQ0ThRYLWQnrfCkgnsU36CK96wqtusixuUekBzY1UUZQ_F91kPLOo4GamUwd-rxwLv5AgqkFom-m5RgVfvs9pQiaaKr1AqhNV6bxSSMbMvvkGrumHxmQWYr2HJa-kI6UX_tOywN8hmBG7vxlFwbtKKd3psYpmxNP_VPO6Z1yv9SacUFUUpjtxPGPqhZ3qUoy9Kyyz60J8OkKZBy8tg8zmW2vpzcvVqDr5wOErCm46nLZmONxAgsHgxsiIJsxTgNLTV_XM95c8dEEKrH0YmcPiJRNeEHkyZYOe16wVEpjayiKb1a3j3zz3oPEWRnAWo8wcWo4637NUP1OsNvw4XlXNWfRlFbQR9l0r9hyMTIH033NgYTyQafoFW_q9cLFJpbCv10ck55oIssk79YPOFCY1XcEG5r3_HZq__Q3qz18nGYmhWrqpM4BfpfYXCJu0zQoiaYtePDZmtIRs269HOoyTebGaTWdYtWzCkLkaWXE2qFC5oLsXp-1IPud8LaB3BHfmWUl2G5Rj8xp6egzy9K_mv-=w475-h356-no" width=500px />
- 차이점;
	-  path: 파일 경로 이름 포함 vs. file : 파일 이름;
	- 인수:
		- arg0 : 프로그램 이름(열로 이름 빼고); and arg1, ..., argn: 프로그램에 입력으로 사용될 인수들, 마지막엔 null ptr;
		- argv[] : 배열로 받기;

- file 이름을 쓰는 경우는?
환경 변수에 의해 설정된 path안의 file; (`$echo $PATH`)

- 예
#### 1	
```c
	#include <unistd.h>
	main() {
			printf("executing a.out\n");
			execl("./a.out", "a.out", "3", (char *)0);
			printf("execl failed to run a.out");
			exit(1);
	}
```
#### 2
```c
	#include <unistd.h>
	main() {
		char *const av[] = {"a.out", "3", (char*)0);
		printf("executing a.out \n");
		execv("./a.out", av);
		printf("execv failed to run a.out");
		exit(1);
```

- exec와 fork를 함께 사용
<여기 이미지 추가>

> **Q.  parent process가 직접 실행하지 않고 exec 하는 이유?**


```c
	#include <unistd.h>
	main() {
		pid_t pid;
		switch(pid=fork()){
		case -1: 	perror("fork failed");
					exit(1);
					break;
		case 0:		execl("/bin/ls", "ls", "-l", (char*)0);
					perror("exec failed");
					exit(1);
					break;
		default:	wait((int*) 0);
					printf("ls completed\n");
					exit(0);
		}
	}
```
### 프로세스의 동기화

### wait System call
```c
#include <sys/wait.h>
#include <sys/types.h>
pid_t wait (int* status);
```
<img src="https://lh3.googleusercontent.com/hIpOk5r0hRyMleKdz7vlOhAB3-zi0Pa-IFn3jophPPwf65gF8SEUokrR-fkm7RfOinBcXu-5tmuaNBrFX5jYwn1-L_FWEcN3yu4dWfp1GB-Qr4X3eFJoYJqltjTJLvQBCNi2-mbhCEMsaZ3tLZw4AnXCsawyeXqmT-zGbw3PK0ITpRFtBW-5NU_dnW5Chkjq9iy5uP4OqxEI5Lqnef16Hmc3GVhWRCgJ9ex5ZNMdScgHTfUGm_hkBK806VyyEhcJ9dHY4Kg_ONEwKMLfoH1-ShTC45uFeLS1qUmIhirH4_RNbQKXk4HrlmEAJ7S8m61Psh9Osw3QZA-8KBAOT7b0w_sXgDRpI5X08FIt0qCWCTefsPctjqPH5sCLJOJlFW-WTzi6Vx2mnimK4g_pEsLCGCKzieI4eAsBioFQahuCZYgVLTrhKKTowlu361gPrdLn34RcrFuTFdld3Dy5CqBolm51cF1azUyxuzR8les07Fgs6lfXH7UjFLhyIcYBNYhMT86n7JJWp58dOEkBEiNo1PNPzJmDVPoGDyf9w1VzSXxDK4v_xn_AP__DX8migGLw5vsK1G7AToVvOEpDYfqKgisicsXcbSlxnYr1LRW_s6NcX2pSGbjETg6i2BDPovu4I5VTGEbXvAiQs6X3YsJs0yNxXJ8d50lSzMXp1NjCH8h3d0oWVPgGjOZNVZLB5O92zzOeQeXiGy8nNQzbV-cdNLxLaDHFh6VMbPCJsOcYzjIJx--O=w958-h857-no" width=500px />

- 하나 이상의 child process 수행 시 아무나 하나가 종료되면 return
- return값
	- 성공 -> 종료된 child의 id
	- 실패 -> -1 (살아 있는 child process가 없는 경우)
- wait ex code)
	```c
	void do_child(void) {
	        printf("%ld\n", getpid());
	        sleep(1);
	        exit(0);
	}

	int main(void){
	        int fd, i, status, n;
	        pid_t pid;

	        for(i = 0; i < 3; i++){
	                pid =fork();
	                if(pid == 0)
	                        do_child();
	        }

	        while((n = wait((int*)0)) != -1){
	                printf("%d\n", n);
	                sleep(1);
	        }

	        return 0;
	}
	*********OUTPUT*************
	32368
	32370
	32371
	32368
	32370
	32371
	*****************************
	```
- status : child의 종료 상태가 전달

<img src="https://lh3.googleusercontent.com/l7cZfh0D-XlXHWzC2qysETKTwaKsxUhwM7LHwSrklG9qJ6WY-hPWRmHTmFf_EZSPonQEWzMH3Lxf6hXQY2Nnydit8vel-S5aLAWAa6L2mWfRQ07Mv3ArZZWdwglO3z4VnboOtl9IRAW6dnukaqLxRjqUqp9fGMZTwioYaehUQnzubWEnwa1UKGwa0m3dRiglAUEfqGcHgt2Cl45z5KM7V2Gn4XQKY574ToACOIV61R21r15xiJmmZrxXrKbgT-Bb2mTZ1a_-tvgIwr02M1aYkR_9CmyCkFxdPbEid4vC1-SCc0b_o8MSTOLeJu1WhCv3327Piqu25ozVfQ8C3FNcMiXO9MQze1_F2zFMw55Fdab6Tq6HizJQUCb7mbFlYQWDRJ_sjyubcr2-xftMC6bbCVQSC1un90n6j1f2mH6zOs9iOMqledGQybuNrrgdYoeI2bYNbUJHF-yc1g7uEoJ6WzXDo-qCdVRxgGGR3dB9JpBIJd9hc8j1cFf6PSTERMJ-uZJVyF-IBF9ijxs9topzItXAGvDt-TacUVRmlHzf9Pw_QAwonMBEBg6jeBFSBPkCJRkfJZC8MJxKxV6MpeKclxgG1BCUn_ONQcQiZ7tbsbsjHH5lboAfGyyAZJafbhVZx6dVtznighHZq0cpB93RsBLOAiuoscx0yEL0I42h-GowcsISI9x8tKnG27wNCbQLFAK3CZVGf0KULCk1dhcmZ6WJDemjg5PubMUpR6gDI7us7mz4=w920-h932-no" width=500px />

- 예제 코드
	```c
	#include <sys/wait.h>
	#include <unistd.h>
	#include <stdlib.h>
	main() {
			pid_t pid;
			int status, exit_status;
			
			if((pid=fork()) <0) {
				perror("fork failed");
				exit(1);
			}
			
			if(pid==0) {
				sleep(4);
				exit(5);
			}

			if((pid=wait(&status)) == -1 ) {
				perror("wait failed");
				exit(2);
			}

			if(WIFEXITED(status)) {
				exit_status = WEXITSTATUS(status);
				printf("Exit status from %d was %d\n", pid, exit_status);
			}

			exit(0);
	}
	```

- #### &status
	<img src="https://lh3.googleusercontent.com/PmugU2l2cwW3qs5s8EJerEPLC8ltSUgaf7AON7lvdm2a485-qOhunkm-Dij3CuqNNGgL0Sxgh8Ov5Y8gbDGoL1NpODDS5moENVyX_axcWOqE8wbdJIcK3MSVaXEY5HXG3wcH2jZtukMNJVli7Suv9tSoWjec0k4o25gK3fBW4fthmZUghtGOf4DtGIoYyOsYEqtG5ViIt7Q1rxKyu_stllJFZ2RR_bw8v3PA6bn-hxA2IHNpSYOvagMPJlrzrdDC9oZzp6sTG-coH8Ixz36s9wL35dLTejnjEo83FyUhNGpIhg7cR5_M4t00xEbPfj1fDs0Vgf5QVJ1P5tcVS5goS_GkaNRSAnNVrgHSblV5C37BzmmOcHo-JjBO2q-D95o36c9wLF7c45IQmPBAkALxQ5pdAZrD_mAqr2ZIB9K8er4ClmL3kEOUlJyvy_QnK8Nr0jUBsx2tZKRc5NO6N-bqEn2AqKb5bYgwMk6AdB3L_CO0MQQ36W-FGLH_oD_EVVcH0NSrEKY226WvI4lJLLUB4MOPPSDjsH3pvVyJ2R6m6BTlr38hPautPipxioCGUzTn9w7LpMVVLNBEGoRNAmkHq9IkUfEOyZJxTfnJ6WrZtX3gvv29dJA1UIrs-BEr4vHBto3ThzidBRRK1DruX-3GZtBJSgvDgYtVwzmzusRYOkpf8ix3xVfPqgNeFQ1xybhl8OuNp3hnYhY6bKWR5d3iFf9znQRBNaAcDdVbVKcGPsOPb9RF=w958-h563-no" width=400px />
	
	- WIFEXITED(status) : status의 하위 8bit가 0인지 검사; 정상종료인지 검사;
	- WEXITSTATUS(status) : status의 상위 비트에 저장된 값을 return;
	- why? child가 parent에게 전달하는 값이 status의 상위 8bit에 전달됨
	> signal을 보내서 다른 process를 종료시킬 수 있다.

### Zombie와 너무 이른 퇴장

1. parent가 wait를 수행하지 않고 있는 상태에서 child가 퇴장 -> child는 **Zombie process**가 된다.

	<img src="https://lh3.googleusercontent.com/T_NDtt8d53jWfHP_0Lh8BN2aA_Gevj97JfOlGyvHzUZi8nMJtHvXS8bg6x0Kl1aI9ctHkOthOFcaqhcLgWa6rghtnXV1BOGPKLpptsYEtVHStPEKJsWyrisYbzpnOjBT3gjd1zeGz4GRd-C2Ht5raoto1NS5pfPUxM8Ojmt3o0P830ic7SIICCv5FeRroiSfK3oYEwgzWjJLUN-HOC6m63vKcfSwj2u6o8Qa8LZBCv47m4xh5uExke_qZ_7sxL4aANQe7Ic_EnsYrGjTq6EyFHVQdnacbNzeYQtnFlYxkJTvm4IJ0mok7hWAPpOX_hc5prxOaQcVwKmEK6bt8YfiNt6hdOcDNgi-4iVwWHv0EQA5DrE9FyAASbD-i6K4UGlTimjONjOfllKVswbyQ1ot47iMUCOQc2HD2PWmYAsFQBCDY5XNn28FgSQ4xUYOzlBPJcKEkuJwfD78NUEAmGdqGWurlCA1_GsrgvZTi0EewJ2aBDw0DEh0mYTMXuG1eYxXlfnFU5F_Ro6vxrut4vDi1KD7T1g52nEhZnyVFyw0kJOhdu02hGCju-eqWVLHvqJSc5emAyeaOZDgA7eDgDu9g8wgnNU515r91BzYP40EMq_Pb4EPt7XlNkBmQ55nzMfHKwbJ35AOuhQw2-J7cSiKgBX3bEThJDuKGbpwt0mF5La9uhxawmtRE50rczQBIyCc-D_uZ4Wd697bvxAYRfybZpdGffW5fpGzcD_g-6uXhl5KAFMi=w797-h943-no" width=400px />

2. 하나 이상의 child process가 수행되고 있는 상태에서 parent가 퇴장할 때 -> child는 **init(pid = 1)**의 child로 남는다.

	<img src="https://lh3.googleusercontent.com/x5LqXBUTJdsV4IQec--kM_vc807BkEwTy0QiDOYBoWX51fT_8hTTQBEAfPHIcfbgdxOYBU1z8SgB7sre9-zwrBBzIyxY0LeCLgJlRyw39Dh0owZrcaReSd0BdpB8FDZ-RFXDj6b32NbzpOH5aCi5XKJXG__K2MY0uA8GpbOI9RZ9CwzySMK0q7vAz87RGFWGUJTCKSzU0ZHelI9-xChezCl5evUKE__fDbMym71GujNcyviZZH_nIIVj10vNADp8uMqwxqVK6UNlFZSzTu-UOwnP1DT9Eneoc-rGF9oJH2ZJEvOEv4C1TcSAlGmVKO-ASCm2m8lqEFuO2WsciC0EoJK41BvwbOX3wGIrBeCDF0gtH55gNLVW8CTuB3MH-bhkn6aHcgxEYhW1IQ7vJ47b0bv7LNR6wGCyUglm41UrJbTtyd8H91hTLePFTwSAYEcUCjuzjFBSmimU3kyC2Mj8VESbqnV5lS0BCPAoTxcwXVFUL2vsdV-Tznip-0-Q651MsVypXL1Q0zOPbLdoKN-V8ifD2y0h8JvaK7swRN9HYhjV1p4mMF-AtjE70Ne7t1H6PLwcKKFKkcAX1I1GwK3DCx481Wd20d5W0ogBu-L-nRQangja3STljgsQXCN4Qt7axzDuwLRzxeNowQLDmMVzMKbuiWIX0MzV3KMmylHTYsAxY3zsVdCgNSxXVblcVDUkmQeXnULPdGaDvWU3SVewJQb3-nMZxptbLb1EzwBOM7Efi9Qj=w962-h680-no" width=400px />

### waitpid system call
```c
#include <sys/types.h>
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *status, int options);
```

- 인자
	- pid : 기다리고 싶은 child의 pid
	- status : child의 종료 상태
	- options : WNOHANG(child가 종료하지 않았으면 0을 return;)

> **WNOHANG** -> child가 종료했는지 확인, 종료하지 않았으면 다른 할 일 하고 중간중간에 확인하겠다.

- return값
	- 0 -> child가 종료하지 않음
	- (> 0) -> child가 종료

	<img src="https://lh3.googleusercontent.com/mqTAikleiGYZyf7LRm3WjmczcYJG4RJxn1BG0HThtnbDnVzgR1QYUxQcHVuNYc3tAkexEDMqkbxo5tHA0Vm3CLloJrau1jd7-8WyH3UEUsehdnbc1sZSwhmU3RbqvHcDfm5oSdUEkPMiFdfB4bOUpvmDytBm4-h3rgDXEN4XyY50EmSibqvr9jvHdR9NjsW5j3mUDn9jXG-nCvv1PQzcZVVGW_sVp-S3vyoZoECEVkN1dYzXMi-FHIcvTs74a1Rf4Wm3IN4Ls8GpKIYmDjk_yLBUuc3yeJ0Sr1fXNuTb6gBQmjByvpkpf2SoxR8DMJ-A6Qnalts_LuIfYV4pew9-7hwKl2rSQ3L3SrriZd8Mb5tIGAUADj7GJ6hPiUNvrb7sdGEI3G28vuQHdKimiLxwnrC-b_2mM_TdqR3tA8oe4lW_aerSiLVZmYj-EHpmp9KfXpJukSp1nmHZqtf90JBijaRl8udwYymdlpqz5mhA8QTzJgdRZpcsQDzMcs9NrcVeXM4D6aju4kAvhPV8uQ-5s-swXglYIzaNfBOPlZaRwL6VvPZN-oM-8wtePOlgAiQosy2YV8mQK_cl6HlxitzPvQmngJjau74uro_mkXUV14E8rS0tEKidvMn8PIx2tRMR97WeKk0HQu1ZtbPRhbVsHUOJ9txAa4uJGmiDJDiKVqXCJwvb-bMiStPqPCUW9H-axfx_AuvxdtmoT86H15XSzkqbHNDvZ0J8Slu7HPIsLSTpdIPq=w734-h941-no" width=400px />

# Signal

### signal?

<img src="https://lh3.googleusercontent.com/qeR_xjunXWUTk_tHYb6CTGXDKA9fECLhV_FpHhrhupvjWB7IfvyGw0pMzxKG057fVYAzmWHxmGovZJwbHkMtg99LvodDD8H9xzr8uWMXsNWMmKyiCylSMXO5I_TP4Sp1OT3MOZtGjvMh2CcTswKWncZJ_HNezITSSc8hoSIdtMlejxfW0nF0ZatfT10GXhDbhTyk0VpTAJP2wMDmb0JEsqZ_2DeuI_qs-nW00WZKKcesSIXGR98294F2t4Aw-uSwant7u_0WpDDRDGjqYOIVIyZqCeHoQPq08Xb8Kn884n2ngFG2sTVeuXjDr8AJAg6kdPu9zi02vfZFCSRZzvLcyqFdubnSJjVligTX1IyPmN6CPUpJUM7n-0k4kUxdYN8GhABS0KAvd6rxuIOd3kpfRmvmqviYQy79EinkoNDvafpY0QA8Sp0Y9TXxtfkbSC5ZYSUvzYRH6Ltzc1oDRAi6U-e2qpm1St_jYykcCSlloFnZB0gnlYMWWhtnqX2tpJuk1pmsuazXQWfa2YSNXZNKzyvFZu4KxKXjOsS58gloequ53LwyjwfFe0IooM6C-YVox3FOwXvSxbkvefyaTMxR5wHbNv6nHoZeahORUHDZbg5aur-ItIqwhcuJYRYFLkzvJWtN7eHnR_VcbGL4CfnWzSeG-nAcYh8r3vJ7eVMOOUtbqjEefHTWPlv-5gwKRc9NQiZfzBeQEIENr1r3tMXcRlntetSmm4sWtUfg95uUbicMVO9_=w963-h554-no" width=500px/>

> **signal을 보낸다는 것**은 (B가 signal을 받는다)라는 의미보다 
>  OS에 B의 **signal table**에 mapping된 Action을 B에서 실행함을 의미
> ex) 무한루프 중 Ctrl + c -> OS가 SIGINT를 받음 -> 종료

- software interrupt
- kernel -> process
- process -> process
- 자료 전송보다는 [비정상적인 상황]을 알릴때 사용
- ex) program 수행중 Ctrl + c (interrupt key)
	-> kernel이 문자 감지, 해당 session에 있는 모든 process에게 "SIGINT"라는 signal을 보냄
	-> 모든 process 종료! **그러나, shell process는 무시**
- <signal.h>
### signal의 기본처리
1. 종료 
-> 내 program은 잘 작동하는데 OS의 사정에 의해 종료 (signal에 의한 정상 종료, default)
2. 코어덤프 후 종료 
-> 내 program 상 비정상적인 작동으로 OS가 중단시킴 (signal에 의한 비정상 종료)
	> core file (종료 직전의 momory의 상태)를 생성 후 종료 : debug
3. 중지 
-> 멈춘 상태로 계속 진행가능
4. 무시
<img src="https://lh3.googleusercontent.com/oqLhcStmYo6-q5qgfvOJfJA6KgdkVM-eSMzjTRyJD-Xxl1IBz8sZM4FHF0U_4fyU6RWLuVonao3Pfk39w8ouqCR871irXvXk2gKG6fJyouqIDWgs8f3bmX5DcdBXpuxXfxmDrV9y8s_LEcIjsM-aNG1D_N3x_C7Au3VWtUJVpnxEGnEd12Kbtvo8X_9NVMF-v9mZ3InLETBR0aQQvuW5QBQDTAqK7_P4iigAdI3vJfxy4GqL6T7_aBWTkzakZYJBsNDPVVIwL0S6a-VRZF29DF_FzqqYGBDWpMn5ww1wvypBOQk-RMVzY1q9YpH22S3mXpiByFIyB0WaY1h5_brdQ8vrQ8KqMDHuK0AAnYpZHxI82pW2suTlnBKCwnCH16a2o21qFWoG2K-CCgpMtUq2IaDaN28B4BNBqgrk_uA8yIrQCVol743sqWKXuCV1SY3bLa2DwN2t0z14qir2l-GEvWp0eaxNMlb-N2vW04qeDMVlp3CFBt_y8oxE03lTTZJ2Ad-9_3lOSQbtf0hXJNfYcKIhoAneCRV7GJD-0IE58v5zAa-VjZmWtJCxSlg44noLQLnFAC9Bx3PSSmAGZJvQDmVcjFurhF76dwvVtcuRssu8O_xgsTik5zuOM76h95-D0yS6tvd_BjSvtz_H3Dkz_r9tP5yTnJhvhiq_wqBXfwO6SoGoMZfHU9lwLClefCcHBNG19u2wYL6kg2MuXiKDPpzrlyEhGeWb_8PPs4NxXLT1kY7X=w717-h923-no" width=250px />


### 기본으로 알아두어야할 signal
- SIGUSR1, SIGUSR2
- 그 외의 signal은 process가 특정 상황에 OS가 사용하기 위한 signal

### child process의 종료 상태 확인
```c
pid=wait(&status);
if(WIFEXITED(status))	// 정상종료
	printf("%d\n", WEXITSTATUS(status));

if(WIFSIGNALED(status))	//signal을 받고 종료
	printf("%d\n", WTERMSIG(status));
```

### signal send
```c
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int sig);
```

- pid : signal을 받게될 process 지정;
- sig : 보낼 signal 지정;
- signal을 받을 process 또는 process group 지정
	- pid > 0 : **해당** id의 process에게 signal 전달
	- pid = 0 : sender와 **같은 process group**에 속하는 **모든** process에게 signal 전달. **(sender 자신 포함)**
	- pid = -1 : uid가 **sender의 euid**와 같은 모든 process에게 signal전달. **(sender 자신 포함)**
	- pid < 0 & pid != -1 : process의 group id가 pid의 절대값과 같은 모든 process에게 signal 전달
		> pid = 345 -> pid가 345인 process에게
		> pid = -345 -> group id가 345인 process에게
	- 만약에 다른 사용자의 process에게 signal을 보내면 ? -1 return

		<img src="https://lh3.googleusercontent.com/4_dDQccWPipzY5tet-ohmZgP4zJJDsuie2BWbdfIaqEEFli1OARiI_3cJ32JrTfAdFzbmSwkJTyGKWPbU7e3cqCM1_4o99HB9BoTqfNQdfkocciRHfDpgnTu1FEHgt55xu2OomhDWBPZgHJv5we-QNfCozqh6ZG5kzUg_Kw_8F614CelNqYp14XN9trGDdbZBZTzZ7NGN_4TGRX9ykqrOJLpvv6D6s6WzEiS-Pd76YMQmSNWbAPsRur099EDGXA_sRr5q2Qk6tKwMbhzf0PzscwNlmDrLX_ROhu5Ek5HCsv-pvtaSkmN33PQ1fVgVR72NYOM48dQQ9dHEI15Rgd6_PKUBMiqcDWaq0lM5rSukWvPbiWZ7bGhKfhVu2Gy92a6NlZTqjrrTTPfMfKJeKEOqIlfktqeZfo2MbAkiYYDo2XzD25U1qrHAM_X84rhYuVDPHcIVyiaMdL_el14UTFPITrvH05vaDuYWvYr6w_ljxsEvdfmZGspVavtz2p1TiIIDO2hmeaSxxsCyPWd6JXVWr8xH30BKDDREndY6IIAY0DgUxUVKiqaeUBEx8tV1uxlsySvPF7IFOQPFpWsh4RZUT5N9yk4dw3NDCTkS74ejPyYactkb-NmRdI9E32Qvyv_zfpg8oJ1y3OuWEdzbQhpd1Oar0G0Vh4mrewalk4vOjfAqQ-M4VkuspEnab1ZdOPZoCtx5CTIgwkC4SHAahg9ZUFVIwfhXhjyfhN5obJccOTK6qnB=w963-h889-no" width=400px />
		
		> 위와 같은 예제의 코드로 
		> 1. pid == -1로 실행한 결과 shell까지 튕김(sender포함)
		> 2. pid == 0로 실행한 결과 a.out2만 튕김
		

- 호출 process에게 sig를 보낸다.
	```c
	#include <signal.h>
	int raise(int sig);
	```
	(주기적으로 내가 무슨일을 반복할때 timing을 알려줄때 사용)

### signal Handling

- default action (프로세스 종료)
- 무시
- 정의된 action (sigaction)

#### sigaction
```c
#include <signal.h>
int sigaction(int signo, const struct sigaction *act, struct sigaction *oact);
```
<img src="https://lh3.googleusercontent.com/gCVygB0TPqFw_Zj8Ctdt91kSi8DlDCLs4KDXnqmd9k8OaVCGV8b-ypTJuFtCa0gULKYgOdD6bXmkLtYLSR8k1fbujQjW94iThR28xbYN39V9mC2VqMvJGdZ8wDrVMBAuO_YmRQhXU76IgIt-4CQThXaUqNsCfK6vcMf6Wy6qBMbKp7bmOgQgzBZ4Xg5EzDwNnc8O2dtAOB7adaF77ZcyIvn7aqIf2vBG8evJ5IlVMWEhw9IWjVwiyeHGIVL8iof7xJifAcuwZSMTy-SRBOdWAWqORfF1SYAhYK99-3ooWGK7gpmDBHCskh8GVcxuknQZ2QeezJpPLzs2aZsDsn-Rzv1rQLQ29R0r8oIFoyYNrOIPWpTKgmijjpnvCAZiEO7Q1XNy6YJ_ZiSUb4ZDJ950jW6R60f5m1oyll9ykFGpwWHkZ2cRprtD1nfFGt5Af4S6qzLWWfPvdyvL0uq__VjtuebbsPl1yirJoyb36rXZvPCevAWzHTXrNJQlk8KmJnkf5y9j1csZ_g00nWez4lkh6Dy92a6AHdCm0vQHFF-bRKDgskkyNxoorpdK4CYQPi67xXzMYOz_F22WkMW6BXcVHfu35pJhkeNoED2mx5R4MNijUYG74dJWT6FVIT2zt_iXFvY0sl1stiDRnhugp_BFBBssZzqbsCgS3ThVRWDbZNaF2DpXcun2VPH3m0rNQqYU1InUCwwc8bwLTwWPI4i7vpwOJL1quP87Jau_h1FiVAde6Oz-=w963-h528-no" width=500px />

- signal 수신 시 원하는 행동을 취할 수 있도록 한다.
	> 예외) SIGSTOP(process의 일시중단), SIGKILL(process의 종료)의 경우 별도의 action을 취할 수 없다.

-  sigaction의 구조
	```c
	struct sigaction {
		void(*sa_handler)(int);
		sigset_t sa_mask;
		int sa_flags;
		void (*sa_sigaction) (int, siginfo_t *, void *);
	}
	```
1. void (*sa_handler)(int)

	- SIG_DEL (default 행동, 즉 종료 선택);
		
	- SIG_IGN (무시) ;

	- 정의된 함수 (signal을 받으면 함수로 제어 이동; 함수 실행 후, signal을 받기 직전의 처리 문장으로 return);

	
	#### SIGNAL Handling 예시 code
	- signal handling 안하는
<img src="https://lh3.googleusercontent.com/mRFf5H-y6RdVqxYQ7fY4F_mb9HT9YMlBWPolus1yztHhVRNAWbU6sd_ih67SIq3jF-Xi8WRh5VZgtGqmjpRiYWpjoqdsRE9h7lvzbioio8n0fjGOzLtSjvecz8oNxKT0xFK6UaWO0GtUvuBZQRT1KP8UnBdTmnBsGrcJUJaOjZmeiiIa6xoQnE0YJzdtz4hHvbWpWFGzk5q6BSS5dPEq4z5IeIkCIfQ3Y8WkuYy69sQXT5H-G8pi2wkPQmRCHAZQXYppPsZy63n1VVkRMsh7P82UsohZB28hpnCIk-EfdAijRX5CXWBV-6dA_mrLEoTx8E4oxXQ5EOld5MvvJytMXRoOAkFy-7kCraq_b-x8UWoJxqOA44vBEGk7-VGvaPi9IgrgfCa35nAw-s9MXRK3oXpwHZLNFB9kdv0SZ5Zx7znADThlpovl6i7djY7kqYhezKXv8iHOx6sbEUwiClMp6vEtYGSXxjzH4Sc879pC1pmyAy3o87tgT3rXx4SWSezfrauZ9s5Xk62Q4mI-2ZkPeCOT4e5M8aBlL0q_Zb90zXjPVxMjq-pYUF5oJMz8QCVK_BhT8yeTCyUBKfMITjgUEKmk5uN0XqP0XKtGkyiFrRPH-IFq2QaJsQ-ZS-jMCyHG3kP5N0g56IIYgxny6BF4-Mm6Uedvt814VC07EwJN59VMj-h0NkpLpAnv2977KBjbPHWIR4sKaQG-wz3Z6bt0Gz9q9cc3uUdFihOQ5QrlY6uu_sO5=w890-h693-no" width=700px />

	- signal Handling 하는
<img src="https://lh3.googleusercontent.com/TOvBYw1QO9NTX-JxJ8QlABctO0s3lNxnIoCQ0tHIuWOcRgEy2F3gGXWfBZFUEMCgyXnJMr8vPzNrFwpvZlif7z1X7WTPcGC4dsK7djGxQa-MumUrXmPeTznHW-joXmuGrJfFa4grr2lA_EQ0TjNyOj2yNP-XCcokWmZzMhyrTKpTuWhDrkxuriN4NNNbKpBIMywabIUCQOWSGWxXYvjXT8vJPKQUWagKClBaQh3ElswVBLo_FK8aE8In23QsbbnZWeBPot1V8fsDdFtuV8EMqJOy3CY-L9TzZqceDuVbizgnDOUVq10sE6SOErA2W0IvKizPhqr7c1b5wqZFY9yn6cXMdDVaknvAZuh50baFKYE-WovDzOdwf_QVguKcUaktDV_ITZhpZ8xQ49SUUn1lAN5OXoiNg4SPkIKR-l2yw5SMWh9FMEJwKvn0OKtjaGt4UyLY-EvUfPXinhEyatLolY5V29Sp-3ewIPa_u4W_QoUIdqn4sJx1_O4CAn_h28GWDigDPdyZHOEpjCemeIiI5LTo_7y3BxSu7Qq6LAIG83n9LL6XemXK_ieqcnl68-n-QR7BoXxsmc7MBtGJIYUz820E6kfeBQYg9g3pDTBgEGbNRZ8FVPDKgq8Qf_B24ZHG4yfgAE8NHUm_iIn6o3nxbjQO_6nuig-UVnoT1vbkYR1M0_1Y039OtNY5vD-gOnQOKADoc1SZKVyr9zJPqxgDzqPK39hWWJBvFmixXippskt41H8p=w941-h931-no" width=700px />

2. sigset_t sa_mask;
	=> 여기 정의된 signal들은, sa_handler에 의해 지정된 함수가 수행되는 동안 blocking된다.
3.  int sa_flags;
	=> SA_RESETHAND : handler로부터 복귀 시 signal action을 SIG_DFL로 재설정;
	=> SA_SIGINFO : sa_handler 대신 sa_sigaction 사용

#### Signal 사용 예
1. SIGINT를 무시;
	```c
	act.sa_hander=SIG_DFL;
	siganction(SIGINT, &act, NULL);	
	```
2. SIGINT시 종료;
	```c
	act.sa_hander=SIG_IGN;
	sigaction(SIGINT< &act, NULL);
	```
3. 여러개의 signal을 무시하려면;(1개의 act.sa_handler + 다수의 sigaction)
	```c
	act.sa_handler=SIG_IGN;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
	```
> **한 process에서 무시되는 signal은 exec() 후에도 계속 무시된다.**(signal table까지 copy한다. 다만, **함수로 수행하는 경우**에는 함수 주호가 다르기때문에 **수행할 수 없다**.)

### signal 집합 지정
Q. signal 처리 중 다른 signal이 올 경우
1) 다른 signal로 넘어간 후 다시 돌아와 남은 signal을 처리한다.
2) 현재 signal 처리 후 다른 signal을 처리한다.

`sigset_t sa_mask` -> 나중에 처리, 즉시 처리하기 위한 blocking을 위한

> **blocking**(언젠가 반드시 처리)과 **무시**(pass)는 다른말이다.

- signal 집합 지정:
	- sigemptyset -> sigaddset (빈 sigset에 signal을 추가)
	- sigfillset -> sigdelset(모든 signal을 추가 후 예외 signal은 제외)

```c
#include <signal.h>
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);

int sigaddset(sigset_t *set, int signo);
int sigdelset(sigset *setm int signo);

int sigismember(sigset_t *set, int signo);
```

### sa_sigaction()에 의한 signal handling
```c
int main(void) {
	static struct sigaction act;
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handler;
	sigaction(SIGUSR1 &act, NULL);
	...
}
void handler(int signo, siginfo_t *sf,ucontext_t *uc){
	printf("%d\n", sf->si_code);
}
```
=> sa_sigaction은 sa_handler와 비슷한 기능을 하지만 조금 더 많은 내용을 표기해 준다.
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTgwNTkwNTIyMCwtMjMxNTMxODAyLDExMz
M0NzA0NjIsMTQxMzAyNjI0MSwtMTM1OTM4NDczNCw5NzY2NzMz
MTQsLTI2Njc4Njc2OSwtMTUzMzkzOTA4MSwxNDk3OTc4NDEzLC
0xMzkzOTE5MjIwLDEzNzE1Mzk0NjQsLTExOTIzNDM0OTIsLTEy
MTA3OTM3NTYsMTQxNjE5MTk4MiwtMjk1ODI4OTQ3LC0xNTAyMD
MzNDM4LDY3OTkwOTg1MSwtMTk4NTU0MjIzMywtOTk2OTg0NDI1
LDIwMzgyOTA1OTldfQ==
-->