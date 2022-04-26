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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_OneParam, int32, NewHP);
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
	void PlayerReady();	// 컨트롤러를 소유한 플레이어가 레디하면 호출되는 함수
	UFUNCTION(BlueprintCallable)
	void PlayerUnready();
	void StartGame();	// 모든 플레이어가 ready하면 호출 (ReadyUI 제거, 게임에 대한 입력 허용)
	void CallDelegateUpdateHP();	// UpdateHP 델리게이트 이벤트 호출
	void Start_Signal()
	{
		bSetStart.store(true);
	}
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void InitPlayerSetting();
	bool UpdateWorldInfo();		// 월드 동기화
	void UpdateNewPlayer();
	void UpdatePlayerInfo(const cCharacter& info);
	void LoadReadyUI();	// ReadyUI 띄우기, UI에 대한 입력만 허용
	void LoadCharacterUI();	// CharacterUI 띄우기, 게임에 대한 입력 허용

public:
	int							iSessionId;			// 캐릭터의 세션 고유 아이디

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bIsReady;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> readyUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UUserWidget* readyUI;	// 레디 UI (레디 버튼 및 텍스트)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> characterUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UUserWidget* characterUI;	// 캐릭터 UI (체력, )

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_OneParam FuncUpdateHPCont;	// 델리게이트 이벤트 UpdateHP

private:
	ClientSocket*			mySocket;
	cCharacter				initInfo;
	LockQueue<shared_ptr<cCharacter>>				newPlayers;			// 플레이어 로그인 시 캐릭터 정보
		//새 플레이어 스폰
	shared_ptr<cCharacter> newplayer;
	//queue<shared_ptr<cCharacter>>				newPlayers;			// 플레이어 로그인 시 캐릭터 정보
	LockQueue<int> newBalls;
	//queue <int>				newBalls;
	cCharactersInfo*		charactersInfo;	// 다른 캐릭터들의 정보

	bool							bNewPlayerEntered;
	bool							bInitPlayerSetting;
	bool							bInGame;
	atomic<bool>                    bSetStart;
	// 스폰시킬 다른 캐릭터
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ACharacter> WhoToSpawn;

	//bool							bIsConnected;	// 서버와 접속 유무
};
