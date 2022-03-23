#pragma once


const short SERVER_PORT = 8000;

const int BUFSIZE = 256;
const int  ReZone_HEIGHT = 2000;
const int  ReZone_WIDTH = 2000;
const int  MAX_NAME_SIZE = 20;
const int  MAX_CHAT_SIZE = 100;
const int  MAX_USER = 10000;
const int  MAX_OBJ = 20;

const char CS_PACKET_LOGIN = 1;
const char CS_PACKET_MOVE = 2;
const char CS_PACKET_ATTACK = 3;
const char CS_PACKET_CHAT = 4;
const char CS_PACKET_TELEPORT = 5;


const char SC_PACKET_LOGIN_OK = 1;
const char SC_PACKET_MOVE = 2;
const char SC_PACKET_PUT_OBJECT = 3;
const char SC_PACKET_REMOVE_OBJECT = 4;
const char SC_PACKET_CHAT = 5;
const char SC_PACKET_LOGIN_FAIL = 6;
const char SC_PACKET_STATUS_CHANGE = 7;
const char SC_PACKET_DISCONNECT = 8;

#pragma pack (push, 1)
struct cs_packet_login {
	unsigned char size;
	char	type;
	char	id[MAX_NAME_SIZE];
	char	pw[MAX_NAME_SIZE];

};

struct sc_packet_login_ok {
	unsigned char size;
	char type;
	char	name[MAX_NAME_SIZE];
	// 세션 아이디
	int		pnum;
	// 위치
	float	x;
	float	y;
	float	z;
	// 회전값
	float	Yaw;
	float	Pitch;
	float	Roll;
	// 속도
	float VX;
	float VY;
	float VZ;

};

struct cs_packet_start { // 게임 레디 요청
	unsigned char size;
	char	type;
	bool	ready;
};
struct sc_packet_ready { // 타 플레이어 레디
	unsigned char size;
	char	type;
	char	name[MAX_NAME_SIZE];
};

struct sc_packet_start_ok { // 스폰
	unsigned char size;
	char type;
	char	name[MAX_NAME_SIZE];
	float x, y, z;
	char image_num;
};

struct cs_packet_move {
	unsigned char size;
	char	type;
	//char	direction;			// 0 : up,  1: down, 2:left, 3:right
	float x, y, z;
	int		move_time;
};

struct cs_packet_attack {
	unsigned char size;
	char	type;
	float   Angle;
};

struct cs_packet_get_item {
	unsigned char size;
	char	type;
	char    item_num;
};

struct cs_packet_chat {
	unsigned char size;
	char	type;
	char	message[MAX_CHAT_SIZE];
};

struct cs_packet_teleport {
	// 서버에서 장애물이 없는 랜덤 좌표로 텔레포트 시킨다.
	// 더미 클라이언트에서 동접 테스트용으로 사용.
	unsigned char size;
	char	type;
};

struct sc_packet_put_object {
	unsigned char size;
	char type;
	int id;
	short x, y, z;
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
	int	 reason;		// 0: 중복 ID,  1:사용자 Full
};

struct sc_packet_status_change {
	unsigned char size;
	char type;
	short   state;
	short	hp, maxhp;
	bool ice[4]; // 사지분해
};

#pragma pack(pop)
