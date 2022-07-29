// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "EditorManager.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "I_Throwable.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"
#include "MySnowball.generated.h"

UCLASS()
class FINAL_PROJECT_API AMySnowball : public AActor, public II_Throwable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySnowball();

	virtual void Tick(float DeltaTime) override;

	// Throwable �������̽����� Throw �̺�Ʈ �߻� �� ȣ��
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Itf")
	void Throw(FVector Direction, float Speed);
	virtual void Throw_Implementation(FVector Direction, float Speed) override;

	// Function that is called when the projectile hits something.
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	int32 GetDamage() const{ return iDamage; };
	
	int32 GetOwnerSessionId() const { return iOwnerSessionId; };
	void SetOwnerSessionId(int32 OwnerSessionId) { iOwnerSessionId = OwnerSessionId; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	USphereComponent* collisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UStaticMeshComponent* meshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UProjectileMovementComponent* projectileMovementComponent;

	AEditorManager* editorManager;

	TArray<UDecalComponent*> paints;

private:
	UPROPERTY(VisibleAnywhere, Category = Projectile)
	int32 iDamage;	// �������� ����� ���� ������ static const float�� �Ŀ� ����

	// ������ (true - ���ư��� ���� �α� ����)
	bool bCheckTrajectory;

	int32 iOwnerSessionId;	// �����̸� ������ ĳ������ session id
};
