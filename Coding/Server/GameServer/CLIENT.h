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

	int _hp; // ü��
	int _at_range; // �⺻ ���� ����
	int _sk_range; // ��ų ����
	bool is_bone;
	atomic_bool is_match;

	int32 iMaxSnowballCount;
	int32 iMaxIceballCount;
	int32 iMaxMatchCount;
	const int32 _max_hp = 390;	// ���� �ִ뺸���� (�ʱ�)
	const int32 _min_hp = 270;// �� �ִ뺸���� (�ʱ�)
	const int32 _BeginSlowHP = 300;	// ĳ���Ͱ� ���ο� ���°� �Ǳ� �����ϴ� hp

	const int32 iOriginMaxMatchCount = 2;	// ���� �ִ뺸���� (�ʱ�)
	const int32 iOriginMaxSnowballCount = 10;// �� �ִ뺸���� (�ʱ�)
	const int32 iOriginMaxIceballCount = 10;     // ���� �ִ뺸���� (�ʱ�)

	const int32 iBagMaxMatchCount = 3;     // ���� �ִ뺸���� (����)
	const int32 iBagMaxSnowballCount = 15;// �� �ִ뺸���� (����)
	const int32 iBagMaxIceballCount = 15;     // ���� �ִ뺸���� (����)

	int32 iCurrentSnowballCount;
	int32 iCurrentIceballCount;
	int32 iCurrentMatchCount;
	bool bHasUmbrella;
	bool bIsRiding;
	bool bHasBag;
	bool bHasShotGun;
	bool bIsSnowman;	// ���� ĳ���Ͱ� ���������
	bool b_ready;
	atomic_bool dot_dam ;


	unordered_set   <int>  viewlist; // �þ� �� ������Ʈ
	mutex vl;
	mutex hp_lock;
	mutex lua_lock;
	mutex state_lock;
	mutex cl_state_lock;
	mutex pl_state_lock;


	CL_STATE cl_state;  //  ���� ����
	STATE  pl_state;    //�ΰ��� ����
	atomic_bool   _is_active;

	ATTACK _combat;

	atomic_int    _count;

	Overlap _recv_over;
	SOCKET  _socket;
	int      _prev_size;
	int      last_move_time;
	int      color;
public:
	CLIENT(); 
	~CLIENT()
	{
		closesocket(_socket);
	}
	void data_init();
	void do_recv();
	void do_send(int num_bytes, void* mess);
	
	bool is_cl_state(CL_STATE value)
	{
		lock_guard<mutex> lock(cl_state_lock);		
		return (value == cl_state);
	}
	bool is_pl_state(STATE value)
	{
		lock_guard<mutex> lock(pl_state_lock);
		return (value == pl_state);
	}

	void set_cl_state(CL_STATE value)
	{
		lock_guard<mutex> lock(cl_state_lock);
	    cl_state = value;
	}

	void set_pl_state(STATE value)
	{
		lock_guard<mutex> lock(pl_state_lock);
		pl_state = value;
	}
};

