// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HoloLens/AllowWindowsPlatformAtomics.h"
#include <thread>
#include <atomic>
//#include <queue>
#include "HoloLens/HideWindowsPlatformAtomics.h"

#include "Final_Project.h"
#include "MyCharacter.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundCue.h"
#include "GameFramework/PlayerController.h"
#include "ClientSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_HP, int32, NewHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_CurrentSnowballCount, int32, NewCurrentSnowballCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_CurrentIceballCount, int32, NewCurrentIceballCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_CurrentMatchCount, int32, NewCurrentMatchCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDele_Dynamic_MaxSnowIceballAndMatchCount, int32, NewMaxSnowballCount, int32, NewMaxIceballCount, int32, NewMaxMatchCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_HasUmbrella, bool, NewHasUmbrella);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_HasBag, bool, NewHasBag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_HasShotgun, bool, NewHasShotgun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_IsFarmingSnowdrift, bool, NewIsFarmingSnowdrift);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_SnowdriftFarmDuration, float, NewSnowdriftFarmDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_SelectedItem, int32, NewSelectedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_SelectedProjectile, int32, NewSelectedProjectile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_SelectedWeapon, int32, NewSelectedWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDele_Dynamic_KillLog, FString, iAttacker, FString, iVictim, int32, Cause, int32, iKillLogType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_GameResult, int32, iWinnerId);


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
	void SetNewTornadoInfo(shared_ptr<cCharacter> newTornado);

	void SetInitInfo(const cCharacter& me);

	void SetDestroySnowdritt(const int s_id);
	void SetDestroyIcedritt(const int obj_id);
	void SetDestroyitembox(const int obj_id);
	void SetDestroySpBox(const int obj_id);
	void SetDestroyPlayer(const int del_sid);
	void SetGameEnd(const int target_id);
	void SetOpenItembox(const int obj_id);
	void SetAttack(const int s_id, int at_type);
	void SetFreeze(const int s_id, int body_type);
	void StartFreeze(AMyCharacter* player_, cCharacter* info);
	void SetItem(const int s_id, int item_type, bool end);
	void SetSocket();
	void GetItem(int sId, int itemType);


	void UpdateTornado();


	void SendPlayerInfo(int input);
	void SendTeleportInfo(int input);
	void SendCheatInfo(int input);

	UFUNCTION(BlueprintCallable)
	void PlayerReady();	// ��Ʈ�ѷ��� ������ �÷��̾ �����ϸ� ȣ��Ǵ� �Լ�
	UFUNCTION(BlueprintCallable)
	void PlayerUnready();
	void StartGame();	// ��� �÷��̾ ready�ϸ� ȣ�� (ReadyUI ����, ���ӿ� ���� �Է� ���)
	void LoadGameResultUI(int winnerSessionId);	// ���� ���� �� ���â ui ���� (CharacterUI ����, ���� id ���ڷ� �޾Ƽ� ��� ���)
	
	UFUNCTION(BlueprintCallable)
	void BtnCreateAccount(FString id, FString pw);
	UFUNCTION(BlueprintCallable)
	void BtnLogin(FString id, FString pw);

	void DeleteLoginUICreateReadyUI();

	UFUNCTION(BlueprintCallable)
	FString GetUserId(int sessionId);

	// UpdateUI ��������Ʈ �̺�Ʈ ȣ��
	void CallDelegateUpdateAllOfUI();
	void CallDelegateUpdateHP();
	void CallDelegateUpdateCurrentSnowballCount();
	void CallDelegateUpdateCurrentIceballCount();
	void CallDelegateUpdateCurrentMatchCount();
	void CallDelegateUpdateMaxSnowIceballAndMatchCount();
	void CallDelegateUpdateHasUmbrella();
	void CallDelegateUpdateHasBag();
	void CallDelegateUpdateHasShotgun();
	void CallDelegateUpdateIsFarmingSnowdrift();
	void CallDelegateUpdateSnowdriftFarmDuration(float farmDuration);
	void CallDelegateUpdateSelectedItem();
	void CallDelegateUpdateSelectedProjectile();
	void CallDelegateUpdateSelectedWeapon();
	void CallDelegateUpdateKillLog(int attacker, int victim, int cause);
	void CallDelegateUpdateGameResult(int winnerId);

	void SpawnSupplyBox(float x, float y, float z = 4500.0f);	// �ش� ��ġ�� ���޻��� ����


	void SetCharacterState(const int s_id, STATE_Type _state)
	{
		charactersInfo->players[s_id].myState = _state;
	}
	void SetCharacterHP(const int s_id, int _hp)
	{
		charactersInfo->players[s_id].HealthValue = _hp;
	}
	void StartSignal()
	{
		//bInGame = true;
		StartGame();
	}
	bool IsStart()
	{
		return bInGame;
	}
	void GetBone()
	{
		bIsBone = true;
	}
	void SetCnt(int iBear, int iSnowman)
	{
		iBearCnt = iBear;
	    iSnowmanCnt = iSnowman;
	}
	UFUNCTION(BlueprintCallable)
	int GetBearCnt()
	{
		return iBearCnt;
	}
	UFUNCTION(BlueprintCallable)
	int GetSnowmanCnt()
	{
		return iSnowmanCnt;
	}
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnPossess(APawn* pawn_) override;

