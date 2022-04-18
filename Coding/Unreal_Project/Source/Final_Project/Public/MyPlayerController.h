// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <thread>
#include <queue>
#include "Final_Project.h"
#include "MyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "ClientSocket.h"
#include "Kismet/KismetMathLibrary.h"

#include "MyPlayerController.generated.h"


/**
 *
 */
UCLASS()
class FINAL_PROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();

	void SetSessionId(const int sessionId) { iSessionId = sessionId; }
	void SetCharactersInfo(cCharactersInfo* ci_) { if (ci_ != nullptr)	CharactersInfo = ci_; }
	void SetNewCharacterInfo(shared_ptr<cCharacter> NewPlayer_);
	void SetInitInfo(const cCharacter& me);

	//void RecvNewPlayer(const cCharacter& info);
	//void RecvNewBall(int s_id);

	////void UpdateNewPlayer(int new_s_id, float new_x, float new_y, float new_z);
	void UpdatePlayerInfo(int input);
	//void UpdateFarming(int item_no);
	//void UpdatePlayerInfo(const cCharacter& info);		// �÷��̾� ����ȭ	
	//void Throw_Snow(FVector MyLocation, FVector MyDirection);
	//
	//void UpdatePlayerS_id(int _s_id);

	// ������ų �ٸ� ĳ����
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ACharacter> WhoToSpawn;

	//void UpdateRotation();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void InitPlayerSetting();
	bool UpdateWorldInfo();		// ���� ����ȭ
	void UpdateNewPlayer();

public:
	int							iSessionId;			// ĳ������ ���� ���� ���̵�
private:
	ClientSocket*			mySocket;
	cCharacter				initInfo;
	queue<shared_ptr<cCharacter>>				NewCharactersInfo;			// �÷��̾� �α��� �� ĳ���� ����
	cCharactersInfo*		CharactersInfo;	// �ٸ� ĳ���͵��� ����
	int							iPlayerCount;

	FTimerHandle			SendPlayerInfoHandle;	// ����ȭ Ÿ�̸� �ڵ鷯

	bool							bNewPlayerEntered;
	bool							bInitPlayerSetting;
	

	//bool							bIsConnected;	// ������ ���� ����
	//void UpdateNewBall();

	//void StartPlayerInfo(const cCharacter& info);
	//
	//int32 iMySessionId;
	//float fMy_x;
	//float fMy_y;
	//float fMy_z;
	//int32 iOtherSessionId;
	//float fOther_x;
	//float fOther_y;
	//float fOther_z;
	//// �� �÷��̾� ����
	//queue <int> iNewPlayers;
	//queue <int> iNewBalls;

	//int	nPlayers;

};
