// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "MyGameModeBase.h"
#include "ClientSocket.h"
#include "MyAnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

#pragma comment(lib, "ws2_32.lib")


AMyPlayerController::AMyPlayerController()
{
	mySocket = ClientSocket::GetSingleton();
	mySocket->SetPlayerController(this);

	//bNewPlayerEntered = false;
	bInitPlayerSetting = false;
	bSetStart = false;
	bInGame = false;
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FClassFinder<UUserWidget> READY_UI(TEXT("/Game/Blueprints/ReadyUI.ReadyUI_C"));
	if (READY_UI.Succeeded() && (READY_UI.Class != nullptr))
	{
		readyUIClass = READY_UI.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> CHARACTER_UI(TEXT("/Game/Blueprints/CharacterUI.CharacterUI_C"));
	if (CHARACTER_UI.Succeeded() && (CHARACTER_UI.Class != nullptr))
	{
		characterUIClass = CHARACTER_UI.Class;
	}
	bIsReady = false;
}


void AMyPlayerController::BeginPlay()
{
	MYLOG(Warning, TEXT("BeginPlay!"));
	mySocket->StartListen();

	// ����� Ŭ������ �ٷ� ����
	//FInputModeGameOnly InputMode;
	//SetInputMode(InputMode);

	LoadReadyUI();	// readyUI ���� ���ӿ� ���� �Է� x, UI�� ���� �Է¸� ����
}

void AMyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//MYLOG(Warning, TEXT("EndPlay!"));
	//mySocket->Send_LogoutPacket(iSessionId);
	//mySocket->CloseSocket();
	//mySocket->StopListen();
	FuncUpdateHPCont.Clear(); // ��������Ʈ ����
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�÷��̾� �ʱ⼳��
	if (bInitPlayerSetting)
		InitPlayerSetting();

	if (newPlayers.TryPop(newplayer))
		UpdateNewPlayer();

	if (bSetStart)
		StartGame();

	// ���� ����ȭ
	UpdateWorldInfo();

	//UpdateRotation();
}

void AMyPlayerController::SetInitInfo(const cCharacter& me)
{
	initInfo = me;
	bInitPlayerSetting = true;
}

void AMyPlayerController::SetNewCharacterInfo(shared_ptr<cCharacter> NewPlayer_)
{
	if (NewPlayer_ != nullptr)
	{
		//bNewPlayerEntered = true;
		newPlayers.Push(NewPlayer_);
	}
}

void AMyPlayerController::SetNewBall(const int s_id)
{
	UWorld* World = GetWorld();
	newBalls.Push(s_id);
}

void AMyPlayerController::InitPlayerSetting()
{
	auto player_ = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!player_) return;
	player_->SetActorLocation(FVector(initInfo.X, initInfo.Y, initInfo.Z));
	player_->iSessionId = initInfo.SessionId;

	//��Ʈ�ѷ��� �ʱ⼳���� �Ұ�����
	SetControlRotation(FRotator(0.0f, initInfo.Yaw, 0.0f));

	player_->SetCharacterMaterial(iSessionId);

	bInitPlayerSetting = false;
}

