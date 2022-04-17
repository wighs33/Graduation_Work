// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "ClientSocket.h"

#pragma comment(lib, "ws2_32.lib")


AMyPlayerController::AMyPlayerController()
{
	mySocket = ClientSocket::GetSingleton();
	mySocket->SetPlayerController(this);

	bNewPlayerEntered = false;
	bInitPlayerSetting = false;

	iPlayerCount = -1;

	PrimaryActorTick.bCanEverTick = true;
}


void AMyPlayerController::BeginPlay()
{
	mySocket->StartListen();

	// ����� Ŭ������ �ٷ� ����
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AMyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	mySocket->CloseSocket();
	mySocket->StopListen();
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�÷��̾� �ʱ⼳��
	if (bInitPlayerSetting)
		InitPlayerSetting();

	//�� �÷��̾� ����
	if (bNewPlayerEntered)
		UpdateNewPlayer();
	
	// ���� ����ȭ
	UpdateWorldInfo();


	////�� ������
	//if (!iNewBalls.empty())
	//	UpdateNewBall();
	////UpdateRotation();

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
		bNewPlayerEntered = true;
		NewCharactersInfo.push(NewPlayer_);

		MYLOG(Warning, TEXT("size : %d"), NewCharactersInfo.size());
	}
}

void AMyPlayerController::InitPlayerSetting()
{
	auto player_ = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!player_) return;
	player_->SetActorLocationAndRotation(FVector(initInfo.X, initInfo.Y, initInfo.Z), FRotator(0.0f, initInfo.Yaw, 0.0f));
	player_->iSessionId = initInfo.SessionId;
	bInitPlayerSetting = false;
}

bool AMyPlayerController::UpdateWorldInfo()
{
	UWorld* const world = GetWorld();
	if (world == nullptr)					return false;
	if (CharactersInfo == nullptr)	return false;

	// �÷��̾� ����, ���������Ʈ
	//UpdatePlayerInfo(CharactersInfo->players[iMySessionId]);

	if (CharactersInfo->players.size() == 1)
	{
		//MYLOG(Warning, TEXT("Only one player"));
		return false;
	}

	// ����ĳ���͵�迭 �ϳ� �����ϰ� ���忡 �ִ� ĳ���͵��� �迭�� �־��ֱ�
	TArray<AActor*> SpawnedCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyCharacter::StaticClass(), SpawnedCharacters);

	if (iPlayerCount == -1)
	{
		iPlayerCount = CharactersInfo->players.size();
		return false;
	}
	else
	{
		for (auto& Character_ : SpawnedCharacters)
		{
			AMyCharacter* OtherPlayer = Cast<AMyCharacter>(Character_);

			if (!OtherPlayer || OtherPlayer->iSessionId == -1 || OtherPlayer->iSessionId == iSessionId)
			{
				continue;
			}

			//Ÿ�÷��̾�
			cCharacter* info = &CharactersInfo->players[OtherPlayer->iSessionId];

			if (info->IsAlive)
			{
				//if (OtherPlayer->HealthValue != info->HealthValue)
				//{
				//	UE_LOG(LogClass, Log, TEXT("other player damaged."));
				//	// �ǰ� ��ƼŬ ��ȯ
				//	FTransform transform(OtherPlayer->GetActorLocation());
				//	UGameplayStatics::SpawnEmitterAtLocation(
				//		world, HitEmiiter, transform, true
				//	);
				//	// �ǰ� �ִϸ��̼� �÷���
				//	OtherPlayer->PlayDamagedAnimation();
				//	OtherPlayer->HealthValue = info->HealthValue;
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

				OtherPlayer->AddMovementInput(CharacterVelocity);
				OtherPlayer->SetActorRotation(CharacterRotation);
				OtherPlayer->SetActorLocation(CharacterLocation);
			}
			else
			{
			/*	UE_LOG(LogClass, Log, TEXT("other player dead."));
				FTransform transform(Character->GetActorLocation());
				UGameplayStatics::SpawnEmitterAtLocation(
					world, DestroyEmiiter, transform, true
				);
				Character->Destroy();*/
			}
		}

	}


	return true;
}

