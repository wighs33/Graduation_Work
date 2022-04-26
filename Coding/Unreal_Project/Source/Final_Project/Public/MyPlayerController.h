// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <thread>
#include <atomic>
//#include <queue>
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
	UFUNCTION(BlueprintCallable)
	void PlayerReady();	// ��Ʈ�ѷ��� ������ �÷��̾ �����ϸ� ȣ��Ǵ� �Լ�
	UFUNCTION(BlueprintCallable)
	void PlayerUnready();
	void StartGame();	// ��� �÷��̾ ready�ϸ� ȣ�� (ReadyUI ����, ���ӿ� ���� �Է� ���)

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void InitPlayerSetting();
	bool UpdateWorldInfo();		// ���� ����ȭ
	void UpdateNewPlayer();
	void UpdatePlayerInfo(const cCharacter& info);
	void LoadReadyUI();	// ReadyUI ����, UI�� ���� �Է¸� ���

public:
	int							iSessionId;			// ĳ������ ���� ���� ���̵�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bIsReady;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> readyUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UUserWidget* readyUI;

private:
	ClientSocket*			mySocket;
	cCharacter				initInfo;
	LockQueue<shared_ptr<cCharacter>>				newPlayers;			// �÷��̾� �α��� �� ĳ���� ����
		//�� �÷��̾� ����
	shared_ptr<cCharacter> newplayer;
	//queue<shared_ptr<cCharacter>>				newPlayers;			// �÷��̾� �α��� �� ĳ���� ����
	LockQueue<int> newBalls;
	//queue <int>				newBalls;
	cCharactersInfo*		charactersInfo;	// �ٸ� ĳ���͵��� ����

	bool							bNewPlayerEntered;
	bool							bInitPlayerSetting;
	atomic<bool>                    bSetStart;
	// ������ų �ٸ� ĳ����
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ACharacter> WhoToSpawn;

	//bool							bIsConnected;	// ������ ���� ����
};
