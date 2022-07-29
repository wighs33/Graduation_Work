// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyAnimInstance.h"
#include "MyPlayerController.h"
#include "Snowdrift.h"
#include "Icedrift.h"
#include "Debug.h"
#include "Kismet/KismetMathLibrary.h"
#include "UmbrellaAnimInstance.h"
#include "EmptyActor.h"
#include "GameFramework/HUD.h"
#include "Jetski.h"
#include "SupplyBox.h"

const int AMyCharacter::iMaxHP = 390;
const int AMyCharacter::iMinHP = 270;
const int AMyCharacter::iBeginSlowHP = 300;	// ĳ���Ͱ� ���ο� ���°� �Ǳ� �����ϴ� hp
const int iNormalSpeed = 600;	// ĳ���� �⺻ �̵��ӵ�
const int iSlowSpeed = 400;		// ĳ���� ���ο� ���� �̵��ӵ�
const int iJetskiSpeed = 1200;		// Jetski ž�� �� �̵��ӵ�
const float fChangeSnowmanStunTime = 3.0f;	// ������ - 10.0f, �����ȭ �� �� ���� �ð�
const float fStunTime = 3.0f;	// ������� ������ �¾��� �� ���� �ð�
const int iOriginMaxSnowballCount = 10;	// ������ �ִ뺸���� (�ʱ�, ����x)
const int iOriginMaxIceballCount = 10;	// ���̽��� �ִ뺸���� (�ʱ�, ����x)
const int iOriginMaxMatchCount = 2;	// ���� �ִ뺸���� (�ʱ�, ����x)
const int iNumOfWeapons = 2;	// ���� ���� ��
const int iNumOfProjectiles = 2;	// �߻�ü ���� ��
const float fAimingTime = 0.2f;		// �����ϴ� �� �ɸ��� �ð� (ī�޶� ��ȯ��, �ִϸ��̼��� ����)
const float fThrowPower = 700.0f;
const float fMaxChargingTime = 5.0f;	// �ִ� ��¡ �ð�
const float fSnowballInitialSpeed = 2000.0f;	// ������ �ʱ� �ӵ�
const int iEachBoneCount = 23; //������ �󸮴� �޽� ����

// ���� �� �ؽ���
FString TextureStringArray[] = {
	TEXT("/Game/Characters/Bear/bear_texture.bear_texture"),
	TEXT("/Game/Characters/Bear/bear_texture_light_red.bear_texture_light_red"),
	TEXT("/Game/Characters/Bear/bear_texture_yellow.bear_texture_yellow"),
	TEXT("/Game/Characters/Bear/bear_texture_light_green.bear_texture_light_green"),
	TEXT("/Game/Characters/Bear/bear_texture_cyan.bear_texture_cyan"),
	TEXT("/Game/Characters/Bear/bear_texture_blue.bear_texture_blue"),
	TEXT("/Game/Characters/Bear/bear_texture_light_gray.bear_texture_light_gray"),
	TEXT("/Game/Characters/Bear/bear_texture_black.bear_texture_black") };

const int iNumOfPathSpline = 35;
FString SplineStringArray[] = {
	TEXT("spline1"), TEXT("spline2"),TEXT("spline3"),TEXT("spline4"),TEXT("spline5"),TEXT("spline6"), TEXT("spline7"),
	TEXT("spline8"),TEXT("spline9"),TEXT("spline10"),TEXT("spline11"),TEXT("spline12"), TEXT("spline13"),TEXT("spline14"),
	TEXT("spline15"), TEXT("spline16"),TEXT("spline17"),TEXT("spline18"),TEXT("spline19"), TEXT("spline20"),TEXT("spline21"),
	TEXT("spline22"), TEXT("spline23"),TEXT("spline24"),TEXT("spline25"),TEXT("spline26"), TEXT("spline27"),TEXT("spline28"),
	TEXT("spline29"), TEXT("spline30"),TEXT("spline31"),TEXT("spline32"),TEXT("spline33"), TEXT("spline34"),TEXT("spline35")
};

// Sets default values
AMyCharacter::AMyCharacter()
{
	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	check(camera != nullptr);

	springArm->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	camera->SetupAttachment(springArm);

	GetCapsuleComponent()->SetCapsuleHalfHeight(74.0f);
	GetCapsuleComponent()->SetCapsuleRadius(37.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));
	GetCapsuleComponent()->SetUseCCD(true);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMyCharacter::OnHit);
	GetCapsuleComponent()->BodyInstance.bNotifyRigidBodyCollision = true;
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -77.0f), FRotator(0.0f, -90.0f, 0.0f));
	springArm->TargetArmLength = 220.0f;
	springArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator::ZeroRotator);
	springArm->bUsePawnControlRotation = true;
	springArm->bInheritPitch = true;
	springArm->bInheritRoll = true;
	springArm->bInheritYaw = true;
	springArm->bDoCollisionTest = true;
	springArm->SocketOffset.Y = 30.0f;
	springArm->SocketOffset.Z = 35.0f;
	bUseControllerRotationYaw = true;

	springArm2 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM2"));
	springArm2->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	springArm2->TargetArmLength = 200.0f;
	springArm2->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(0.0f, -30.0f, 0.0f));
	springArm2->bUsePawnControlRotation = true;
	springArm2->bInheritPitch = true;
	springArm2->bInheritRoll = true;
	springArm2->bInheritYaw = true;
	springArm2->bDoCollisionTest = true;
	springArm2->SocketOffset.Y = 90.0f;
	springArm2->SocketOffset.Z = 35.0f;

	springArm3 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM3"));
	springArm3->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	springArm3->TargetArmLength = 220.0f;
	springArm3->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 80.0f), FRotator::ZeroRotator);
	springArm3->bUsePawnControlRotation = true;
	springArm3->bInheritPitch = true;
	springArm3->bInheritRoll = true;
	springArm3->bInheritYaw = true;
	springArm3->bDoCollisionTest = true;

	camera3 = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA3"));
	check(camera3 != nullptr);
	camera3->SetupAttachment(springArm3);

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

	static ConstructorHelpers::FClassFinder<UAnimInstance> BEAR_ANIM(TEXT("/Game/Animations/Bear/BearAnimBP.BearAnimBP_C"));	// _C ���̱�
	if (BEAR_ANIM.Succeeded())
	{
		bearAnim = BEAR_ANIM.Class;
		GetMesh()->SetAnimInstanceClass(BEAR_ANIM.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> SNOWMAN_ANIM(TEXT("/Game/Animations/Snowman/SnowmanAnimBP.SnowmanAnimBP_C"));
	if (SNOWMAN_ANIM.Succeeded())
	{
		snowmanAnim = SNOWMAN_ANIM.Class;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>BearMaterial(TEXT("/Game/Characters/Bear/M_Bear.M_Bear"));
	if (BearMaterial.Succeeded())
	{
		bearMaterial = BearMaterial.Object;
	}

	//static ConstructorHelpers::FObjectFinder<UMaterialInstance>BearMaterial(TEXT("/Game/Characters/Bear/M_Bear_Inst.M_Bear_Inst"));
	//if (BearMaterial.Succeeded())
	//{
	//	bearMaterial = BearMaterial.Object;
	//}

	// ��� ������ �� �ؽ��� �ε��ؼ� ����
	for (int i = 0; i < 8; ++i)
	{
		ConstructorHelpers::FObjectFinder<UTexture>BearTexture(*(TextureStringArray[i]));
		if (BearTexture.Succeeded())
		{
			bearTextureArray.Add(BearTexture.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>SnowmanMaterial(TEXT("/Game/Characters/Snowman/M_Snowman.M_Snowman"));
	if (SnowmanMaterial.Succeeded())
	{
		snowmanMaterial = SnowmanMaterial.Object;
	}

	if (!shotgunMeshComponent)
	{
		shotgunMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("shotgunMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SHOTGUN(TEXT("/Game/NonCharacters/Shotgun_SM.Shotgun_SM"));
		if (SM_SHOTGUN.Succeeded())
		{
			shotgunMeshComponent->SetStaticMesh(SM_SHOTGUN.Object);
			shotgunMeshComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));

			FAttachmentTransformRules atr = FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
			shotgunMeshComponent->SetupAttachment(GetMesh(), TEXT("ShotgunSocket"));
			//shotgunMeshComponent->AttachToComponent(GetMesh(), atr, TEXT("ShotgunSocket"));

			shotgunMeshComponent->SetVisibility(false);

			for (int i = 0; i < 8; ++i)
			{
				snowballBombDirArray.Add(FVector());
			}
		}
	}

	if (!umbrella1MeshComponent)
	{
		umbrella1MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("umbrella1MeshComponent"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_UMBRELLA1(TEXT("/Game/NonCharacters/umbrella1.umbrella1"));
		if (SK_UMBRELLA1.Succeeded())
		{
			umbrella1MeshComponent->SetSkeletalMesh(SK_UMBRELLA1.Object);
			umbrella1MeshComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
			umbrella1MeshComponent->SetupAttachment(GetMesh(), TEXT("UmbrellaSocket"));
			umbrella1MeshComponent->SetVisibility(false);
		}
	}

	if (!umbrella2MeshComponent)
	{
		umbrella2MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("umbrella2MeshComponent"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_UMBRELLA2(TEXT("/Game/NonCharacters/umbrella2.umbrella2"));
		if (SK_UMBRELLA2.Succeeded())
		{
			umbrella2MeshComponent->SetSkeletalMesh(SK_UMBRELLA2.Object);
			umbrella2MeshComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
			umbrella2MeshComponent->SetupAttachment(GetMesh(), TEXT("UmbrellaSocket"));
			umbrella2MeshComponent->SetVisibility(false);
		}
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> UMB1_ANIM(TEXT("/Game/Animations/Umbrella/Umb1AnimBP.Umb1AnimBP_C"));
	if (UMB1_ANIM.Succeeded())
	{
		umbrella1Anim = UMB1_ANIM.Class;
		umbrella1MeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		umbrella1MeshComponent->SetAnimInstanceClass(UMB1_ANIM.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> UMB2_ANIM(TEXT("/Game/Animations/Umbrella/Umb2AnimBP.Umb2AnimBP_C"));
	if (UMB2_ANIM.Succeeded())
	{
		umbrella2Anim = UMB2_ANIM.Class;
		umbrella2MeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		umbrella2MeshComponent->SetAnimInstanceClass(UMB2_ANIM.Class);
	}

	if (!umb1CollisionComponent)
	{
		umb1CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("umb1CollisionComponent"));
		umb1CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("BlockAll"));
		umb1CollisionComponent->SetBoxExtent(FVector(4.0f, 4.0f, 39.0f));
		umb1CollisionComponent->SetupAttachment(umbrella1MeshComponent);
		umb1CollisionComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 44.0f));

		umb1CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		umb1CollisionComponent->BodyInstance.bNotifyRigidBodyCollision = true;
		umb1CollisionComponent->SetUseCCD(true);
	}

	if (!umb2CollisionComponent)
	{
		umb2CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("umb2CollisionComponent"));
		umb2CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("BlockAll"));
		umb2CollisionComponent->SetBoxExtent(FVector(55.0f, 55.0f, 9.0f));
		umb2CollisionComponent->SetupAttachment(umbrella2MeshComponent);
		umb2CollisionComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 68.0f));

		umb2CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		umb2CollisionComponent->BodyInstance.bNotifyRigidBodyCollision = true;
		umb2CollisionComponent->SetUseCCD(true);
	}

	if (!bagMeshComponent)
	{
		bagMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bagMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BAG(TEXT("/Game/NonCharacters/Backpack_SM.Backpack_SM"));
		if (SM_BAG.Succeeded())
		{
			bagMeshComponent->SetStaticMesh(SM_BAG.Object);
			bagMeshComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));

			bagMeshComponent->SetupAttachment(GetMesh(), TEXT("BagSocket"));

			bagMeshComponent->SetVisibility(false);
		}
	}

	if (!projectilePath)
	{
		projectilePath = CreateDefaultSubobject<USplineComponent>(TEXT("ProjectilePath"));
		projectilePath->SetupAttachment(GetMesh());
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Spline(TEXT("/Game/NonCharacters/Spline_SM.Spline_SM"));
	if (SM_Spline.Succeeded())
	{
		for (int i = 0; i < iNumOfPathSpline; ++i)
		{
			USplineMeshComponent* splineMesh = CreateDefaultSubobject<USplineMeshComponent>(*(SplineStringArray[i]));
			splineMesh->SetStaticMesh(SM_Spline.Object);
			splineMesh->SetVisibility(false);
			//splineMesh->bOnlyOwnerSee = true;	// �ش� �÷��̾ ������ ���̵���
			splineMesh->SetCastShadow(false);	// ������ �׸��� �Ⱥ��̵���
			splineMeshComponents.Add(splineMesh);
		}
	}

	if (!projectilePathStartPos)
	{
		projectilePathStartPos = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectilePathStartPos"));
		projectilePathStartPos->SetupAttachment(GetMesh());
		projectilePathStartPos->SetRelativeLocation(FVector(-32.0f, 38.012852f, 116.264641f));
	}

	if (!jetskiMeshComponent)
	{
		jetskiMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("jetskiMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Jetski(TEXT("/Game/NonCharacters/SM_Jetski.SM_Jetski"));
		if (SM_Jetski.Succeeded())
		{
			jetskiMeshComponent->SetStaticMesh(SM_Jetski.Object);
			jetskiMeshComponent->BodyInstance.SetCollisionProfileName(TEXT("Jetski"));
			jetskiMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			jetskiMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -56.0f));
			jetskiMeshComponent->SetVisibility(false);
		}
	}

	if (!driveAnimAsset)
	{
		static ConstructorHelpers::FObjectFinder<UAnimationAsset> ANIM_Drive(TEXT("/Game/Animations/Bear/bear_driving_Anim.bear_driving_Anim"));
		if (ANIM_Drive.Succeeded())
		{
			driveAnimAsset = ANIM_Drive.Object;
		}
	}

	SettingHead();
	SettingLeftForearm();
	SettingLeftUpperArm();
	SettingRightForearm();
	SettingRightUpperArm();
	SettingCenter();
	SettingLeftThigh();
	SettingLeftCalf();
	SettingRightThigh();
	SettingRightCalf();

	stunNiagara = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/FX/Stun/NS_Stun.NS_Stun"), nullptr, LOAD_None, nullptr);
	changeNiagara = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/AssetFolder/MagicSpells_Ice/Effects/Sistems/NS_FrostExplosion.NS_FrostExplosion"), nullptr, LOAD_None, nullptr);

	//tmpNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("tmpComponent"));
	//static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS_TMP(TEXT("/Game/AssetFolder/MagicSpells_Ice/Effects/Sistems/NS_FrostSpray.NS_FrostSpray"));
	//tmpNiagara->SetAsset(NS_TMP.Object);
	//tmpNiagara->SetupAttachment(GetMesh());
	//tmpNiagara->SetRelativeLocation(FVector(0.0f, 0.0f, 160.0f));
	//tmpNiagara->SetVisibility(true);

	GetCharacterMovement()->JumpZVelocity = 800.0f;
	isAttacking = false;

	projectileClass = AMySnowball::StaticClass();
	shotgunProjectileClass = ASnowballBomb::StaticClass();
	iceballClass = AIceball::StaticClass();
	jetskiClass = AJetski::StaticClass();

	iSessionId = -1;
	iColor = 0;

	iCurrentHP = iMaxHP;	// ���� ������
	//iCurrentHP = iMinHP + 1;	// ������ - ���ð� �� ��������� ��ȭ

	snowball = nullptr;
	iceball = nullptr;
	
	iMaxSnowballCount = iOriginMaxSnowballCount;
	iCurrentSnowballCount = 0;	// ���� ������
	//iCurrentSnowballCount = 10;	// ������
	iMaxIceballCount = iOriginMaxIceballCount;
	iCurrentIceballCount = 0;	// ���� ������
	//iCurrentIceballCount = 10;	// ������
	iMaxMatchCount = iOriginMaxMatchCount;
	iCurrentMatchCount = 0;
	bHasUmbrella = false;
	bHasBag = false;
	bHasShotgun = false;

	farmingItem = nullptr;
	bIsFarming = false;
	
	bIsInsideOfBonfire = false;

	iCharacterState = CharacterState::AnimalNormal;
	bIsSnowman = false;
	GetCharacterMovement()->MaxWalkSpeed = iNormalSpeed;	// ĳ���� �̵��ӵ� ����
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->PushForceFactor = 75000.0f;		// �⺻�� 750000.0f
	GetCharacterMovement()->MaxAcceleration = 1024.0f;		// �⺻�� 2048.0f

	iSelectedItem = ItemTypeList::Match;	// ���õ� ������ �⺻�� - ����
	
	bIsInTornado = false;
	rotateCont = false;

	iSelectedWeapon = Weapon::Hand;
	iSelectedProjectile = Projectile::Snowball;

	iUmbrellaState = UmbrellaState::UmbClosed;
	bReleaseUmbrella = true;

	smokeNiagara = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/FX/NS_Smoke.NS_Smoke"), nullptr, LOAD_None, nullptr);

	overlappedTornado = nullptr;

	bIsAiming = false;
	fAimingElapsedTime = 0.0f;

	bIsRiding = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ���� �ʿ� - ĳ������ session id�� ������ �� �� �Լ��� ȣ��ǵ���
	SetCharacterMaterial(iColor);	// ĳ���� ��Ƽ���� ����(����)

	playerController = Cast<APlayerController>(GetController());	// �����ڿ��� �ϸ� x (��Ʈ�ѷ��� �����Ǳ� ���ε�)
	localPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	
	SetAimingCameraPos();

	WaitForStartGame();	// ���ð�

	camera3->Deactivate();
}

void AMyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	MYLOG(Warning, TEXT("endplay"));
	//AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	//PlayerController->GetSocket()->StopListen();

	if (iSessionId == localPlayerController->iSessionId)
	{
		localPlayerController->mySocket->Send_LogoutPacket(iSessionId);
		SleepEx(0, true);
		localPlayerController->mySocket->CloseSocket();
		localPlayerController->mySocket->~ClientSocket();
		//delete[] localPlayerController->mySocket;
	}
 }


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bIsSnowman)
	{
		UpdateFarming(DeltaTime);
		UpdateHP();
		UpdateSpeed();
		UpdateAiming();
		UpdateJetski();
	}

	UpdateZByTornado();		// ĳ���Ͱ� ����̵� ������ ��� z�� ����
	UpdateControllerRotateByTornado();	// ����̵��� ���� ���ϻ����� ��� ȸ���ϵ���

	FreezeAnimationEndCheck(headHandle, bHeadAnimEnd);
	FreezeAnimationEndCheck(leftForearmHandle, bLeftForearmAnimEnd);
	FreezeAnimationEndCheck(leftUpperarmHandle, bLeftUpperarmAnimEnd);
	FreezeAnimationEndCheck(rightForearmHandle, bRightForearmAnimEnd);
	FreezeAnimationEndCheck(rightUpperarmHandle, bRightUpperarmAnimEnd);
	FreezeAnimationEndCheck(centerHandle, bCenterAnimEnd);
	FreezeAnimationEndCheck(leftThighHandle, bLeftThighAnimEnd);
	FreezeAnimationEndCheck(leftCalfHandle, bLeftCalfAnimEnd);
	FreezeAnimationEndCheck(rightThighHandle, bRightThighAnimEnd);
	FreezeAnimationEndCheck(rightCalfHandle, bRightCalfAnimEnd);
}

void AMyCharacter::init_Socket()
{
	/*AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerController->mySocket = nullptr;
	PlayerController->mySocket = new ClientSocket();
	PlayerController->mySocket->SetPlayerController(PlayerController);
	g_socket = PlayerController->mySocket;
	PlayerController->mySocket->Connect();
	
	DWORD recv_flag = 0;
	ZeroMemory(&g_socket->_recv_over._wsa_over, sizeof(g_socket->_recv_over._wsa_over));
	g_socket->_recv_over._wsa_buf.buf = reinterpret_cast<char*>(g_socket->_recv_over._net_buf + g_socket->_prev_size);
	g_socket->_recv_over._wsa_buf.len = sizeof(g_socket->_recv_over._net_buf) - g_socket->_prev_size;
	WSARecv(g_socket->_socket, &g_socket->_recv_over._wsa_buf, 1, 0, &recv_flag, &g_socket->_recv_over._wsa_over, recv_callback);
	g_socket->Send_LoginPacket();
	SleepEx(0, true);*/
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	myAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	MYCHECK(nullptr != myAnim);

	myAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);

	umb1AnimInst = Cast<UUmbrellaAnimInstance>(umbrella1MeshComponent->GetAnimInstance());
	umb2AnimInst = Cast<UUmbrellaAnimInstance>(umbrella2MeshComponent->GetAnimInstance());
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
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, this, &AMyCharacter::ReleaseAttack);
	PlayerInputComponent->BindAction(TEXT("Farming"), EInputEvent::IE_Pressed, this, &AMyCharacter::StartFarming);
	PlayerInputComponent->BindAction(TEXT("Farming"), EInputEvent::IE_Released, this, &AMyCharacter::EndFarming);

	PlayerInputComponent->BindAction(TEXT("SelectMatch"), EInputEvent::IE_Pressed, this, &AMyCharacter::SelectMatch);
	PlayerInputComponent->BindAction(TEXT("SelectUmbrella"), EInputEvent::IE_Pressed, this, &AMyCharacter::SelectUmbrella);
	PlayerInputComponent->BindAction(TEXT("UseSelectedItem"), EInputEvent::IE_Pressed, this, &AMyCharacter::UseSelectedItem);
	PlayerInputComponent->BindAction(TEXT("UseSelectedItem"), EInputEvent::IE_Released, this, &AMyCharacter::ReleaseRightMouseButton);

	PlayerInputComponent->BindAction(TEXT("ChangeWeapon"), EInputEvent::IE_Pressed, this, &AMyCharacter::ChangeWeapon);
	PlayerInputComponent->BindAction(TEXT("ChangeProjectile"), EInputEvent::IE_Pressed, this, &AMyCharacter::ChangeProjectile);

	PlayerInputComponent->BindAction(TEXT("GetOnOffJetski"), EInputEvent::IE_Pressed, this, &AMyCharacter::GetOnOffJetski);

	// Cheat Key
	PlayerInputComponent->BindAction(TEXT("Cheat_Teleport1"), EInputEvent::IE_Pressed, this, &AMyCharacter::Cheat_Teleport1);
	PlayerInputComponent->BindAction(TEXT("Cheat_Teleport2"), EInputEvent::IE_Pressed, this, &AMyCharacter::Cheat_Teleport2);
	PlayerInputComponent->BindAction(TEXT("Cheat_Teleport3"), EInputEvent::IE_Pressed, this, &AMyCharacter::Cheat_Teleport3);
	PlayerInputComponent->BindAction(TEXT("Cheat_Teleport4"), EInputEvent::IE_Pressed, this, &AMyCharacter::Cheat_Teleport4);

	PlayerInputComponent->BindAction(TEXT("Cheat_IncreaseHP"), EInputEvent::IE_Pressed, this, &AMyCharacter::Cheat_IncreaseHP);
	PlayerInputComponent->BindAction(TEXT("Cheat_DecreaseHP"), EInputEvent::IE_Pressed, this, &AMyCharacter::Cheat_DecreaseHP);

	PlayerInputComponent->BindAction(TEXT("Cheat_IncreaseSnowball"), EInputEvent::IE_Pressed, this, &AMyCharacter::Cheat_IncreaseSnowball);

	PlayerInputComponent->BindAction(TEXT("Cheat_SpawnSupplyBox"), EInputEvent::IE_Pressed, this, &AMyCharacter::Cheat_SpawnSupplyBox);
}

