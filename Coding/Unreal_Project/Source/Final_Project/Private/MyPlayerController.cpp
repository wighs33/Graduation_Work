// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "MyGameModeBase.h"
#include "ClientSocket.h"
#include "MyAnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Debug.h"
#include "Snowdrift.h"
#include "Itembox.h"
#include "Tornado.h"

#pragma comment(lib, "ws2_32.lib")

ClientSocket* g_socket = nullptr;

void CALLBACK recv_callback(DWORD err, DWORD num_byte, LPWSAOVERLAPPED recv_over, DWORD recv_flag)
{
	//MYLOG(Warning, TEXT("recv_callback"));
	if (num_byte == 0) {
		g_socket->CloseSocket();
		g_socket = nullptr; 
		MYLOG(Warning, TEXT("recv_error"));

		return;
	}
	unsigned char* packet_start = g_socket->_recv_over._net_buf;
	int packet_size = packet_start[0];
	int remain_data = num_byte + g_socket->_prev_size;
	g_socket->process_data(g_socket->_recv_over._net_buf, remain_data);
	recv_flag = 0;
	ZeroMemory(&g_socket->_recv_over._wsa_over, sizeof(g_socket->_recv_over._wsa_over));
	g_socket->_recv_over._wsa_buf.buf = reinterpret_cast<char*>(g_socket->_recv_over._net_buf + g_socket->_prev_size);
	g_socket->_recv_over._wsa_buf.len = sizeof(g_socket->_recv_over._net_buf) - g_socket->_prev_size;
	WSARecv(g_socket->_socket, &g_socket->_recv_over._wsa_buf, 1, 0, &recv_flag, &g_socket->_recv_over._wsa_over, recv_callback);
	SleepEx(0, true);
}

AMyPlayerController::AMyPlayerController()
{
	//bNewPlayerEntered = false;
	bInitPlayerSetting = false;
	bSetStart.store(false);
	victory_player.store(-1);
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
	static ConstructorHelpers::FClassFinder<UUserWidget> GAMERESULT_UI(TEXT("/Game/Blueprints/GameOverUI.GameOverUI_C"));
	if (GAMERESULT_UI.Succeeded() && (GAMERESULT_UI.Class != nullptr))
	{
		gameResultUIClass = GAMERESULT_UI.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> LOGIN_UI(TEXT("/Game/Blueprints/LoginUI.LoginUI_C"));
	if (LOGIN_UI.Succeeded() && (LOGIN_UI.Class != nullptr))
	{
		loginUIClass = LOGIN_UI.Class;
	}

	bIsReady = false;
	loginInfoText = TEXT("LOGIN");
}

void AMyPlayerController::OnPossess(APawn* pawn_)
{
	Super::OnPossess(pawn_);

	//mySocket->StartListen();
	
	localPlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	LoadLoginUI();	// loginUI ?????? ?????? ???? ???? x, UI?? ???? ?????? ????
	//LoadReadyUI();	// readyUI ?????? ?????? ???? ???? x, UI?? ???? ?????? ????
	
	FPlatformProcess::Sleep(0);
}

void AMyPlayerController::BeginPlay()
{
	MYLOG(Warning, TEXT("BeginPlay!"));
	
	// ?????? ???????? ???? ????
	//SetSocket();
	//SleepEx(0, true);
	SetSocket();
	//mySocket->StartListen();
	SleepEx(0, true);
}

void AMyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	MYLOG(Warning, TEXT("EndPlay!"));
	//mySocket->Send_LogoutPacket(iSessionId);
	////SleepEx(0, true);
	//mySocket->CloseSocket();
	//g_socket = nullptr;

	// ?????????? ????
	FuncUpdateHP.Clear();
	FuncUpdateCurrentSnowballCount.Clear();
	FuncUpdateCurrentIceballCount.Clear();
	FuncUpdateCurrentMatchCount.Clear();
	FuncUpdateMaxSnowIceballAndMatchCount.Clear();
	FuncUpdateHasUmbrella.Clear();
	FuncUpdateHasBag.Clear();
	FuncUpdateIsFarmingSnowdrift.Clear();
	FuncUpdateSnowdriftFarmDuration.Clear();
	FuncUpdateSelectedItem.Clear();
	FuncUpdateSelectedProjectile.Clear();
	FuncUpdateGameResult.Clear();
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	////??????
	//TArray<AActor*> Itemboxes;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItembox::StaticClass(), Itemboxes);

	//for (auto ib : Itemboxes)
	//{
	//	AItembox* itembox = Cast<AItembox>(ib);

	//	MYLOG(Warning, TEXT("id : %d"), itembox->GetId());
	//}
	
	// ???? ??????
	UpdateWorldInfo();

	SendPlayerInfo(COMMAND_MOVE);

	SleepEx(0, true);

	//FixRotation();	// ?????? ????(????)????
}

