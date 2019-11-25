# Socket

## TCP/IP Socket 프로그래밍
네트워크 프로그래밍...

- TCP / UDP - 네트워크 연결 방식...

### Socket(네트워크 프로그래밍 인터페이스)
- 네트워크 통신 프로그래밍을 하기위해서 사용하는 입출력 인터페이스
 - Server나 Client의 정보를 저장하고 있다.
 - IP주소 , 주소체계 , port 번호.
 - 네트워크 통신을 하게되고 하드웨어의 제어 , 신호처리등등의 일을 하게 된다.
 - 함수 호출 작업으로 이루어 진다.


> 소켓의 시초 (버클리소켓 - 유닉스 운영체제 기반의 소켓)
버클리소켓을 기반으로 MS사에서 윈도우 운영체제에 사용할수 있는 소켓을 만들었다
 
 > winsock.h -> 95년도에 winsock2.h로 버전 업이 되었다...

### WSAData
```c
typedef struct WSAData {
        WORD wVersion; //윈도우 소켓의 버전
        WORD wHighVersion;//최상위 윈도우 소켓 버전
        char szDescription[WSADESCRIPTION_LEN+1];// 구성정보
        char szSystemStatus[WSASYS_STATUS_LEN+1];// 소켓의 상태
        unsigned short iMaxSockets; //최대 소켓개수
        unsigned short iMaxUdpDg; // 데이터 그램의 최대 크기
        char FAR * lpVendorInfo; //벤더 정보(의미 없음...)
} WSADATA, FAR * LPWSADATA;
```

## Window Socket 실행 및 초기화 함수
### 실행함수 WSAStartup
```c
int WSAStartup(
    _In_ WORD wVersionRequested, // - WinSock의 버전
    _Out_ LPWSADATA lpWSAData // WSADATA의 주소값
    );
```

### 종료함수 WSACleanup
```c
int WSACleanup( void );
```
 - 소켓의 자원을 반환하는 함수
 - 운영체제에 사용을 중지를 요청하는 함수

## Socket Address
### SOCKADDR_IN
```c
typedef struct sockaddr_in {
    ADDRESS_FAMILY sin_family; 	// 주소 체계
    USHORT sin_port; 			// 16bit  TCP/UDP Port번호
    IN_ADDR sin_addr; 			// 32bit IP주소
    CHAR sin_zero[8]; 			//의미 없음...사용하지 않는다...
} SOCKADDR_IN, *PSOCKADDR_IN;
```
#### <인자>

- sin_family
	- 주소체계를 담당하는 멤버
	 - 현재 IPV4가 주류...
	 - IPV4(4byte)를 통해 할당할수 있는 IP주소가 고갈될것을 대비해 IPV6(16byte)를 만들어 놓았다...

	 > **자주 사용하는 변수**
	 > 1. IPV4 - AF_INET
	 > 2. IPV6 - AF_INET6
	 > 3. 로컬 통신을 위한 유닉스 주소 체계 : AF_LOCAL , AF_UNIX

- sin_port
	 - 컴퓨터에 들어오고 나오는 데이터들을 구분할수 있는 번호
	 - 2byte로 처리된다
	 - 0 ~ 65535의 범위를 가진다...
	 - 0 ~ 1023까지는 특정 프로그램에 할당하기로 예약이 되어있는 포트번호
	 - HTTP/HTTPS - 8080
	 - DataBase - 1521

	 > netstat -na - 사용중 포트번호 확인

	- <port 특징>
		 - 하나의 포트번호에 여러개 설정 불가, 일대일 매칭이 되어야 한다
		 - TCP와 UDP는 포트번호를 공유하지 않기 때문에 같은 번호 할당 가능
		 - 네트워크 바이트 순서로 저장하여야 한다


- sin_addr
	 - IP주소를 저장하는 멤버
	 - 각각의 컴퓨터를 구분하기 위한 번호
	 - 네트워크 바이트 순서로 저장하여야 한다

- sin_zero[8]
	 - 특별한 의미를 가지고 있지 않다
	 - bind함수의 요구사항이 0으로 전부 채워져 있어야 한다
	 - sockaddr의 크기와 sockaddr_in의 크기를 일치시키기기위해 삽입된 멤버

### SOCKADDR
```c
typedef struct sockaddr {
    ADDRESS_FAMILY sa_family;           // 주소체계
    CHAR sa_data[14];                   // 주소정보
} SOCKADDR, *PSOCKADDR, FAR *LPSOCKADDR;
```
#### <인자>
 - sa_data
   - 주소 정보를 저장하는 멤버
   - IP주소와 Port번호를 포함시켜서 저장하는 멤버
   - 데이터를 저장하고 관리하기가 불편하기 때문에 SOCKADDR_IN을 만들었다
   - 두가지의 정보를 저장하고 나머지 공간은 0으로 채워져야 한다

## 네트워크 바이트 순서