void AMyCharacter::UpDown(float NewAxisValue)
{
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void AMyCharacter::LeftRight(float NewAxisValue)
{
	if (bIsRiding) return;	// ���� �߿��� �¿� �̵� x
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

void AMyCharacter::Jump()
{
	if (bIsRiding) return;	// ���� �߿��� ���� x
	Super::Jump();
}

void AMyCharacter::Attack()
{
	if (isAttacking) return;
	if (bIsSnowman) return;
	if (bIsRiding) return;	// ���� �߿��� ���� x
	// ��� ��� �� ���� x�ϵ���

	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (iSessionId == PlayerController->iSessionId)
	{
		switch (iSelectedProjectile) {
		case Projectile::Snowball:
			switch (iSelectedWeapon) {
			case Weapon::Hand:
				if (iCurrentSnowballCount <= 0) return;	// �����̸� �����ϰ� ���� ������ ���� x
				PlayerController->SendPlayerInfo(COMMAND_SNOWBALL);
				isAttacking = true;
				break;
			case Weapon::Shotgun:
				if (!bHasShotgun) return;	// ������ �����ϰ� ���� ������ ���� x
				if (iCurrentSnowballCount < 5) return;	// �����̰� 5�� �̻� ������ ���� x
				PlayerController->SendPlayerInfo(COMMAND_SHOTGUN);
				MYLOG(Warning, TEXT("gunattack"));
				//ShotgunAttack();
				isAttacking = true;
				break;
			default:
				break;
			}
			break;
		case Projectile::Iceball:
			switch (iSelectedWeapon) {
			case Weapon::Hand:
				if (iCurrentIceballCount <= 0) return;	// ���̽����� �����ϰ� ���� ������ ���� x
				PlayerController->SendPlayerInfo(COMMAND_ICEBALL);
				//IceballAttack();
				isAttacking = true;
				break;
			case Weapon::Shotgun:
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
#ifdef SINGLEPLAY_DEBUG
	SnowBallAttack();
#endif
}

void AMyCharacter::ReleaseAttack()
{	// ���� �� release
	if (bIsRiding) return;
	
	if (iSelectedProjectile == Projectile::Iceball)
	{
		if (iSessionId == localPlayerController->iSessionId)
		{
			SendCancelAttack(BULLET_ICEBALL);
		}
	}
	else
	{
		if (iSessionId == localPlayerController->iSessionId)
		{
			SendCancelAttack(BULLET_SNOWBALL);
		}
	}
}

//�ڱ� �ڽ��� �����Ͽ� �������� ����� ���� �۾��� �����ϰ���
void AMyCharacter::CancelSnowBallAttack()
{
	if (myAnim->bThrowing)
	{
		myAnim->PlayAttack2MontageSectionEnd();
	}
	else
	{	// �����̸� �������ٰ� ���콺 ��ư�� �������ؼ� ��ҵ� ���
		StopAnimMontage();
		if (snowball)
		{
			snowball->Destroy();
			snowball = nullptr;
		}
	}

	if (iSessionId == localPlayerController->iSessionId)
	{

		localPlayerController->SetViewTargetWithBlend(this, fAimingTime);	// ���� ī�޶�� ��ȯ
		localPlayerController->GetHUD()->bShowHUD = true;	// ũ�ν���� ���̵���
	}
}

//�ڱ� �ڽ��� �����Ͽ� �������� ����� ���� �۾��� �����ϰ���
void AMyCharacter::CancelIceBallAttack()
{
	//if (iSelectedProjectile == Projectile::Iceball)
	//{
		if (myAnim->bThrowing)
		{
			myAnim->PlayAttack2MontageSectionEnd();
		}
		else
		{	// �����̸� �������ٰ� ���콺 ��ư�� �������ؼ� ��ҵ� ���
			StopAnimMontage();
			if (iceball)
			{
				iceball->Destroy();
				iceball = nullptr;
			}
		}

		if (iSessionId == localPlayerController->iSessionId)
		{

			localPlayerController->SetViewTargetWithBlend(this, fAimingTime);	// ���� ī�޶�� ��ȯ
			localPlayerController->GetHUD()->bShowHUD = true;	// ũ�ν���� ���̵���
		}
	//}
}

void AMyCharacter::SnowBallAttack()
{
	//if (bIsSnowman) return;
	//if (iCurrentSnowballCount <= 0) return;	// �����̸� �����ϰ� ���� ������ ���� x

	//myAnim->PlayAttackMontage();
	myAnim->PlayAttack2Montage();

	// Attempt to fire a projectile.
	if (projectileClass)
	{
		UWorld* World = GetWorld();

		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			FTransform SnowballSocketTransform = GetMesh()->GetSocketTransform(TEXT("SnowballSocket"));
			snowball = World->SpawnActor<AMySnowball>(projectileClass, SnowballSocketTransform, SpawnParams);
			FAttachmentTransformRules atr = FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
			snowball->AttachToComponent(GetMesh(), atr, TEXT("SnowballSocket"));
			snowball->SetOwnerSessionId(iSessionId);	// �����̿� �ڽ��� ������ ĳ������ session id ����
		}
	}

	if (iSessionId == localPlayerController->iSessionId)
	{
		localPlayerController->SetViewTargetWithBlend(aimingCameraPos, fAimingTime);	// ���� �� ī�޶� ��ġ�� ��ȯ
		localPlayerController->GetHUD()->bShowHUD = false;	// ũ�ν���� �Ⱥ��̵���
		bIsAiming = true;
	}
}

void AMyCharacter::ShotgunAttack()
{
	MYLOG(Warning, TEXT("AttackShotGun"));

	myAnim->PlayAttackShotgunMontage();

	// ������ - ������ �ּ� ����
	//iCurrentSnowballCount -= 4;	// ���� �� ������ ������ 4 ����
	UpdateUI(UICategory::CurSnowball);

}

void AMyCharacter::IceballAttack()
{
	//if (bIsSnowman) return;
	//if (iCurrentIceballCount <= 0) return;	// ���̽����� �����ϰ� ���� ������ ���� x

	//myAnim->PlayAttackMontage();
	myAnim->PlayAttack2Montage();

	// Attempt to fire a projectile.
	if (iceballClass)
	{
		UWorld* World = GetWorld();

		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			FTransform SnowballSocketTransform = GetMesh()->GetSocketTransform(TEXT("SnowballSocket"));
			iceball = World->SpawnActor<AIceball>(iceballClass, SnowballSocketTransform, SpawnParams);
			FAttachmentTransformRules atr = FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
			iceball->AttachToComponent(GetMesh(), atr, TEXT("SnowballSocket"));
		}
	}

	if (iSessionId == localPlayerController->iSessionId)
	{
		localPlayerController->SetViewTargetWithBlend(aimingCameraPos, fAimingTime);	// ���� �� ī�޶� ��ġ�� ��ȯ
		localPlayerController->GetHUD()->bShowHUD = false;	// ũ�ν���� �Ⱥ��̵���
		bIsAiming = true;
	}
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	isAttacking = false;
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (!bIsSnowman)
	{	// ������ ��� ü�� ����
#ifdef SINGLEPLAY_DEBUG
		iCurrentHP = FMath::Clamp<int>(iCurrentHP - FinalDamage, iMinHP, iMaxHP);
		UpdateUI(UICategory::HP);	// ����� ü������ ui ����

		MYLOG(Warning, TEXT("Actor : %s took Damage : %f, HP : %d"), *GetName(), FinalDamage, iCurrentHP);
#endif
		if (iSessionId == localPlayerController->iSessionId)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TakeDamage")));
			localPlayerController->SendPlayerInfo(COMMAND_DAMAGE);
		}
	}
	else
	{	// ������� ��� ����
		StartStun(fStunTime);
#ifdef SINGLEPLAY_DEBUG
		MYLOG(Warning, TEXT("Actor : %s stunned, HP : %d"), *GetName(), iCurrentHP);
#endif
	}
	return FinalDamage;
}

//�߻�
void AMyCharacter::SendReleaseBullet(int bullet)
{
	if (iSessionId != localPlayerController->iSessionId) return;

	switch (bullet)
	{
	case BULLET_SNOWBALL: {
		if (IsValid(snowball))
		{
			localPlayerController->SendPlayerInfo(COMMAND_THROW_SB);
		}
		break;
	}
	case BULLET_ICEBALL: {
		if (IsValid(iceball))
		{
			localPlayerController->SendPlayerInfo(COMMAND_THROW_IB);
		}
		break;
	}
	default:
		break;
	}
}

//���� ���
void AMyCharacter::SendCancelAttack(int bullet)
{
	if (iSessionId != localPlayerController->iSessionId) return;
	switch (bullet)
	{
	case BULLET_SNOWBALL: {
		if (IsValid(snowball))
		{
			localPlayerController->SendPlayerInfo(COMMAND_CANCEL_SB);

		}
		break;
	}
	case BULLET_ICEBALL: {
		if (IsValid(iceball))
		{
			localPlayerController->SendPlayerInfo(COMMAND_CANCEL_IB);

		}
		break;
	}
	default:
		break;
	}

}


void AMyCharacter::SendSpawnSnowballBomb()
{
	if (iSessionId != localPlayerController->iSessionId) return;
	if (shotgunProjectileClass)
	{
		MYLOG(Warning, TEXT("SendSpawnSnowballBomb"));
		localPlayerController->SendPlayerInfo(COMMAND_GUNFIRE);

	}
}


void AMyCharacter::ReleaseSnowball()
{
	if (IsValid(snowball))
	{
		FDetachmentTransformRules dtr = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
		snowball->DetachFromActor(dtr);

		if (snowball->GetClass()->ImplementsInterface(UI_Throwable::StaticClass()))
		{
			//���� ���� rotatiom ���� speed ��
#ifdef MULTIPLAY_DEBUG
			AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
			FRotator cameraRotation;
			cameraRotation.Yaw = PlayerController->GetCharactersInfo()->players[iSessionId].fCYaw;
			cameraRotation.Pitch = PlayerController->GetCharactersInfo()->players[iSessionId].fCPitch;
			cameraRotation.Roll = PlayerController->GetCharactersInfo()->players[iSessionId].fCRoll;

			float speed = PlayerController->GetCharactersInfo()->players[iSessionId].fSpeed;

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("[release snowball] \n [x] : %f \n [y] : %f \n [z] : %f \n [speed] : %f "), cameraRotation.Vector().X, cameraRotation.Vector().Y, cameraRotation.Vector().Z, speed));

			FVector SnowBallLocation;
			SnowBallLocation.X = PlayerController->GetCharactersInfo()->players[iSessionId].fSBallX;
			SnowBallLocation.Y = PlayerController->GetCharactersInfo()->players[iSessionId].fSBallY;
			SnowBallLocation.Z = PlayerController->GetCharactersInfo()->players[iSessionId].fSBallZ;
			snowball->SetActorLocation(SnowBallLocation);
			II_Throwable::Execute_Throw(snowball, cameraRotation.Vector(), speed);
			// �ӵ� ���� �߰�
#endif
#ifdef SINGLEPLAY_DEBUG
			FVector cameraLocation;
			FRotator cameraRotation;
			GetActorEyesViewPoint(cameraLocation, cameraRotation);
			AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());

			II_Throwable::Execute_Throw(snowball, cameraRotation.Vector());
#endif
			snowball = nullptr;

			bIsAiming = false;
			fAimingElapsedTime = 0.0f;
		}

	}
}

void AMyCharacter::ReleaseIceball()
{
	if (IsValid(iceball))
	{
		//iCurrentIceballCount -= 1;	// ���� �� ���̽��� ������ 1 ����
		//UpdateUI(UICategory::CurIceball);

		FDetachmentTransformRules dtr = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
		iceball->DetachFromActor(dtr);

		if (iceball->GetClass()->ImplementsInterface(UI_Throwable::StaticClass()))
		{

#ifdef MULTIPLAY_DEBUG
			//���� ���� rotatiom ���� speed ��
			AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
			FRotator cameraRotation;
			cameraRotation.Yaw = PlayerController->GetCharactersInfo()->players[iSessionId].fCYaw;
			cameraRotation.Pitch = PlayerController->GetCharactersInfo()->players[iSessionId].fCPitch;
			cameraRotation.Roll = PlayerController->GetCharactersInfo()->players[iSessionId].fCRoll;

			float speed = PlayerController->GetCharactersInfo()->players[iSessionId].fSpeed;

			FVector IceBallLocation;
			IceBallLocation.X = PlayerController->GetCharactersInfo()->players[iSessionId].fIBallX;
			IceBallLocation.Y = PlayerController->GetCharactersInfo()->players[iSessionId].fIBallY;
			IceBallLocation.Z = PlayerController->GetCharactersInfo()->players[iSessionId].fIBallZ;
			iceball->SetActorLocation(IceBallLocation);


			II_Throwable::Execute_IceballThrow(iceball, cameraRotation, speed);

			iceball = nullptr;

			bIsAiming = false;
			fAimingElapsedTime = 0.0f;

#endif
#ifdef SINGLEPLAY_DEBUG
			FVector cameraLocation;
			FRotator cameraRotation;
			GetActorEyesViewPoint(cameraLocation, cameraRotation);
			AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());

			float speed = fSnowballInitialSpeed + fAimingElapsedTime * fThrowPower;

			II_Throwable::Execute_IceballThrow(iceball, cameraRotation, speed);

			iceball = nullptr;

			bIsAiming = false;
			fAimingElapsedTime = 0.0f;
#endif
		}
	}
}

void AMyCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
#ifdef MULTIPLAY_DEBUG
	if (!iSessionId == PlayerController->iSessionId || !PlayerController->IsStart()) return;
#endif

	auto MySnowball = Cast<AMySnowball>(OtherActor);

	if (nullptr != MySnowball)
	{
	}
	AMyCharacter* otherCharacter = Cast<AMyCharacter>(OtherActor);
	if (!otherCharacter) return;

	// �ڽ� - �����, ���ϻ��� x
	// ��� - ����
	if (bIsSnowman && iCharacterState != CharacterState::SnowmanStunned)
	{
		if (!(otherCharacter->GetIsSnowman()))
		{	// ���� ����ȭ(��Ȱ), ��� ĳ���� �����ȭ(���)
#ifdef MULTIPLAY_DEBUG
			//bIsSnowman = false;
			//PlayerController->SetCharacterState(iSessionId, ST_ANIMAL);
			//PlayerController->SetCharacterHP(iSessionId, iMaxHP);
			//ChangeAnimal();
			PlayerController->GetSocket()->Send_StatusPacket(ST_ANIMAL, iSessionId);
			//UpdateTemperatureState();
			PlayerController->GetSocket()->Send_StatusPacket(ST_SNOWMAN, otherCharacter->iSessionId);
			
			//PlayerController->SetCharacterState(iSessionId, ST_ANIMAL);
			//otherCharacter->ChangeSnowman();
			//PlayerController->SetCharacterState(otherCharacter->iSessionId, ST_SNOWMAN);

#endif
#ifdef SINGLEPLAY_DEBUG
			ChangeAnimal();
			otherCharacter->ChangeSnowman();
#endif
			UE_LOG(LogTemp, Warning, TEXT("%s catch %s"), *GetName(), *(otherCharacter->GetName()));
			return;
		}
	}
	/*else if (!bIsSnowman && otherCharacter->GetIsSnowman())
	{
		bIsSnowman = true;
		PlayerController->SetCharacterState(iSessionId, ST_SNOWMAN);
		ChangeSnowman();
		if (iSessionId == PlayerController->iSessionId && PlayerController->IsStart())
			PlayerController->GetSocket()->Send_StatusPacket(ST_SNOWMAN);
		
	}*/

	//auto MyCharacter = Cast<AMyCharacter>(OtherActor);

	//if (nullptr != MyCharacter)
	//{
	//	FDamageEvent DamageEvent;
	//	MyCharacter->TakeDamage(iDamage, DamageEvent, false, this);
	//}
}

void AMyCharacter::StartFarming()
{
	if (!IsValid(farmingItem)) return;	//�������ϸ� �ٷ� �־���
	if (bIsSnowman) return;
	if (bIsRiding) return; // ���� �� ������ �Ĺ� x
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (iSessionId != PlayerController->iSessionId || !PlayerController->IsStart()) return;
	
	if (Cast<ASnowdrift>(farmingItem))
	{
		if (iCurrentSnowballCount >= iMaxSnowballCount) return;	// ������ �ִ뺸���� �̻��� �� ������ �Ĺ� ���ϵ���
		bIsFarming = true;
		UpdateUI(UICategory::IsFarmingSnowdrift);
	}
	else if (Cast<AIcedrift>(farmingItem))
	{
		if (iCurrentIceballCount >= iMaxIceballCount) return;	// ���̽��� �ִ뺸���� �̻��� �� ������ �Ĺ� ���ϵ���
		bIsFarming = true;
		UpdateUI(UICategory::IsFarmingSnowdrift);
	}
	else if (Cast<AItembox>(farmingItem))
	{
		AItembox* itembox = Cast<AItembox>(farmingItem);
		switch (itembox->GetItemboxState())
		{
		case ItemboxState::Closed:
			itembox->SetItemboxState(ItemboxState::Opening);
			PlayerController->GetSocket()->Send_OpenBoxPacket(itembox->GetId());
			break;
		case ItemboxState::Opened:
			// �����۹ڽ����� ���빰 �Ĺֿ� �����ϸ� �����۹ڽ����� ������ ���� (�ڽ��� �״�� ������Ŵ)
			if (GetItem(itembox->GetItemType())) { 
				MYLOG(Warning, TEXT("item %d"), itembox->GetItemType());

				MYLOG(Warning, TEXT("item %d"), itembox->GetItemType());
				//������ �Ĺ� �� ���� ����
#ifdef MULTIPLAY_DEBUG
				PlayerController->GetSocket()->Send_ItemPacket(itembox->GetItemType(), itembox->GetId());
#endif

				//itembox->DeleteItem(); //�������� ��Ŷ�޾��� �� ó��
				//�ڽ��� ������ �������� ������ ����ȭ
			}
			break;
		// �����۹ڽ��� ������ ���̰ų� ����ִ� ���
		case ItemboxState::Opening:
			break;
		case ItemboxState::Empty:
			break;
		default:
			break;
		}
	}
	else if (Cast<ASupplyBox>(farmingItem))
	{
		ASupplyBox* spbox = Cast<ASupplyBox>(farmingItem);
		PlayerController->GetSocket()->Send_ItemPacket(ITEM_SPBOX, spbox->iSpBoxId);
		//GetSupplyBox();
	}
}

