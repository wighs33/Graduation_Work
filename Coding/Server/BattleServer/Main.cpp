//==================================================================================================================================
//�� ������ ��Ī�� �÷��̾���� �ΰ��� ������ ���� �մϴ�. 
// FPS ������ ���� �÷��̵Ǵ� ����
//==================================================================================================================================

#include "pch.h"
#include "CorePch.h"
#include "CLIENT.h"
#include "Overlap.h"
#include "LockQueue.h"
//#include "GameDataBase.h"
#include "BattleServer.h"

int main(int argc, char* argv[])
{

	wcout.imbue(locale("korean"));
	short server_port = -1;
	if (argc > 1)
		server_port = atoi(argv[1]);
	//Init_DB();
	server.Init(MAX_TH);
	if (server_port != -1)
	    server.BindListen(server_port);
	else
		server.BindListen(SERVER_PORT);
	server.Connect();

	server.Run();
	server.End();
	//DB_Rel();

}
