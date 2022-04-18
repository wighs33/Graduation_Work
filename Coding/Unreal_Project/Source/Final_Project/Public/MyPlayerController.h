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

	void RecvNewPlayer(const cCharacter& info);
	void RecvNewBall(int s_id);

	//void UpdateNewPlayer(int new_s_id, float new_x, float new_y, float new_z);
	void UpdatePlayerInfo(int input);
	void UpdateFarming(int item_no);
	void SendPlayerInfo();		// �÷��̾� ��ġ �۽�
	bool UpdateWorldInfo();		// ���� ����ȭ
	void UpdatePlayerInfo(const cCharacter& info);		// �÷��̾� ����ȭ	
	void Throw_Snow(FVector MyLocation, FVector MyDirection);
	
	void UpdatePlayerS_id(int _s_id);

	// ������ų �ٸ� ĳ����
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ACharacter> WhoToSpawn;

	//void UpdateRotation();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	ClientSocket* myClientSocket;
	cCharactersInfo* CharactersInfo;	// �ٸ� ĳ������ ����
	cCharacter* NewPlayer;
	void UpdateNewPlayer();
	void UpdateNewBall();

	void StartPlayerInfo(const cCharacter& info);

	void RecvWorldInfo(cCharactersInfo* ci_)
	{
		if (ci_ != nullptr)
		{
			CharactersInfo = ci_;
		}

	}
	bool				bIsConnected;	// ������ ���� ����
	
	int32 iMySessionId;
;
	float fMy_x;
	float fMy_y;
	float fMy_z;
	int32 iOtherSessionId;
	float fOther_x;
	float fOther_y;
	float fOther_z;
	bool bSetPlayer = false;
	// �� �÷��̾� ����
	queue <int> iNewPlayers;
	queue <int> iNewBalls;

	int	nPlayers;
	//UPROPERTY(EditAnywhere, Category = "Spawning")
	//TSubclassOf<class AMyCharacter> WhoToSpawn;

};
