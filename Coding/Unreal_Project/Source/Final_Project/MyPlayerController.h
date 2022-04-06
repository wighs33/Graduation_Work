// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <thread>
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

	// ???�레?�어 ?�데?�트
	void RecvNewPlayer(int sessionID, float x, float y, float z);
	void UpdateNewPlayer();		// ?�레?�어 ?�기??

	void UpdateNewPlayer(int new_s_id, float new_x, float new_y, float new_z);		// ?�레?�어 ?�기??
	void UpdatePlayerInfo(int input);
	void UpdatePlayerS_id(int _s_id);

	//void UpdateRotation();	// 카메???�칭(?�하) ?�한

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	ClientSocket* _cs;

	int _my_session_id;
	int _my_x;
	int _my_y;
	int _my_z;
	int _other_session_id;
	int _other_x;
	int _other_y;
	int _other_z;

	// ?�폰?�킬 ?�른 캐릭??
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ACharacter> WhoToSpawn;

protected:
	bool bNewPlayerEntered;
};