void AMyPlayerController::UpdateNewPlayer()
{
	UWorld* const World = GetWorld();

	int size_ = NewCharactersInfo.size();

	for (int i = 0; i < size_; ++i)
	{
		MYLOG(Warning, TEXT("it's %d"), NewCharactersInfo.front()->SessionId);
		// ���ο� �÷��̾ �ڱ� �ڽ��̸� ����
		if (NewCharactersInfo.front()->SessionId == iSessionId)
		{
			MYLOG(Warning, TEXT("%d %d It's my character"), NewCharactersInfo.front()->SessionId, iSessionId);
			NewCharactersInfo.front() = nullptr;
			NewCharactersInfo.pop();
			continue;
		}

		// ���ο� �÷��̾ �ʵ忡 ����
		FVector SpawnLocation_;
		SpawnLocation_.X = NewCharactersInfo.front()->X;
		SpawnLocation_.Y = NewCharactersInfo.front()->Y;
		SpawnLocation_.Z = NewCharactersInfo.front()->Z;

		FRotator SpawnRotation;
		SpawnRotation.Yaw = NewCharactersInfo.front()->Yaw;
		SpawnRotation.Pitch = 0.0f;
		SpawnRotation.Roll = 0.0f;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.Name = FName(*FString(to_string(NewCharactersInfo.front()->SessionId).c_str()));

		WhoToSpawn = AMyCharacter::StaticClass();
		AMyCharacter* SpawnCharacter = World->SpawnActor<AMyCharacter>(WhoToSpawn, SpawnLocation_, SpawnRotation, SpawnParams);
		SpawnCharacter->SpawnDefaultController();
		SpawnCharacter->iSessionId = NewCharactersInfo.front()->SessionId;

		MYLOG(Warning, TEXT("who %d"), SpawnCharacter->iSessionId);

		// �ʵ��� �÷��̾� ������ �߰�
		if (CharactersInfo != nullptr)
		{
			cCharacter info;
			info.SessionId = NewCharactersInfo.front()->SessionId;
			info.X = NewCharactersInfo.front()->X;
			info.Y = NewCharactersInfo.front()->Y;
			info.Z = NewCharactersInfo.front()->Z;

			info.Yaw = NewCharactersInfo.front()->Yaw;

			CharactersInfo->players[NewCharactersInfo.front()->SessionId] = info;
			iPlayerCount = CharactersInfo->players.size();
		}

		MYLOG(Warning, TEXT("other player spawned."));

		NewCharactersInfo.front() = nullptr;
		NewCharactersInfo.pop();
	}
	bNewPlayerEntered = false;
}

void AMyPlayerController::UpdatePlayerInfo(int input)
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
	if (input == COMMAND_MOVE)
		mySocket->Send_MovePacket(iSessionId, MyLocation, MyRotation, MyVelocity);
	else if (input == COMMAND_ATTACK) 
		mySocket->ReadyToSend_Throw_Packet(iSessionId, MyCameraLocation, MyCameraRotation.Vector());
	else if (input == COMMAND_DAMAGE)
		mySocket->ReadyToSend_DamgePacket();

}
//
////�÷��̾� ���� ������Ʈ
//void AMyPlayerController::StartPlayerInfo(const cCharacter& info)
//{
//	auto Player_ = Cast<AMyCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
//	if (!Player_)
//		return;
//
//
//	UWorld* const world = GetWorld();
//	if (!world)
//		return;
//
//	if (bSetPlayer) {
//		iMySessionId = info.SessionId;
//		FVector _CharacterLocation;
//		_CharacterLocation.X = info.X;
//		_CharacterLocation.Y = info.Y;
//		_CharacterLocation.Z = info.Z;
//		Player_->SetActorLocation(_CharacterLocation);
//		bSetPlayer = false;
//	}
//
//	auto m_Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
//	if (!m_Player)
//		return;
//	auto MyLocation = m_Player->GetActorLocation();
//	auto MyRotation = m_Player->GetActorRotation();
//
//	mySocket->fMy_x = MyLocation.X;
//	mySocket->fMy_y = MyLocation.Y;
//	mySocket->fMy_z = MyLocation.Z;
//	MYLOG(Warning, TEXT("i'm player init spawn : (%f, %f, %f)"), MyLocation.X, MyLocation.Y, MyLocation.Z);
//
//
//	if (!info.IsAlive)
//	{
//		/*UE_LOG(LogClass, Log, TEXT("Player Die"));
//		FTransform transform(Player->GetActorLocation());
//		UGameplayStatics::SpawnEmitterAtLocation(
//			world, DestroyEmiiter, transform, true
//		);
//		Player->Destroy();
//
//		CurrentWidget->RemoveFromParent();
//		GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
//		if (GameOverWidget != nullptr)
//		{
//			GameOverWidget->AddToViewport();
//		}*/
//	}
//	else
//	{
//		//// ĳ���� �Ӽ� ������Ʈ
//		//if (Player->HealthValue != info.HealthValue)
//		//{
//		//	UE_LOG(LogClass, Log, TEXT("Player damaged"));
//		//	// �ǰ� ��ƼŬ ����
//		//	FTransform transform(Player->GetActorLocation());
//		//	UGameplayStatics::SpawnEmitterAtLocation(
//		//		world, HitEmiiter, transform, true
//		//	);
//		//	// �ǰ� �ִϸ��̼� ����
//		//	Player->PlayDamagedAnimation();
//		//	Player->HealthValue = info.HealthValue;
//		//}
//	}
//}
//
////�÷��̾� ���� ������Ʈ
//void AMyPlayerController::UpdatePlayerInfo(const cCharacter& info)
//{
//	auto Player_ = Cast<AMyCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
//	if (!Player_)
//		return;
//	
//	
//	UWorld* const world = GetWorld();
//	if (!world)
//		return;
//
//	if (bSetPlayer) {
//		FVector _CharacterLocation;
//		_CharacterLocation.X = info.X;
//		_CharacterLocation.Y = info.Y;
//		_CharacterLocation.Z = info.Z;
//		Player_->SetActorLocation(_CharacterLocation);
//		bSetPlayer = false;
//	}
//
//	auto m_Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
//	if (!m_Player)
//		return;
//	auto MyLocation = m_Player->GetActorLocation();
//	auto MyRotation = m_Player->GetActorRotation();
//	
//	mySocket->fMy_x = MyLocation.X;
//	mySocket->fMy_y = MyLocation.Y;
//	mySocket->fMy_z = MyLocation.Z;
//	//MYLOG(Warning, TEXT("i'm player init spawn : (%f, %f, %f)"), MyLocation.X, MyLocation.Y, MyLocation.Z);
//
//
//	if (!info.IsAlive)
//	{
//		/*UE_LOG(LogClass, Log, TEXT("Player Die"));
//		FTransform transform(Player->GetActorLocation());
//		UGameplayStatics::SpawnEmitterAtLocation(
//			world, DestroyEmiiter, transform, true
//		);
//		Player->Destroy();
//
//		CurrentWidget->RemoveFromParent();
//		GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
//		if (GameOverWidget != nullptr)
//		{
//			GameOverWidget->AddToViewport();
//		}*/
//	}
//	else
//	{
//		//// ĳ���� �Ӽ� ������Ʈ
//		//if (Player->HealthValue != info.HealthValue)
//		//{
//		//	UE_LOG(LogClass, Log, TEXT("Player damaged"));
//		//	// �ǰ� ��ƼŬ ����
//		//	FTransform transform(Player->GetActorLocation());
//		//	UGameplayStatics::SpawnEmitterAtLocation(
//		//		world, HitEmiiter, transform, true
//		//	);
//		//	// �ǰ� �ִϸ��̼� ����
//		//	Player->PlayDamagedAnimation();
//		//	Player->HealthValue = info.HealthValue;
//		//}
//	}
//}
//
//
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
//
//void AMyPlayerController::RecvNewBall(int s_id)
//{
//	//MYLOG(Warning, TEXT("recv ok player%d : %f, %f, %f"), sessionID, x, y, z);
//
//	UWorld* World = GetWorld();
//	iNewBalls.push(s_id);
//}
//