bool AMyPlayerController::UpdateWorldInfo()
{
	UWorld* const world = GetWorld();
	if (world == nullptr)					return false;
	if (charactersInfo == nullptr)	return false;

	// �÷��̾��ڽ� ü��, ���������Ʈ
	UpdatePlayerInfo(charactersInfo->players[iSessionId]);

	if (charactersInfo->players.size() == 1)
	{
		//MYLOG(Warning, TEXT("Only one player"));
		return false;
	}

	// ����ĳ���͵�迭 �ϳ� �����ϰ� ���忡 �ִ� ĳ���͵��� �迭�� �־��ֱ�
	TArray<AActor*> SpawnedCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyCharacter::StaticClass(), SpawnedCharacters);

	//�� ������
	int new_ball;
	while (newBalls.TryPop(new_ball))
	{
		charactersInfo->players[new_ball].new_ball = true;
	}

	for (auto& Character_ : SpawnedCharacters)
	{
		//�ڽ����� ����÷��̾�
		AMyCharacter* player_ = Cast<AMyCharacter>(Character_);

		cCharacter* info = &charactersInfo->players[player_->iSessionId];
		if (!info->IsAlive) continue;

		if (info->new_ball) { 
			player_->SnowAttack();
			info->new_ball = false;
		}


		//Ÿ�÷��̾� ����
		if (!player_ || player_->iSessionId == -1 || player_->iSessionId == iSessionId)
		{
			continue;
		}


		//if (player_->iCurrentHP != info->HealthValue)
		//{
		//	MYLOG(Warning, TEXT("other player damaged."));
		//	//// �ǰ� ��ƼŬ ��ȯ
		//	//FTransform transform(OtherPlayer->GetActorLocation());
		//	//UGameplayStatics::SpawnEmitterAtLocation(
		//	//	world, HitEmiiter, transform, true
		//	//);
		//	//// �ǰ� �ִϸ��̼� �÷���
		//	//player_->PlayDamagedAnimation();
		//	//player_->iCurrentHP = info->HealthValue;
		//}

		//// �������϶� Ÿ�� �ִϸ��̼� �÷���
		//if (info->IsAttacking)
		//{
		//	UE_LOG(LogClass, Log, TEXT("other player hit."));
		//	OtherPlayer->PlayHitAnimation();
		//}

		FVector CharacterLocation;
		CharacterLocation.X = info->X;
		CharacterLocation.Y = info->Y;
		CharacterLocation.Z = info->Z;

		FRotator CharacterRotation;
		CharacterRotation.Yaw = info->Yaw;
		CharacterRotation.Pitch = 0.0f;
		CharacterRotation.Roll = 0.0f;

		FVector CharacterVelocity;
		CharacterVelocity.X = info->VX;
		CharacterVelocity.Y = info->VY;
		CharacterVelocity.Z = info->VZ;

		player_->AddMovementInput(CharacterVelocity);
		player_->SetActorRotation(CharacterRotation);
		player_->SetActorLocation(CharacterLocation);
		player_->GetAnim()->SetDirection(info->direction);

		//����� ��ȭ
		if (!player_->IsSnowman())
		{
			if (info->My_State == ST_SNOWMAN)
			{
				player_->ChangeSnowman();
			}
		}
		else {
			if (info->My_State != ST_SNOWMAN)
			{
				player_->ChangeAnimal();
			}
		}
	}
	return true;
}

void AMyPlayerController::UpdateNewPlayer()
{
	UWorld* const World = GetWorld();

	int size_ = newPlayers.Size();


	// ���ο� �÷��̾ �ʵ忡 ����
	FVector SpawnLocation_;
	SpawnLocation_.X = newplayer.get()->X;
	SpawnLocation_.Y = newplayer.get()->Y;
	SpawnLocation_.Z = newplayer.get()->Z;

	FRotator SpawnRotation;
	SpawnRotation.Yaw = newplayer.get()->Yaw;
	SpawnRotation.Pitch = 0.0f;
	SpawnRotation.Roll = 0.0f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.Name = FName(*FString(to_string(newplayer.get()->SessionId).c_str()));

	WhoToSpawn = AMyCharacter::StaticClass();
	AMyCharacter* SpawnCharacter = World->SpawnActor<AMyCharacter>(WhoToSpawn, SpawnLocation_, SpawnRotation, SpawnParams);
	SpawnCharacter->SpawnDefaultController();
	SpawnCharacter->iSessionId = newplayer.get()->SessionId;
	SpawnCharacter->SetCharacterMaterial(SpawnCharacter->iSessionId);

	// �ʵ��� �÷��̾� ������ �߰�
	if (charactersInfo != nullptr)
	{
		cCharacter info;
		info.SessionId = newplayer.get()->SessionId;
		info.X = newplayer.get()->X;
		info.Y = newplayer.get()->Y;
		info.Z = newplayer.get()->Z;

		info.Yaw = newplayer.get()->Yaw;

		charactersInfo->players[newplayer.get()->SessionId] = info;
	}

	newplayer = NULL;

	//MYLOG(Warning, TEXT("other player(id : %d) spawned."), newPlayers.front()->SessionId);

	//bNewPlayerEntered = false;
}

void AMyPlayerController::SendPlayerInfo(int input)
{
	auto m_Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!m_Player)
		return;
	auto MyLocation = m_Player->GetActorLocation();
	auto MyRotation = m_Player->GetActorRotation();
	auto MyVelocity = m_Player->GetVelocity();
	FVector MyCameraLocation;
	FRotator MyCameraRotation;
	m_Player->GetActorEyesViewPoint(MyCameraLocation, MyCameraRotation);
	float dir = m_Player->GetAnim()->GetDirection();

	if (input == COMMAND_MOVE)
		mySocket->Send_MovePacket(iSessionId, MyLocation, MyRotation, MyVelocity, dir);
	else if (input == COMMAND_ATTACK)
		mySocket->Send_Throw_Packet(iSessionId, MyCameraLocation, MyCameraRotation.Vector());
	else if (input == COMMAND_DAMAGE)
		mySocket->Send_DamagePacket();
}

