#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// winsock2 ����� ���� �Ʒ� �ڸ�Ʈ �߰�
#include <WinSock2.h>
#include <iostream>
#include <map>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <concurrent_priority_queue.h>
#pragma comment (lib, "WS2_32.LIB")
#pragma comment (lib, "MSWSock.LIB")

#include "Runtime/Core/Public/HAL/Runnable.h"

using namespace std;

#define	MAX_BUFFER		4096
#define SERVER_PORT		8000
#define SERVER_IP		"127.0.0.1"
#define MAX_CLIENTS		100

// ���� ��� ����ü
struct stSOCKETINFO
{
	WSAOVERLAPPED	overlapped;
	WSABUF			dataBuf;
	SOCKET			socket;
	char			messageBuffer[MAX_BUFFER];
	int				recvBytes;
	int				sendBytes;
};

// ��Ŷ ����
enum EPacketType
{
	LOGIN,
	ENROLL_PLAYER,
	SEND_PLAYER,
	RECV_PLAYER,
	LOGOUT_PLAYER,
	HIT_PLAYER,
	DAMAGED_PLAYER,
	CHAT,
	ENTER_NEW_PLAYER,
	SIGNUP,
	HIT_MONSTER,
	SYNC_MONSTER,
	SPAWN_MONSTER,
	DESTROY_MONSTER
};

struct Tmp {
	EPacketType type;
	float x;
};



class ARENABATTLE_API ClientSocket : public FRunnable
{
public:
	char 	recvBuffer[MAX_BUFFER];		// ���� ���� ��Ʈ��	
	SOCKET _socket;				// ������ ������ ����

	//atomic_int    _count;
	int      _type;  

	//Overlap _recv_over;
	
	int      _prev_size;
	int      last_move_time;

	ClientSocket();
	virtual ~ClientSocket();

	// ���� ��� �� ����
	bool InitSocket();
	// ������ ����
	bool Connect(const char* pszIP, int nPort);

	//////////////////////////////////////////////////////////////////////////
	// ������ ���
	//////////////////////////////////////////////////////////////////////////

	void do_recv()
	{
		/*DWORD recv_flag = 0;
		ZeroMemory(&_recv_over._wsa_over, sizeof(_recv_over._wsa_over));
		_recv_over._wsa_buf.buf = reinterpret_cast<char*>(_recv_over._net_buf + _prev_size);
		_recv_over._wsa_buf.len = sizeof(_recv_over._net_buf) - _prev_size;
		int ret = WSARecv(_socket, &_recv_over._wsa_buf, 1, 0, &recv_flag, &_recv_over._wsa_over, NULL);
		if (SOCKET_ERROR == ret) {
			int error_num = WSAGetLastError();
			if (ERROR_IO_PENDING != error_num)
				error_display(error_num);
		}*/
	}

	void do_send(int num_bytes, void* mess)
	{
		/*Overlap* ex_over = new Overlap(OP_SEND, num_bytes, mess);
		int ret = WSASend(_socket, &ex_over->_wsa_buf, 1, 0, 0, &ex_over->_wsa_over, NULL);
		if (SOCKET_ERROR == ret) {
			int error_num = WSAGetLastError();
			if (ERROR_IO_PENDING != error_num)
				error_display(error_num);
		}*/
	}


	// FRunnable Thread members	
	FRunnableThread* Thread;
	FThreadSafeCounter StopTaskCounter;


};
