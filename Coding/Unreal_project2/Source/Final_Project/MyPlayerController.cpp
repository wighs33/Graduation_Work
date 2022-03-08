// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

DWORD WINAPI ClientMain(LPVOID arg);
SOCKET clientSocket;

AMyPlayerController::AMyPlayerController()
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);
}

void AMyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// ���� ���ҽ� ��ȯ
	::closesocket(clientSocket);

	// ���� ����
	::WSACleanup();
}

DWORD WINAPI ClientMain(LPVOID arg)
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		MYLOG(Warning, TEXT("Failed to start wsa"));

	clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
		MYLOG(Warning, TEXT("Failed to start socket"));

	u_long on = 1;
	if (::ioctlsocket(clientSocket, FIONBIO, &on) == INVALID_SOCKET)
		MYLOG(Warning, TEXT("Failed to start iostlsocket"));

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	// Connect
	while (true)
	{
		if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{
			// ���� ����߾�� �ߴµ�... �ʰ� ����ŷ���� �϶��?
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			// �̹� ����� ���¶�� break
			if (::WSAGetLastError() == WSAEISCONN)
				break;
			// Error
			break;
		}
	}

	MYLOG(Warning, TEXT("Connected to Server!"));

	char sendBuffer[100] = "Hello World";
	WSAEVENT wsaEvent = ::WSACreateEvent();
	WSAOVERLAPPED overlapped = {};
	overlapped.hEvent = wsaEvent;

	// Send
	while (true)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = sendBuffer;
		wsaBuf.len = 100;

		DWORD sendLen = 0;
		DWORD flags = 0;
		if (::WSASend(clientSocket, &wsaBuf, 1, &sendLen, flags, &overlapped, nullptr) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSA_IO_PENDING)
			{
				// Pending
				::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
				::WSAGetOverlappedResult(clientSocket, &overlapped, &sendLen, FALSE, &flags);
			}
			else
			{
				// ��¥ ���� �ִ� ��Ȳ
				break;
			}
		}

		MYLOG(Warning, TEXT("Send Data ! Len = "));

		FPlatformProcess::Sleep(1.0f);
	}

	return 0;
}
