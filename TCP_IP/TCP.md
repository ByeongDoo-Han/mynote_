# TCP

## TCP / UDP
- 공통점
	 - port번호 사용
	 - 데이터 오류를 체크하는 방식

| TCP  | UDP |
|--|--|
| 연결형 | 비연결형 |
| 신뢰성데이터전송 | 비신뢰성데이터전송 |
| 1:1 통신 | 1:1통신 or 1:통신 |

#### 예) 전화
| 받는 사람 | 거는 사람 |
|--|--|
| 전화 | 전화 |
| 전화번호 할당 | 전화번호 할당 |
| 전화를 기다림 | 전화를 걸기 |
| 전화받기 |  |
| 연결 | 연결 |
| 대화 | 대화 |
| 전화 끊기 | 전화 끊기 |


#### Server-Client
|Server| Client |
|--|--|
| Socket 생성 - socket | Socket 생성 - socket |
| Socket 정의 - bind |  |
| 연결대기 - listen | 연결 요청 - connect |
| 요청 수락 - accept |  |
| 데이터전송 - read, write | 데이터전송 - read, write |
| 연결 종료 - closesocket | 연결 종료 - closesocket |

## Socket 생성 함수

```cpp
SOCKET socket(
    int af,			// 주소체계
    int type,		// 소켓 타입 지정
    int protocol	// 사용할 프로토콜(통신방법)지정
);
```

#### <인자>
- type
  - TCP - SOCK_STREAM
  - UDP - SOCK_DGRAM
- int protocol
  - TCP - IPPROTO_TCP
  - UDP - IPPROTO_UDP
  - 주소체계와 소켓 타입만으로 운영체제가 자동으로 지원된다

#### <반환값>
- 반환값 
	- 성공 -> 소켓 반환
	- 실패 -> **INVALID_SOCKET**을 반환한다


## Socket종료 함수

```cpp
int closesocket( SOCKET s );	
```
#### <인자>
 - SOCKET s - 종료시킬 소켓
#### <반환값>
 - 성공시 0을반환 실패시에는 SOCKET_ERROR를 반환 한다

## Socket 정의 함수(bind 함수)
 - 서버의 IP주소와 포트번호를 결정하는 함수
 - 사용할 소켓을 정의 한다

```cpp
int bind(
	SOCKET s,					// 접속을 허용할 소켓을 지정한다
	const struct sockaddr * name,// IP/PORT의 정보를 담고 있는 소켓 구조체 주소
	int namelen					// 소켓 주소를 담은 구조체의 길이
);
```
#### <반환값>
 - 성공시 0을 반환 실패시에는 SOCKET_ERROR를 반환한다...

## Socket 연결대기(listen 함수)
 - TCP포트의 활성화(Listening 상태) 설정함수
 - 클라이언트에서 들어오는 접속을 받을 준비하는 함수
```cpp
int listen(
	SOCKET s,	// 접속을 허용할 소켓을 지정,bind 함수를 통해 IP/PORT를 설정한 소켓
	int backlog
);
```
#### <인자>
 - int backlog 
	 - 접속 가능한 클라이언트의 개수 , 최대값 SOMAXCONN
     - 연결 요청 대기 큐의 크기정보 전달

#### <반환값>
 - 성공시 0을 반환 실패시에는 SOCKET_ERROR를 반환한다

## Socket 연결 요청 함수(connect 함수)
 - 클라이언트에서 사용하는 함수
 - 접속할 서버와 논리적인 연결을 설정하는 함수

```cpp
int connect( 
	SOCKET s,					// 접속을 허용할 소켓을 지정한다
	const struct sockaddr * name,// IP/PORT의 정보를 담고 있는 소켓 구조체 주소
	int namelen					// 소켓 주소를 담은 구조체의 길이
);
```
### <반환값>
 - 성공시 0을 반환 실패시에는 SOCKET_ERROR를 반환한다...

## Socket 요청 수락(accept)

 - 클라이언트와 통실할 새로운 소켓을 생성후 리턴
 - 접속할 클라이언트의 IP/PORT를 알수 있다
 - 연결 신호가 들어올때까지 대기하고 있다가 신호가 들어왔을때 실행된다

```cpp
SOCKET accept(
	SOCKET s,				// TCP포트의 활성화 상태의 소켓
	struct sockaddr * addr,	// 클라이언트의 IP/PORT의 정보를 저장할 구조체
	int * addrlen			// 주소를 담은 구조체의 길이를 저장한 공간의 주소
);
```
### <반환값>
 - 성공시 소켓 반환  실패시에는 INVALID_SOCKET을 반환한다