### 바이트 정렬 함수
매개변수는 항상 unsigned 타입의 정수값이어야 한다
```c
u_short htons()		// host to network short
u_long htonl()		// host to network long
u_short ntohs();	// network to host short
u_long ntohl();		// network to host long
```
#### <특징>
 - CPU마다 정수값을 저장하는 방식이 다르다
 - 데이터가 그대로 저장되기도 하지만 거꾸로 저장되기도 한다
 - 저장순서가 다르기 때문에 그냥 데이터 송수신을 하면 문제가 발생된다
 - 네트워크에서는 빅엔디안 방식으로 통일하여 데이터를 전송
 - 바이드 정렬 함수를 이용하면 된다
#### <방식>
 - 빅엔디안(Big Endian) - 상위바이트의 값을 작은 번지수에 저장하는 방식
 - 리틀엔디안(Little Endian) - 상위바이트의 값을 큰 번지수에 저장하는 방식


---
```c
#pragma comment(lib,"ws2_32")
#include<stdio.h>
#include<WinSock2.h>

int main(void) {

	short a = 0x1234;
	int b = 0x12345678;

	printf("호스트 바이트 -> 네트워크 바이트\n");
	printf("%X -> %X\n", a, htons(a));		// 1234 ->3412
	printf("%X -> %X\n", b, htonl(b));		// 12345678 -> 78563412

	short a1 = htons(a);
	int b1 = htonl(b);
	printf("네트워크 바이트 -> 호스트 바이트\n");
	printf("%X -> %X\n", a1, ntohs(a1));	// 3412 -> 1234
	printf("%X -> %X\n", b1, ntohl(b1));	// 78563412 -> 12345678

	return 0;
}
```

## IP주소 변환 함수
 - 192.168.0.2
 - 문자열 형태의 ip주소를 32비트의 숫자로 변환하는 함수
 - 네트워크 바이트로 정렬하여 변환한다

### inet_addr
```c
unsigned long inet_addr(
    const char * cp	// 문자열 형태의 아이피 주소
);
```
> visual 2019에서 사용하기 위해 아래와 같은 매크로 선언이 include문 전에 필요
> **#define _WINSOCK_DEPRECATED_NO_WARNINGS**
> 또는
> inet_pton을 사용할 것
- #### <반환값>
	- 성공 -> 32bit로 변환된 ip주소
	- 실패 -> -1을 반환

### inet_pton
```c
INT inet_pton(
    INT Family, //주소체계
    PCSTR pszAddrString,// 문자열형태의 IP주소
    PVOID pAddrBuf // 저장할 공간의 주소
);
```
### inet_ntop
```c
PCSTR inet_ntop(
    INT             Family, //주소체계
    const VOID *    pAddr, //변환된 ip주소 저장공간의 주소
    PSTR            pStringBuf, //변환될 ip가 저장될 주소
    size_t          StringBufSize //buf의 크기
);
```

### inet_ntoa
- 정수로 변환된 IP주소를 문자열로 바꾸는 함수
- 호스트 바이트로 정렬 변환한다
```c
char * inet_ntoa(
    struct in_addr in	// 32bit의 ip주소
);
```

- #### <반환값>
	- 성공 -> ip문자열의 주소
	- 실패 ->  -1


## Socket 생성
```c
//동적라이브러리...DLL을 불러와야 WinSock2.h에 있는 함수들을 사용할수 있다...
#pragma comment(lib,"ws2_32")
#include<stdio.h>
#include<WinSock2.h>

int main(void) {

	WSADATA ws;

	int code = 0;

	//printf("%X\n", MAKEWORD(2, 2));	// 202(0x0202)

	WSAStartup(MAKEWORD(2, 2), &ws);

	printf("wVersion:%#x\n", ws.wVersion);				//0x202
	printf("wHighVersion:%#x\n", ws.wHighVersion);		//0x202
	printf("szDescription:%s\n", ws.szDescription);		//WinSock 2.0
	printf("szSystemStatus:%s\n", ws.szSystemStatus);	//Running
	printf("iMaxSocket:%d\n", ws.iMaxSockets);			//0
	printf("iMaxUdpDg:%d\n", ws.iMaxUdpDg);				//0
	printf("lpVendorInfo:%s\n", ws.lpVendorInfo);
	
	WSACleanup();

	return 0;
}
```

## IP주소 변환
```cpp
#pragma comment(lib,"ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>//TCP/IP에 관련된 함수들이 정의된 헤더파일...


int main(void) {

	in_addr a;

	printf("%d\n", inet_addr("192.168.0.2"));

	inet_pton(AF_INET, "192.168.0.2", &a);

	printf("%p\n", a.S_un);


	in_addr in = (in_addr)a;

	char* pt = inet_ntoa((in_addr)a);

	printf("%s\n", pt);

	char buf[20];
	inet_ntop(AF_INET, &a, buf, sizeof(buf));

	printf("%s\n", buf);



	return 0;
}
```

## socket 생성 & IP 설정
```cpp
#pragma comment(lib,"ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>//TCP/IP에 관련된 함수들이 정의된 헤더파일...


int main(void) {

	SOCKADDR_IN sock = { NULL };

	sock.sin_family = AF_INET;
	sock.sin_port = htons(12121);
	inet_pton(AF_INET, "192.168.0.2", &sock.sin_addr);


	return 0;
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTQyODc2OTUyOV19
-->