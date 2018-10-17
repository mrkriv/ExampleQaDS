#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"
#include "InventoryComponent.generated.h"

USTRUCT(Blueprintable)
struct DIALOGPLUGINEXAMPLE_API FItemSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItem Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Location;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIALOGPLUGINEXAMPLE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FItemSlot> Slots;

public:
	UInventoryComponent();

	FItemSlot* GetSlotByLocation(const FVector2D& Location);
	int RemoveByLocation(const FVector2D& Location, int Count = -1);
	void Add(const FItem& Prototype, int Count = 1);
	//void Remove(const FItem& Prototype, int Count = 1);

protected:
	virtual void BeginPlay() override;	
};
