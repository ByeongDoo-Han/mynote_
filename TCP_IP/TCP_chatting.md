# TCP_chatting
## Server
```cpp
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

int main(void) {
	int err = 0;

	WSADATA ws = { NULL };

	WSAStartup(MAKEWORD(2, 2), &ws);

	/*make socket*/
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET) {
		printf("소켓 생성 실패\n");
		return sock;
	}
	else {
		printf("소켓 생성 성공\n");
	}

	/*bind*/
	SOCKADDR_IN addr = { NULL };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12121);
	inet_pton(AF_INET, "192.168.0.103", &addr.sin_addr);

	err = bind(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (err != SOCKET_ERROR) {
		printf("바인드 성공!\n");
	}
	else {
		printf("바인드 실패!\n");
		return err;
	}

	/*listen*/
	err = listen(sock, SOMAXCONN);
	if (err != SOCKET_ERROR) {
		printf("활성화 성공!\n");
	}
	else {
		printf("활성화 실패!\n");
		return err;
	}

	/*accept*/
	SOCKET cls = NULL;
	SOCKADDR_IN clsaddr = { NULL };
	int clslen = sizeof(clsaddr);

	cls = accept(sock, (SOCKADDR*)&clsaddr, &clslen);
	if (cls == INVALID_SOCKET) {
		printf("클라이언트 소켓 생성 실패!!\n");
		return cls;
	}
	else {
		printf("클라이너트 소켓 생성 성공!!\n");
	}

	/*DATA*/
	while (1) {
		char buf[128] = { NULL };

		int size = recv(cls, buf, sizeof(buf), 0);
		if (strcmp(buf, "EXIT") == 0) {
			/*close socket*/
			err = closesocket(sock);
			if (err != SOCKET_ERROR) {
				printf("종료 성공\n");
			}
			else {
				printf("종료 실패\n");
				return err;
			}
			break;
		}
		else {
			printf("수신 데이터의 크기 : %dbyte\n", size);
			printf("수신 데이터 : %s\n", buf);

			memset(buf, 0, sizeof(buf));
			printf("송신 데이터를 입력하세요 : ");
			scanf_s("%s", buf, sizeof(buf));
			size = send(cls, buf, strlen(buf) + 1, 0);
			if (strcmp(buf, "EXIT") == 0) {
				/*close socket*/
				err = closesocket(sock);
				if (err != SOCKET_ERROR) {
					printf("종료 성공\n");
				}
				else {
					printf("종료 실패\n");
					return err;
				}
				break;
			}
			printf("송신 데이터의 크기 : %dbyte\n", size);
		}
	}
	WSACleanup();
}
```

## client
```cpp
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

int main(void) {
	int err;
	WSADATA ws = { NULL };

	/*make socket*/
	WSAStartup(MAKEWORD(2, 2), &ws);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET) {
		printf("소켓 생성 실패!\n");
		return sock;
	}
	else {
		printf("소켓 생성 성공!!\n");
	}

	/*connect*/
	SOCKADDR_IN addr = { NULL };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12121);
	inet_pton(AF_INET, "192.168.0.103", &addr.sin_addr);

	err = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (err != SOCKET_ERROR) {
		printf("연결 성공!!\n");
	}
	else {
		printf("연결 실패!!\n");
		return err;
	}

	/*DATA*/
	while (1) {
		char buf[128] = { NULL };
		printf("송신 데이터 입력하세요 : ");
		scanf_s("%s", buf, sizeof(buf));
		int size = send(sock, buf, strlen(buf) + 1, 0);
		if (strcmp(buf, "EXIT") == 0) {
			/*close socket*/
			err = closesocket(sock);
			if (err != SOCKET_ERROR)
				printf("종료 성공!\n");
			else {
				printf("종료 실패!\n");
				return err;
			}
			break;
		}
		else {
			printf("송신 데이터의 크기 : %dbyte\n", size);
			memset(buf, 0, sizeof(buf));
			size = recv(sock, buf, sizeof(buf), 0);
			if (strcmp(buf, "EXIT") == 0) {
				/*close socket*/
				err = closesocket(sock);
				if (err != SOCKET_ERROR)
					printf("종료 성공!\n");
				else {
					printf("종료 실패!\n");
					return err;
				}
				break;
			}
			printf("받은 데이터 : %s\n", buf);
			printf("받은 데이터 크기 : %dbyte\n", size);
		}
	}
	WSACleanup();
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbOTY3ODE0MjI4XX0=
-->