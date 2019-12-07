# UDP
## UDP 동작 순서

| Server  | Client |
|--|--|
| socket() | socket() |
| bind() |  |
| recvfrom() | sendto() |
| closesocket() | closesocket() |

## UDP 함수
### sendto()
 - UDP에서 데이터 전송 함수

```cpp
int sendto( 
	SOCKET s,					// 소켓
	const char * buf, 			// 버퍼의 주소
	int len, 					// 버퍼의 크기
	int flags, 					// 행동 옵션... 0
	const struct sockaddr * to, // 수신사의 소켓 주소
	int tolen					// 수신자 소켓의 크기
);				
```
### recvfrom()
 - UDP에서 데이터 수신 함수

```cpp
int recvfrom( 
	SOCKET s, 				// 소켓
	char * buf, 			// 버퍼의 주소
	int len, 				// 버퍼의 크기
	int flags, 				// 행동 옵션 ... 0
	struct sockaddr * from, // 송신자의 소켓 주소
	int * fromlen 			// 송신자 소켓의 크기를 저장한 공간의 크기
);
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbNzAzMjcxNjZdfQ==
-->