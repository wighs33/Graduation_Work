#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS

const short SERVER_PORT = 9090;

const int32 BUFSIZE = 2048;
const int32  ReZone_HEIGHT = 2000;
const int32  ReZone_WIDTH = 2000;
const int32  MAX_NAME_SIZE = 20;
const int32  MAX_CHAT_SIZE = 100;
const int32  MAX_USER = 10;
const int32  MAX_OBJ = 20;
const int32  MAX_SNOWDRIFT = 1000;

const int32  MAX_ITEM = 100;
const int32  MAX_BULLET_RANG = 8;

const char CS_PACKET_LOGIN = 1;
const char CS_PACKET_MOVE = 2;
const char CS_PACKET_ATTACK = 3;
const char CS_PACKET_CHAT = 4;
const char CS_PACKET_TELEPORT = 5;
const char CS_PACKET_THROW_SNOW = 6;
const char CS_PACKET_DAMAGE = 7;
const char CS_PACKET_GET_ITEM = 8;
const char CS_PACKET_LOGOUT = 9;
const char CS_PACKET_STATUS_CHANGE = 10;
const char CS_PACKET_READY = 11;
const char CS_PACKET_STOP_SNOW_FARMING = 12;
const char CS_PACKET_MATCH = 13;
const char CS_PACKET_OPEN_BOX = 14;
const char CS_PACKET_GUNATTACK = 15;
const char CS_PACKET_GUNFIRE = 16;
const char CS_PACKET_UMB = 17;


const char SC_PACKET_LOGIN_OK = 1;
const char SC_PACKET_MOVE = 2;
const char SC_PACKET_PUT_OBJECT = 3;
const char SC_PACKET_REMOVE_OBJECT = 4;
const char SC_PACKET_CHAT = 5;
const char SC_PACKET_LOGIN_FAIL = 6;
const char SC_PACKET_STATUS_CHANGE = 7;
const char SC_PACKET_DISCONNECT = 8;
const char SC_PACKET_HP = 9;
const char SC_PACKET_THROW_SNOW = 10;
const char SC_PACKET_ATTACK = 11;
const char SC_PACKET_GET_ITEM = 12;
const char SC_PACKET_READY = 13;
const char SC_PACKET_START = 14;
const char SC_PACKET_STOP_SNOW_FARMING = 15;
const char SC_PACKET_IS_BONE = 16;
const char SC_PACKET_LOGOUT = 17;
const char SC_PACKET_END = 18;
const char SC_PACKET_OPEN_BOX = 19;
const char SC_PACKET_GUNATTACK = 20;
const char SC_PACKET_GUNFIRE = 21;
const char SC_PACKET_TELEPORT = 22;
const char SC_PACKET_UMB = 23;


#pragma pack (push, 1)
struct cs_packet_login {
	unsigned char size;
	char	type;
	char	id[MAX_NAME_SIZE];
	char	pw[MAX_NAME_SIZE];
	float z;
};

struct sc_packet_login_ok {
	unsigned char size;
	char type;
	// 세션 아이디
	int32		s_id;
	float x, y, z;
	float yaw;
};

struct cs_packet_logout {
	unsigned char size;
	char	type;
	int32     s_id;
};

struct cs_packet_move {
	unsigned char size;
	char	type;
	//char	direction;			// 0 : up,  1: down, 2:left, 3:right
	int32 sessionID;
	float x, y, z;
	// 속도
	float vx, vy, vz;
	// 회전값
	float yaw;
	float direction;
	//long long move_time;
};

struct sc_packet_put_object {
	unsigned char size;
	char type;
	int32 s_id;
	float x, y, z;
	float yaw;
	char object_type;
	char	name[MAX_NAME_SIZE];
};


struct sc_packet_get_item {
	unsigned char size;
	char	type;
	int32     s_id;;
	int32     item_type;
	int32     destroy_obj_id;
};

struct cs_packet_throw_snow {
	unsigned char size;
	char	type;
	int32 s_id;
	bool    mode;
	float x, y, z;
	float dx, dy, dz;
};

struct cs_packet_fire {
	unsigned char size;
	char	type;
	int32 s_id;
	float pitch;
    int32 rand_int[MAX_BULLET_RANG];
};

struct cs_packet_damage {
	unsigned char size;
	char type;
};

struct sc_packet_hp_change {
	unsigned char size;
	char type;
	int32 s_id;
	int32 hp;
};

struct cs_packet_ready { // 게임 레디 요청
	unsigned char size;
	char	type;
};
struct sc_packet_ready { // 타 플레이어 레디
	unsigned char size;
	char	type;
	int32	s_id;
};

struct sc_packet_start { // 스폰
	unsigned char size;
	char type;

};

struct cs_packet_attack {
	unsigned char size;
	char	type;
	int32 s_id;
};

struct cs_packet_get_item {
	unsigned char size;
	char	type;
	int32 s_id;
	int32 item_type;
	int32 current_snowball;
	int32 destroy_obj_id;
};

struct cs_packet_open_box {
	unsigned char size;
	char	type;
	int32 open_obj_id;
};

//struct cs_packet_stop_snow_farming {
//	unsigned char size;
//	char	type;
//	int32 s_id;
//};

struct cs_packet_chat {
	unsigned char size;
	char	type;
	int32 s_id;
	int32 cheat_type;
};


struct cs_packet_teleport {
	unsigned char size;
	char	type;
	int	    Point;
};

struct cs_packet_match {
	unsigned char size;
	char	type;
};
struct cs_packet_umb {
	unsigned char size;
	char	type;
	int     s_id;
	bool    end;

};
struct sc_packet_is_bone {
	unsigned char size;
	char	type;
};

struct sc_packet_game_end {
	unsigned char size;
	char	type;
	int     s_id;
};


struct sc_packet_remove_object {
	unsigned char size;
	char type;
	int32 s_id;
};

struct sc_packet_chat {
	unsigned char size;
	char type;
	int32 id;
	char message[MAX_CHAT_SIZE];
};

struct sc_packet_login_fail {
	unsigned char size;
	char type;
	int32	 reason;		// 0: 중복 ID,  1:사용자 Full
};

struct sc_packet_status_change {
	unsigned char size;
	char type;
	int32 s_id;
	short   state;
};
#pragma pack(pop)
