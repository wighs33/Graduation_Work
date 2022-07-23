// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/Character.h"
#include "MySnowball.h"
#include "Iceball.h"
#include "Itembox.h"
#include "Bonfire.h"
#include "ClientSocket.h"
#include "SnowballBomb.h"	
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Tornado.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "MyCharacter.generated.h"

enum CharacterState {
	// ����,		����,			����,		�����,			�����
	AnimalNormal, AnimalSlow, AnimalStunned, SnowmanNormal, SnowmanStunned
};

enum UICategory {
	HP, CurSnowball, CurIceball, CurMatch, MaxSnowIceballAndMatch, HasUmbrella, HasBag, HasShotgun, IsFarmingSnowdrift, SnowdriftFarmDuration, 
	SelectedItem, SelectedProjectile, SelectedWeapon, AllOfUI
};

enum Weapon {
	Hand, Shotgun
};

enum Projectile {
	Snowball, Iceball
};

// �Ŀ� Umbrella Ŭ���� ���� �����ؼ� �и��ؾ��� (�ӽ�)
enum UmbrellaState {
	UmbClosed, UmbOpening, UmbOpened, UmbClosing
};

UCLASS()
class FINAL_PROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = GamePlay)
	void ReleaseSnowball();
	void SendReleaseBullet(int bullet);
	void SendCancelAttack(int bullet);

	void ReleaseIceball();

	void SendSpawnSnowballBomb();


	void SetIsFarming(bool value) { bIsFarming = value; };
	bool GetIsFarming() { return bIsFarming; };
	void SetCanFarmItem(AActor* item) { farmingItem = item; };
	bool GetItem(int itemType);

	bool GetIsInsideOfBonfire() { return bIsInsideOfBonfire; };
	void SetIsInsideOfBonfire(bool value) { bIsInsideOfBonfire = value; };
	void UpdateTemperatureState();
	void UpdateSpeed();
	int GetCharacterState() { return iCharacterState; };
	bool IsSnowman() { return bIsSnowman; };

	UFUNCTION(BlueprintCallable, Category = GamePlay)
	void StartStun(float waitTime);
	UFUNCTION(BlueprintCallable, Category = GamePlay)
	void EndStun(float waitTime);

	void ResetHasItems();	// ������ ������ �� ȿ���� �ʱ�ȭ
	void Attack();
	void ReleaseAttack();      //ȣ��� ������ �޼��� ����
	void Cancel_SnowBallAttack(); //�������� ������ ������
	void Cancel_IceBallAttack(); //�������� ������ ������

	void SnowBallAttack();    // �Ϲ� ����
	void ShotgunAttack(); //���� ����
	void IceballAttack();    // ���̽��� ����
	void ShowProjectilePath();
	void HideProjectilePath();

	void ChangeSnowman();
	void ChangeAnimal();	// ĳ���͸� ����ȭ (��Ȱ)
	void SetCharacterMaterial(int Id = 0);	// ĳ���� ���� ����, ����->����� ��Ƽ���� ����
	void UpdateUI(int uiCategory, float farmDuration=0.0f);

	void StartFarming();
	void EndFarming();

	void SelectMatch();
	void SelectUmbrella();
	void UseSelectedItem();
	void ReleaseRightMouseButton();


	UFUNCTION(BlueprintCallable, Category = GamePlay)
	bool GetIsInTornado() { return bIsInTornado; };
	UFUNCTION(BlueprintCallable, Category = GamePlay)
	void SetIsInTornado(bool value) { bIsInTornado = value; };

	void ShowShotgun();
	void HideShotgun();
	void SpawnSnowballBomb();

	void StartUmbrella();
	void ShowUmbrella();
	void HideUmbrella();
	void OpenUmbrellaAnim();
	void CloseUmbrellaAnim();


	int GetUmbrellaState() { return iUmbrellaState; };
	void SetUmbrellaState(int umbrellaState) { iUmbrellaState = umbrellaState; };
	void ReleaseUmbrella();

	void GetBag();

	void GetOnOffJetski();
	void GetOnJetski();
	void GetOffJetski();
	float Getfspeed();

	void Cheat_Teleport1();
	void Cheat_Teleport2();
	void Cheat_Teleport3();
	void Cheat_Teleport4();
	void Cheat_IncreaseHP();
	void Cheat_DecreaseHP();
	void Cheat_IncreaseSnowball();

	UFUNCTION()
	class UMyAnimInstance* GetAnim() const { return myAnim; }

	void init_Socket();

	void FreezeHead();
	void FreezeAnimation(TArray<UStaticMeshComponent*> bones, int& frame, bool& end);
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void UpdateFarming(float deltaTime);
	void UpdateHP();
	
	bool GetIsSnowman() { return bIsSnowman; };
	void WaitForStartGame();	// ���� ���� �� ���

	void UpdateZByTornado();
	void UpdateControllerRotateByTornado();

	void ChangeWeapon();
	void ChangeProjectile();

	void SetAimingCameraPos();
	void UpdateAiming();

	void UpdateJetski();

	void SettingHead();

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* springArm2;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	AActor* aimingCameraPos;

	// jetski ž�� �� ���� spring arm, camera
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* springArm3;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* camera3;

	// ���� �տ� ����ִ� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	AMySnowball* snowball;

	// ���� �տ� ����ִ� ���̽���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	AIceball* iceball;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Data", Meta = (AllowPrivateAccess = true))
	int32 iSessionId;

	// ��� ĳ���� ���� & ����� �� x
	static const int iMaxHP;
	static const int iMinHP;
	static const int iBeginSlowHP;	// ĳ���Ͱ� ���ο� ���°� �Ǳ� �����ϴ� hp

	bool	IsAlive;		// ����ִ���
	bool	IsAttacking;	// ����������


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iCurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iMaxSnowballCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iCurrentSnowballCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iMaxIceballCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iCurrentIceballCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iMaxMatchCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iCurrentMatchCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bHasUmbrella;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bHasBag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bHasShotgun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	bool bIsSnowman;	// ���� ĳ���Ͱ� ���������

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* dynamicMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* bearMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTexture*> bearTextureArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* snowmanMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iSelectedItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool rotateCont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iSelectedWeapon;	// ���� ���õ� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iSelectedProjectile;	// ���� ���õ� �߻�ü

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UStaticMeshComponent* shotgunMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> snowballBombDirArray;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	USkeletalMeshComponent* umbrella1MeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	USkeletalMeshComponent* umbrella2MeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UBoxComponent* umb1CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UBoxComponent* umb2CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UNiagaraSystem* smokeNiagara;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UStaticMeshComponent* bagMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UStaticMeshComponent* jetskiMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bIsRiding;		// jetski ž�� ������

	bool bReleaseUmbrella;
	
	ATornado* overlappedTornado;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	class AMyPlayerController* localPlayerController;

	bool bFreeze;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UStaticMeshComponent* head1;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UStaticMeshComponent* head2;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UStaticMeshComponent* head3;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UStaticMeshComponent* head4;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UStaticMeshComponent* head5;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UStaticMeshComponent* head6;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UStaticMeshComponent* head7;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UStaticMeshComponent* head8;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UStaticMeshComponent* head9;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
		UStaticMeshComponent* head10;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
		UStaticMeshComponent* head11;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
		UStaticMeshComponent* head12;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
		UStaticMeshComponent* head13;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
		UStaticMeshComponent* head14;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
		UStaticMeshComponent* head15;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
		UStaticMeshComponent* head16;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
		UStaticMeshComponent* head17;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
		UStaticMeshComponent* head18;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
		UStaticMeshComponent* head19;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
		UStaticMeshComponent* head20;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
		UStaticMeshComponent* head21;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
		UStaticMeshComponent* head22;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
		UStaticMeshComponent* head23;

	TArray<UStaticMeshComponent*> heads;
	int32 iHeadFrame;
	bool bHeadAnimEnd = false;
	FTimerHandle HeadHandle;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AMySnowball> projectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ASnowballBomb> shotgunProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AIceball> iceballClass;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool isAttacking;

	UPROPERTY()
	class UMyAnimInstance* myAnim;

	UPROPERTY()
	class USkeletalMesh* bear;

	UPROPERTY()
	class USkeletalMesh* snowman;

	UPROPERTY()
	TSubclassOf<class UAnimInstance> bearAnim;

	UPROPERTY()
	TSubclassOf<class UAnimInstance> snowmanAnim;

	UPROPERTY()
	class UUmbrellaAnimInstance* umb1AnimInst;

	UPROPERTY()
	class UUmbrellaAnimInstance* umb2AnimInst;

	UPROPERTY()
	TSubclassOf<class UAnimInstance> umbrella1Anim;

	UPROPERTY()
	TSubclassOf<class UAnimInstance> umbrella2Anim;

	// ĳ���Ͱ� �������� Ʈ���� �ȿ� ���ͼ� ���� �Ĺ��� �� �ִ� ������
	UPROPERTY(VisibleAnywhere, Category = Farm)
	AActor* farmingItem;

	// ���� �Ĺ�������
	UPROPERTY(VisibleAnywhere, Category = Farm)
	bool bIsFarming;

	// ���� ��ں� �������� �ܺ�����
	UPROPERTY(VisibleAnywhere, Category = State)
	bool bIsInsideOfBonfire;

	// �ʴ� ü�� ������Ű�� Ÿ�̸� �ڵ鷯
	FTimerHandle temperatureHandle;

	UPROPERTY(VisibleAnywhere, Category = State)
	int32 iCharacterState;	// ���� ĳ������ ���� (AnimalNormal, AnimalSlow, SnowmanNormal, SnowmanStunned)

	UPROPERTY(VisibleAnywhere, Category = "Data")
	APlayerController* playerController;

	// ���� �����ϴ� Ÿ�̸� �ڵ鷯
	FTimerHandle stunHandle;

	bool bIsInTornado;

	int32 iUmbrellaState;

	USplineComponent* projectilePath;
	TArray<USplineMeshComponent*> splineMeshComponents;
	USceneComponent* projectilePathStartPos;

	bool bIsAiming;
	float fAimingElapsedTime;

	UPROPERTY(VisibleAnywhere, Category = Jetski)
	TSubclassOf<class AJetski> jetskiClass;

	UAnimationAsset* driveAnimAsset;
};
