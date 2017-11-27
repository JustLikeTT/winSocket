// winSocketClient.cpp: �w�q�D���x���ε{�����i�J�I�C

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
	
	//�}�l Winsock-DLL
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 1), &wsaData);

	//�ŧi�� socket �ϥΪ� sockadder_in ���c
	SOCKADDR_IN addr;
	int addlen = sizeof(addr);

	//�]�w socket
	SOCKET sConnect;
	sConnect = socket(AF_INET, SOCK_STREAM, NULL);

	//�]�w���s�u��Server����}��T
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
				//�o�e�T���� server ��
				scanf("%s", string);
				if (!strcmp(string, "exit"))
					break;		
				byteSend = send(sConnect, string, sizeof(string), 0);

				//���� server �ݪ��T��
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