void AMyPlayerController::SetSocket()
{
    mySocket = new ClientSocket();         // ????????
	//mySocket = ClientSocket::GetSingleton(); // ?????? ??

	mySocket->SetPlayerController(this);
	g_socket = mySocket;
	mySocket->Connect();

	DWORD recv_flag = 0;
	ZeroMemory(&g_socket->_recv_over._wsa_over, sizeof(g_socket->_recv_over._wsa_over));
	g_socket->_recv_over._wsa_buf.buf = reinterpret_cast<char*>(g_socket->_recv_over._net_buf + g_socket->_prev_size);
	g_socket->_recv_over._wsa_buf.len = sizeof(g_socket->_recv_over._net_buf) - g_socket->_prev_size;
	WSARecv(g_socket->_socket, &g_socket->_recv_over._wsa_buf, 1, 0, &recv_flag, &g_socket->_recv_over._wsa_over, recv_callback);
	//g_socket->Send_LoginPacket();
	SleepEx(0, true); 
	
}

void AMyPlayerController::SetInitInfo(const cCharacter& me)
{
	initInfo = me;
	bInitPlayerSetting = true;
	InitPlayerSetting();
	
}

void AMyPlayerController::SetNewCharacterInfo(shared_ptr<cCharacter> NewPlayer_)
{
	if (NewPlayer_ != nullptr)
	{
		//bNewPlayerEntered = true;
		newplayer = NewPlayer_;
		UpdateNewPlayer();
	}
}

void AMyPlayerController::SetNewBall(const int s_id)
{
	UWorld* World = GetWorld();
	
	charactersInfo->players[s_id].canSnowBall = true;
	
}

void AMyPlayerController::SetRelAttack(const int s_id)
{
	UWorld* World = GetWorld();
    charactersInfo->players[s_id].relATTACK = true;

}

void AMyPlayerController::SetUmb(const int s_id, bool end)
{
	UWorld* World = GetWorld();
	if (!end) {
		//charactersInfo->players[s_id].end_umb = true;
		charactersInfo->players[s_id].start_umb = true;
	}
	else {
		//charactersInfo->players[s_id].start_umb = true;
		charactersInfo->players[s_id].end_umb = true;
	}
}

void AMyPlayerController::SetAttack(const int s_id)
{
	UWorld* World = GetWorld();

	charactersInfo->players[s_id].canAttack = true;

}

void AMyPlayerController::SetShotGun(const int s_id)
{
	UWorld* World = GetWorld();
	charactersInfo->players[s_id].canShot = true;
	//MYLOG(Warning, TEXT("SetShotGun %d"), s_id);
}

void AMyPlayerController::SetGunFire(const int s_id)
{
	UWorld* World = GetWorld();

	charactersInfo->players[s_id].canSnowBomb = true;

}

void AMyPlayerController::SetJetSki(const int s_id)
{
	UWorld* World = GetWorld();

	charactersInfo->players[s_id].operate_jet = true;

}


void AMyPlayerController::SetDestroyPlayer(const int del_sid)
{
	UWorld* World = GetWorld();

	TArray<AActor*> delCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyCharacter::StaticClass(), delCharacters);

	for (auto sd : delCharacters)
	{
		AMyCharacter* Del_Player = Cast<AMyCharacter>(sd);

		if (Del_Player->iSessionId == del_sid)
		{
			charactersInfo->players.erase(Del_Player->iSessionId);
			Del_Player->Destroy();
			Del_Player = nullptr;

		}
	}
	
}

