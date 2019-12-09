# BroadCast & MultiCast
 - 1 : 多의 데이터를 주고 받기 위한 방식
 - UDP기반으로 구현된다


#### 브로드캐스트
 - 서로 연결된 아이피주소에서 데이터를 주고받는 방식...

#### 멀티캐스트
 - 같은 그룹에 가입된 모든 개체에게 데이터를 전송하는 방식

## Socket Options
- Socket의 기본 동작을 변경하거나 세부적인 제어를 하는 옵션

### setsockopt - 옵션값을 설정하는 함수

```cpp
int setsockopt( 
	SOCKET s, 
	int level, 
	int optname, 
	const char * optval, 
	int optlen 
);
```

#### <인자>
- SOCKET s - 옵션을 적용할 소켓
- int level - Socket Option Level
	 - IPPROTO_TCP : TCP사용을 위한 옵션
	 - IPPROTO_IP : 멀티캐스트를 위한 옵션
	 - SOL_SOCKET : 브로드 캐스트를  위한 옵션

- int optname - 옵션 이름 설정
	- const char * optval - 적용할 옵션의 값을 저장한 변수의 주소
	 - [BOOL] SO_BROADCAST  - 브로드캐스트 허용 
	 - [BOOL] SO_KEEPALIVE  - 연결 상태 확인
	 - [ linger{}] SO_LINGER - closesocket함수의 리턴 지연 시간 설정
	 - [int] SO_SNDBUF  / SO_RCVBUF - 소켓 송/수신 버퍼의 크기 설정
	 - [int] SO_SNDTIMEO /    SO_RCVTIMEO - 데이터 송/수신 함수 호출시 timeout값 설정
- int optlen - 옵션값을 저장한 변수의 크기

### getsockopt - 옵션 값을 받아오는 함수

```cpp
 int getsockopt( SOCKET s, int level, int optname, char * optval, int * optlen );
 ```
 
#### < 인자>
  - SOCKET s - 옵션이 적용된 소켓
  - int level - Socket Option Level
  - int optname - 옵션 이름
  - char * optval - 적용된 옵션의 값을 저장할 변수의 주소
  - int * optlen - 옵션값을 저장할 변수의 크기를 저장한 변수의 주소

## MultiCast

### Multicast
1. 같은 그룹에 가입된 모든 개체에게 데이터를 전송하는 방식
2. 그룹 가입과 탈퇴는 클라이언트쪽에서 한다
3. 그룹 구성원 모두가 평등하다
4. TTL 값은 최소 2로 설정된다
5. 브로드캐스트와 같이 멀티 캐스트는 사용시 예약된 IP주소를 사용한다
 - 224.0.0.0 ~ 239.255.255.255 범위가 예약된 IP주소 대역

### 사용하는 options
- IPPROTO_IP
	 - 소켓의 IP에 관한 설정을 하는 옵션 레벨
	 - MultiCast를 할때 이용하는 옵션

 - [IN_ADDR{}]  IP_MULTICAST_IF - 멀티 캐스트패킷을 보낼 인터페이스 선택
 - [BOOL] IP_MULTICAST_LOOP - 멀티캐스트 루프백 여부
 - [IP_MREQ{}] IP_ADD_MEMBERSHIP /    IP_DROP_MEMBERSHIP - 멀티 캐스트 그룹 가입 및 탈퇴
 - [int]  IP_MULTICAST_TTL -멀티캐스트의 TTL값 설정


### mreq 구조체

```c
typedef struct ip_mreq {
    IN_ADDR imr_multiaddr;  // 멀티 캐스트 그룹 주소
    IN_ADDR imr_interface;  // 자신의 IP주소
} IP_MREQ, *PIP_MREQ;
```











<!--stackedit_data:
eyJoaXN0b3J5IjpbMTg4NjMyMDc3NV19
-->