# Permission
rwx | rwx | rwx
User | Group | Others

ex) 0600 0700 0640

- **r**(4) : 디렉토리 내부를 읽을 수 있다.(ls 사용)
- **w**(2) : 다른 사용자가 내 디렉토리에 파일을 생성할 수 있다.
- **x**(1) : 접근. (cd 명령 사용)

## File IO
1. System call을 호출 했을 떄 User가 Os에 알려줘야 할 정보들(U -> O)
2. 작업을 끝나고 나서 OS가 User에 돌려주는 정보들(O -> U) ex) return값 = 주소
3. OS 내부에 어떤 변화? ex) 내부 자료구조 변화

- 고수준 파일 입출력
`fprintf`를 적다가 5번째에서 error가 발생했을 때 중간값을 확인해봐도 저장 내용이 없다 -> fprintf는 내부 buffer로 동작하기 때문에

- 저수준 파일 입출력
	**최소의 System call**을 사용할것 (Switching이 잦으면 비효율적)

> **고수준 저수준을 나누는 기준**은
>  HW에 얼마나 더 가까이 접근하는가로 나눈다.

### file descriptor
- 현재 open된 file을 구분할 목적으로 UNIX가 붙여 놓은 번호
-  User마다 file table을 가지고 있다.
<img src="https://sites.cs.ucsb.edu/~rich/class/old.cs170/notes/FileSystem/filetable.rich.jpg" width="400px" height="400px" />

> **file table**
>  각 table은 index를 가지고 있음
>  0(표준 입력), 1(표준 출력), 2(표준 에러 출력)
>  3~> 사용가능

- 한 프로세스가 동시에 open할 수 있는 file의 개수에 제한이 있음. (Close를 사용하는 이유)

> ### !!! Header는 따로 외우는 것이 아니라 Header를 모아서 따로 저장해두자

## Open System call
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
 
### * EXCL
 - file 존재(open fail)
 - 존재X (**CREAT와 같이 사용해서** open)
	```c
	fd = open("./test", O_WRONLY | O_CREAT | O_EXCL, 0644);
	```
### * TRUNC 
- file 존재 (기존삭제=덮어씀 open)
- 존재X (새로 Create 후 open)
- mode: file을 새로 생성할때만 사용(Permission)
ex) 0600, 0664...

- return 값
	- 성공 -> file descriptor(>=3)(음이 아닌정수)
	- 실패 -> -1
	- 실패 이유)
		-  1. 너무 많은 open 
		-  2. CREAT + EXCL 
		-  3. Permission과 flag가 맞지 않음

> Open에 관해 잘정리된 사이트[http://forum.falinux.com/zbxe/?mid=C_LIB&page=5&document_srl=408448](http://forum.falinux.com/zbxe/?mid=C_LIB&page=5&document_srl=408448)

## Creat System call
```c
int creat(const char *filename, mode_t mode);
```
=> file을 생성하여 open하거나, 기존 file을 open하는 system call

<주의사항>
1. WRONLY로 open한다.
2. file이 이미 존재하면, 두번째 인자 무시; 기존 file은 0으로 open한다(WRONLY + CREAT + TRUNC)

## Close System call
```c
int close(int filedes);
```

=> open된 file을 close할때 사용
process 종료 시 open된 file들은 자동으로 close But, 동시에 open할 수 있는 file 수 제한 때문에 close를 사용;

<인수>
- filedes : open된 file의 file descriptor
- return 값
	- 성공 -> 0
	- 실패 -> 1

## Read System call
```c
ssize_t read(int filedes, void *buffer, size_t nbytes);
```

=> open된 file로부터 지정한 byte수 만큼의 data를 읽어 지정된 저장장소에 저장하는 명령
- file pointer(read-write pointer) : 읽혀질 다음 byte의 위치를 나타냄

> #### 현재의 file pointer에 각별히 주의하면서 사용!!

<인수>
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

## Write System call

=> 지정된 저장장소에서 지정한 byte수 만큼의 data를 읽어 open된 file에 쓰는 명령(file pointer | r/w pointer : 쓰여질 다음 byte의 위치)

```c
ssize_t write(int filedes, const void *buffer, size_t nbytes);
```

<인수>
- filedes : write할 file descriptor
- *buffer : write할 내용이 들어 있는 저장 장소의 주소;
- nbytes : write할 byte 수;
- return값
	-  성공 (보통)-> 쓰여진 byte수 (n)
	-  (return 값 < n) -> 쓰는 도중 media가 full
	- 실패(쓰기전에 꽉참)  -> -1

<주의 사항>
- 기존 file을 open system call로 open하고 바로 write를 하면?
	```c
	fd = open("data", O_WRONLY | O_APPEND);
	```
	-> open 하자마자, file pointer가 file의 끝으로 이동..

### Read/Write 효율성
- File을 copy하는 프로그램 실행 시간은
	-  BUF_SIZE가 **512(disk blocking factor)의 배수**일 때 효율적
	- System call의 수가 **적을수록** 효율적

	#### (1 Read -> 1 Write시)
	1. [1문자씩 읽었을때]- block의 크기와 차이가 크면 빠르지만 block이랑 차이가 적으면 느림.
	2. [1 block(512) 읽었을때] , 3. [1 block X (배수) 읽었을때] - 비슷

## lseek와 임의 접근
=> open된file내의 특정 위치로 file pointer를 이동하는 system call

```c
off_t lseek(int filedes, off_t offset, int whence);
```

<인수>
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

## file의 제거
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

#### =>unlink는 실제 file의 삭제보다는 file에 연결된 link 제거.

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
<!--stackedit_data:
eyJoaXN0b3J5IjpbODczNjgwOThdfQ==
-->