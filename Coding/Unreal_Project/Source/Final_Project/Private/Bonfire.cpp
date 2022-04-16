// Fill out your copyright notice in the Description page of Project Settings.


#include "Bonfire.h"
#include "MyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

const int ABonfire::iHealAmount = 10;
const int ABonfire::iDamageAmount = 1;

// Sets default values
ABonfire::ABonfire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!sphereComponent)
	{
		sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComponent"));
		sphereComponent->BodyInstance.SetCollisionProfileName(TEXT("Trigger"));
		sphereComponent->SetSphereRadius(500.0f);
		sphereComponent->SetHiddenInGame(false);	// ������ (��ں� ���� ���̵���)

		RootComponent = sphereComponent;
	}

	sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABonfire::OnComponentBeginOverlap);
	sphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABonfire::OnComponentEndOverlap);
	sphereComponent->OnComponentActivated.AddDynamic(this, &ABonfire::OnComponentActivated);
}

// Called when the game starts or when spawned
void ABonfire::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateOverlapCharacters();	// ó�� ������ ���� begin overlap �̺�Ʈ�� �߻����� �ʾƼ� �̷��� ó�����־�� �Ѵ�
}

// Called every frame
void ABonfire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABonfire::OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* mycharacter = Cast<AMyCharacter>(OtherActor);
	if (mycharacter)
	{	// begin overlap �ϴ� ĳ������ ü�� �����ϵ��� ����
		mycharacter->SetIsInsideOfBonfire(true);
		mycharacter->UpdateTemperatureState();
	}
}

void ABonfire::OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	// end overlap �ϴ� ĳ������ ü�� �����ϵ��� ����
	AMyCharacter* mycharacter = Cast<AMyCharacter>(OtherActor);
	if (mycharacter)
	{
		mycharacter->SetIsInsideOfBonfire(false);
		mycharacter->UpdateTemperatureState();
	}
}

void ABonfire::UpdateOverlapCharacters()
{
	TArray<AActor*> overlapActorsArray;	// overlap ���� ĳ���͵��� ���� �迭
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // overlap ������ ������Ʈ Ÿ�� - MyCharacter
	TEnumAsByte<EObjectTypeQuery> MyCharacter = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1); // ECC_GameTraceChannel1 - MyCharacter
	ObjectTypes.Add(MyCharacter);
	TArray<AActor*> emptyArray;

	// overlap ���� ĳ���͵� �迭�� ���
	UKismetSystemLibrary::ComponentOverlapActors(sphereComponent, GetActorTransform(), ObjectTypes, nullptr, emptyArray, overlapActorsArray);

	for (AActor* actor : overlapActorsArray)
	{	// overlap ���� ĳ���͵��� ü�� �����ϵ��� ����
		AMyCharacter* character = Cast<AMyCharacter>(actor);
		if (character)
		{
			character->SetIsInsideOfBonfire(true);
			character->UpdateTemperatureState();
		}
	}
}