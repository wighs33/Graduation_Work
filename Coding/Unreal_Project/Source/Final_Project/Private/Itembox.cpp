// Fill out your copyright notice in the Description page of Project Settings.


#include "Itembox.h"
#include "MyCharacter.h"

// �����ۺ� ����ƽ�޽�
FString ItemMeshStringArray[] = {
	TEXT("/Game/NonCharacters/match.match"),								// ����
	TEXT("/Game/NonCharacters/umbrellaForItemBox.umbrellaForItemBox"),		// ���
	TEXT("/Game/NonCharacters/Backpack_SM.Backpack_SM")};					// ����

int AItembox::iIdCountHelper = -1;

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
		
		// ��� ������ ������ ����ƽ�޽� �ε��ؼ� ����
		for (int i = 0; i < 3; ++i)
		{
			ConstructorHelpers::FObjectFinder<UStaticMesh>ItemMesh(*(ItemMeshStringArray[i]));
			if (ItemMesh.Succeeded())
			{
				itemMeshArray.Add(ItemMesh.Object);
			}
		}
		itemMeshComponent->SetupAttachment(RootComponent);
	}

	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AItembox::OnComponentBeginOverlap);
	boxComponent->OnComponentEndOverlap.AddDynamic(this, &AItembox::OnComponentEndOverlap);

	iItemboxState = ItemboxState::Closed;
	fSumRotation = 0.0f;
	
	//iItemType = ItemTypeList::Random;
	//BeginPlay()�� �Űܰ�
	
	// �����ۺ� �޽� �׽�Ʈ��
	//SetItem(ItemTypeList::Match);
	//SetItem(ItemTypeList::Umbrella);
	//SetItem(ItemTypeList::Bag);
}

// Called when the game starts or when spawned
void AItembox::BeginPlay()
{
	Super::BeginPlay();
	
	// �����δ� �������� SetItem(������ Ÿ��) ���� �������� ������ �����ָ� ��
	// ������ - �����ϰ� ������ Ÿ�� �� �޽� ����
	
	//�ӽ÷� �����ϰ� ������ ������Ÿ���� ������ id%3���� �ٲ���
	iId = ++iIdCountHelper;
	iItemType = iId % 3;
	//UE_LOG(LogTemp, Warning, TEXT("iItemType %d"), iItemType);

	SetItem(iItemType);
	//// ������ - Ư�� ���������� ����
	//SetItem(ItemTypeList::Match);
	//SetItem(ItemTypeList::Umbrella);
	//SetItem(ItemTypeList::Bag);

	//iId = ++iIdCountHelper;
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
	iItemboxState = ItemboxState::Empty;
}

void AItembox::SetItem(int itemType)
{
	switch (itemType) {
	case ItemTypeList::Match:
		itemMeshComponent->SetStaticMesh(itemMeshArray[ItemTypeList::Match]);
		itemMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
		itemMeshComponent->SetRelativeLocation(FVector(0.0f, 30.0f, 0.0f));
		iItemType = ItemTypeList::Match;
		break;
	case ItemTypeList::Umbrella:
		itemMeshComponent->SetStaticMesh(itemMeshArray[ItemTypeList::Umbrella]);
		itemMeshComponent->SetRelativeLocation(FVector(0.0f, 31.0f, -3.0f));
		itemMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
		itemMeshComponent->SetRelativeScale3D(FVector(0.5f, 0.25f, 0.75f));
		iItemType = ItemTypeList::Umbrella;
		break;
	case ItemTypeList::Bag:
		itemMeshComponent->SetStaticMesh(itemMeshArray[ItemTypeList::Bag]);
		itemMeshComponent->SetRelativeRotation(FRotator(-90.0f, -26.56f, 656.56f));
		itemMeshComponent->SetRelativeLocation(FVector(2.0f, 21.0f, -12.0f));
		iItemType = ItemTypeList::Bag;
		break;
	case ItemTypeList::Random:
		SetItem(RandomItemType());
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Itembox set item failed"));
		break;
	}
}

int AItembox::RandomItemType()
{
	int value;
	int random = UKismetMathLibrary::RandomIntegerInRange(1, 100);
	// ���� / ��� / ���� Ȯ��	(50/25/25)
	if (random <= 50) value = ItemTypeList::Match;
	else if (random <= 75) value = ItemTypeList::Umbrella;
	else value = ItemTypeList::Bag;

	return value;
}