private:
	void InitPlayerSetting();   
	bool UpdateWorldInfo();		// ���� ����ȭ
	void UpdateNewPlayer();     // Ÿ �÷��̾� ����
	void UpdateNewTornado();    // ����̵� ����
	void UpdatePlayerInfo(cCharacter& info); // �÷��̾� ���� ������Ʈ
	void UpdatePlayerLocation();
	void LoadReadyUI();	// ReadyUI ����, UI�� ���� �Է¸� ���
	void LoadCharacterUI();	// CharacterUI ����, ���ӿ� ���� �Է� ���
	void LoadLoginUI();	// LoginUI ����, UI�� ���� �Է¸� ���
	void Reset_Items(int s_id);
	void FixRotation();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int							iSessionId;			// ĳ������ ���� ���� ���̵�
	bool                        bTornado;       // ����̵� ���� ����       


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bIsReady;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> readyUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UUserWidget* readyUI;	// ���� UI (���� ��ư �� �ؽ�Ʈ)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> characterUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UUserWidget* characterUI;	// ĳ���� UI (ü��, )

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> gameResultUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UUserWidget* gameResultUI;	// ���� ��� UI (�������� �� ���� ui)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> loginUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UUserWidget* loginUI;	// �α��� UI (���� ���� ��ư, �α��� ��ư, id pw �Է�)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString loginInfoText;	// �α��� UI info text�� ������ text

	// ��������Ʈ �̺�Ʈ UpdateUI
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_HP FuncUpdateHP;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_CurrentSnowballCount FuncUpdateCurrentSnowballCount;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_CurrentIceballCount FuncUpdateCurrentIceballCount;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_CurrentMatchCount FuncUpdateCurrentMatchCount;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_MaxSnowIceballAndMatchCount FuncUpdateMaxSnowIceballAndMatchCount;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_HasUmbrella FuncUpdateHasUmbrella;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_HasBag FuncUpdateHasBag;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_HasShotgun FuncUpdateHasShotgun;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_IsFarmingSnowdrift FuncUpdateIsFarmingSnowdrift;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_SnowdriftFarmDuration FuncUpdateSnowdriftFarmDuration;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_SelectedItem FuncUpdateSelectedItem;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_SelectedProjectile FuncUpdateSelectedProjectile;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_SelectedWeapon FuncUpdateSelectedWeapon;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_KillLog FuncUpdateKillLog;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic_GameResult FuncUpdateGameResult;

	ClientSocket* mySocket;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	bool	bInGame = false;

	UAudioComponent* AudioComponent;
	USoundCue* LoginCue;
	USoundCue* ReadyCue;
	USoundCue* InGameCue;
	USoundBase* ThrowS;
	USoundBase* ShootS;
	USoundBase* WalkS;
	USoundBase* ClickS;
	USoundBase* FreezeS;


private:
	
	cCharacter				initInfo;
	LockQueue<shared_ptr<cCharacter>>				newPlayers;			// �÷��̾� �α��� �� ĳ���� ����
	LockQueue<shared_ptr<cCharacter>>				newTornados;			// ����̵� �α��� �� ĳ���� ����

		//�� �÷��̾� ����
	shared_ptr<cCharacter> newplayer;
	shared_ptr<cCharacter> newtornado;

	//queue<shared_ptr<cCharacter>>				newPlayers;			// �÷��̾� �α��� �� ĳ���� ����
	atomic<int> ivictoryPlayer;


	//queue <int>				newBalls;
	cCharactersInfo*		charactersInfo;	// �ٸ� ĳ���͵��� ����

	bool							bNewPlayerEntered;
	bool							bInitPlayerSetting;
	bool							bIsBone = false;
	int                             iBearCnt;
	int                             iSnowmanCnt;


	atomic<bool>                    bSetStart;
	// ������ų �ٸ� ĳ����
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ACharacter> WhoToSpawn;
	TSubclassOf<class APawn> TornadoToSpawn;


	//bool							bIsConnected;	// ������ ���� ����

	AMyCharacter* localPlayerCharacter;

	float fOldYaw = 0.0f;
	float bIsSpeedZero = false;

	UPROPERTY(VisibleAnywhere, Category = Class)
	TSubclassOf<class AActor> supplyboxClass;

 


};
