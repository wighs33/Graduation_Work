// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <thread>
#include "Final_Project.h"
#include "MyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "ClientSocket.h"

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
	ClientSocket* _cs;
	int _my_session_id;
	int _my_x;
	int _my_y;
	int _my_z;
	int _other_session_id;
	int _other_x;
	int _other_y;
	int _other_z;

	// ������ų �ٸ� ĳ����
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ACharacter> WhoToSpawn;

	// �� �÷��̾� ������Ʈ
	void RecvNewPlayer(int sessionID, float x, float y, float z);

	void UpdateNewPlayer();		// �÷��̾� ����ȭ
	void UpdatePlayerInfo(int input);
	void UpdatePlayerS_id(int _s_id);

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool bNewPlayerEntered;
};