//�÷��̾� ���� ������Ʈ
void AMyPlayerController::UpdatePlayerInfo(const cCharacter& info)
{
	UWorld* const world = GetWorld();
	if (!world)
		return;

	auto player_ = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!player_)
		return;

	if (!info.IsAlive)
	{
		/*UE_LOG(LogClass, Log, TEXT("Player Die"));
		FTransform transform(Player->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(
			world, DestroyEmiiter, transform, true
		);
		Player->Destroy();

		CurrentWidget->RemoveFromParent();
		GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		if (GameOverWidget != nullptr)
		{
			GameOverWidget->AddToViewport();
		}*/
	}
	else
	{
		// ĳ���� �Ӽ� ������Ʈ
		if (player_->iCurrentHP != info.HealthValue)
		{
			//MYLOG(Warning, TEXT("Player damaged hp: %d"), info.HealthValue);
			player_->iCurrentHP = info.HealthValue;
			CallDelegateUpdateHP();
			//// �ǰ� ��ƼŬ ����
			//FTransform transform(player_->GetActorLocation());
			//UGameplayStatics::SpawnEmitterAtLocation(
			//	world, HitEmiiter, transform, true
			//);
			// �ǰ� �ִϸ��̼� ����
			//player_->PlayDamagedAnimation();
			//player_->HealthValue = info.HealthValue;
		}
	}
}


//void AMyPlayerController::UpdateFarming(int item_no)
//{
//		mySocket->ReadyToSend_ItemPacket(item_no);
//}
//
//void AMyPlayerController::UpdatePlayerS_id(int id)
//{
//	iMySessionId = id;
//	auto m_Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
//	if (!m_Player)
//		return;
//	m_Player->iSessionID = id;
//	m_Player->SessionId = id;
//	m_Player->SetActorLocationAndRotation(FVector(id * 100.0f, id * 100.0f, m_Player->GetActorLocation().Z), FRotator(0.0f, -90.0f, 0.0f));
//}
//
//void AMyPlayerController::RecvNewPlayer(const cCharacter& info)
//{
//	//MYLOG(Warning, TEXT("recv ok player%d : %f, %f, %f"), sessionID, x, y, z);
//
//	UWorld* World = GetWorld();
//	iNewPlayers.push(info.SessionId);
//}

void AMyPlayerController::LoadReadyUI()
{
	if (readyUIClass)
	{
		readyUI = CreateWidget<UUserWidget>(GetWorld(), readyUIClass);
		if (readyUI)
		{
			readyUI->AddToViewport();

			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
	}
}

void AMyPlayerController::PlayerReady()
{
	// ������ �����ߴٰ� ����
	mySocket->Send_ReadyPacket();
	UE_LOG(LogTemp, Warning, TEXT("PlayerReady"));
	bIsReady = true;
}

void AMyPlayerController::PlayerUnready()
{
	// ������ �𷹵��ߴٰ� ����
	UE_LOG(LogTemp, Warning, TEXT("PlayerUnready"));
	bIsReady = false;
#ifdef SINGLEPLAY_DEBUG
	StartGame();	// ������ - �����ư ������ startgame ȣ��
#endif
}

void AMyPlayerController::StartGame()
{
	if (!bInGame) {
		UE_LOG(LogTemp, Warning, TEXT("StartGame"));
		bInGame = true;
		readyUI->RemoveFromParent();	// ReadyUI ����
		LoadCharacterUI(); // CharacterUI ����
		// ����� Ŭ������ �ٷ� ����
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = false;
	}
	// ���� ���۵Ǹ� �����ų �ڵ�� �ۼ�
}

void AMyPlayerController::LoadCharacterUI()
{
	if (characterUIClass)
	{
		characterUI = CreateWidget<UUserWidget>(GetWorld(), characterUIClass);
		if (characterUI)
		{
			characterUI->AddToViewport();
			CallDelegateUpdateHP();
		}
	}
}

void AMyPlayerController::CallDelegateUpdateHP()
{
	if (!characterUI) return;	// CharacterUI�� �����Ǳ� ���̸� ���� x
	AMyCharacter* localPlayer = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (FuncUpdateHPCont.IsBound() == true) FuncUpdateHPCont.Broadcast(localPlayer->iCurrentHP);	// ��������Ʈ ȣ��

	UE_LOG(LogTemp, Warning, TEXT("call delegate update hp %d"), localPlayer->iCurrentHP);
}