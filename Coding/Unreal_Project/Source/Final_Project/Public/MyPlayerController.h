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

	cCharactersInfo* GetCharactersInfo() const { return charactersInfo; }
	ClientSocket* GetSocket() const { return mySocket; }
	void SetSessionId(const int sessionId) { iSessionId = sessionId; }
	void SetCharactersInfo(cCharactersInfo* ci_) { if (ci_ != nullptr)	charactersInfo = ci_; }
	void SetNewCharacterInfo(shared_ptr<cCharacter> NewPlayer_);
	void SetInitInfo(const cCharacter& me);
	void SetNewBall(const int s_id);

	void SendPlayerInfo(int input);
	//void UpdateFarming(int item_no);

	// ������ų �ٸ� ĳ����
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ACharacter> WhoToSpawn;

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
	queue<shared_ptr<cCharacter>>				newPlayers;			// �÷��̾� �α��� �� ĳ���� ����
	queue <int>				newBalls;
	cCharactersInfo*		charactersInfo;	// �ٸ� ĳ���͵��� ����

	FTimerHandle			SendPlayerInfoHandle;	// ����ȭ Ÿ�̸� �ڵ鷯

	bool							bNewPlayerEntered;
	bool							bInitPlayerSetting;

	

	//bool							bIsConnected;	// ������ ���� ����

};