void AMyPlayerController::SetDestroySnowdritt(const int obj_id)
{
	UWorld* World = GetWorld();
	TArray<AActor*> Snowdrifts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASnowdrift::StaticClass(), Snowdrifts);
	 
	if (obj_id == -1) return;
	for (auto sd : Snowdrifts)
	{
		ASnowdrift* snowdrift = Cast<ASnowdrift>(sd);

		if (snowdrift->GetId() == obj_id)
		{
			snowdrift->Destroy();
			snowdrift = nullptr;
		}
	}
		
}

void AMyPlayerController::SetOpenItembox(const int obj_id)
{
	UWorld* World = GetWorld();
	TArray<AActor*> ItemBox;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItembox::StaticClass(), ItemBox);

	for (auto sd : ItemBox)
	{
		AItembox* itembox = Cast<AItembox>(sd);
		//if (!itembox) continue;
		if (itembox->GetId() == obj_id)
		{
			itembox->SetItemboxState(ItemboxState::Opening);
		}
	}

	
}

void AMyPlayerController::SetGameEnd(const int target_id)
{
	UWorld* World = GetWorld();
	LoadGameResultUI(target_id);
}

void AMyPlayerController::SetDestroyitembox(const int obj_id)
{
	UWorld* World = GetWorld();
	
	
		TArray<AActor*> ItemBox;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItembox::StaticClass(), ItemBox);

		for (auto sd : ItemBox)
		{
			AItembox* itembox = Cast<AItembox>(sd);

			if (itembox->GetId() == obj_id)
			{
				itembox->DeleteItem();

			}
		}
	
}



void AMyPlayerController::get_item(int itemType)
{

	auto player_ = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!player_)
		return;
	//player_->GetItem(itemType);
}



