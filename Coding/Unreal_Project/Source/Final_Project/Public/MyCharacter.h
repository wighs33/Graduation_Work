// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/Character.h"
#include "MySnowball.h"
#include "Itembox.h"
#include "Bonfire.h"
#include "MyCharacter.generated.h"

enum CharacterState {
	// ����,		����,		�����,			�����
	AnimalNormal, AnimalSlow, SnowmanNormal, SnowmanStunned
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

	bool CanSetItem();
	void SetItem(class AMyItem* NewItem);
	void SetIsFarming(bool value) { bIsFarming = value; };
	void SetCanFarmItem(AActor* item) { farmingItem = item; };

	bool GetIsInsideOfBonfire() { return bIsInsideOfBonfire; };
	void SetIsInsideOfBonfire(bool value) { bIsInsideOfBonfire = value; };
	void UpdateTemperatureState();
	//void UpdateTemperatureByMatch();
	void UpdateSpeed();
	int GetCharacterState() { return iCharacterState; };
	bool IsSnowman() { return bIsSnowman; };
	void StartStun(float waitTime);
	void EndStun(float waitTime);
	void ResetHasItems();	// ������ ������ �� ȿ���� �ʱ�ȭ
	void Attack();
	

protected:
	virtual void BeginPlay() override;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void StartFarming();
	void EndFarming();
	void UpdateFarming(float deltaTime);
	void UpdateHP();
	void ChangeSnowman();
	void WaitForStartGame();
	bool GetItem(int itemType);

public:	

	//UPROPERTY(VisibleAnywhere, Category = Item)
	//class AMyItem* CurrentItem;

	//UPROPERTY(VisibleAnywhere, Category = Item)
	//USkeletalMeshComponent* Item;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* camera;

	// ���� �տ� ����ִ� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	AMySnowball* snowball;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iSessionID;

	// ��� ĳ���� ���� & ����� �� x
	static const int iMaxHP;
	static const int iMinHP;
	int		SessionId;		// �÷��̾� ���� ���̵�
	bool	IsAlive;		// ����ִ���
	bool	IsAttacking;	// ����������


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iCurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iMaxSnowballCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iCurrentSnowballCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iMaxMatchCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 iCurrentMatchCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bHasUmbrella;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bHasBag;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AMySnowball> projectileClass;

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

	//UPROPERTY(VisibleAnywhere, Category = Temperature)
	//float fMatchDuration;	// �Ŀ� ���� Ŭ���� �����ϸ� ���� Ŭ���� ������ �ű��

	//bool match;

	UPROPERTY(VisibleAnywhere, Category = State)
	int32 iCharacterState;	// ���� ĳ������ ���� (AnimalNormal, AnimalSlow, SnowmanNormal, SnowmanStunned)

	bool bIsSnowman;	// ���� ĳ���Ͱ� ���������

	UPROPERTY(VisibleAnywhere, Category = "Data")
	APlayerController* playerController;

	// ���� �����ϴ� Ÿ�̸� �ڵ鷯
	FTimerHandle stunHandle;
};
