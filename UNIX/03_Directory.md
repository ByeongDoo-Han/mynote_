# directory
## directory 생성 및 제거
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

## rename System call
```c
#include <stdio.h>
int rename (const char* oldpathname, const char* newpathname);
```
- file과 directory 둘다 rename 가능
- **주의** newpathname이 존재해도 -> -1 return하는게 아님! **기존 file을 제거하고 새 이름을 부여!!!**

## getcwd System call
```c
#include <unistd.h>
char* getcwd(char* name, size_t size);
```

- return값 
	- 성공 -> current working directory의 경로 이름
	- 실패 -> null pointer
- **size**는 실제 길이보다 **+1**이 커야한다.('\0' 포함)

## chdir System call
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
	
## directory 열기와 닫기
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

## directory 읽기
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
## dircectory tree의 산책
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

- ftw 예시 code
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

<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE2ODQxNDgxMThdfQ==
-->