void AMyPlayerController::InitPlayerSetting()
{
	if (!localPlayerCharacter) return;
	localPlayerCharacter->SetActorLocation(FVector(initInfo.X, initInfo.Y, initInfo.Z));
	localPlayerCharacter->iSessionId = initInfo.SessionId;

	//?????????? ????
	SetControlRotation(FRotator(0.0f, initInfo.Yaw, 0.0f));
	if (itonardoId == 0)
		localPlayerCharacter->SetCharacterMaterial(iSessionId - 1);
	else
		localPlayerCharacter->SetCharacterMaterial(iSessionId);
	bInitPlayerSetting = false;
}
void AMyPlayerController::UpdateTornado()
{
	UWorld* World = GetWorld();
	TArray<AActor*> SpawnedTornado;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATornado::StaticClass(), SpawnedTornado);
	if (itonardoId == -1) return;
	for (auto sd : SpawnedTornado)
	{
		ATornado* tornado = Cast<ATornado>(sd);

		cCharacter* info = &charactersInfo->players[itonardoId];
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

		//tornado->AddMovementInput(CharacterVelocity);
		//tornado->SetActorRotation(CharacterRotation);
		tornado->SetActorLocation(CharacterLocation);
		//MYLOG(Warning, TEXT("tornado %f, %f, %f"), info->X, info->Y, info->Z);

	}


}
bool AMyPlayerController::UpdateWorldInfo()
{
	UWorld* const world = GetWorld();
	if (world == nullptr)					return false;
	if (charactersInfo == nullptr)	return false;

	// ???????????? ????, ????????????
	UpdatePlayerInfo(charactersInfo->players[iSessionId]);
	UpdateTornado();
	//if (charactersInfo->players.size() == 1)
	//{
	//	//MYLOG(Warning, TEXT("Only one player"));
	//	return false;
	//}
	
	
	// ???????????????? ???? ???????? ?????? ???? ?????????? ?????? ????????
	TArray<AActor*> SpawnedCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyCharacter::StaticClass(), SpawnedCharacters);
	
	
	for (auto& Character_ : SpawnedCharacters)
	{
		//???????? ????????????
		AMyCharacter* player_ = Cast<AMyCharacter>(Character_);

		cCharacter* info = &charactersInfo->players[player_->iSessionId];
		if (!info->IsAlive) continue;

		if (info->canAttack) { 
			player_->SnowAttack();
			info->canAttack = false;
		}
		
		if (info->canShot) {
			player_->AttackShotgun();
			info->canShot = false;
		}

		if (info->relATTACK) {
			player_->Recv_ReleaseAttack();
			info->relATTACK = false;
		}

		if (info->canSnowBall) {
			player_->ReleaseSnowball();
			info->canSnowBall = false;
			info->current_snow_count--;
		}

		if (info->canSnowBomb) {
			MYLOG(Warning, TEXT("canSnowBomb"));

			player_->SpawnSnowballBomb();
			info->canSnowBomb = false;
			info->current_snow_count-=5;
		}

		if (info->start_umb) {
			MYLOG(Warning, TEXT("start_umb"));
			player_->StartUmbrella();
			info->start_umb = false;
		}

		if (info->end_umb) {
			MYLOG(Warning, TEXT("end_umb"));
			//player_->ReleaseUmbrella();
			player_->GetAnim()->ResumeUmbrellaMontage();
			player_->CloseUmbrellaAnim();
			info->end_umb = false;
		}
		if (info->operate_jet) {
			MYLOG(Warning, TEXT("jetski"));
			player_->GetOnOffJetski();
			info->operate_jet = false;
		}
		//?????????? ????
		if (!player_ || player_->iSessionId == -1 || player_->iSessionId == iSessionId)
		{
			continue;
		}


		//if (player_->iCurrentHP != info->HealthValue)
		//{
		//	MYLOG(Warning, TEXT("other player damaged."));
		//	//// ???? ?????? ????
		//	//FTransform transform(OtherPlayer->GetActorLocation());
		//	//UGameplayStatics::SpawnEmitterAtLocation(
		//	//	world, HitEmiiter, transform, true
		//	//);
		//	//// ???? ?????????? ??????
		//	//player_->PlayDamagedAnimation();
		//	//player_->iCurrentHP = info->HealthValue;
		//}

		//// ?????????? ???? ?????????? ??????
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
		//?????? ????
		if (!player_->IsSnowman())
		{
			if (info->My_State == ST_SNOWMAN)
			{
				info->current_snow_count = 0;
				Reset_Items(player_->iSessionId);
				player_->ChangeSnowman();
			}
		}
		else if (player_->IsSnowman())
		{
			if (info->My_State == ST_ANIMAL)
			{
				Reset_Items(player_->iSessionId);
				player_->ChangeAnimal();
			}
		}
		// ?????? ???? ????????
		if (player_->iCurrentSnowballCount != info->current_snow_count)
		{
			//MYLOG(Warning, TEXT("Player damaged hp: %d"), info.HealthValue);
			player_->iCurrentSnowballCount = info->current_snow_count;
		}
		//if (info->start_farming_item == ITEM_MAT);
		//if (info->start_farming_item == ITEM_UMB);
		//if (info->start_farming_item == ITEM_BAG);
		//if (info->start_farming_item == ITEM_SNOW)
		//{
		//	player_->StartFarming();
		//	info->start_farming_item = -1;
		//}
		//if (info->end_farming == true)
		//{
		//	player_->EndFarming();
		//	info->end_farming = false;
		//}
	}
	return true;
}

