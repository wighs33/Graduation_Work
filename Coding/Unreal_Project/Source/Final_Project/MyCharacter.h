// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MySnowball.h"
#include "MyCharacter.generated.h"

UCLASS()
class FINAL_PROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool CanSetItem();
	void SetItem(class AMyItem* NewItem);

	void SetIsFarming(bool value) { bIsFarming = value; };
	void SetCanFarmItem(AActor* item) { canFarmItem = item; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AMySnowball> ProjectileClass;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	void Attack();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable, Category = GamePlay)
	void ReleaseSnowball();

	void StartFarming();
	void EndFarming();
	void UpdateFarming(float deltaTime);

public:	
	UPROPERTY(VisibleAnywhere, Category = Item)
	class AMyItem* CurrentItem;

	UPROPERTY(VisibleAnywhere, Category = Item)
	USkeletalMeshComponent* Item;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UMyCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay) 
	FVector MuzzleOffset; 

	int		_SessionId;		// �÷��̾� ���� ���̵�

	// ���� �տ� ����ִ� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	AMySnowball* Snowball;

	// ���� ������ ������ ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int iSnowballCount;

	// ���� ������ ������ �ִ� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int iSnowballMaxCount;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY()
	class UMyAnimInstance* MyAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

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
	AActor* canFarmItem;

	// ���� �Ĺ�������
	UPROPERTY(VisibleAnywhere, Category = Farm)
	bool bIsFarming;
};
