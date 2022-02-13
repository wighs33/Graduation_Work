// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"
#include <sstream>
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformAffinity.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include <algorithm>
#include <string>

ClientSocket::ClientSocket()
	:StopTaskCounter(0)
{
}

ClientSocket::~ClientSocket()
{
	delete Thread;
	Thread = nullptr;

	closesocket(ServerSocket);
	WSACleanup();
}

bool ClientSocket::InitSocket()
{
	WSADATA wsaData;
	// ���� ������ 2.2�� �ʱ�ȭ
	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nRet != 0) {
		return false;
	}

	// TCP ���� ����	
	ServerSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (ServerSocket == INVALID_SOCKET) {
		return false;
	}

	return true;
}

bool ClientSocket::Connect(const char* pszIP, int nPort)
{
	// ������ ���� ������ ������ ����ü
	SOCKADDR_IN stServerAddr;

	stServerAddr.sin_family = AF_INET;
	// ������ ���� ��Ʈ �� IP
	stServerAddr.sin_port = htons(nPort);
	stServerAddr.sin_addr.s_addr = inet_addr(pszIP);

	int nRet = connect(ServerSocket, (sockaddr*)&stServerAddr, sizeof(sockaddr));
	if (nRet == SOCKET_ERROR) {
		return false;
	}

	return true;
}

void ClientSocket::SendTmp(Tmp& info, float x)
{
	// ��û ����
	info.type = EPacketType::SEND_PLAYER;
	info.x = x;

	// ĳ���� ���� ����
	int nSendLen = send(
		ServerSocket, (CHAR*)info, sizeof(info), 0
	);

	if (nSendLen == -1)
	{
		return;
	}
}