bool AMyCharacter::GetItem(int itemType)
{
	
	switch (itemType) {
	case ItemTypeList::Match:
		if (iCurrentMatchCount >= iMaxMatchCount) return false;	// ���� �ִ뺸������ �Ѿ �Ĺ����� ���ϵ���
		iCurrentMatchCount += 1;
		UpdateUI(UICategory::CurMatch);
		return true;
		break;
	case ItemTypeList::Umbrella:
		if (bHasUmbrella) return false;	// ����� ���� ���̸� ��� �Ĺ� ���ϵ���
		bHasUmbrella = true;
		UpdateUI(UICategory::HasUmbrella);
		return true;
		break;
	case ItemTypeList::Bag:
		if (bHasBag) return false;	// ������ ���� ���̸� ���� �Ĺ� ���ϵ���
		GetBag();
		return true;
		break;
	default:
		return false;
		break;
	}
}

void AMyCharacter::EndFarming()
{
	if (bIsSnowman) return;

	if (IsValid(farmingItem))
	{
		if (Cast<ASnowdrift>(farmingItem))
		{

			if (iCurrentSnowballCount >= iMaxSnowballCount) return;

			// FŰ�� �� ������ �Ĺ� �� FŰ release �� �� ������ duration �ʱ�ȭ
			ASnowdrift* snowdrift = Cast<ASnowdrift>(farmingItem);
			snowdrift->SetFarmDuration(ASnowdrift::fFarmDurationMax);
			bIsFarming = false;
			UpdateUI(UICategory::IsFarmingSnowdrift);
		}
		else if (Cast<AIcedrift>(farmingItem))
		{

			if (iCurrentIceballCount >= iMaxIceballCount) return;

			// FŰ�� ���� ������ �Ĺ� �� FŰ release �� ���� ������ duration �ʱ�ȭ
			AIcedrift* icedrift = Cast<AIcedrift>(farmingItem);
			icedrift->SetFarmDuration(AIcedrift::fFarmDurationMax);
			bIsFarming = false;
			UpdateUI(UICategory::IsFarmingSnowdrift);
		}
	}
}

void AMyCharacter::UpdateFarming(float deltaTime)
{
	if (!bIsFarming) return;
	if (!IsValid(farmingItem)) return;
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (iSessionId != PlayerController->iSessionId || !PlayerController->IsStart()) return;

	// �� ������ �Ĺ�
	ASnowdrift* snowdrift = Cast<ASnowdrift>(farmingItem);
	if (snowdrift)
	{	// �� ������ duration ��ŭ FŰ�� ������ ������ ĳ������ ������ �߰� 
		float lastFarmDuration = snowdrift->GetFarmDuration();
		float newFarmDuration = lastFarmDuration - deltaTime;
		snowdrift->SetFarmDuration(newFarmDuration);
		UpdateUI(UICategory::SnowdriftFarmDuration, newFarmDuration);

		if (newFarmDuration <= 0)
		{
			//������ �Ĺ� �� ���� ����
#ifdef MULTIPLAY_DEBUG
			PlayerController->GetSocket()->Send_ItemPacket(ITEM_SNOW, snowdrift->GetId());
#endif
			UpdateUI(UICategory::CurSnowball);
			bIsFarming = false;	// �������� �Ĺ� ������ false�� ���� �� UI ���� (�������� �Ĺ� �� ui �Ⱥ��̵���)
			UpdateUI(UICategory::IsFarmingSnowdrift);
			//snowdrift->Destroy(); //�������� ���̵� ��ȯ�� ó��
			//snowdrift = nullptr;
		}
	}

	// ���� ������ �Ĺ�
	AIcedrift* icedrift = Cast<AIcedrift>(farmingItem);
	if (icedrift)
	{	// ���� ������ duration ��ŭ FŰ�� ������ ������ ĳ������ ���̽��� �߰� 
		float lastFarmDuration = icedrift->GetFarmDuration();
		float newFarmDuration = lastFarmDuration - deltaTime;
		icedrift->SetFarmDuration(newFarmDuration);
		UpdateUI(UICategory::SnowdriftFarmDuration, newFarmDuration);

		if (newFarmDuration <= 0)
		{
			//���� �Ĺ� �� ���� ����
#ifdef MULTIPLAY_DEBUG
			PlayerController->GetSocket()->Send_ItemPacket(ITEM_ICE, icedrift->GetId());
#endif
			//iCurrentIceballCount = FMath::Clamp<int>(iCurrentIceballCount + icedrift->iNumOfIceball, 0, iMaxIceballCount);
			UpdateUI(UICategory::CurIceball);
			bIsFarming = false;	// ���������� �Ĺ� ������ false�� ���� �� UI ���� (�������� �Ĺ� �� ui �Ⱥ��̵���)
			UpdateUI(UICategory::IsFarmingSnowdrift);
			//icedrift->Destroy();
			//icedrift = nullptr;
		}
	}
}

void AMyCharacter::UpdateHP()
{
	if (iCurrentHP <= iMinHP)
	{
		ChangeSnowman();
	}
}

void AMyCharacter::UpdateSpeed()
{
	if (bIsRiding) return;	// jetski ž�� �߿��� ü�¿����� speed x

	switch (iCharacterState) {
	case CharacterState::AnimalNormal:
		if (iCurrentHP <= iBeginSlowHP)
		{
			iCharacterState = CharacterState::AnimalSlow;
			GetCharacterMovement()->MaxWalkSpeed = iSlowSpeed;
		}
		break;
	case CharacterState::AnimalSlow:
		if (iCurrentHP > iBeginSlowHP)
		{
			iCharacterState = CharacterState::AnimalNormal;
			GetCharacterMovement()->MaxWalkSpeed = iNormalSpeed;
		}
		break;
	default:
		break;
	}
}

void AMyCharacter::ChangeSnowman()
{
	bIsSnowman = true;

	InitializeFreeze();

	// ���̷�Ż�޽�, �ִϸ��̼� �������Ʈ ����
	myAnim->SetDead();
	GetMesh()->SetSkeletalMesh(snowman);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetAnimInstanceClass(snowmanAnim);
	SetCharacterMaterial();	// ��������� ��Ƽ���� ������ ���� ���� �ʿ� x (����Ʈ������)

	iCurrentHP = iMinHP;
	GetWorldTimerManager().ClearTimer(temperatureHandle);	// ������ �������̴� ü�� ���� �ڵ鷯 �ʱ�ȭ (ü�� ��ȭ���� �ʵ���)
	ResetHasItems();
	UpdateUI(UICategory::AllOfUI);
	

#ifdef SINGLEPLAY_DEBUG
	UpdateTemperatureState();
	UpdateUI(UICategory::HP);	// ����� ü������ ui ����
#endif

	GetCharacterMovement()->MaxWalkSpeed = iSlowSpeed;	// ������� �̵��ӵ��� ���ο� ������ ĳ���Ϳ� �����ϰ� ����
	
	StartStun(fChangeSnowmanStunTime);

	isAttacking = false;	// ���� ���߿� ���� ���� �� �߻��ϴ� ���� ����

	CloseUmbrellaAnim();
	HideUmbrella();

	//���� ����Ʈ
	if (changeNiagara) {
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), changeNiagara, GetActorLocation(), FRotator(1), FVector(1), true, true, ENCPoolMethod::AutoRelease, true);
	}
}

void AMyCharacter::WaitForStartGame()
{
	//Delay �Լ�
	FTimerHandle WaitHandle;
	float WaitTime = 3.0f;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			UpdateTemperatureState();
		}), WaitTime, false);
}

void AMyCharacter::UpdateTemperatureState()
{
	if (bIsSnowman) return;
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (iSessionId != PlayerController->iSessionId || !PlayerController->IsStart()) return;
#ifdef SINGLEPLAY_DEBUG
	GetWorldTimerManager().ClearTimer(temperatureHandle);	// ������ �������̴� �ڵ鷯 �ʱ�ȭ
#endif
	//if (match)
	//{
	//	GetWorldTimerManager().SetTimer(temperatureHandle, this, &AMyCharacter::UpdateTemperatureByMatch, 1.0f, true);
	//}
	//else
	//{
		if (bIsInsideOfBonfire)
		{	// ��ں� ������ ��� �ʴ� ü�� ���� (�ʴ� ȣ��Ǵ� �����Լ�)
#ifdef SINGLEPLAY_DEBUG
			GetWorldTimerManager().SetTimer(temperatureHandle, FTimerDelegate::CreateLambda([&]()
				{
					iCurrentHP += ABonfire::iHealAmount;
					iCurrentHP = FMath::Clamp<int>(iCurrentHP, iMinHP, iMaxHP);
					UpdateUI(UICategory::HP);	// ����� ü������ ui ����
				}), 1.0f, true);
#endif
			if (iSessionId == PlayerController->iSessionId && PlayerController->IsStart()) {
				PlayerController->GetSocket()->Send_StatusPacket(ST_INBURN, iSessionId);
			}
		}
		else
		{	// ��ں� �ܺ��� ��� �ʴ� ü�� ���� (�ʴ� ȣ��Ǵ� �����Լ�)
#ifdef SINGLEPLAY_DEBUG
			GetWorldTimerManager().SetTimer(temperatureHandle, FTimerDelegate::CreateLambda([&]()
				{
					iCurrentHP -= ABonfire::iDamageAmount;
					iCurrentHP = FMath::Clamp<int>(iCurrentHP, iMinHP, iMaxHP);
					UpdateUI(UICategory::HP);	// ����� ü������ ui ����
				}), 1.0f, true);
#endif
			if (iSessionId == PlayerController->iSessionId && PlayerController->IsStart())
				PlayerController->GetSocket()->Send_StatusPacket(ST_OUTBURN, iSessionId);
		}
	//}
}

void AMyCharacter::StartStun(float waitTime)
{
	//if (!playerController) return;	// �ٸ� �÷��̾��� ĳ���ʹ� �÷��̾� ��Ʈ�ѷ��� ���� x?

	if (iCharacterState == CharacterState::SnowmanStunned)
	{	// ���� ���¿��� �� ������ ������ �������̴� stunhandle �ʱ�ȭ (�ڵ鷯 ���� �����ϵ���)
		GetWorldTimerManager().ClearTimer(stunHandle);
	}

	// �÷��̾��� �Է��� �����ϵ��� (������ �� ����, �þߵ� ����, ���� - Stunned)
	iCharacterState = bIsSnowman ? CharacterState::SnowmanStunned : CharacterState::AnimalStunned;
	DisableInput(playerController);
	GetMesh()->bPauseAnims = true;	// ĳ���� �ִϸ��̼ǵ� ����

	EndStun(waitTime);	// waitTime�� ������ stun�� ��������

	if (bIsInTornado)
	{	// ����̵��� ���� �����̸� ĳ���Ͱ� ȸ���ϵ���, �ִϸ��̼��� ����ǵ���
		rotateCont = true;
		GetMesh()->bPauseAnims = false;
	}

	//���� ����Ʈ
	if (stunNiagara) {
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), stunNiagara, GetActorLocation() + FVector(0.0f, -40.0f, 90.0f), FRotator(1), FVector(1), true, true, ENCPoolMethod::AutoRelease, true);
	}
}

void AMyCharacter::EndStun(float waitTime)
{
	//if (!playerController) return;

	// x�� �Ŀ� �ٽ� �Է��� ���� �� �ֵ��� (�����Ӱ� �þ� ���� ����, ���� - Snowman)
	GetWorld()->GetTimerManager().SetTimer(stunHandle, FTimerDelegate::CreateLambda([&]()
		{
			iCharacterState = bIsSnowman ? CharacterState::SnowmanNormal : CharacterState::AnimalNormal;
			EnableInput(playerController);
			GetMesh()->bPauseAnims = false;

			rotateCont = false;		// ����̵��� ���ؼ� ȸ�� ���̸� ���߱�
		}), waitTime, false);
}

void AMyCharacter::ResetHasItems()
{
	iCurrentSnowballCount = 0;
	iMaxSnowballCount = iOriginMaxSnowballCount;
	iCurrentIceballCount = 0;
	iMaxIceballCount = iOriginMaxIceballCount;
	iCurrentMatchCount = 0;
	iMaxMatchCount = iOriginMaxMatchCount;
	bHasUmbrella = false;
	bHasBag = false;
	bHasShotgun = false;
	bagMeshComponent->SetVisibility(false);


	UpdateUI(UICategory::AllOfUI);
}

