// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "I_Throwable.h"
#include "Components/SphereComponent.h"
#include "MySnowball.generated.h"

UCLASS()
class FINAL_PROJECT_API AMySnowball : public AActor, public II_Throwable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySnowball();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Throwable �������̽����� Throw �̺�Ʈ �߻� �� ȣ��
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Itf")
	void Throw(FVector Direction);
	virtual void Throw_Implementation(FVector Direction) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* MeshComponent;
};
