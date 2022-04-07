// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "ClientSocket.h"

#pragma comment(lib, "ws2_32.lib")


AMyPlayerController::AMyPlayerController()
{

	myClientSocket = ClientSocket::GetSingleton();
	/*myClientSocket->h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(myClientSocket->_socket), myClientSocket->h_iocp, 0, 0);
	*///int ret = myClientSocket->Connect();
	//if (ret)
	//{
		//UE_LOG(LogClass, Log, TEXT("IOCP Server connect success!"));
		myClientSocket->SetPlayerController(this);
		
	//}


	PrimaryActorTick.bCanEverTick = true;

	bNewPlayerEntered = false;
}


void AMyPlayerController::BeginPlay()
{
	//Super::BeginPlay(); //���� ���ᰡ �ȵ�

	//auto m_Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	//if (!m_Player)
	//	return;
	//auto MyLocation = m_Player->GetActorLocation();
	//auto MyRotation = m_Player->GetActorRotation();

	auto m_Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!m_Player)
		return;
	auto MyLocation = m_Player->GetActorLocation();
	auto MyRotation = m_Player->GetActorRotation();
	myClientSocket->fMy_x = MyLocation.X;
	myClientSocket->fMy_y = MyLocation.Y;
	myClientSocket->fMy_z = MyLocation.Z;
	myClientSocket->StartListen();
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AMyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	myClientSocket->CloseSocket();
	myClientSocket->StopListen();
	//Super::EndPlay(EndPlayReason);
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bNewPlayerEntered)
		UpdateNewPlayer();

	//UpdateRotation();
}


void AMyPlayerController::UpdatePlayerInfo(int input)
{
	auto m_Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!m_Player)
		return;
	iMySessionId = m_Player->iSessionID;
	auto MyLocation = m_Player->GetActorLocation();
	auto MyRotation = m_Player->GetActorRotation();
	if (input == COMMAND_MOVE)
		myClientSocket->ReadyToSend_MovePacket(iMySessionId, MyLocation.X, MyLocation.Y, MyLocation.Z);
	else if (input == COMMAND_ATTACK)
		myClientSocket->ReadyToSend_AttackPacket();

}

void AMyPlayerController::UpdatePlayerS_id(int id)
{
	iMySessionId = id;
	auto m_Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!m_Player)
		return;
	m_Player->iSessionID = id;
	m_Player->SetActorLocationAndRotation(FVector(id * 100.0f, id * 100.0f, m_Player->GetActorLocation().Z), FRotator(0.0f, -90.0f, 0.0f));
}

void AMyPlayerController::RecvNewPlayer(int sessionID, float x, float y, float z)
{
	MYLOG(Warning, TEXT("recv ok player%d : %f, %f, %f"), sessionID, x, y, z);

	UWorld* World = GetWorld();

	bNewPlayerEntered = true;
	iOtherSessionId = sessionID;
	fOther_x = x;
	fOther_y = y;
	fOther_z = z;
}

void AMyPlayerController::UpdateNewPlayer()
{
	UWorld* const World = GetWorld();

	// ���ο� �÷��̾ �ڱ� �ڽ��̸� ����
	if (iOtherSessionId == iMySessionId)
	{
		bNewPlayerEntered = false;
		return;
	}

	bNewPlayerEntered = true;

	// ���ο� �÷��̾ �ʵ忡 ����
	FVector SpawnLocation_;
	SpawnLocation_.X = fOther_x;
	SpawnLocation_.Y = fOther_y;
	SpawnLocation_.Z = fOther_z;

	FRotator SpawnRotation;
	SpawnRotation.Yaw = 0.0f;
	SpawnRotation.Pitch = 0.0f;
	SpawnRotation.Roll = 0.0f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	//SpawnParams.Name = FName(*FString(to_string(iOtherSessionId).c_str()));

	TSubclassOf<class AMyCharacter> WhoToSpawn;
	WhoToSpawn = AMyCharacter::StaticClass();
	AMyCharacter* SpawnCharacter = World->SpawnActor<AMyCharacter>(WhoToSpawn, SpawnLocation_, SpawnRotation, SpawnParams);

	if (nullptr == SpawnCharacter)
	{
		MYLOG(Warning, TEXT("spawn fail"));
		return;
	}
	MYLOG(Warning, TEXT("spawn ok player%d : %f, %f, %f"), iOtherSessionId, fOther_x, fOther_y, fOther_z);

	SpawnCharacter->SpawnDefaultController();
	SpawnCharacter->iSessionID = iOtherSessionId;

	bNewPlayerEntered = false;
}

//void AMyPlayerController::UpdateRotation()
//{
//	float pitch, yaw, roll;
//	UKismetMathLibrary::BreakRotator(GetControlRotation(), roll, pitch, yaw);
//	pitch = UKismetMathLibrary::ClampAngle(pitch, -15.0f, 30.0f);
//	FRotator newRotator = UKismetMathLibrary::MakeRotator(roll, pitch, yaw);
//	SetControlRotation(newRotator);
//}