void AMyCharacter::ChangeAnimal()
{
	bIsSnowman = false;

	// ���̷�Ż�޽�, �ִϸ��̼� �������Ʈ ����
	GetMesh()->SetSkeletalMesh(bear);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetAnimInstanceClass(bearAnim);
	SetCharacterMaterial(iColor);

	myAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	MYCHECK(nullptr != myAnim);
	myAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
	
	iCurrentHP = iBeginSlowHP;	// ����� -> ���� ��Ȱ �� ü�� ���� ����(���ο�)�� ���� (30.0 - ü���� 1/4)
	GetWorldTimerManager().ClearTimer(temperatureHandle);
	ResetHasItems();
	UpdateUI(UICategory::AllOfUI);
	UpdateTemperatureState();
	
#ifdef SINGLEPLAY_DEBUG
	UpdateTemperatureState();
	UpdateUI(UICategory::HP);	// ����� ü������ ui ����
#endif

	GetCharacterMovement()->MaxWalkSpeed = iNormalSpeed;

	iCharacterState = CharacterState::AnimalNormal;

	isAttacking = false;	// ���� ���߿� ���� ���� �� �߻��ϴ� ���� ����

	//q ����Ʈ
	if (changeNiagara) {
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), changeNiagara, GetActorLocation(), FRotator(1), FVector(1), true, true, ENCPoolMethod::AutoRelease, true);
	}
}

void AMyCharacter::SetCharacterMaterial(int id)
{
	if (id < 0) id = 0;	// id�� ��ȿ���� ���� ��� (�̱��÷���)
	if (!bIsSnowman)
	{	// �� ��Ƽ����� ����, ���� ������ �� �ؽ��� ����
		GetMesh()->SetMaterial(0, bearMaterial);
		dynamicMaterialInstance = GetMesh()->CreateDynamicMaterialInstance(0);
		dynamicMaterialInstance->SetTextureParameterValue(FName("Tex"), bearTextureArray[id]);	// ���� ������ �� �ؽ��� ���
		iId = id;
	}
	else
	{	// ����� ��Ƽ����� ����
		GetMesh()->SetMaterial(0, snowmanMaterial);
	}
}

void AMyCharacter::UpdateUI(int uiCategory, float farmDuration)
{
#ifdef MULTIPLAY_DEBUG
	if (iSessionId != localPlayerController->iSessionId) return;	// �����÷��̾��� ��츸 update
#endif
	switch (uiCategory)
	{
	case UICategory::HP:
		localPlayerController->CallDelegateUpdateHP();	// ü�� ui ����
		break;
	case UICategory::CurSnowball:
		localPlayerController->CallDelegateUpdateCurrentSnowballCount();	// CurSnowball ui ����
		break;
	case UICategory::CurIceball:
		localPlayerController->CallDelegateUpdateCurrentIceballCount();		// CurIceball ui ����
		break;
	case UICategory::CurMatch:
		localPlayerController->CallDelegateUpdateCurrentMatchCount();	// CurMatch ui ����
		break;
	case UICategory::MaxSnowIceballAndMatch:
		localPlayerController->CallDelegateUpdateMaxSnowIceballAndMatchCount();	// MaxSnowIceballAndMatch ui ����
		break;
	case UICategory::HasUmbrella:
		localPlayerController->CallDelegateUpdateHasUmbrella();	// HasUmbrella ui ����
		break;
	case UICategory::HasBag:
		localPlayerController->CallDelegateUpdateHasBag();	// HasBag ui ����
		break;
	case UICategory::HasShotgun:
		localPlayerController->CallDelegateUpdateHasShotgun();	// HasShotgun ui ����
		break;
	case UICategory::IsFarmingSnowdrift:
		localPlayerController->CallDelegateUpdateIsFarmingSnowdrift();	// snowdrift farming ui visible ����
		break;
	case UICategory::SnowdriftFarmDuration:
		localPlayerController->CallDelegateUpdateSnowdriftFarmDuration(farmDuration);	// snowdrift farm duration ui ����
		break;
	case UICategory::SelectedItem:
		localPlayerController->CallDelegateUpdateSelectedItem();
		break;
	case UICategory::SelectedProjectile:
		localPlayerController->CallDelegateUpdateSelectedProjectile();
		break;
	case UICategory::SelectedWeapon:
		localPlayerController->CallDelegateUpdateSelectedWeapon();
		break;
	case UICategory::AllOfUI:
		localPlayerController->CallDelegateUpdateAllOfUI();	// ��� ĳ���� ui ����
		break;
	default:
		break;
	}
}

void AMyCharacter::SelectMatch()
{
	iSelectedItem = ItemTypeList::Match;
	UpdateUI(UICategory::SelectedItem);
}

void AMyCharacter::SelectUmbrella()
{
	iSelectedItem = ItemTypeList::Umbrella;
	UpdateUI(UICategory::SelectedItem);
}

void AMyCharacter::UseSelectedItem()
{
	if (bIsSnowman) return;		// ������� ������ ��� x
	if (isAttacking) return;	// ���� �� ������ ��� x
	if (bIsRiding) return;		// ���� �߿��� ������ ��� x

	switch (iSelectedItem) {
	case ItemTypeList::Match: {	// ���� ������ ��� ��
		if (iCurrentMatchCount <= 0) return;	// ������ ������ ���� ��� ����
		// ü�� += 20 (ü�����δ� 2.0��)
		if (iSessionId == localPlayerController->iSessionId)
		{
			localPlayerController->SendPlayerInfo(COMMAND_MATCH);
		}
		iCurrentMatchCount -= 1;
		UpdateUI(UICategory::CurMatch);
		break;
	}
	case ItemTypeList::Umbrella:	// ��� ������ ��� ��
		if(!bHasUmbrella) break;   // ��� �������� ���� ��� ����
		if (iSessionId == localPlayerController->iSessionId)
		{
			MYLOG(Warning, TEXT("select_umb"));
			localPlayerController->SendPlayerInfo(COMMAND_UMB_START);
			isAttacking = true;
		}
		
		// ������ - �����δ� �ּ� ����
		//StartUmbrella();
		break;
	default:
		break;
	}
}

void AMyCharacter::UpdateZByTornado()
{	// ĳ���Ͱ� ����̵� ������ ��� z�� ����
	if (bIsInTornado)
	{
		if (!(iSessionId == localPlayerController->iSessionId)) return;

		LaunchCharacter(FVector(0.0f, 0.0f, 20.0f), true, false);

		// ����̵��� �۾��� ĳ������ x, y�� ����̵��� x, y������ �����ؼ� ���� �����̵���
		if (overlappedTornado != nullptr)
		{
			SetActorLocation(FVector(overlappedTornado->GetActorLocation().X, overlappedTornado->GetActorLocation().Y, GetActorLocation().Z));
		}
	}
}

void AMyCharacter::UpdateControllerRotateByTornado()
{
	if (rotateCont)
	{
		if (!(iSessionId == localPlayerController->iSessionId)) return;

		FRotator contRot = localPlayerController->GetControlRotation();
		FRotator newContRot = FRotator(contRot.Pitch, contRot.Yaw + 5.0f, contRot.Roll);
		localPlayerController->SetControlRotation(newContRot);
	}
}



void AMyCharacter::ChangeWeapon()
{
	iSelectedWeapon = (iSelectedWeapon + 1) % iNumOfWeapons;
	UpdateUI(UICategory::SelectedWeapon);
}

void AMyCharacter::ChangeProjectile()
{
	iSelectedProjectile = (iSelectedProjectile + 1) % iNumOfProjectiles;
	UpdateUI(UICategory::SelectedProjectile);
}

void AMyCharacter::ShowShotgun()
{
	shotgunMeshComponent->SetVisibility(true);
}

void AMyCharacter::HideShotgun()
{
	shotgunMeshComponent->SetVisibility(false);
}

void AMyCharacter::SpawnSnowballBomb()
{
	if (shotgunProjectileClass)
	{
		UWorld* World = GetWorld();

		if (World)
		{
			FVector cameraLocation;
			FRotator cameraRotation;

			GetActorEyesViewPoint(cameraLocation, cameraRotation);
			
		

			FVector rightVec1 = GetActorRightVector() / 15;
			FVector rightVec2 = GetActorRightVector() / 25;
			FVector rightVec3 = GetActorRightVector() / 35;

			float up1 = 0.055f;
			float up2 = 0.035f;
			float up3 = -0.025f;
			float up4 = -0.05f;
			
			AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
			
			cameraRotation.Pitch = PlayerController->GetCharactersInfo()->players[iSessionId].Pitch;

			snowballBombDirArray[0] = cameraRotation.Vector();
			snowballBombDirArray[1] = FVector(cameraRotation.Vector() + FVector(0.0f, 0.0f, up1));
			snowballBombDirArray[2] = FVector(cameraRotation.Vector() + FVector(0.0f, 0.0f, up2) + rightVec2);
			snowballBombDirArray[3] = FVector(cameraRotation.Vector() + FVector(0.0f, 0.0f, up2) - rightVec2);
			snowballBombDirArray[4] = FVector(cameraRotation.Vector() + FVector(0.0f, 0.0f, up3) + rightVec1);
			snowballBombDirArray[5] = FVector(cameraRotation.Vector() + FVector(0.0f, 0.0f, up3) - rightVec1);
			snowballBombDirArray[6] = FVector(cameraRotation.Vector() + FVector(0.0f, 0.0f, up4) + rightVec3);
			snowballBombDirArray[7] = FVector(cameraRotation.Vector() + FVector(0.0f, 0.0f, up4) - rightVec3);

			MYLOG(Warning, TEXT("pitch %f"), cameraRotation.Pitch);


			//TArray<int> randInt;
			//// 0~7 �߿��� �ߺ����� 5���� ���� ����
			//while (randInt.Num() < 5)
			//{
			//	int random = UKismetMathLibrary::RandomIntegerInRange(0, 7);
			//	if (randInt.Find(random) == -1)
			//	{
			//		randInt.Add(random);
			//	}
			//}

			// ������ 5���� ��ġ�� snowball bomb ���� �� ������
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			FTransform muzzleSocketTransform = shotgunMeshComponent->GetSocketTransform(TEXT("Muzzle1Socket"));
			FTransform smokeSocketTransform = shotgunMeshComponent->GetSocketTransform(TEXT("SmokeSocket"));
			
			for (int i = 0; i < 5; ++i)
			{
				ASnowballBomb* snowballBomb = GetWorld()->SpawnActor<ASnowballBomb>(shotgunProjectileClass, muzzleSocketTransform, SpawnParams);
				snowballBomb->SetOwnerSessionId(iSessionId);	// ������ ��ź�� �ڽ��� ������ ĳ������ session id ����

				II_Throwable::Execute_Throw(snowballBomb, snowballBombDirArray[PlayerController->GetCharactersInfo()->players[iSessionId].iRandBulletArr[i]], 0.0f);
			}

			if (smokeNiagara) {
				UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), smokeNiagara, smokeSocketTransform.GetLocation(), FRotator(1), FVector(1), true, true, ENCPoolMethod::AutoRelease, true);
				//NiagaraComp->SetNiagaraVariableFloat(FString("StrengthCoef"), CoefStrength);
			}

			// ������ - muzzle socket 8������ ��� �߻�
			//for (int i = 0; i < 8; ++i)
			//{
			//	ASnowballBomb* snowballBomb = GetWorld()->SpawnActor<ASnowballBomb>(shotgunProjectileClass, muzzleSocketTransform, SpawnParams);

			//	II_Throwable::Execute_Throw(snowballBomb, snowballBombDirArray[i]);
			//}
		}
	}
}

void AMyCharacter::Cheat_Teleport1()
{
	//��ں�
	if (iSessionId != localPlayerController->iSessionId) return;
	FVector CharacterLocation;
	CharacterLocation.X = -1441.876221;
	CharacterLocation.Y = 349.000549;
	CharacterLocation.Z = -2801.376953;
	SetActorLocation(CharacterLocation);
	//localPlayerController->SendTeleportInfo(TEL_FIRE);
}
void AMyCharacter::Cheat_Teleport2()
{
	//�ٸ�
	if (iSessionId != localPlayerController->iSessionId) return;
	FVector CharacterLocation;
	CharacterLocation.X = 1077.395142;
	CharacterLocation.Y = 4947.352051;
	CharacterLocation.Z = -3300.918213;
	SetActorLocation(CharacterLocation);
	//localPlayerController->SendTeleportInfo(TEL_BRIDGE);

}

void AMyCharacter::Cheat_Teleport3()
{
	//ž ��(�� �տ� ž)
	if (iSessionId != localPlayerController->iSessionId) return;
	FVector CharacterLocation;
	CharacterLocation.X = -2666.638428;
	CharacterLocation.Y = 11660.651367;
	CharacterLocation.Z = 940.448914;
	SetActorLocation(CharacterLocation);
	//localPlayerController->SendTeleportInfo(TEL_TOWER);

}
void AMyCharacter::Cheat_Teleport4()
{
	//����
	if (iSessionId != localPlayerController->iSessionId) return;
	FVector CharacterLocation;
	CharacterLocation.X = -7328.154785;
	CharacterLocation.Y = -1974.412354;
	CharacterLocation.Z = -3816.000000;
	SetActorLocation(CharacterLocation);
	//localPlayerController->SendCheatInfo(TEL_ICE);

}
void AMyCharacter::Cheat_IncreaseHP()
{
	localPlayerController->SendCheatInfo(CHEAT_HP_UP);
}
void AMyCharacter::Cheat_DecreaseHP()
{
	localPlayerController->SendCheatInfo(CHEAT_HP_DOWN);

	// �ӽ� - ���� ȹ�� ġƮŰ
	bHasShotgun = true;
	UpdateUI(UICategory::HasShotgun);
}
void AMyCharacter::Cheat_IncreaseSnowball()
{
	if (iSelectedProjectile == Projectile::Iceball)
	{
		localPlayerController->SendCheatInfo(CHEAT_ICE_PLUS);
	}
	else 
	{
		localPlayerController->SendCheatInfo(CHEAT_SNOW_PLUS);
	}
}

