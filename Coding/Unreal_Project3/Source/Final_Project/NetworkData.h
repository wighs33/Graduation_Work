// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// winsock2 ����� ���� �Ʒ� �ڸ�Ʈ �߰�
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <map>

#include "Final_Project.h"

#define	MAX_BUFFER		4096
#define SERVER_PORT		8000
#define SERVER_IP		"127.0.0.1"
#define MAX_CLIENTS		100

const int  MAX_NAME_SIZE = 20;
const int  MAX_CHAT_SIZE = 100;
const int BUF_SIZE = 255;
const static int MAX_BUFF_SIZE = 255;

// ���� ��� ����ü

// ��Ŷ ����
enum CS_PacketType
{
	CS_PACKET_LOGIN = 1,
	CS_PACKET_MOVE,
	CS_PACKET_ATTACK,
	CS_PACKET_CHAT,
	CS_PACKET_TELEPORT
};

enum SC_PacketType
{
	SC_PACKET_LOGIN_OK = 1,
	SC_PACKET_MOVE,
	SC_PACKET_PUT_OBJECT,
	SC_PACKET_REMOVE_OBJECT,
	SC_PACKET_CHAT,
	SC_PACKET_LOGIN_FAIL,
	SC_PACKET_STATUS_CHANGE,
	SC_PACKET_DISCONNECT
};

// ��Ŷ

#pragma pack (push, 1)
struct cs_packet_login {
	unsigned char size;
	char	type;
	char	id[MAX_NAME_SIZE];
	char	pw[MAX_NAME_SIZE];
};

struct cs_packet_move {
	unsigned char size;
	char	type;
	char	direction;			// 0 : up,  1: down, 2:left, 3:right
	int		move_time;
};

struct cs_packet_attack {
	unsigned char size;
	char	type;
};

struct cs_packet_chat {
	unsigned char size;
	char	type;
	char	message[MAX_CHAT_SIZE];
};

struct cs_packet_teleport {
	// �������� ��ֹ��� ���� ���� ��ǥ�� �ڷ���Ʈ ��Ų��.
	// ���� Ŭ���̾�Ʈ���� ���� �׽�Ʈ������ ���.
	unsigned char size;
	char	type;
};
struct sc_packet_login_ok {
	unsigned char size;
	char type;
	char	name[MAX_NAME_SIZE];
	// ���� ���̵�
	int		pnum;
	// ��ġ
	float	x;
	float	y;
	float	z;
	// ȸ����
	float	Yaw;
	float	Pitch;
	float	Roll;
	// �ӵ�
	float VX;
	float VY;
	float VZ;

};


struct sc_packet_move {
	unsigned char size;
	char type;
	int		id;
	short  x, y;
	int		move_time;
};

struct sc_packet_put_object {
	unsigned char size;
	char type;
	int id;
	short x, y;
	char object_type;
	char	name[MAX_NAME_SIZE];
};

struct sc_packet_remove_object {
	unsigned char size;
	char type;
	int id;
};

struct sc_packet_chat {
	unsigned char size;
	char type;
	int id;
	char message[MAX_CHAT_SIZE];
};

struct sc_packet_login_fail {
	unsigned char size;
	char type;
	int	 reason;		// 0: �ߺ� ID,  1:����� Full
};

struct sc_packet_status_change {
	unsigned char size;
	char type;
	short	level;
	short	hp, maxhp;
	int		exp;
};



enum OPTYPE { OP_SEND, OP_RECV, OP_DO_MOVE };

class Overlap {
public:
	WSAOVERLAPPED   _wsa_over;
	OPTYPE         _op;
	WSABUF         _wsa_buf;
	unsigned char   _net_buf[BUF_SIZE];
	int            _target;
public:
	Overlap(OPTYPE _op, char num_bytes, void* mess) : _op(_op)
	{
		ZeroMemory(&_wsa_over, sizeof(_wsa_over));
		_wsa_buf.buf = reinterpret_cast<char*>(_net_buf);
		_wsa_buf.len = num_bytes;
		memcpy(_net_buf, mess, num_bytes);
	}

	Overlap(OPTYPE _op) : _op(_op) {}

	Overlap()
	{
		_op = OP_RECV;
	}

	~Overlap()
	{
	}
};

class FINAL_PROJECT_API NetworkData
{
public:
};