void AMyPlayerController::UpdateNewPlayer()
{
	UWorld* const World = GetWorld();

	int size_ = newPlayers.Size();


	// ?????? ?????????? ?????? ????
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

	if (newplayer.get()->SessionId != itonardoId)
	{
		WhoToSpawn = AMyCharacter::StaticClass();
		AMyCharacter* SpawnCharacter = World->SpawnActor<AMyCharacter>(WhoToSpawn, SpawnLocation_, SpawnRotation, SpawnParams);
		SpawnCharacter->SpawnDefaultController();
		SpawnCharacter->iSessionId = newplayer.get()->SessionId;
		if(itonardoId == 0) 
			SpawnCharacter->SetCharacterMaterial(SpawnCharacter->iSessionId -1);
		else
			SpawnCharacter->SetCharacterMaterial(SpawnCharacter->iSessionId);
	}
	else if(newplayer.get()->SessionId == itonardoId)
	{
		if (itonardoId == -1) return;

		TornadoToSpawn = ATornado::StaticClass();
		ATornado* SpawnTornado = World->SpawnActor<ATornado>(TornadoToSpawn, SpawnLocation_, SpawnRotation, SpawnParams);
		SpawnTornado -> SpawnDefaultController();
	}
	// ?????? ???????? ?????? ????
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


void AMyPlayerController::Reset_Items(int s_id)
{
	// ?????? ???????? ?????? ????
	if (charactersInfo != nullptr)
	{
		charactersInfo->players[s_id].current_snow_count = 0;
		//charactersInfo->players[s_id].current_match_count = 0;
		//charactersInfo->players[s_id].bHasUmbrella = false;
		//charactersInfo->players[s_id].bHasBag = false;;
	
	}
	/*iCurrentSnowballCount = 0;
	iMaxSnowballCount = iOriginMaxSnowballCount;
	iCurrentMatchCount = 0;
	iMaxMatchCount = iOriginMaxMatchCount;
	bHasUmbrella = false;
	bHasBag = false;*/

	//UpdateUI(UICategory::AllOfUI);
}

void AMyPlayerController::SendPlayerInfo(int input)
{
	if (!localPlayerCharacter)
		return;
	auto loc = localPlayerCharacter->GetActorLocation();
	float fNewYaw = localPlayerCharacter->GetActorRotation().Yaw;		//yaw ???? ??????
	auto vel = localPlayerCharacter->GetVelocity();
	FVector MyCameraLocation;
	FRotator MyCameraRotation;
	localPlayerCharacter->GetActorEyesViewPoint(MyCameraLocation, MyCameraRotation);
	float dir = localPlayerCharacter->GetAnim()->GetDirection();

	if (input == COMMAND_MOVE) 
	mySocket->Send_MovePacket(iSessionId, loc, fNewYaw, vel, dir);
	else if (input == COMMAND_ATTACK)
		mySocket->Send_AttackPacket(iSessionId);
	else if (input == COMMAND_GUNATTACK)
		mySocket->Send_GunAttackPacket(iSessionId);
	else if (input == COMMAND_THROW)
		mySocket->Send_Throw_Packet(iSessionId, MyCameraLocation, MyCameraRotation.Vector(),false);
	else if (input == COMMAND_R_ATTACK)
		mySocket->Send_Throw_Packet(iSessionId, MyCameraLocation, MyCameraRotation.Vector(),true);
	else if (input == COMMAND_GUNFIRE)
		mySocket->Send_GunFire_Packet(iSessionId, MyCameraLocation, MyCameraRotation);
	else if (input == COMMAND_DAMAGE)
		mySocket->Send_DamagePacket();
	else if (input == COMMAND_MATCH)
		mySocket->Send_MatchPacket();
	else if (input == COMMAND_UMB_START)
		mySocket->Send_UmbPacket(false);
	else if (input == COMMAND_UMB_END)
		mySocket->Send_UmbPacket(true);
	
}

void AMyPlayerController::SendTeleportInfo(int input)
{
	/*if (!localPlayerCharacter)
		return;
	
	if (input == TEL_FIRE)
		
	else if (input == TEL_BRIDGE)
		
	else if (input == TEL_TOWER)
		
	else if (input == TEL_ICE)
	*/	
}

void AMyPlayerController::SendCheatInfo(int input)
{
	if (!localPlayerCharacter)
		return;
	auto loc = localPlayerCharacter->GetActorLocation();
	float fNewYaw = localPlayerCharacter->GetActorRotation().Yaw;		//yaw ???? ??????
	auto vel = localPlayerCharacter->GetVelocity();
	FVector MyCameraLocation;
	FRotator MyCameraRotation;
	localPlayerCharacter->GetActorEyesViewPoint(MyCameraLocation, MyCameraRotation);
	float dir = localPlayerCharacter->GetAnim()->GetDirection();

	if (input == CHEAT_HP_UP)
		mySocket->Send_ChatPacket(CHEAT_HP_UP);
	else if (input == CHEAT_HP_DOWN)
		mySocket->Send_ChatPacket(CHEAT_HP_DOWN);
	else if (input == CHEAT_SNOW_PLUS)
		mySocket->Send_ChatPacket(CHEAT_SNOW_PLUS);

}

//???????? ???? ????????
void AMyPlayerController::UpdatePlayerInfo(cCharacter& info)
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
		if (bisBone) { 
			player_->UpdateTemperatureState();
			bisBone = false;
		}
		//?????? ????
		if (player_->IsSnowman())
		{
			if (info.My_State == ST_ANIMAL)
			{
				Reset_Items(player_->iSessionId);
				info.HealthValue = player_->iBeginSlowHP;
				player_->ChangeAnimal();
			}
		}
		else if (!player_->IsSnowman())
		{
			if (info.My_State == ST_SNOWMAN)
			{
				Reset_Items(player_->iSessionId);
				info.HealthValue = player_->iMinHP;
				player_->ChangeSnowman();
			}
		}
		// ?????? ???? ????????
		if (player_->iCurrentHP != info.HealthValue)
		{
			//MYLOG(Warning, TEXT("Player damaged hp: %d"), info.HealthValue);
			player_->iCurrentHP = info.HealthValue;
			CallDelegateUpdateHP();

		}
		// ?????? ???? ????????
		if (player_->iCurrentSnowballCount != info.current_snow_count)
		{
			//MYLOG(Warning, TEXT("Player damaged hp: %d"), info.HealthValue);
			player_->iCurrentSnowballCount = info.current_snow_count;
			CallDelegateUpdateCurrentSnowballCount();
		}

	}
}

