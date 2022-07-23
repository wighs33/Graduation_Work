#include "pch.h"
#include "CLIENT.h"

CLIENT::CLIENT() : _state(ST_FREE), _combat(COMBAT_END), _prev_size(0)
{
	_s_id = {}; //�÷��̾� �迭 �ѹ�
	// ��ġ
	x = {};
	y = {};
	z = {};
	// ȸ����
	Yaw = {};
	Pitch = {};
	Roll = {};
	// �ӵ�
	VX = {};
	VY = {};
	VZ = {};
	direction = {};
	_max_hp = 390; // �ִ� ü��
	_min_hp = 270;
	_BeginSlowHP = 300;	// ĳ���Ͱ� ���ο� ���°� �Ǳ� �����ϴ� hp

	_hp = {}; // ü��
	_at_range = 1; // �⺻ ���� ����
	_sk_range = 2; // ��ų ����
	is_bone = true;
	is_match = false;
	iMaxSnowballCount = 10;
	iMaxMatchCount = 3;
	iOriginMaxMatchCount = 2;	// ���� �ִ뺸���� (�ʱ�, ����x)
	iOriginMaxSnowballCount = 10;
	iCurrentSnowballCount = 0;
	iCurrentMatchCount = 0;
	bHasUmbrella = false;
	bIsRiding = false;
	bHasBag = false;
	bIsSnowman = false;	// ���� ĳ���Ͱ� ���������
	b_ready = false;
	dot_dam = false;
	viewlist = {}; // �þ� �� ������Ʈ
	_is_active = false;
	_count = {};
	_recv_over = {};
	_prev_size = {};
	last_move_time = {};
}

void CLIENT::do_recv()
{
	DWORD recv_flag = 0;
	ZeroMemory(&_recv_over._wsa_over, sizeof(_recv_over._wsa_over));
	_recv_over._wsa_buf.buf = reinterpret_cast<char*>(_recv_over._net_buf + _prev_size);
	_recv_over._wsa_buf.len = sizeof(_recv_over._net_buf) - _prev_size;
	int ret = WSARecv(_socket, &_recv_over._wsa_buf, 1, 0, &recv_flag, &_recv_over._wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)
			error_display(error_num);
	}
}

void CLIENT::do_send(int num_bytes, void* mess)
{
	Overlap* ex_over = new Overlap(COMMAND::OP_SEND, num_bytes, mess);
	int ret = WSASend(_socket, &ex_over->_wsa_buf, 1, 0, 0, &ex_over->_wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)
			error_display(error_num);
	}
}

void CLIENT::data_init()
{
	int _max_hp = 390; // �ִ� ü��
	int _min_hp = 270;
	int _BeginSlowHP = 300;	// ĳ���Ͱ� ���ο� ���°� �Ǳ� �����ϴ� hp
	int _hp = _max_hp; // ü�� ����
	bool is_bone = true;
	atomic_bool is_match = false;
	atomic_bool   _is_active = false;
	int32 iMaxSnowballCount = 10;
	int32 iOriginMaxSnowballCount = 10;	// ������ �ִ뺸���� (�ʱ�, ����x)
	int32 iOriginMaxMatchCount = 2;	// ���� �ִ뺸���� (�ʱ�, ����x)
	int32 iMaxMatchCount = 3;
	int32 iCurrentSnowballCount = 0;
	int32 iCurrentMatchCount = 0;
	bool bHasUmbrella = false;
	bool bHasBag = false;
	bool bIsSnowman = false;	// ���� ĳ���Ͱ� ���������
	bool b_ready = false;
}
