// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Bonfire.generated.h"

UCLASS()
class FINAL_PROJECT_API ABonfire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABonfire();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void UpdateOverlapCharacters(); // overlap ���� ĳ���͸� ã�Ƽ� ü�� ���� ���¸� ����

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// ���� - ȸ����, �ܺ� - ���ط�
	static const int iHealAmount;
	static const int iDamageAmount;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	USphereComponent* sphereComponent;
};
