#include "RoomActor.h"
#include "PaperTileMap.h"
#include "Net/UnrealNetwork.h"

ARoomActor::ARoomActor()
{
	// Set up replication
	SetReplicates(true);
	//SetAttributesRecoveryItemClass();
	bAlwaysRelevant = true;
	// Create the tile map component
	TileMapComponent = CreateDefaultSubobject<URoom>(TEXT("TileMapComponent"));
	TileMapComponent->SetIsReplicated(true);
	RootComponent = TileMapComponent;
	static ConstructorHelpers::FClassFinder<AAttributesRecoveryItem> ItemBPClass(TEXT("/Game/Items/StaminaRecoveryItemBP"));
	if (ItemBPClass.Succeeded())
	{
		AttributesRecoveryItemClass = ItemBPClass.Class;
	}
}

void ARoomActor::BeginPlay()
{
	TArray<int> EmptySet;
	Super::BeginPlay();
	SetActorRotation(FRotator(0.0f, 90.0f, -90.0f));
	if (!HasAuthority())
	{
		OnRepMap();
	}
}

void ARoomActor::Init(const TArray<int> &Doors, int Width, int Height, const TArray<int> &Walls, int Side, int TemplateNum)
{
	//UE_LOG(LogTemp, Warning, TEXT("In init"))
	if (HasAuthority())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Init has outh"))
		ServerGenerateMap(Doors, Width, Height, Walls, Side, TemplateNum);
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Init !has outh"))
		OnRepMap();
	}
	SetRootComponent(TileMapComponent);
}

int32 ARoomActor::GetRandomTileMapIndex() const
{
	return FMath::RandRange(0, 29);
}

void ARoomActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoomActor, RandomIndex);
	DOREPLIFETIME(ARoomActor, RandomItemLocationsX);
	DOREPLIFETIME(ARoomActor, RandomItemLocationsY);
	DOREPLIFETIME(ARoomActor, MDoors);
	DOREPLIFETIME(ARoomActor, MWalls);
	DOREPLIFETIME(ARoomActor, MWidth);
	DOREPLIFETIME(ARoomActor, MHeight);
	DOREPLIFETIME(ARoomActor, MTemplateNum);
	DOREPLIFETIME(ARoomActor, TileMapComponent);
}

void ARoomActor::ServerGenerateMap_Implementation(const TArray<int> &Doors, int Width, int Height, const TArray<int> &Walls, int Side, int TemplateNum)
{
	TArray<bool> BDoors;
	TArray<bool> BWalls;
	BWalls.Init(true, 4);
	BDoors.Init(false, 4);
	for (const int &x : Doors)
	{
		BDoors[x] = true;
	}
	for (const int &x : Walls)
	{
		BWalls[x] = false;
	}
	MDoors = BDoors;
	MWalls = BWalls;
	MWidth = Width;
	MHeight = Height;
	MTemplateNum = TemplateNum;
	TileMapComponent->Doors = BDoors;
	TileMapComponent->Walls = BWalls;
	TileMapComponent->CreateRoom(Width, Height, Side, TemplateNum);
}


void ARoomActor::OnRepMap()
{
	TileMapComponent->Doors = MDoors;
	TileMapComponent->Walls = MWalls;
	TileMapComponent->CreateRoom(MWidth, MHeight, 0, MTemplateNum);
}
