// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyAnimInstance.h"
#include "MySnow.h"
#include "MyItem.h"
#include "MyPlayerController.h"
#include "MyCharacterStatComponent.h"
#include "Snowdrift.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UMyCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	check(Camera != nullptr);
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(80.0f);
	GetCapsuleComponent()->SetCapsuleRadius(35.0f);
	SpringArm->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -79.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 220.0f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	SpringArm->SocketOffset.Y = 30.0f;
	SpringArm->SocketOffset.Z = 35.0f;
	bUseControllerRotationYaw = true;

	bear = CreateDefaultSubobject<USkeletalMesh>(TEXT("BEAR"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BEAR(TEXT("/Game/Characters/Bear/bear.bear"));
	if (SK_BEAR.Succeeded())
	{
		bear = SK_BEAR.Object;
		GetMesh()->SetSkeletalMesh(bear);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	snowman = CreateDefaultSubobject<USkeletalMesh>(TEXT("SNOWMAN"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_SNOWMAN(TEXT("/Game/Characters/Snowman/snowman.snowman"));
	if (SK_SNOWMAN.Succeeded())
	{
		snowman = SK_SNOWMAN.Object;
	}
	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> BEAR_ANIM(TEXT("/Game/Animations/Bear/BearAnimBP.BearAnimBP_C"));
	if (BEAR_ANIM.Succeeded())
	{
		bearAnim = BEAR_ANIM.Class;
		GetMesh()->SetAnimInstanceClass(BEAR_ANIM.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> SNOWMAN_ANIM(TEXT("/Game/Animations/Snowman/SnowMan_AnimBP.SnowMan_AnimBP_C"));
	if (SNOWMAN_ANIM.Succeeded())
	{
		snowmanAnim = SNOWMAN_ANIM.Class;
	}

	GetCharacterMovement()->JumpZVelocity = 800.0f;
	IsAttacking = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));   

	ProjectileClass = AMySnowball::StaticClass();

	Snowball = NULL;

	//iSnowballCount = 0;	// 실제 설정값
	iSnowballCount = 10;	// 디버깅용
	iSnowballMaxCount = 30;	// 임시값

	canFarmItem = NULL;
	bIsFarming = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateFarming(DeltaTime);

	FVector cameraLocation;
	FRotator cameraRotation;
	GetActorEyesViewPoint(cameraLocation, cameraRotation);
	//UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), cameraRotation.Vector().X, cameraRotation.Vector().Y, cameraRotation.Vector().Z);
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	MYCHECK(nullptr != MyAnim);

	MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);

	CharacterStat->OnHPIsZero.AddLambda([this]()->void {

		MYLOG(Warning, TEXT("OnHPIsZero"));
		IsDead = true;
		if (IsDead)
		{
			GetMesh()->SetSkeletalMesh(snowman);
			GetMesh()->SetAnimInstanceClass(snowmanAnim);
		}
		SetActorEnableCollision(false);

		});
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Farming"), EInputEvent::IE_Pressed, this, &AMyCharacter::StartFarming);
	PlayerInputComponent->BindAction(TEXT("Farming"), EInputEvent::IE_Released, this, &AMyCharacter::EndFarming);
}

bool AMyCharacter::CanSetItem()
{
	return (nullptr == CurrentItem);
}

void AMyCharacter::SetItem(AMyItem* NewItem)
{
	MYCHECK(nullptr != NewItem && nullptr == CurrentItem);
	FName ItemSocket(TEXT("hand_rSocket"));
	auto CurItem = GetWorld()->SpawnActor<AMyItem>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != NewItem)
	{
		NewItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemSocket);
		NewItem->SetOwner(this);
		CurrentItem = NewItem;
	}
}

void AMyCharacter::UpDown(float NewAxisValue)
{
	if (NewAxisValue != 0)
	{
		AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
		PlayerController->UpdatePlayerInfo(COMMAND_MOVE);
	}
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void AMyCharacter::LeftRight(float NewAxisValue)
{
	if (NewAxisValue != 0)
	{
		AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
		PlayerController->UpdatePlayerInfo(COMMAND_MOVE);
	}
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}

void AMyCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AMyCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void AMyCharacter::Attack()
{
	if (IsAttacking) return;
	if (iSnowballCount <= 0) return;	// 눈덩이를 소유하고 있지 않으면 공격 x

	MyAnim->PlayAttackMontage();
	IsAttacking = true;
	// 디버깅용 - 실제는 주석 해제
	//iSnowballCount -= 1;	// 공격 시 눈덩이 소유량 1 감소

	// Attempt to fire a projectile.
	if (ProjectileClass)
	{
		UWorld* World = GetWorld();

		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			FTransform SnowballSocketTransform = GetMesh()->GetSocketTransform(TEXT("SnowballSocket"));
			Snowball = World->SpawnActor<AMySnowball>(ProjectileClass, SnowballSocketTransform, SpawnParams);
			FAttachmentTransformRules atr = FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
			Snowball->AttachToComponent(GetMesh(), atr, TEXT("SnowballSocket"));
		}
	}
}
//void AMyCharacter::Attack()
//{
//	if (IsAttacking) return;
//
//
//	MyAnim->PlayAttackMontage();
//	IsAttacking = true;
//
//	// Attempt to fire a projectile.
//	if (ProjectileClass)
//	{
//		// Get the camera transform.
//		FVector CameraLocation;
//		FRotator CameraRotation;
//		GetActorEyesViewPoint(CameraLocation, CameraRotation);
//
//		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
//		MuzzleOffset.Set(100.0f, 50.0f, -100.0f);
//
//		FVector MuzzleLocation = CameraLocation+FTransform(CameraRotation).TransformVector(MuzzleOffset);
//		FRotator MuzzleRotation = CameraRotation;
//
//		MuzzleRotation.Pitch += 10.0f; 
//		UWorld* World = GetWorld();
//
//		if (World)
//		{
//			FActorSpawnParameters SpawnParams;
//			SpawnParams.Owner = this; 
//			SpawnParams.Instigator = GetInstigator();
//			AMySnow* Projectile = World->SpawnActor<AMySnow>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
//			if (Projectile)
//			{
//				FVector LaunchDirection = MuzzleRotation.Vector(); 
//				Projectile->FireInDirection(LaunchDirection);
//				Projectile->SetAttack(CharacterStat->GetAttack());
//			}
//		}
//	}
//}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//CharacterStat->SetDamage(FinalDamage);
	//if (CurrentState == ECharacterState::DEAD)
	//{
	//	if (EventInstigator->IsPlayerController())
	//	{
	//		auto instigator = Cast<AABPlayerController>(EventInstigator);
	//		ABCHECK(nullptr != instigator, 0.0f);
	//		instigator->NPCKill(this);
	//	}
	//}
	MYLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

void AMyCharacter::ReleaseSnowball()
{
	if (IsValid(Snowball))
	{
		FDetachmentTransformRules dtr = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
		Snowball->DetachFromActor(dtr);

		if (Snowball->GetClass()->ImplementsInterface(UI_Throwable::StaticClass()))
		{
			FVector cameraLocation;
			FRotator cameraRotation;
			GetActorEyesViewPoint(cameraLocation, cameraRotation);

			II_Throwable::Execute_Throw(Snowball, cameraRotation.Vector());
			Snowball = NULL;
		}

	}
}

void AMyCharacter::StartFarming()
{
	if (IsValid(canFarmItem))
	{
		ASnowdrift* snowdrift = Cast<ASnowdrift>(canFarmItem);
		if (snowdrift)
		{
			bIsFarming = true;
		}
	}
}

void AMyCharacter::EndFarming()
{
	if (IsValid(canFarmItem))
	{
		ASnowdrift* snowdrift = Cast<ASnowdrift>(canFarmItem);
		if (snowdrift)
		{	// F키로 눈 무더기 파밍 중 F키 release 시 눈 무더기 duration 초기화
			snowdrift->SetFarmDuration(ASnowdrift::fFarmDurationMax);
			bIsFarming = false;
		}
	}
}

void AMyCharacter::UpdateFarming(float deltaTime)
{
	if (bIsFarming)
	{
		if (IsValid(canFarmItem))
		{
			ASnowdrift* snowdrift = Cast<ASnowdrift>(canFarmItem);
			if (snowdrift)
			{	// 눈 무더기 duration 만큼 F키를 누르고 있으면 캐릭터의 눈덩이 추가 
				float lastFarmDuration = snowdrift->GetFarmDuration();
				float newFarmDuration = lastFarmDuration - deltaTime;
				snowdrift->SetFarmDuration(newFarmDuration);

				if (newFarmDuration <= 0)
				{
					iSnowballCount += 10;
					if (iSnowballCount >= iSnowballMaxCount)
					{
						iSnowballCount = iSnowballMaxCount;
					}
					snowdrift->Destroy();
				}
			}
		}
	}
}