// winSocketClient.cpp: 定義主控台應用程式的進入點。

#include "stdafx.h"
#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string.h>
int sendMes(SOCKET,char*);

void main()
{
	char confirm;
	
	//開始 Winsock-DLL
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 1), &wsaData);

	//宣告給 socket 使用的 sockadder_in 結構
	SOCKADDR_IN addr;
	int addlen = sizeof(addr);

	//設定 socket
	SOCKET sConnect;
	sConnect = socket(AF_INET, SOCK_STREAM, NULL);

	//設定欲連線的Server的位址資訊
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);

	printf("connect to server?[Y] or [N] --> ");
	scanf_s("%c", &confirm, 1);
	if (confirm == 'N')
	{
		exit(1);
	}
	else {
		if (confirm == 'Y')
		{
			connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));

			char string[100] ;
			int byteSend, byteRecv;
			
			while (true)
			{
				//發送訊息到 server 端
				scanf("%s", string);
				if (!strcmp(string, "exit"))
					break;		
				byteSend = send(sConnect, string, sizeof(string), 0);

				//接收 server 端的訊息
				char *recvMsg = " ";
				recvMsg = (char*)calloc(1000, sizeof(char));
				byteRecv = recv(sConnect, recvMsg, sizeof(recvMsg), 0);
				if (byteRecv == 0)
					printf("Server is closed\n");
				else if (byteRecv == -1)
					printf("Recv error\n");
				else
					printf("recv byte :%d\t", byteRecv);
			/*	for (int i = 0; i < sizeof(recvMsg); i++)
				{
					printf("%c", *(recvMsg + i));
					if (i % 100 == 0)
						printf("\n");
				}
				printf("\n");*/
				printf("\nThe message from server: %s\n", recvMsg);
				//memset(recvMsg, 0, 1000);
			}	
		}
	}

	closesocket(sConnect);
	WSACleanup();
	printf("bye bye...\n");
	system("pause");
}