//void AMyPlayerController::SendFarming(int item_no)
//{
//		mySocket->Send_ItemPacket(item_no);
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
	// ?????? ?????????? ????
	mySocket->Send_ReadyPacket();
	UE_LOG(LogTemp, Warning, TEXT("PlayerReady"));
	bIsReady = true;
#ifdef SINGLEPLAY_DEBUG
	StartGame();	// ???????? - ???????? ?????? startgame ????
#endif
}

void AMyPlayerController::PlayerUnready()
{
	// ?????? ???????????? ????
	UE_LOG(LogTemp, Warning, TEXT("PlayerUnready"));
	bIsReady = false;
}

void AMyPlayerController::StartGame()
{
	if (!bInGame) {
		UE_LOG(LogTemp, Warning, TEXT("StartGame"));
		bInGame = true;
		readyUI->RemoveFromParent();	// ReadyUI ????
		LoadCharacterUI(); // CharacterUI ??????
		// ?????? ???????? ???? ????
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = false;
		auto m_Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		m_Player->UpdateTemperatureState();
	}
	// ???? ???????? ???????? ?????? ????
}

void AMyPlayerController::LoadCharacterUI()
{
	if (characterUIClass)
	{
		characterUI = CreateWidget<UUserWidget>(GetWorld(), characterUIClass);
		if (characterUI)
		{
			characterUI->AddToViewport();
			CallDelegateUpdateAllOfUI();	// ???? ?????? ui ????
		}
	}
}

