//==================================================================================================================================
//�� ������ �÷��̾���� ��Ī �� ���̴� �κ� ���� �մϴ�
//==================================================================================================================================

#include "pch.h"
#include "CorePch.h"
#include "CLIENT.h"
#include "Overlap.h"
#include "LockQueue.h"
#include "GameDataBase.h"
#include "LobbyServer.h"
#include "BattleServer.h"

int main()
{

	wcout.imbue(locale("korean"));
	LobbyServer server;
	Init_DB();
	server.Init(MAX_TH);
	server.BindListen(SERVER_PORT);
	server.Run();
	server.End();
	DB_Rel();

}
