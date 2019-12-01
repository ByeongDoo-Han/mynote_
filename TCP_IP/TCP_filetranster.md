# TCP_FILE_Transfer
## file_Server

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
	if (sock != INVALID_SOCKET) {
		printf("소켓 생성 성공\n");
	}
	else {
		printf("소켓 생성 실패!\n");
		return sock;
	}

	/*bind*/
	SOCKADDR_IN addr = { NULL };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12121);
	inet_pton(AF_INET, "192.168.0.88", &addr.sin_addr);

	err = bind(sock, (SOCKADDR*)& addr, sizeof(addr));
	if (err != SOCKET_ERROR) {
		printf("바인드 성공!!\n");
	}
	else {
		printf("바인드 실패!!\n");
		return err;
	}

	/*listen*/
	err = listen(sock, SOMAXCONN);
	if (err != SOCKET_ERROR) {
		printf("활성화 성공!!\n");
	}
	else {
		printf("활성화 실패!!\n");
		return err;
	}

	/*accept*/
	SOCKET cls = NULL;
	SOCKADDR_IN clsaddr = { NULL };
	int clslen = sizeof(clsaddr);

	cls = accept(sock, (SOCKADDR*)&clsaddr, &clslen);

	if (cls != INVALID_SOCKET) {
		printf("success to make Server socket !!\n");	
	}
	else {
		printf("fali to make Server socket!!\n");
		return cls;
	}

	/*DATA*/
	FILE *file;
	int fs;
	char* buf;
	if (fopen_s(&file, "png_example_copy.png", "wb") == 0) {

		recv(cls, (char*)& fs, sizeof(int), 0);
		buf = (char*)malloc(sizeof(char) * fs);

		recv(cls, buf, fs, 0);
		fwrite(buf, sizeof(char), fs, file);
		fclose(file);
	}

	//char buf[128];
	/*while (1) {
		memset(buf, 0, sizeof(buf));
		int size = recv(cls, buf, sizeof(buf), 0);
		int n = fwrite(buf, sizeof(char), size, file);
		if (n < 128) {
			printf("finish file transfer!\n");
			break;
		}
	}*/
	//fclose(file);

	/*close socket*/
	err = closesocket(sock);

	if (err != SOCKET_ERROR) {
		printf("종료 성공!!\n");
	}
	else {
		printf("종료 실패!!\n");
		return err;
	}

	WSACleanup();
}
```

## file_Client

```cpp
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int err;
	WSADATA ws = { NULL };

	WSAStartup(MAKEWORD(2, 2), &ws);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock != INVALID_SOCKET) {
		printf("success to make socket!!\n");
	}
	else {
		printf("fail to make socket!\n");
	}

	/*connect*/
	SOCKADDR_IN addr = { NULL };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12121);
	inet_pton(AF_INET, "192.168.0.88", &addr.sin_addr);

	err = connect(sock, (SOCKADDR*)& addr, sizeof(addr));
	if (err != SOCKET_ERROR) {
		printf("success to connect!!\n");
	}
	else {
		printf("fail to connect!!\n");
		return err;
	}

	/*DATA*/
	FILE* file;
	char* buf;

	if (fopen_s(&file, "png_example.png", "rb") == 0) {
		fseek(file, 0, SEEK_END);
		int fs = ftell(file);
		fseek(file, 0, SEEK_SET);
		buf = (char*)malloc(sizeof(char) * fs);
		memset(buf, 0, fs);
		
		fread(buf, sizeof(char), fs, file);
		
		send(sock, (char*)&fs, sizeof(int), 0);
		send(sock, buf, fs, 0);

		//char buf[128];
		/*while (1) {
			memset(buf, 0, sizeof(buf));
			int n = fread(buf, sizeof(char), sizeof(buf), file);
			int size = send(sock, buf, sizeof(buf), 0);
			if (n < 128) {
				printf("finish to send\n");
				break;
			}
		}*/

		free(buf);
		fclose(file);
	}

	/*close socket*/
	err = closesocket(sock);
	if (err != SOCKET_ERROR) {
		printf("success to close\n");
	}
	else {
		printf("fail to close\n");
		return err;
	}
	
	WSACleanup();
}
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTg2NDI2OTAyOV19
-->