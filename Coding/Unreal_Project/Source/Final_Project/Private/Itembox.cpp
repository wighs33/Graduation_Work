// Fill out your copyright notice in the Description page of Project Settings.


#include "Itembox.h"

int RandomItemType()
{
	int value;
	int random = UKismetMathLibrary::RandomIntegerInRange(1, 100);
	// ���� / ��� / ���� Ȯ��
	if (random <= 50) value = ItemTypeList::Match;
	else if (random <= 75) value = ItemTypeList::Umbrella;
	else value = ItemTypeList::Bag;

	return value;
}

// Sets default values
AItembox::AItembox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!boxComponent)
	{
		boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComponent"));
		boxComponent->BodyInstance.SetCollisionProfileName(TEXT("Trigger"));
		boxComponent->SetBoxExtent(FVector(60.0f, 70.0f, 20.0f));

		RootComponent = boxComponent;
	}

	if (!meshComponentDown)
	{
		meshComponentDown = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemboxDownMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshDown(TEXT("/Game/NonCharacters/itembox_down.itembox_down"));
		if (MeshDown.Succeeded())
		{
			meshComponentDown->SetStaticMesh(MeshDown.Object);
		}
		meshComponentDown->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
		meshComponentDown->SetupAttachment(RootComponent);
	}

	if (!meshComponentUp)
	{
		meshComponentUp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemboxUpMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshUp(TEXT("/Game/NonCharacters/itembox_up.itembox_up"));
		if (MeshUp.Succeeded())
		{
			meshComponentUp->SetStaticMesh(MeshUp.Object);
		}
		meshComponentUp->SetRelativeLocation(FVector(-25.0f, 0.0f, 7.0f));
		meshComponentUp->SetupAttachment(RootComponent);
	}

	if (!itemMeshComponent)
	{
		itemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
		
		//iItemType = RandomItemType();	// ���� �����ؾ� �� �� (������ �������� ����)
		iItemType = ItemTypeList::Umbrella;	// ������

		// ����, ���� ���ҽ� �����ϸ� �ش� ����ƽ�޽÷� �����ʿ� (���� �ӽ÷� �ٸ� �޽� ���)
		static ConstructorHelpers::FObjectFinder<UStaticMesh>ItemMesh1(TEXT("/Game/NonCharacters/snowball1.snowball1"));	// ���� �ʿ�
		static ConstructorHelpers::FObjectFinder<UStaticMesh>ItemMesh2(TEXT("/Game/NonCharacters/umbrellaForItemBox.umbrellaForItemBox"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>ItemMesh3(TEXT("/Game/NonCharacters/snowdrift1.snowdrift1"));	// ���� �ʿ�

		switch (iItemType) {
		case ItemTypeList::Match:
			if (ItemMesh1.Succeeded()) { itemMeshComponent->SetStaticMesh(ItemMesh1.Object); }
			break;
		case ItemTypeList::Umbrella:
			if (ItemMesh2.Succeeded()) { itemMeshComponent->SetStaticMesh(ItemMesh2.Object); }
			itemMeshComponent->SetRelativeLocation(FVector(0.0f, 31.0f, -3.0f));
			itemMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
			itemMeshComponent->SetRelativeScale3D(FVector(0.5f, 0.25f, 0.75f));
			break;
		case ItemTypeList::Bag:
			if (ItemMesh3.Succeeded()) { itemMeshComponent->SetStaticMesh(ItemMesh3.Object); }
			break;
		default:
			break;
		}
		itemMeshComponent->SetupAttachment(RootComponent);
	}

	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AItembox::OnComponentBeginOverlap);
	boxComponent->OnComponentEndOverlap.AddDynamic(this, &AItembox::OnComponentEndOverlap);

	iItemboxState = ItemboxState::Closed;
	fSumRotation = 0.0f;
}

// Called when the game starts or when spawned
void AItembox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItembox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRotation(DeltaTime);
}

void AItembox::OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* mycharacter = Cast<AMyCharacter>(OtherActor);
	if (mycharacter)
	{
		mycharacter->SetCanFarmItem(this);
	}
}

void AItembox::OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMyCharacter* mycharacter = Cast<AMyCharacter>(OtherActor);
	if (mycharacter)
	{
		mycharacter->SetCanFarmItem(nullptr);
	}
}

void AItembox::UpdateRotation(float DeltaTime)
{
	if (iItemboxState == ItemboxState::Opening)
	{
		meshComponentUp->AddLocalRotation(FRotator(5.0f, 0.0f, 0.0f));
		fSumRotation += 5.0f;
		if (fSumRotation >= 200.0f)
		{
			iItemboxState = ItemboxState::Opened;
		}
	}
}

void AItembox::DeleteItem()
{
	itemMeshComponent->DestroyComponent();
}