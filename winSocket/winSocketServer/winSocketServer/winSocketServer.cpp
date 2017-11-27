// winSocketServer.cpp: 定義主控台應用程式的進入點。
//
#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <stdio.h>
#define MAX_CONNECTIONS 10

#pragma comment(lib, "Ws2_32.lib")

DWORD WINAPI RunForClientThread(LPVOID);
SOCKET connections[MAX_CONNECTIONS];
int connection_count = 0;

int main()
{
	DWORD ThreadID;
	HANDLE ThreadHandle;

	//用WSAStartup開始Winsock-DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 1), &wsaData); //MAKEWORD(2, 1)為Winsocket-DLL版本

										  //宣告socket位址資訊
	struct sockaddr_in addr;
	int addrLen = sizeof(addr);

	//建立socket
	SOCKET sConnect;
	sConnect = socket(AF_INET, SOCK_STREAM, NULL);

	//設定位址資訊的資料
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);	//協定 
		addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);

	//設定監聽Listen Socket
	SOCKET sListen;
	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (sockaddr*)&addr, addrLen);
	listen(sListen, SOMAXCONN);

	//宣告clientAddr儲存client的位址資訊
	SOCKADDR_IN clientAddr;
	printf("Server starting...\n");
	while (true)
	{
		//等待client連線
		if (sConnect = accept(sListen, (SOCKADDR*)&clientAddr, &addrLen))
		{
			printf("a connection was found!!\n");

			//檢查是否有未建立連線的Socket可用
			int sokcet_index = -1;
			for (int i = 0; i<MAX_CONNECTIONS; i++)
			{
				if (connections[i] == 0)
				{
					sokcet_index = i;
					break;
				}
			}
			if (sokcet_index == -1)
			{
				printf("Connection full... \n");
				return 1;
			}
			connections[sokcet_index] = sConnect;
			connection_count++;

			ThreadHandle = CreateThread(NULL, 0, RunForClientThread, &sokcet_index, 0, &ThreadID);
		}
	}

	for (int i = 0; i<MAX_CONNECTIONS; i++)
		closesocket(connections[i]);
	WSACleanup();
	return 0;
}

DWORD WINAPI RunForClientThread(LPVOID input_sIndex) {

	char sendbuf[200];
	int socketIndex = *(int*)input_sIndex;

	//傳送訊息給 client 端
	//strcpy_s(sendbuf, "Hello client!! ");
	//send(connections[socketIndex], sendbuf, (int)strlen(sendbuf), 0);

	//接收 client 端的訊息, 
	int dataByte;
	do
	{
		char *recvBuf=" ";
		recvBuf = (char*)calloc(1000, sizeof(char));
		dataByte = recv(connections[socketIndex], recvBuf, sizeof(sendbuf), 0);
		if (dataByte == 0)
			printf("Server is closed\n");
		else if (dataByte < 0)
			printf("%d\n",WSAGetLastError());
		else
			printf("recv byte :%d\t", dataByte);
		printf("from user %d : %s\n", socketIndex, recvBuf);
		int sendByte;
		sendByte = send(connections[socketIndex], recvBuf, (int)strlen(sendbuf), 0);
		if (sendByte == SOCKET_ERROR)
		{
			printf("%d\n",WSAGetLastError());
		}
		printf("%d\n", sendByte);
		memset(recvBuf, 0, 1000);
	} while (dataByte > 0);
	
	//printf("%d : %s", socktIndex, sendbuf);
	return 0;
}