void AMyCharacter::Cheat_SpawnSupplyBox()
{	// ĳ���� �Ӹ� ���� SupplyBox ����
	FVector vector = GetActorLocation();
	vector.Z += 600.0f;

	localPlayerController->GetSocket()->SendPutObjPacket(SUPPLYBOX, 0, vector, 0.0);
	//localPlayerController->SpawnSupplyBox(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 600.0f);
}

void AMyCharacter::ReleaseRightMouseButton()
{
	switch (iSelectedItem) {
	case ItemTypeList::Umbrella:	// ��� ��� ����
		//MYLOG(Warning, TEXT("relese_mouse"));
		ReleaseUmbrella();
		break;
	default:
		break;
	}
}

void AMyCharacter::StartUmbrella()
{
	myAnim->PlayUmbrellaMontage();

	bReleaseUmbrella = false;
}

void AMyCharacter::ShowUmbrella()
{
	// ��� �޽� ���̵���
	umbrella1MeshComponent->SetVisibility(true);
	umbrella2MeshComponent->SetVisibility(true);

	// ��� �޽� �ݸ��� Ȱ��ȭ
	umb1CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	umb2CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	OpenUmbrellaAnim();		// ��� ��ġ�� �ִϸ��̼�
}

void AMyCharacter::HideUmbrella()
{
	// ��� �޽� �Ⱥ��̵���
	umbrella1MeshComponent->SetVisibility(false);
	umbrella2MeshComponent->SetVisibility(false);

	// ��� �޽� �ݸ��� ��Ȱ��ȭ
	umb1CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	umb2CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	umb1AnimInst->SetClose(false);
	umb2AnimInst->SetClose(false);

	iUmbrellaState = UmbrellaState::UmbClosed;
}

void AMyCharacter::OpenUmbrellaAnim()
{
	iUmbrellaState = UmbrellaState::UmbOpening;

	// ��� �޽� �ִϸ��̼� ��� (��ġ��)
	umb1AnimInst->SetOpen(true);
	umb2AnimInst->SetOpen(true);
}

void AMyCharacter::CloseUmbrellaAnim()
{
	iUmbrellaState = UmbrellaState::UmbClosing;

	// ��� �޽� �ִϸ��̼� ��� (������)
	umb1AnimInst->SetClose(true);
	umb2AnimInst->SetClose(true);

	umb1AnimInst->SetOpen(false);
	umb2AnimInst->SetOpen(false);
}

void AMyCharacter::ReleaseUmbrella()
{
	bReleaseUmbrella = true;

	switch (iUmbrellaState) {
	case UmbrellaState::UmbClosed:
		break;
	case UmbrellaState::UmbOpening:
		break;
	case UmbrellaState::UmbOpened:
		if (iSessionId == localPlayerController->iSessionId)
		{
			MYLOG(Warning, TEXT("ReleaseUmbrella"));
			localPlayerController->SendPlayerInfo(COMMAND_UMB_END);
		}
		//myAnim->ResumeUmbrellaMontage();
		//CloseUmbrellaAnim();
		break;
	case UmbrellaState::UmbClosing:
		break;
	default:
		break;
	}
}

void AMyCharacter::GetBag()
{
	bHasBag = true;
	iMaxSnowballCount += 5;	// ������ 10 -> 15 ���� ���� ����
	iMaxIceballCount += 5;	// ���̽��� 10 -> 15 ���� ���� ����
	iMaxMatchCount += 1;	// ���� 2 -> 3 ���� ���� ����
	UpdateUI(UICategory::HasBag);
	UpdateUI(UICategory::MaxSnowIceballAndMatch);

	bagMeshComponent->SetVisibility(true);
}

void AMyCharacter::ShowProjectilePath()
{
	if (iSessionId != localPlayerController->iSessionId) return;	// ������ ������ �׸�����

	projectilePath->ClearSplinePoints();
	HideProjectilePath();

	if (myAnim->bThrowing)
	{
		FHitResult OutHitResult;	// ��� x
		TArray<FVector> OutPathPositions;
		FVector OutLastTraceDestination;	// ��� x

		//FVector StartPos = GetMesh()->GetSocketLocation(TEXT("SnowballSocket"));
		FVector StartPos = projectilePathStartPos->GetComponentLocation();
		FVector cameraLocation;
		FRotator cameraRotation;
		GetActorEyesViewPoint(cameraLocation, cameraRotation);
		FVector LaunchVelocity = (cameraRotation.Vector() + FVector(0.0f, 0.0f, 0.15f)) * (2000.0f + fAimingElapsedTime * fThrowPower);
		//FVector LaunchVelocity = (cameraRotation.Vector() + FVector(0.0f, 0.0f, 0.15f)) * 2500.0f;
		// bool bTracePath, float ProjectileRadius, TEnumAsByte<ECollisionChannel> TraceChannel, bool bTraceComplex,
		TArray<AActor*> actorsToIgnore;
		actorsToIgnore.Add(this);
		actorsToIgnore.Add(aimingCameraPos);
		//EDrawDebugTrace::Type DrawDebugType

		// float DrawDebugTime, float SimFrequency, float MaxSimTime, float OverrideGravityZ;

		UGameplayStatics::Blueprint_PredictProjectilePath_ByTraceChannel(GetWorld(), OutHitResult, OutPathPositions, OutLastTraceDestination,
			StartPos, LaunchVelocity, true, 0.0f, ECollisionChannel::ECC_Camera, false, actorsToIgnore, EDrawDebugTrace::None,
			0.0f, 15.0f, 2.0f, 0.0f);

		for (int i = 0; i < OutPathPositions.Num(); ++i)
		{
			projectilePath->AddSplinePointAtIndex(OutPathPositions[i], i, ESplineCoordinateSpace::Local);
		}

		int lastIndex = projectilePath->GetNumberOfSplinePoints() - 1;
		if (lastIndex > iNumOfPathSpline - 1) lastIndex = iNumOfPathSpline - 1;

		for (int i = 0; i < lastIndex; ++i)
		{
			FVector startPos, startTangent, endPos, endTangent;
			startPos = projectilePath->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			startTangent = projectilePath->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
			endPos = projectilePath->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
			endTangent = projectilePath->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

			splineMeshComponents[i]->SetStartAndEnd(startPos, startTangent, endPos, endTangent);
			splineMeshComponents[i]->SetVisibility(true);
		}

		//Delay �Լ�
		FTimerHandle WaitHandle;
		float WaitTime = GetWorld()->GetDeltaSeconds();
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				ShowProjectilePath();
			}), WaitTime, false);
	}
}

void AMyCharacter::HideProjectilePath()
{
	for (int i = 0; i < iNumOfPathSpline; ++i)
	{
		splineMeshComponents[i]->SetVisibility(false);
	}
}

void AMyCharacter::SetAimingCameraPos()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	aimingCameraPos = GetWorld()->SpawnActor<AEmptyActor>(AEmptyActor::StaticClass(), GetActorTransform(), SpawnParams);
	FAttachmentTransformRules atr = FAttachmentTransformRules(
		EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
	aimingCameraPos->AttachToComponent(springArm2, atr);
}

void AMyCharacter::UpdateAiming()
{
	if (!bIsAiming) return;
	if (fAimingElapsedTime >= fMaxChargingTime) return;
	fAimingElapsedTime += GetWorld()->GetDeltaSeconds();
}

void AMyCharacter::GetOnOffJetski()
{
	if (bIsSnowman) return;		// ������� jetski ž�� x
	if (isAttacking) return;	// ���� �� ��Ʈ��Ű ž�� �� ���� x
	if (GetMovementComponent()->IsFalling()) return;	// ���߿����� ��Ʈ��Ű ž�� �� ���� x

	if (iSessionId == localPlayerController->iSessionId)
	{
		//��Ʈ��Ű ž�½� ���� ����
		localPlayerController->GetSocket()->Send_ItemPacket(ITEM_JET, 0);
	}

	if (!bIsRiding) GetOnJetski();
	else GetOffJetski();
}


void AMyCharacter::GetOnJetski()
{	// jetski ž��

	TArray<AActor*> overlapActorsArray;	// overlap ���� jetski�� ���� �迭
	GetOverlappingActors(overlapActorsArray, jetskiClass);

	if (overlapActorsArray.Num() == 1)
	{	
		bIsRiding = true;

		// �ִϸ��̼� BP ��� drive �ִϸ��̼� ���
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		GetMesh()->PlayAnimation(driveAnimAsset, true);
		
		// jetski ����, �浹, �þ� Ȱ��ȭ
		jetskiMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		jetskiMeshComponent->SetSimulatePhysics(true);
		jetskiMeshComponent->SetVisibility(true);

		// ��ġ�� jetski�� ��ġ�� ĳ���� �̵� �� ȸ��
		SetActorLocation(overlapActorsArray[0]->GetActorLocation());
		if (iSessionId == localPlayerController->iSessionId)
		{
			FRotator newRotation = FRotator(
				GetControlRotation().Pitch, overlapActorsArray[0]->GetActorRotation().Yaw, GetControlRotation().Roll);
			localPlayerController->SetControlRotation(newRotation);
		}

		// ��ġ�� jetski ����
		overlapActorsArray[0]->Destroy();

		// jetski �޽ÿ� ĳ���� �޽� attach
		FAttachmentTransformRules atr = FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
		GetMesh()->AttachToComponent(jetskiMeshComponent, atr);
		GetMesh()->SetRelativeLocation(FVector(-8.68f, -4.0f, 15.35f));

		// ������ ī�޶�� ��ȯ, ũ�ν���� ����
		camera->Deactivate();
		camera3->Activate();
		if (iSessionId == localPlayerController->iSessionId)
		{
			localPlayerController->GetHUD()->bShowHUD = false;
		}

		// �̵��ӵ� ����
		GetCharacterMovement()->MaxWalkSpeed = iJetskiSpeed;
	}
}

void AMyCharacter::GetOffJetski()
{	// jetski ����

	bIsRiding = false;

	// jetski �������� ĳ���� �̵� �� ȸ��
	SetActorLocation(FVector(GetActorLocation() - jetskiMeshComponent->GetRightVector() * 80));
	localPlayerController->SetControlRotation(FRotator(GetControlRotation().Pitch, GetControlRotation().Yaw, 0.0f));

	// jetski ����, �浹, �þ� ��Ȱ��ȭ
	jetskiMeshComponent->SetSimulatePhysics(false);
	jetskiMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	jetskiMeshComponent->SetVisibility(false);

	// ĳ���� �޽� jetski �޽ÿ��� detach
	GetMesh()->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	FAttachmentTransformRules atr = FAttachmentTransformRules(
		EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
	GetMesh()->AttachToComponent(GetCapsuleComponent(), atr);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -77.0f), FRotator(0.0f, -90.0f, 0.0f));

	// �ش� ��ġ�� jetski ����
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	FVector location = FVector(jetskiMeshComponent->GetComponentLocation() + FVector(0.0f, 0.0f, 27.5f));
	FTransform transform = FTransform(jetskiMeshComponent->GetComponentRotation(), location, jetskiMeshComponent->GetRelativeScale3D());
	GetWorld()->SpawnActor<AJetski>(jetskiClass, transform, SpawnParams);

	// ���� �ִϸ��̼� BP�� ����
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetAnimInstanceClass(bearAnim);
	myAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	MYCHECK(nullptr != myAnim);
	myAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);

	// ���� ī�޶�� ��ȯ, ũ�ν���� ����
	camera->Activate();
	camera3->Deactivate();
	localPlayerController->GetHUD()->bShowHUD = true;

	// �̵��ӵ� ����
	GetCharacterMovement()->MaxWalkSpeed = iNormalSpeed;
}

void AMyCharacter::UpdateJetski()
{
	if (bIsRiding)
	{	// jetski �޽� ��ġ �� ȸ�� ����, �������� ����
		jetskiMeshComponent->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 56.0f));
		float pitch = UKismetMathLibrary::ClampAngle(jetskiMeshComponent->GetComponentRotation().Pitch, -20.0f, 20.0f);
		float roll = UKismetMathLibrary::ClampAngle(jetskiMeshComponent->GetComponentRotation().Roll, -10.0f, 10.0f);
		//FRotator newRotation = FRotator(jetskiMeshComponent->GetComponentRotation().Pitch, GetActorRotation().Yaw, roll);
		FRotator newRotation = FRotator(pitch, GetActorRotation().Yaw, roll);
		jetskiMeshComponent->SetWorldRotation(newRotation);
	}
}