##  TCP에서의 데이터 송수신 함수
### 데이터 수신 함수(recv)

```cpp
int recv(
	SOCKET s,	// 대상과 연결된 소켓
	char * buf,	// 받을 데이터를 저장할 공간의 주소
	int len,	// 수신 버퍼에서 복사할 최대 데이터의 크기
	int flags	//  recv함수의 동작 변경 옵션,운영체제가 무시하기때문에 보통 0을 적는다
);
```
#### <반환값>
 - 성공시 데이터 크기 반환 실패시에는 SOCKET_ERROR를 반환

### 데이터 송신 함수(send) 

```cpp
int send(
	SOCKET s,			// 대상과 연결된 소켓
	const char * buf,	// 보낼 데이터가 저장된 공간의 주소
	int len,			// 보낼데이터의 크기
	int flags	// send함수의 동작 변경 옵션,운영체제가 무시하기때문에 보통 0을 적는다
);
```
#### <반환값>
 - 성공시 데이터 크기 반환 실패시에는 SOCKET_ERROR를 반환

## Server code

```cpp
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

int main(void) {
	int err = 0;

	WSADATA ws = { NULL };

	WSAStartup(MAKEWORD(2, 2), &ws);
	//////////////////////////////////////////////////////// make socket

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET) {
		printf("소켓 생성 실패!!\n");
		return sock;
	}
	else {
		printf("소캣 생성 성공!!!\n");
	}

	//////////////////////////////////////////////////////// bind
	SOCKADDR_IN addr = { NULL };
	//memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12121);
	inet_pton(AF_INET, "192.168.0.88", &addr.sin_addr);

	err = bind(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (err != SOCKET_ERROR) {
		printf("바인드 성공!!\n");
	}
	else {
		printf("바인드 실패!!\n");
		return err;
	}
	////////////////////////////////////////////////////////// listen
	err = listen(sock, SOMAXCONN);
	if (err != SOCKET_ERROR) {
		printf("활성화 성공!!\n");
	}
	else {
		printf("활성화 실패!!\n");
		return err;
	}
	////////////////////////////////////////////////////////// accept
	SOCKET cls = NULL;
	SOCKADDR_IN clsaddr = { NULL };
	int clslen = sizeof(clsaddr);

	cls = accept(sock, (SOCKADDR*)&clsaddr, &clslen);

	if (cls == INVALID_SOCKET) {
		printf("클라이언트 소켓 생성 실패!!\n");
		return cls;
	}
	else {
		printf("클라이언트 소켓 생성 성공!!!\n");
	}
	////////////////////////////////////////////////////////// DATA
	char buf[128];

	int size = recv(cls, buf, sizeof(buf), 0);
	printf("수신 데이터의 크기 : %dbyte\n", size);
	printf("수신 데이터 : %s\n", buf);

	//////////////////////////////////////////////////////////close socket
	err = closesocket(sock);

	if (err != SOCKET_ERROR) {
		printf("종료 성공!!\n");
	}
	else {
		printf("종료 실패!!\n");
		return err;
	}
	///////////////////////////////////////////////////////////
	WSACleanup();
}
```

## Client Code

```cpp
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

int main(void) {
	int err;
	WSADATA ws = { NULL };

	//////////////////////////////////////////////////////// make socket
	WSAStartup(MAKEWORD(2, 2), &ws);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET) {
		printf("소켓 생성 실패!!\n");
		return sock;
	}
	else {
		printf("소캣 생성 성공!!!\n");
	}

	//////////////////////////////////////////////////////// connect
	SOCKADDR_IN addr = { NULL };
	//memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12121);
	inet_pton(AF_INET, "192.168.0.2", &addr.sin_addr);

	err = connect(sock, (SOCKADDR*)& addr, sizeof(addr));	// &addr은 Server의 SOCKADDR 정보랑 같음
	if (err != SOCKET_ERROR) {
		printf("연결 성공!!\n");
	}
	else {
		printf("연결 실패!!\n");
		return err;
	}
	////////////////////////////////////////////////////////// DATA
	char buf[128] = "끝이다 !!!!!!!";
	//int size = send(sock, buf, sizeof(buf), 0);
	int size = send(sock, buf, strlen(buf) + 1, 0);
	printf("%dbyte전송\n", size);

	////////////////////////////////////////////////////////// close socket

	err = closesocket(sock);

	if (err != SOCKET_ERROR) {
		printf("종료 성공!!\n");
	}
	else {
		printf("종료 실패!!\n");
		return err;
	}
	///////////////////////////////////////////////////////////
	WSACleanup();
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTk5MzA3NzM5MywtODU0NjUxMTQ3XX0=
-->