//
//void AMyPlayerController::UpdateNewBall()
//{
//	UWorld* const World = GetWorld();
//	// ���ο� �÷��̾ �ڱ� �ڽ��̸� ����
//	int new_s_id = iNewBalls.front();
//	FVector C_Location_;
//	C_Location_.X = CharactersInfo->players[new_s_id].fCx;
//	C_Location_.Y = CharactersInfo->players[new_s_id].fCy;
//	C_Location_.Z = CharactersInfo->players[new_s_id].fCz;
//	FVector CD_Location_;
//	CD_Location_.X = CharactersInfo->players[new_s_id].fCDx;
//	CD_Location_.Y = CharactersInfo->players[new_s_id].fCDy;
//	CD_Location_.Z = CharactersInfo->players[new_s_id].fCDz;
//	CharactersInfo->players[new_s_id].FMyLocation = C_Location_;
//	CharactersInfo->players[new_s_id].FMyDirection = CD_Location_;
//	
//	if (new_s_id == iMySessionId)
//	{
//		auto m_Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
//		
//		m_Player->SnowAttack();
//		iNewBalls.pop();
//		return;
//	}
//
//	TArray<AActor*> SpawnedCharacters;
//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyCharacter::StaticClass(), SpawnedCharacters);
//
//	for (auto& Character_ : SpawnedCharacters)
//	{
//		AMyCharacter* OtherPlayer = Cast<AMyCharacter>(Character_);
//
//		if (!OtherPlayer || OtherPlayer->iSessionID == -1 || new_s_id == iMySessionId )
//		{
//			continue;
//		}
//        
//		if (new_s_id == OtherPlayer->iSessionID) {
//			cCharacter* info = &CharactersInfo->players[new_s_id];
//			if (info->IsAlive)
//			{				
//				OtherPlayer->SnowAttack();
//				iNewBalls.pop();
//				return;
//			}
//		}
//	}
//	iNewPlayers.pop();
//
//}
//
//void AMyPlayerController::Throw_Snow(FVector MyLocation, FVector MyDirection)
//{
//	mySocket->ReadyToSend_Throw_Packet(iMySessionId, MyLocation, MyDirection);
//
//};

//void AMyPlayerController::UpdateRotation()
//{
//	float pitch, yaw, roll;
//	UKismetMathLibrary::BreakRotator(GetControlRotation(), roll, pitch, yaw);
//	pitch = UKismetMathLibrary::ClampAngle(pitch, -15.0f, 30.0f);
//	FRotator newRotator = UKismetMathLibrary::MakeRotator(roll, pitch, yaw);
//	SetControlRotation(newRotator);
//}
