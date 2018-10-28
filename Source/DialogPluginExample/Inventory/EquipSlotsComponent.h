#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"
#include "InventoryComponent.h"
#include "EquipSlotsComponent.generated.h"

USTRUCT(Blueprintable)
struct DIALOGPLUGINEXAMPLE_API FEquipSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SlotName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemSlotType Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D Location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D Size;

	UPROPERTY(BlueprintReadWrite)
	FItem Item;

	UPROPERTY(BlueprintReadWrite)
	int Count;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIALOGPLUGINEXAMPLE_API UEquipSlotsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipSlotsComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FEquipSlot> Slots;
	
	FEquipSlot* GetSlotByName(const FName& SlotName);

	UFUNCTION(BlueprintCallable)
	void Unequip(UInventoryComponent* Inventory, const FName& SlotName);

	UFUNCTION(BlueprintCallable)
	bool Equip(UInventoryComponent* Inventory, const FName& SlotName, const FVector2D& InventorySlotLocation);

protected:
	virtual void BeginPlay() override;	
};
