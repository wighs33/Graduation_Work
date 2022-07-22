#pragma once
#include "pch.h"
#include "CorePch.h"
#include "Enum.h"

class CLIENT
{
public:
	int _s_id; //�÷��̾� �迭 �ѹ�
	char name[MAX_NAME_SIZE]; //�÷��̾� nick
	char _id[MAX_NAME_SIZE]; // id
	char _pw[MAX_NAME_SIZE];  // pw
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

	float direction;

	int _max_hp; // �ִ� ü��
	int _min_hp;
	int _BeginSlowHP;	// ĳ���Ͱ� ���ο� ���°� �Ǳ� �����ϴ� hp

	int _hp; // ü��
	int _at_range; // �⺻ ���� ����
	int _sk_range; // ��ų ����
	bool is_bone;
	atomic_bool is_match;

	int32 iMaxSnowballCount;
	int32 iMaxMatchCount;
	int32 iOriginMaxMatchCount;	// ���� �ִ뺸���� (�ʱ�, ����x)
	int32 iOriginMaxSnowballCount;
	int32 iCurrentSnowballCount;
	int32 iCurrentMatchCount;
	bool bHasUmbrella;
	bool bIsRiding;
	bool bHasBag;
	bool bIsSnowman;	// ���� ĳ���Ͱ� ���������
	bool b_ready;
	atomic_bool dot_dam ;


	unordered_set   <int>  viewlist; // �þ� �� ������Ʈ
	mutex vl;
	mutex hp_lock;
	mutex lua_lock;

	mutex state_lock;
	CL_STATE _state;
	atomic_bool   _is_active;

	COMBAT _combat;

	atomic_int    _count;

	Overlap _recv_over;
	SOCKET  _socket;
	int      _prev_size;
	int      last_move_time;
public:
	CLIENT(); 
	~CLIENT()
	{
		closesocket(_socket);
	}
	void data_init();
	void do_recv();
	void do_send(int num_bytes, void* mess);

};

