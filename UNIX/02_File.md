# file
파일 관련 각종 정보를 알아볼 수 있는 System call
[https://downman.tistory.com/126](https://downman.tistory.com/126)

## 파일 정보의 획득

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

## file mode
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

## access System call
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

## chmod System call
=> 특정 파일의 access permission을 변경하는 System call

```c
#include <sys/types.h>
#include <sys/stat.h>
int chmod(const char* pathname, mode_t mode);
int fchmod(int fd, mode_t mode);
```

> **소유자만 사용가능하다.**

## link System call
=> 기존 파일에 새로운 이름을 부여하는 System call(hard link)
link count : link의 수

### hardlink
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

### symbolic link
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
<!--stackedit_data:
eyJoaXN0b3J5IjpbNjcwMjkyNjk2LDEzMzYzNDg3NjBdfQ==
-->