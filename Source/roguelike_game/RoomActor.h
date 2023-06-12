// Include necessary headers
#include "GameFramework/Actor.h"
#include "PaperTileMapComponent.h"
#include "Items/AttributesRecoveryItem.h"
#include "Room.h"
#include "RoomActor.generated.h"

UCLASS()
class ROGUELIKE_GAME_API ARoomActor : public AActor
{
	GENERATED_BODY()

public:
	// Constructor
	ARoomActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// The tile map component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRepMap, Category = "Room", meta = (AllowPrivateAccess = "true"))
	URoom* TileMapComponent;

	UPROPERTY(Replicated)
	int32 RandomIndex;

	UPROPERTY(Replicated)
	TArray<int> RandomItemLocationsX;

	UPROPERTY(Replicated)
	TArray<int> RandomItemLocationsY;

	UPROPERTY(Replicated)
	TArray<bool> MDoors;

	UPROPERTY(Replicated)
	TArray<bool> MWalls;

	UPROPERTY(Replicated)
	int MWidth;

	UPROPERTY(Replicated)
	int MHeight;

	UPROPERTY(Replicated)
	int MTemplateNum;

	// Random number for tile map selection
	UFUNCTION()
	int32 GetRandomTileMapIndex() const;

public:
	// Replication for tile map changes
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void ServerGenerateMap(const TArray<int> &Doors, int Width, int Height, const TArray<int> &Walls, int Side, int TemplateNum);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGenerator, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AAttributesRecoveryItem> AttributesRecoveryItemClass;

	UFUNCTION()
	void OnRepMap();

	UFUNCTION()
	void Init(const TArray<int> &Doors, int Width, int Height, const TArray<int> &Walls, int Side, int TemplateNum);
};