void AMyCharacter::SettingHead() 
{
	TArray<const wchar_t*> refs
		= { TEXT("/Game/FX/Frozen/Meshes/head_2.head_2"), TEXT("/Game/FX/Frozen/Meshes/head_3.head_3"), TEXT("/Game/FX/Frozen/Meshes/head_4.head_4"),
	TEXT("/Game/FX/Frozen/Meshes/head_5.head_5"), TEXT("/Game/FX/Frozen/Meshes/head_6.head_6"), TEXT("/Game/FX/Frozen/Meshes/head_7.head_7"),
	TEXT("/Game/FX/Frozen/Meshes/head_8.head_8"), TEXT("/Game/FX/Frozen/Meshes/head_9.head_9"), TEXT("/Game/FX/Frozen/Meshes/head_10.head_10"),
	TEXT("/Game/FX/Frozen/Meshes/head_11.head_11"), TEXT("/Game/FX/Frozen/Meshes/head_12.head_12"), TEXT("/Game/FX/Frozen/Meshes/head_13.head_13"),
	TEXT("/Game/FX/Frozen/Meshes/head_14.head_14"), TEXT("/Game/FX/Frozen/Meshes/head_15.head_15"), TEXT("/Game/FX/Frozen/Meshes/head_16.head_16"),
	TEXT("/Game/FX/Frozen/Meshes/head_17.head_17"), TEXT("/Game/FX/Frozen/Meshes/head_18.head_18"), TEXT("/Game/FX/Frozen/Meshes/head_19.head_19"),
	TEXT("/Game/FX/Frozen/Meshes/head_20.head_20"), TEXT("/Game/FX/Frozen/Meshes/head_21.head_21"), TEXT("/Game/FX/Frozen/Meshes/head_22.head_22"),
	TEXT("/Game/FX/Frozen/Meshes/head_23.head_23"), TEXT("/Game/FX/Frozen/Meshes/head_24.head_24") };

	//����ƽ �޽ø��� ���� �ʿ�
	for (int i = 0; i < 23; ++i)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> headMesh(refs[i]);
		if (headMesh.Succeeded())
		{
			headMeshes.Add(headMesh.Object);
		}
	}

	//����ƽ �޽�������Ʈ�� �޽� �������� ����
	headComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("head"));
	headComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	headComponent->SetupAttachment(GetMesh(), TEXT("HeadSocket"));
	headComponent->SetVisibility(true);
}

void AMyCharacter::SettingLeftForearm()
{
	TArray<const wchar_t*> refs
		= { TEXT("/Game/FX/Frozen/Meshes/left_forearm_2.left_forearm_2"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_3.left_forearm_3"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_4.left_forearm_4"),
	TEXT("/Game/FX/Frozen/Meshes/left_forearm_5.left_forearm_5"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_6.left_forearm_6"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_7.left_forearm_7"),
	TEXT("/Game/FX/Frozen/Meshes/left_forearm_8.left_forearm_8"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_9.left_forearm_9"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_10.left_forearm_10"),
	TEXT("/Game/FX/Frozen/Meshes/left_forearm_11.left_forearm_11"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_12.left_forearm_12"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_13.left_forearm_13"),
	TEXT("/Game/FX/Frozen/Meshes/left_forearm_14.left_forearm_14"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_15.left_forearm_15"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_16.left_forearm_16"),
	TEXT("/Game/FX/Frozen/Meshes/left_forearm_17.left_forearm_17"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_18.left_forearm_18"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_19.left_forearm_19"),
	TEXT("/Game/FX/Frozen/Meshes/left_forearm_20.left_forearm_20"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_21.left_forearm_21"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_22.left_forearm_22"),
	TEXT("/Game/FX/Frozen/Meshes/left_forearm_23.left_forearm_23"), TEXT("/Game/FX/Frozen/Meshes/left_forearm_24.left_forearm_24") };

	//����ƽ �޽ø��� ���� �ʿ�
	for (int i = 0; i < 23; ++i)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> leftForearmMesh(refs[i]);
		if (leftForearmMesh.Succeeded())
		{
			leftForearmMeshes.Add(leftForearmMesh.Object);
		}
	}

	//����ƽ �޽�������Ʈ�� �޽� �������� ����
	leftForearmComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leftForearm"));
	leftForearmComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	leftForearmComponent->SetupAttachment(GetMesh(), TEXT("LeftForearmSocket"));
	leftForearmComponent->SetVisibility(true);
}

void AMyCharacter::SettingLeftUpperArm()
{
	TArray<const wchar_t*> names
		= { TEXT("leftUpperarm1"), TEXT("leftUpperarm2"), TEXT("leftUpperarm3"), TEXT("leftUpperarm4"), TEXT("leftUpperarm5"), TEXT("leftUpperarm6"), TEXT("leftUpperarm7"), TEXT("leftUpperarm8"),
		TEXT("leftUpperarm9"), TEXT("leftUpperarm10"), TEXT("leftUpperarm11"), TEXT("leftUpperarm12"), TEXT("leftUpperarm13"), TEXT("leftUpperarm14"), TEXT("leftUpperarm15"), TEXT("leftUpperarm16"),
		TEXT("leftUpperarm17"), TEXT("leftUpperarm18"), TEXT("leftUpperarm19"), TEXT("leftUpperarm20"), TEXT("leftUpperarm21"), TEXT("leftUpperarm22"), TEXT("leftUpperarm23") };

	TArray<const wchar_t*> refs
		= { TEXT("/Game/FX/Frozen/Meshes/left_upperarm_2.left_upperarm_2"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_3.left_upperarm_3"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_4.left_upperarm_4"),
	TEXT("/Game/FX/Frozen/Meshes/left_upperarm_5.left_upperarm_5"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_6.left_upperarm_6"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_7.left_upperarm_7"),
	TEXT("/Game/FX/Frozen/Meshes/left_upperarm_8.left_upperarm_8"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_9.left_upperarm_9"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_10.left_upperarm_10"),
	TEXT("/Game/FX/Frozen/Meshes/left_upperarm_11.left_upperarm_11"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_12.left_upperarm_12"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_13.left_upperarm_13"),
	TEXT("/Game/FX/Frozen/Meshes/left_upperarm_14.left_upperarm_14"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_15.left_upperarm_15"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_16.left_upperarm_16"),
	TEXT("/Game/FX/Frozen/Meshes/left_upperarm_17.left_upperarm_17"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_18.left_upperarm_18"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_19.left_upperarm_19"),
	TEXT("/Game/FX/Frozen/Meshes/left_upperarm_20.left_upperarm_20"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_21.left_upperarm_21"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_22.left_upperarm_22"),
	TEXT("/Game/FX/Frozen/Meshes/left_upperarm_23.left_upperarm_23"), TEXT("/Game/FX/Frozen/Meshes/left_upperarm_24.left_upperarm_24") };

	//����ƽ �޽ø��� ���� �ʿ�
	for (int i = 0; i < 23; ++i)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> leftUpperarmMesh(refs[i]);
		if (leftUpperarmMesh.Succeeded())
		{
			leftUpperarmMeshes.Add(leftUpperarmMesh.Object);
		}
	}

	//����ƽ �޽�������Ʈ�� �޽� �������� ����
	leftUpperarmComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leftUpperarm"));
	leftUpperarmComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	leftUpperarmComponent->SetupAttachment(GetMesh(), TEXT("LeftUpperarmSocket"));
	leftUpperarmComponent->SetVisibility(true);
}

void AMyCharacter::SettingRightForearm()
{
	TArray<const wchar_t*> refs
		= { TEXT("/Game/FX/Frozen/Meshes/right_forearm_2.right_forearm_2"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_3.right_forearm_3"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_4.right_forearm_4"),
	TEXT("/Game/FX/Frozen/Meshes/right_forearm_5.right_forearm_5"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_6.right_forearm_6"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_7.right_forearm_7"),
	TEXT("/Game/FX/Frozen/Meshes/right_forearm_8.right_forearm_8"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_9.right_forearm_9"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_10.right_forearm_10"),
	TEXT("/Game/FX/Frozen/Meshes/right_forearm_11.right_forearm_11"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_12.right_forearm_12"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_13.right_forearm_13"),
	TEXT("/Game/FX/Frozen/Meshes/right_forearm_14.right_forearm_14"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_15.right_forearm_15"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_16.right_forearm_16"),
	TEXT("/Game/FX/Frozen/Meshes/right_forearm_17.right_forearm_17"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_18.right_forearm_18"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_19.right_forearm_19"),
	TEXT("/Game/FX/Frozen/Meshes/right_forearm_20.right_forearm_20"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_21.right_forearm_21"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_22.right_forearm_22"),
	TEXT("/Game/FX/Frozen/Meshes/right_forearm_23.right_forearm_23"), TEXT("/Game/FX/Frozen/Meshes/right_forearm_24.right_forearm_24") };

	//����ƽ �޽ø��� ���� �ʿ�
	for (int i = 0; i < 23; ++i)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> rightForearmMesh(refs[i]);
		if (rightForearmMesh.Succeeded())
		{
			rightForearmMeshes.Add(rightForearmMesh.Object);
		}
	}

	//����ƽ �޽�������Ʈ�� �޽� �������� ����
	rightForearmComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightForearm"));
	rightForearmComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	rightForearmComponent->SetupAttachment(GetMesh(), TEXT("RightForearmSocket"));
	rightForearmComponent->SetVisibility(true);
}

void AMyCharacter::SettingRightUpperArm()
{
	TArray<const wchar_t*> refs
		= { TEXT("/Game/FX/Frozen/Meshes/right_upperarm_2.right_upperarm_2"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_3.right_upperarm_3"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_4.right_upperarm_4"),
	TEXT("/Game/FX/Frozen/Meshes/right_upperarm_5.right_upperarm_5"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_6.right_upperarm_6"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_7.right_upperarm_7"),
	TEXT("/Game/FX/Frozen/Meshes/right_upperarm_8.right_upperarm_8"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_9.right_upperarm_9"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_10.right_upperarm_10"),
	TEXT("/Game/FX/Frozen/Meshes/right_upperarm_11.right_upperarm_11"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_12.right_upperarm_12"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_13.right_upperarm_13"),
	TEXT("/Game/FX/Frozen/Meshes/right_upperarm_14.right_upperarm_14"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_15.right_upperarm_15"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_16.right_upperarm_16"),
	TEXT("/Game/FX/Frozen/Meshes/right_upperarm_17.right_upperarm_17"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_18.right_upperarm_18"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_19.right_upperarm_19"),
	TEXT("/Game/FX/Frozen/Meshes/right_upperarm_20.right_upperarm_20"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_21.right_upperarm_21"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_22.right_upperarm_22"),
	TEXT("/Game/FX/Frozen/Meshes/right_upperarm_23.right_upperarm_23"), TEXT("/Game/FX/Frozen/Meshes/right_upperarm_24.right_upperarm_24") };

	//����ƽ �޽ø��� ���� �ʿ�
	for (int i = 0; i < 23; ++i)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> rightUpperarmMesh(refs[i]);
		if (rightUpperarmMesh.Succeeded())
		{
			rightUpperarmMeshes.Add(rightUpperarmMesh.Object);
		}
	}

	//����ƽ �޽�������Ʈ�� �޽� �������� ����
	rightUpperarmComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightUpperarm"));
	rightUpperarmComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	rightUpperarmComponent->SetupAttachment(GetMesh(), TEXT("RightUpperarmSocket"));
	rightUpperarmComponent->SetVisibility(true);
}

void AMyCharacter::SettingCenter()
{
	TArray<const wchar_t*> refs
		= { TEXT("/Game/FX/Frozen/Meshes/center_2.center_2"), TEXT("/Game/FX/Frozen/Meshes/center_3.center_3"), TEXT("/Game/FX/Frozen/Meshes/center_4.center_4"),
	TEXT("/Game/FX/Frozen/Meshes/center_5.center_5"), TEXT("/Game/FX/Frozen/Meshes/center_6.center_6"), TEXT("/Game/FX/Frozen/Meshes/center_7.center_7"),
	TEXT("/Game/FX/Frozen/Meshes/center_8.center_8"), TEXT("/Game/FX/Frozen/Meshes/center_9.center_9"), TEXT("/Game/FX/Frozen/Meshes/center_10.center_10"),
	TEXT("/Game/FX/Frozen/Meshes/center_11.center_11"), TEXT("/Game/FX/Frozen/Meshes/center_12.center_12"), TEXT("/Game/FX/Frozen/Meshes/center_13.center_13"),
	TEXT("/Game/FX/Frozen/Meshes/center_14.center_14"), TEXT("/Game/FX/Frozen/Meshes/center_15.center_15"), TEXT("/Game/FX/Frozen/Meshes/center_16.center_16"),
	TEXT("/Game/FX/Frozen/Meshes/center_17.center_17"), TEXT("/Game/FX/Frozen/Meshes/center_18.center_18"), TEXT("/Game/FX/Frozen/Meshes/center_19.center_19"),
	TEXT("/Game/FX/Frozen/Meshes/center_20.center_20"), TEXT("/Game/FX/Frozen/Meshes/center_21.center_21"), TEXT("/Game/FX/Frozen/Meshes/center_22.center_22"),
	TEXT("/Game/FX/Frozen/Meshes/center_23.center_23"), TEXT("/Game/FX/Frozen/Meshes/center_24.center_24") };

	//����ƽ �޽ø��� ���� �ʿ�
	for (int i = 0; i < 23; ++i)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> centerMesh(refs[i]);
		if (centerMesh.Succeeded())
		{
			centerMeshes.Add(centerMesh.Object);
		}
	}

	//����ƽ �޽�������Ʈ�� �޽� �������� ����
	centerComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("center"));
	centerComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	centerComponent->SetupAttachment(GetMesh(), TEXT("CenterSocket"));
	centerComponent->SetVisibility(true);
}

