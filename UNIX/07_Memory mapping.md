# Memory mapping
<img src="https://lh3.googleusercontent.com/U_f2jiVqweb8QhfRElchZhGNoCnbaLjGXrR4fGS0keQWklhe-eJcmNAAHS01Fg9d7exW-pS7IImQiV6WHVBJ1qBYqphHyBR2aPerxH9YN8UFlqevK_GIg6cdgXrMUNHYQWx2Pm4onLCpCh3mvqoy5pBb6tYnXC10VJ4rOfMgkeuNdiHp3t94q_YYVjj2kUl9TX3p7FrJDFUlSVJlgkU8vfKgUkPcinIupIsGuViWSpnUzu-2MpuyAitKiLWjB-ctSy7QFkLJxFFipgIVj-bW1o40EU6SIVM66U1G00xEL1owI6mqaGMfEHOSKCM76m-gm_cFgdxocGsjLFcqC60-ug4gOi8H_9ygnb-B4xyBDAeRuQ4sgTdS4gmnxnmtECK8LDgLM7mV-KPb8bmpk6kPIqBGlaurphMrTwVAAmyzvBJzmhRm3VjCwrpfx0ajmKZ9vQGokVky2F8VrCsbwkYtj5KENnkIOkofGtCLErjOSGvq3-ZlP29MbgFrXg8hd47zpJ9hQHorwzg6yaaYylnknlrqtqiA67h-nUsdeWY5RTs-PVj5wxf17Jn2iOs0O5IplvOI60jS4tGuvZmzDca1d3eDBs8CRF6serVSArdexhLGD59Pu9Om9rSWX6Vk5RhqCvmW_LKGGjB_4rJu_Byxdl_ZcIjEfJvpykSrDVqeW8CFUV3tfMzwtoDpZ6-vBF42a8k7IJQYQVgdQiaxo40L0--y37AqW0mZyxcQ_JoTSO-w9bEX=w468-h931-no" height=600px/>

- memory mapping : file의 프로세스의 memory에 mapping

## Memory mapping
``` c
#include <sys/mman.h>
void *mmap(void *addr, size_t len, int prot, int flags, int filedes, off_t off);
```
=> **filedes**가 가리키는 파일에서 **off**로 지정한 위치부터 **len**만큼의 데이터를 읽어 **addr**이 가리키는 메모리 공간에 매핑한다. (동적할당과 유사하다)

<인자>
- addr : 매핑할 메모리 주소 (NULL인 경우는 자동)
	- ex) A배열 k번째 뒤, 주의사항은 이미 값이 있는지 확인!! 
- len : 메모리 공간의 크기
- prot : 보호모드(**open할때의 file descriptor option과 일치해야함!**)
	- PROT_READ : 읽기 허용
	- PROT_WRITE : 쓰기 허용
- flags : 매핑된 데이터의 처리 방법을 지정하는 상수
	- MAP_SHARED : 변경 내용 공유
	- MAP_PRIVATE : 변경 내용 공유 X
		- 나만 배열처럼 사용 (가져간 data는 다른 process와 file에 영향 X,  munmap후에도 다른 곳에선 변경점 없음)
- filedes : file descriptor
- off : file offset
	- 여기서 off는 **page 크기의 배수**가 중요!! (ex. 보통 1024크기의 page단위)

<return 값>
=> mapping된 주소 (type은 void*로 정수, 실수 ...)
=> 매핑된 영역을 벗어나면, SIGBUS, SIGSEGV 발생

<예제>
```c
int main(void) {
	int fd;
	char *addr;
	fd=open("data", O_RDWR);
	addr=mmap(NULL, 512, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	printf("%s\n", addr);
```

## memory mapping 해제
```c
#include <sys/mman.h>
int munmap(void *addr, size_t len);
```

- addr : mmap의 반환값

## 보호 모드 변경
```c
#include <sys/mman.h>
int mprotect(void *addr, size_t len, int prot);
```

## 파일 크기 변경
(5byte -> 10byte -> 30byte)
#### 새로 생성한 file을 mapping하기 위해서는 꼭 필요한 option!
```c
#include <unistd.h>
int truncate(const char *path, off_t len); // file open을 하지 않은채로

int ftruncate(int filedes, off_t len);
```

## 매핑된 메모리 동기화
#### Mapping은 실시간 통신을 보장하지 않는다.
<= OS가 여유 있을 때 mapping한 작업을 수행하기 때문에 언제 작동할지 모른다.

```c
#include <sys/mman.h>
int msync(void *addr, size_t len, int flags);
```
- flags 인자
	- MS_ASYNC : 비동기적 쓰기 작업
		- 쓰고 다른작업(return은 썼는지 안썼는지 보장 X)
	- MS_SYNC : 동기적 쓰기 작업
		- 중요한 data를 쓰기 까지 기다림(return은 썼다를 보장)
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTA5MDM4NDU0MV19
-->