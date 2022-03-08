//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "Final_Project.h"
//// winsock2 ����� ���� �Ʒ� �ڸ�Ʈ �߰�
//#pragma comment(lib, "ws2_32.lib")
//#include <WinSock2.h>
//#include <iostream>
//#include <chrono>
//#include <string>
//#include <map>
//#include "Runtime/Core/Public/HAL/Runnable.h"
//
//class AMyGameMode;
//class AMyPlayerController;
//
//using namespace std;
//
//#define	MAX_BUFFER		4096
//#define SERVER_PORT		8000
//#define SERVER_IP		"127.0.0.1"
//#define MAX_CLIENTS		100
//const int  MAX_NAME_SIZE = 20;
//const int  MAX_CHAT_SIZE = 100;
//const int BUF_SIZE = 255;
////const static int MAX_PACKET_SIZE = 255;
//const static int MAX_BUFF_SIZE = 255;
//// ���� ��� ����ü
//
//// ��Ŷ ����
//const char CS_PACKET_LOGIN = 1;
//const char CS_PACKET_MOVE = 2;
//const char CS_PACKET_ATTACK = 3;
//const char CS_PACKET_CHAT = 4;
//const char CS_PACKET_TELEPORT = 5;
//
//
//const char SC_PACKET_LOGIN_OK = 1;
//const char SC_PACKET_MOVE = 2;
//const char SC_PACKET_PUT_OBJECT = 3;
//const char SC_PACKET_REMOVE_OBJECT = 4;
//const char SC_PACKET_CHAT = 5;
//const char SC_PACKET_LOGIN_FAIL = 6;
//const char SC_PACKET_STATUS_CHANGE = 7;
//
//
//
//// ��Ŷ
////���� ��Ŷ�� ��� ����� ���о� �ͼ� �ϴ� ���⿡ ����
//#pragma pack (push, 1)
//struct cs_packet_login {
//	unsigned char size;
//	char	type;
//	char	name[MAX_NAME_SIZE];
//};
//
//struct cs_packet_move {
//	unsigned char size;
//	char	type;
//	char	direction;			// 0 : up,  1: down, 2:left, 3:right
//	int		move_time;
//};
//
//struct cs_packet_attack {
//	unsigned char size;
//	char	type;
//};
//
//struct cs_packet_chat {
//	unsigned char size;
//	char	type;
//	char	message[MAX_CHAT_SIZE];
//};
//
//struct cs_packet_teleport {
//	// �������� ��ֹ��� ���� ���� ��ǥ�� �ڷ���Ʈ ��Ų��.
//	// ���� Ŭ���̾�Ʈ���� ���� �׽�Ʈ������ ���.
//	unsigned char size;
//	char	type;
//};
//
//struct sc_packet_login_ok {
//	unsigned char size;
//	char type;
//	int		id;
//	short	x, y;
//	short	level;
//	short	hp, maxhp;
//	int		exp;
//};
//
//struct sc_packet_move {
//	unsigned char size;
//	char type;
//	int		id;
//	short  x, y;
//	int		move_time;
//};
//
//struct sc_packet_put_object {
//	unsigned char size;
//	char type;
//	int id;
//	short x, y;
//	char object_type;
//	char	name[MAX_NAME_SIZE];
//};
//
//struct sc_packet_remove_object {
//	unsigned char size;
//	char type;
//	int id;
//};
//
//struct sc_packet_chat {
//	unsigned char size;
//	char type;
//	int id;
//	char message[MAX_CHAT_SIZE];
//};
//
//struct sc_packet_login_fail {
//	unsigned char size;
//	char type;
//	int	 reason;		// 0: �ߺ� ID,  1:����� Full
//};
//
//struct sc_packet_status_change {
//	unsigned char size;
//	char type;
//	short	level;
//	short	hp, maxhp;
//	int		exp;
//};
//
//static HANDLE g_hiocp;
//
//enum OPTYPE { OP_SEND, OP_RECV, OP_DO_MOVE };
//
//
//// �÷��̾� ����
//class cCharacter {
//public:
//	cCharacter() {};
//	~cCharacter() {};
//
//	// ���� ���̵�
//	int _Id;
//	// ��ġ
//	int m_x, m_y, m_z;
//	//Ÿ��
//	int _type; //������Ʈ Ÿ��
//	// ü��
//	int _max_hp; // �ִ� ü��
//	int _hp; // ü��
//	// ȸ����
//	float Yaw;
//	float Pitch;
//	float Roll;
//	// �ӵ�
//	float VX;
//	float VY;
//	float VZ;
//	// �Ӽ�
//	bool	IsAlive;
//	float	HealthValue;
//	bool	IsAttacking;
//	bool    m_showing; //�÷��̾� �þ߿� ǥ�ÿ���
//	chrono::system_clock::time_point m_mess_end_time;
//
//
//	cCharacter(int x, int y, int z)
//	{
//		m_showing = false;
//		set_name("NONAME");
//		m_mess_end_time = chrono::system_clock::now();
//	}
//	void set_name(const char str[]) {}
//	void show()
//	{
//		m_showing = true;
//	}
//	void hide()
//	{
//		m_showing = false;
//	}
//
//	void a_move(int x, int y) {
//
//	}
//
//	void a_draw() {
//
//	}
//
//	void move(int x, int y) {
//		;
//	}
//	friend ostream& operator<<(ostream& stream, cCharacter& info)
//	{
//		/*stream << info.SessionId << endl;
//		stream << info.X << endl;
//		stream << info.Y << endl;
//		stream << info.Z << endl;
//		stream << info.VX << endl;
//		stream << info.VY << endl;
//		stream << info.VZ << endl;
//		stream << info.Yaw << endl;
//		stream << info.Pitch << endl;
//		stream << info.Roll << endl;
//		stream << info.IsAlive << endl;
//		stream << info.HealthValue << endl;
//		stream << info.IsAttacking << endl;*/
//
//		return stream;
//	}
//
//	friend istream& operator>>(istream& stream, cCharacter& info)
//	{
//		/*stream >> info.SessionId;
//		stream >> info.X;
//		stream >> info.Y;
//		stream >> info.Z;
//		stream >> info.VX;
//		stream >> info.VY;
//		stream >> info.VZ;
//		stream >> info.Yaw;
//		stream >> info.Pitch;
//		stream >> info.Roll;
//		stream >> info.IsAlive;
//		stream >> info.HealthValue;
//		stream >> info.IsAttacking;*/
//
//		return stream;
//	}
//};
//
//// �÷��̾� ����ȭ/������ȭ Ŭ����
//class cCharactersInfo
//{
//public:
//	cCharactersInfo() {};
//	~cCharactersInfo() {};
//
//	map<int, cCharacter> players;
//
//	friend ostream& operator<<(ostream& stream, cCharactersInfo& info)
//	{
//		stream << info.players.size() << endl;
//		for (auto& kvp : info.players)
//		{
//			stream << kvp.first << endl;
//			stream << kvp.second << endl;
//		}
//
//		return stream;
//	}
//
//	friend istream& operator>>(istream& stream, cCharactersInfo& info)
//	{
//		int nPlayers = 0;
//		int SessionId = 0;
//		cCharacter Player;
//		info.players.clear();
//
//		stream >> nPlayers;
//		for (int i = 0; i < nPlayers; i++)
//		{
//			stream >> SessionId;
//			stream >> Player;
//			info.players[SessionId] = Player;
//		}
//
//		return stream;
//	}
//};
//
//
//struct OverlappedEx {
//	WSAOVERLAPPED over;
//	WSABUF wsabuf;
//	unsigned char IOCP_buf[MAX_BUFF_SIZE];
//	OPTYPE event_type;
//	int event_target;
//};
//
//class FINAL_PROJECT_API ClientSocket
//{
//public:
//	int _myid;
//	int* _m_hp;
//	int* _hp;
//	int* _level;
//	int* _exp;
//	int _x_origin;
//	int _y_origin;
//	bool _login_ok;
//	char _chat_buf[BUF_SIZE];
//	// ��ġ
//	int m_x, m_y, m_z;
//	string _name;
//
//	atomic_bool connected;
//
//	SOCKET _socket;
//	OverlappedEx recv_over;
//	unsigned char packet_buf[MAX_PACKET_SIZE];
//	int prev_packet_data;
//	int curr_packet_size;
//	//high_resolution_clock::time_point last_move_time;
//
//	void client_initialize();
//	void client_finish();
//	void ProcessPacket(char* ptr);
//	void process_data(char* net_buf, size_t io_byte);
//	bool client_main();
//	void send_move_packet(char dr);
//	void send_attack_packet();
//	void send_login_packet();
//	//bool Connect();
//	bool InitSocket();
//	bool Connect(const char* pszIP, int nPort);
//	void SendPacket(void* packet);
//
//	//virtual uint32 Run();
//
//	// �̱��� ��ü ��������
//	static ClientSocket* GetSingleton()
//	{
//		static ClientSocket ins;
//		return &ins;
//	}
//
//	ClientSocket() {
//		wcout.imbue(locale("korean"));
//		WSADATA WSAData;
//		WSAStartup(MAKEWORD(2, 2), &WSAData);
//		_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
//
//	};
//	~ClientSocket();
//};
//
//