void AMyCharacter::SettingLeftThigh()
{
	TArray<const wchar_t*> refs
		= { TEXT("/Game/FX/Frozen/Meshes/left_thigh_2.left_thigh_2"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_3.left_thigh_3"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_4.left_thigh_4"),
	TEXT("/Game/FX/Frozen/Meshes/left_thigh_5.left_thigh_5"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_6.left_thigh_6"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_7.left_thigh_7"),
	TEXT("/Game/FX/Frozen/Meshes/left_thigh_8.left_thigh_8"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_9.left_thigh_9"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_10.left_thigh_10"),
	TEXT("/Game/FX/Frozen/Meshes/left_thigh_11.left_thigh_11"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_12.left_thigh_12"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_13.left_thigh_13"),
	TEXT("/Game/FX/Frozen/Meshes/left_thigh_14.left_thigh_14"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_15.left_thigh_15"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_16.left_thigh_16"),
	TEXT("/Game/FX/Frozen/Meshes/left_thigh_17.left_thigh_17"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_18.left_thigh_18"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_19.left_thigh_19"),
	TEXT("/Game/FX/Frozen/Meshes/left_thigh_20.left_thigh_20"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_21.left_thigh_21"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_22.left_thigh_22"),
	TEXT("/Game/FX/Frozen/Meshes/left_thigh_23.left_thigh_23"), TEXT("/Game/FX/Frozen/Meshes/left_thigh_24.left_thigh_24") };

	//����ƽ �޽ø��� ���� �ʿ�
	for (int i = 0; i < 23; ++i)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> leftThighMesh(refs[i]);
		if (leftThighMesh.Succeeded())
		{
			leftThighMeshes.Add(leftThighMesh.Object);
		}
	}

	//����ƽ �޽�������Ʈ�� �޽� �������� ����
	leftThighComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leftThigh"));
	leftThighComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	leftThighComponent->SetupAttachment(GetMesh(), TEXT("LeftThighSocket"));
	leftThighComponent->SetVisibility(true);
}

void AMyCharacter::SettingLeftCalf()
{
	TArray<const wchar_t*> refs
		= { TEXT("/Game/FX/Frozen/Meshes/left_calf_2.left_calf_2"), TEXT("/Game/FX/Frozen/Meshes/left_calf_3.left_calf_3"), TEXT("/Game/FX/Frozen/Meshes/left_calf_4.left_calf_4"),
	TEXT("/Game/FX/Frozen/Meshes/left_calf_5.left_calf_5"), TEXT("/Game/FX/Frozen/Meshes/left_calf_6.left_calf_6"), TEXT("/Game/FX/Frozen/Meshes/left_calf_7.left_calf_7"),
	TEXT("/Game/FX/Frozen/Meshes/left_calf_8.left_calf_8"), TEXT("/Game/FX/Frozen/Meshes/left_calf_9.left_calf_9"), TEXT("/Game/FX/Frozen/Meshes/left_calf_10.left_calf_10"),
	TEXT("/Game/FX/Frozen/Meshes/left_calf_11.left_calf_11"), TEXT("/Game/FX/Frozen/Meshes/left_calf_12.left_calf_12"), TEXT("/Game/FX/Frozen/Meshes/left_calf_13.left_calf_13"),
	TEXT("/Game/FX/Frozen/Meshes/left_calf_14.left_calf_14"), TEXT("/Game/FX/Frozen/Meshes/left_calf_15.left_calf_15"), TEXT("/Game/FX/Frozen/Meshes/left_calf_16.left_calf_16"),
	TEXT("/Game/FX/Frozen/Meshes/left_calf_17.left_calf_17"), TEXT("/Game/FX/Frozen/Meshes/left_calf_18.left_calf_18"), TEXT("/Game/FX/Frozen/Meshes/left_calf_19.left_calf_19"),
	TEXT("/Game/FX/Frozen/Meshes/left_calf_20.left_calf_20"), TEXT("/Game/FX/Frozen/Meshes/left_calf_21.left_calf_21"), TEXT("/Game/FX/Frozen/Meshes/left_calf_22.left_calf_22"),
	TEXT("/Game/FX/Frozen/Meshes/left_calf_23.left_calf_23"), TEXT("/Game/FX/Frozen/Meshes/left_calf_24.left_calf_24") };

	//����ƽ �޽ø��� ���� �ʿ�
	for (int i = 0; i < 23; ++i)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> leftCalfMesh(refs[i]);
		if (leftCalfMesh.Succeeded())
		{
			leftCalfMeshes.Add(leftCalfMesh.Object);
		}
	}

	//����ƽ �޽�������Ʈ�� �޽� �������� ����
	leftCalfComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leftCalf"));
	leftCalfComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	leftCalfComponent->SetupAttachment(GetMesh(), TEXT("LeftCalfSocket"));
	leftCalfComponent->SetVisibility(true);
}

void AMyCharacter::SettingRightThigh()
{
	TArray<const wchar_t*> refs
		= { TEXT("/Game/FX/Frozen/Meshes/right_thigh_2.right_thigh_2"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_3.right_thigh_3"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_4.right_thigh_4"),
	TEXT("/Game/FX/Frozen/Meshes/right_thigh_5.right_thigh_5"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_6.right_thigh_6"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_7.right_thigh_7"),
	TEXT("/Game/FX/Frozen/Meshes/right_thigh_8.right_thigh_8"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_9.right_thigh_9"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_10.right_thigh_10"),
	TEXT("/Game/FX/Frozen/Meshes/right_thigh_11.right_thigh_11"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_12.right_thigh_12"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_13.right_thigh_13"),
	TEXT("/Game/FX/Frozen/Meshes/right_thigh_14.right_thigh_14"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_15.right_thigh_15"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_16.right_thigh_16"),
	TEXT("/Game/FX/Frozen/Meshes/right_thigh_17.right_thigh_17"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_18.right_thigh_18"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_19.right_thigh_19"),
	TEXT("/Game/FX/Frozen/Meshes/right_thigh_20.right_thigh_20"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_21.right_thigh_21"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_22.right_thigh_22"),
	TEXT("/Game/FX/Frozen/Meshes/right_thigh_23.right_thigh_23"), TEXT("/Game/FX/Frozen/Meshes/right_thigh_24.right_thigh_24") };

	//����ƽ �޽ø��� ���� �ʿ�
	for (int i = 0; i < 23; ++i)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> rightThighMesh(refs[i]);
		if (rightThighMesh.Succeeded())
		{
			rightThighMeshes.Add(rightThighMesh.Object);
		}
	}

	//����ƽ �޽�������Ʈ�� �޽� �������� ����
	rightThighComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightThigh"));
	rightThighComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	rightThighComponent->SetupAttachment(GetMesh(), TEXT("RightThighSocket"));
	rightThighComponent->SetVisibility(true);
}

void AMyCharacter::SettingRightCalf()
{
	TArray<const wchar_t*> refs
		= { TEXT("/Game/FX/Frozen/Meshes/right_calf_2.right_calf_2"), TEXT("/Game/FX/Frozen/Meshes/right_calf_3.right_calf_3"), TEXT("/Game/FX/Frozen/Meshes/right_calf_4.right_calf_4"),
	TEXT("/Game/FX/Frozen/Meshes/right_calf_5.right_calf_5"), TEXT("/Game/FX/Frozen/Meshes/right_calf_6.right_calf_6"), TEXT("/Game/FX/Frozen/Meshes/right_calf_7.right_calf_7"),
	TEXT("/Game/FX/Frozen/Meshes/right_calf_8.right_calf_8"), TEXT("/Game/FX/Frozen/Meshes/right_calf_9.right_calf_9"), TEXT("/Game/FX/Frozen/Meshes/right_calf_10.right_calf_10"),
	TEXT("/Game/FX/Frozen/Meshes/right_calf_11.right_calf_11"), TEXT("/Game/FX/Frozen/Meshes/right_calf_12.right_calf_12"), TEXT("/Game/FX/Frozen/Meshes/right_calf_13.right_calf_13"),
	TEXT("/Game/FX/Frozen/Meshes/right_calf_14.right_calf_14"), TEXT("/Game/FX/Frozen/Meshes/right_calf_15.right_calf_15"), TEXT("/Game/FX/Frozen/Meshes/right_calf_16.right_calf_16"),
	TEXT("/Game/FX/Frozen/Meshes/right_calf_17.right_calf_17"), TEXT("/Game/FX/Frozen/Meshes/right_calf_18.right_calf_18"), TEXT("/Game/FX/Frozen/Meshes/right_calf_19.right_calf_19"),
	TEXT("/Game/FX/Frozen/Meshes/right_calf_20.right_calf_20"), TEXT("/Game/FX/Frozen/Meshes/right_calf_21.right_calf_21"), TEXT("/Game/FX/Frozen/Meshes/right_calf_22.right_calf_22"),
	TEXT("/Game/FX/Frozen/Meshes/right_calf_23.right_calf_23"), TEXT("/Game/FX/Frozen/Meshes/right_calf_24.right_calf_24") };

	//����ƽ �޽ø��� ���� �ʿ�
	for (int i = 0; i < 23; ++i)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> rightCalfMesh(refs[i]);
		if (rightCalfMesh.Succeeded())
		{
			rightCalfMeshes.Add(rightCalfMesh.Object);
		}
	}

	//����ƽ �޽�������Ʈ�� �޽� �������� ����
	rightCalfComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightCalf"));
	rightCalfComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	rightCalfComponent->SetupAttachment(GetMesh(), TEXT("RightCalfSocket"));
	rightCalfComponent->SetVisibility(true);
}

void AMyCharacter::FreezeHead()
{
	FreezeAnimation(headHandle, iHeadFrame, bHeadAnimEnd, headComponent, headMeshes);
}

void AMyCharacter::FreezeLeftForearm()
{
	FreezeAnimation(leftForearmHandle, iLeftForearmFrame, bLeftForearmAnimEnd, leftForearmComponent, leftForearmMeshes);
}

void AMyCharacter::FreezeLeftUpperarm()
{
	FreezeAnimation(leftUpperarmHandle, iLeftUpperarmFrame, bLeftUpperarmAnimEnd, leftUpperarmComponent, leftUpperarmMeshes);
}

void AMyCharacter::FreezeRightForearm()
{
	FreezeAnimation(rightForearmHandle, iRightForearmFrame, bRightForearmAnimEnd, rightForearmComponent, rightForearmMeshes);
}

void AMyCharacter::FreezeRightUpperarm()
{
	FreezeAnimation(rightUpperarmHandle, iRightUpperarmFrame, bRightUpperarmAnimEnd, rightUpperarmComponent, rightUpperarmMeshes);
}

void AMyCharacter::FreezeCenter()
{
	FreezeAnimation(centerHandle, iCenterFrame, bCenterAnimEnd, centerComponent, centerMeshes);
}

void AMyCharacter::FreezeLeftThigh()
{
	FreezeAnimation(leftThighHandle, iLeftThighFrame, bLeftThighAnimEnd, leftThighComponent, leftThighMeshes);
}

void AMyCharacter::FreezeLeftCalf()
{
	FreezeAnimation(leftCalfHandle, iLeftCalfFrame, bLeftCalfAnimEnd, leftCalfComponent, leftCalfMeshes);
}

void AMyCharacter::FreezeRightThigh()
{
	FreezeAnimation(rightThighHandle, iRightThighFrame, bRightThighAnimEnd, rightThighComponent, rightThighMeshes);
}

void AMyCharacter::FreezeRightCalf()
{
	FreezeAnimation(rightCalfHandle, iRightCalfFrame, bRightCalfAnimEnd, rightCalfComponent, rightCalfMeshes);
}

void AMyCharacter::FreezeAnimation(FTimerHandle& timerHandle, int& frame, bool& end, UStaticMeshComponent*& bone, TArray<UStaticMesh*>& FrozenMeshes)
{
	//bone->SetVisibility(true);

	if (GetIsSnowman())
	{
		InitializeFreeze();
		return;
	}

	float WaitTime = 0.1f;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]()
		{
			//MYLOG(Warning, TEXT("%d"), frame);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d"), frame));

			bone->SetStaticMesh(FrozenMeshes[frame]);

			if (bone == headComponent)
			{
				auto mat = bone->CreateDynamicMaterialInstance(1);
				mat->SetScalarParameterValue(TEXT("Emissive"), frame * 0.125);
				mat->SetTextureParameterValue(FName("Tex"), bearTextureArray[iId]);	// ���� ������ �� �ؽ��� ���
			}

			//���� �󸮴� ���� �ʱ�ȭ
			if (GetIsSnowman())
			{
				InitializeFreeze();
				end = true;
				frame = 0;
			}

			//�迭 �� ����
			if (iEachBoneCount - 1 == frame)
			{
				end = true;
				frame = 0;
			}
			else
				frame += 1;
		}), WaitTime, true);
}

void AMyCharacter::FreezeAnimationEndCheck(FTimerHandle& timerHandle, bool& end)
{
	if (end)
	{
		GetWorld()->GetTimerManager().ClearTimer(timerHandle);
		end = false;
	}
}

void AMyCharacter::InitializeFreeze()
{
	headComponent->SetStaticMesh(nullptr);
	if (headComponent->GetStaticMesh())
		headComponent->CreateDynamicMaterialInstance(1)->SetScalarParameterValue(TEXT("Emissive"), 0.0f);

	leftForearmComponent->SetStaticMesh(nullptr);
	leftUpperarmComponent->SetStaticMesh(nullptr);
	rightForearmComponent->SetStaticMesh(nullptr);
	rightUpperarmComponent->SetStaticMesh(nullptr);
	centerComponent->SetStaticMesh(nullptr);
	leftThighComponent->SetStaticMesh(nullptr);
	leftCalfComponent->SetStaticMesh(nullptr);
	rightThighComponent->SetStaticMesh(nullptr);
	rightCalfComponent->SetStaticMesh(nullptr);
}

float AMyCharacter::Getfspeed()
{
	float speed = fSnowballInitialSpeed + fAimingElapsedTime * fThrowPower;
	return speed;
}

void AMyCharacter::GetSupplyBox()
{
	// ������, ���̽���, ���� �ִ�ġ�� ȹ�� & ���� ȹ��)
	iCurrentSnowballCount = iMaxSnowballCount;
	iCurrentIceballCount = iMaxIceballCount;
	iCurrentMatchCount = iMaxMatchCount;
	bHasShotgun = true;
	UpdateUI(UICategory::CurSnowball);
	UpdateUI(UICategory::CurIceball);
	UpdateUI(UICategory::CurMatch);
	UpdateUI(UICategory::HasShotgun);

	//farmingItem->Destroy();
	SetCanFarmItem(nullptr);
}