// ???? ???? ?? ?????? ui ?????? (???? id ?????? ?????? ????, ???? ui ?????? ??????)
void AMyPlayerController::LoadGameResultUI(int winnerSessionId)
{	// remove character ui 
	if (gameResultUIClass)
	{
		gameResultUI = CreateWidget<UUserWidget>(GetWorld(), gameResultUIClass);
		if (gameResultUI)
		{
			characterUI->RemoveFromParent();	// character ui ????
			gameResultUI->AddToViewport();		// game result ui ??????
			
			bool bIsWinner = (iSessionId == winnerSessionId) ? true : false;
			CallDelegateUpdateGameResult(bIsWinner);

			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
	}
}

void AMyPlayerController::CallDelegateUpdateAllOfUI()
{
	CallDelegateUpdateHP();
	CallDelegateUpdateCurrentSnowballCount();
	CallDelegateUpdateCurrentIceballCount();
	CallDelegateUpdateCurrentMatchCount();
	CallDelegateUpdateMaxSnowIceballAndMatchCount();
	CallDelegateUpdateHasUmbrella();
	CallDelegateUpdateHasBag();

	CallDelegateUpdateSelectedItem();
	CallDelegateUpdateSelectedProjectile();
}

void AMyPlayerController::CallDelegateUpdateHP()
{
	if (!characterUI) return;	// CharacterUI?? ???????? ?????? ???? x

	if (FuncUpdateHP.IsBound() == true) FuncUpdateHP.Broadcast(localPlayerCharacter->iCurrentHP);	// ?????????? ????

	//UE_LOG(LogTemp, Warning, TEXT("call delegate update hp %d"), localPlayer->iCurrentHP);
}

void AMyPlayerController::CallDelegateUpdateCurrentSnowballCount()
{
	if (!characterUI) return;

	if (FuncUpdateCurrentSnowballCount.IsBound() == true) FuncUpdateCurrentSnowballCount.Broadcast(localPlayerCharacter->iCurrentSnowballCount);

	//UE_LOG(LogTemp, Warning, TEXT("call delegate update current snowball count %d"), localPlayerCharacter->iCurrentSnowballCount);
}

void AMyPlayerController::CallDelegateUpdateCurrentIceballCount()
{
	if (!characterUI) return;

	if (FuncUpdateCurrentIceballCount.IsBound() == true) FuncUpdateCurrentIceballCount.Broadcast(localPlayerCharacter->iCurrentIceballCount);

	//UE_LOG(LogTemp, Warning, TEXT("call delegate update current iceball count %d"), localPlayerCharacter->iCurrentIceballCount);
}

void AMyPlayerController::CallDelegateUpdateCurrentMatchCount()
{
	if (!characterUI) return;

	if (FuncUpdateCurrentMatchCount.IsBound() == true) FuncUpdateCurrentMatchCount.Broadcast(localPlayerCharacter->iCurrentMatchCount);

	//UE_LOG(LogTemp, Warning, TEXT("call delegate update current match count %d"), localPlayerCharacter->iCurrentMatchCount);
}

void AMyPlayerController::CallDelegateUpdateMaxSnowIceballAndMatchCount()
{
	if (!characterUI) return;

	if (FuncUpdateMaxSnowIceballAndMatchCount.IsBound() == true)
		FuncUpdateMaxSnowIceballAndMatchCount.Broadcast(
			localPlayerCharacter->iMaxSnowballCount, localPlayerCharacter->iMaxIceballCount, localPlayerCharacter->iMaxMatchCount);

	//UE_LOG(LogTemp, Warning, TEXT("call delegate update max snowball count %d, max iceball count %d, max match count %d"), 
	//	localPlayerCharacter->iMaxSnowballCount, localPlayerCharacter->iMaxIceballCount, localPlayerCharacter->iMaxMatchCount);
}

void AMyPlayerController::CallDelegateUpdateHasUmbrella()
{
	if (!characterUI) return;

	if (FuncUpdateHasUmbrella.IsBound() == true) FuncUpdateHasUmbrella.Broadcast(localPlayerCharacter->bHasUmbrella);

	//UE_LOG(LogTemp, Warning, TEXT("call delegate update has umbrella %d"), localPlayerCharacter->bHasUmbrella);
}

void AMyPlayerController::CallDelegateUpdateHasBag()
{
	if (!characterUI) return;

	if (FuncUpdateHasBag.IsBound() == true) FuncUpdateHasBag.Broadcast(localPlayerCharacter->bHasBag);

	//UE_LOG(LogTemp, Warning, TEXT("call delegate update has bag %d"), localPlayerCharacter->bHasBag);
}

void AMyPlayerController::CallDelegateUpdateIsFarmingSnowdrift()
{
	if (!characterUI) return;

	if (FuncUpdateIsFarmingSnowdrift.IsBound() == true) FuncUpdateIsFarmingSnowdrift.Broadcast(localPlayerCharacter->GetIsFarming());

	//UE_LOG(LogTemp, Warning, TEXT("call delegate update is farming snowdrift %d"), localPlayerCharacter->GetIsFarming());
}

void AMyPlayerController::CallDelegateUpdateSnowdriftFarmDuration(float farmDuration)
{
	if (!characterUI) return;

	if (FuncUpdateSnowdriftFarmDuration.IsBound() == true) FuncUpdateSnowdriftFarmDuration.Broadcast(farmDuration);

	//UE_LOG(LogTemp, Warning, TEXT("call delegate update snowdrift farm duration %f"), farmDuration);
}

void AMyPlayerController::CallDelegateUpdateSelectedItem()
{
	if (!characterUI) return;

	if (FuncUpdateSelectedItem.IsBound() == true) FuncUpdateSelectedItem.Broadcast(localPlayerCharacter->iSelectedItem);

	//UE_LOG(LogTemp, Warning, TEXT("call delegate update selected item %d"), localPlayerCharacter->iSelectedItem);
}

void AMyPlayerController::CallDelegateUpdateSelectedProjectile()
{
	if (!characterUI) return;

	if (FuncUpdateSelectedProjectile.IsBound() == true) FuncUpdateSelectedProjectile.Broadcast(localPlayerCharacter->iSelectedProjectile);

	//UE_LOG(LogTemp, Warning, TEXT("call delegate update selected projectile %d"), localPlayerCharacter->iSelectedProjectile);
}

void AMyPlayerController::CallDelegateUpdateGameResult(bool isWinner)
{
	if (!gameResultUI) return;
	
	if (FuncUpdateGameResult.IsBound() == true) FuncUpdateGameResult.Broadcast(isWinner);
}

void AMyPlayerController::FixRotation()
{
	float pitch, yaw, roll;
	UKismetMathLibrary::BreakRotator(GetControlRotation(), roll, pitch, yaw);
	pitch = UKismetMathLibrary::ClampAngle(pitch, -15.0f, 30.0f);
	FRotator newRotator = UKismetMathLibrary::MakeRotator(roll, pitch, yaw);
	SetControlRotation(newRotator);
}

void AMyPlayerController::LoadLoginUI()
{
	if (loginUIClass)
	{
		loginUI = CreateWidget<UUserWidget>(GetWorld(), loginUIClass);
		if (loginUI)
		{
			loginUI->AddToViewport();

			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
	}
}

void AMyPlayerController::BtnCreateAccount(FString id, FString pw)
{
	//UE_LOG(LogTemp, Warning, TEXT("CREATE id : %s, pw : %s"), *id, *pw);

	// id, pw?? ???? ???????? ???? ???? return
	if (id.Len() == 0 && pw.Len() == 0)
	{
		loginInfoText = TEXT("Input ID and PW");
		return;
	}
	else if (id.Len() == 0)
	{
		loginInfoText = TEXT("Input ID");
		return;
	}
	else if (pw.Len() == 0)
	{
		loginInfoText = TEXT("Input PW");
		return;
	}

	// id, pw?? ?????? ???? ?????? ????
	// id, pw?? ???? ???? ?? Ready UI?? ?????????? ??????

	//DeleteLoginUICreateReadyUI();	// Ready UI?? ?????????? ???? ????
}

void AMyPlayerController::BtnLogin(FString id, FString pw)
{
	//UE_LOG(LogTemp, Warning, TEXT("LOGIN id : %s, pw : %s"), *id, *pw);

	
	// ???????? - id, pw?? ???? x?? ???? ???? ?????? ??????
	if (id.Len() == 0 && pw.Len() == 0)
	{
		g_socket->Send_LoginPacket(TCHAR_TO_UTF8(*id), TCHAR_TO_UTF8(*pw));
		//DeleteLoginUICreateReadyUI();	// Ready UI?? ?????????? ???? ????
	}
	else
	{
		if (id.Len() == 0)
		{
			loginInfoText = TEXT("Input ID");
			return;
		}
		if (pw.Len() == 0)
		{
			loginInfoText = TEXT("Input PW");
			return;
		}

		g_socket->Send_LoginPacket(TCHAR_TO_UTF8(*id), TCHAR_TO_UTF8(*pw));


		// id, pw?? DB?? ?????? ???? ?????????? ????

		// id?? ???????? ???? ???? return
		//if ()
		//{
		//	loginInfoText = TEXT("Invalid ID");
		//	return;
		//}
		// pw ?????? ?????? ???? return
		//if ()
		//{
		//	loginInfoText = TEXT("Wrong PW");
		//	return;
		//}

		// id, pw?? ?????? ????
		//DeleteLoginUICreateReadyUI();	// Ready UI?? ?????????? ???? ????
	}
}

void AMyPlayerController::DeleteLoginUICreateReadyUI()
{
	loginUI->RemoveFromParent();	// LoginUI ????
	LoadReadyUI(); // ReadyUI ??????
}