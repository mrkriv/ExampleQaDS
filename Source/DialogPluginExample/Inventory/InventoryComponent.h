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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryChangeSignature, class UInventoryComponent*, Inventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryChangeItemSignature, class UInventoryComponent*, Inventory, const FItemSlot&, Slot);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIALOGPLUGINEXAMPLE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FItemSlot> Slots;

public:
	UPROPERTY(BlueprintAssignable)
	FInventoryChangeSignature OnChange;

	UPROPERTY(BlueprintAssignable)
	FInventoryChangeItemSignature OnAddItem;

	UPROPERTY(BlueprintAssignable)
	FInventoryChangeItemSignature OnRemoveItem;

	UInventoryComponent();
	
	UFUNCTION(BlueprintCallable)
	FItemSlot RemoveByLocation(const FVector2D& Location, int Count = -1);

	UFUNCTION(BlueprintCallable)
	bool Move(const FVector2D& LocationDist, const FVector2D& LocationSource, int Count);

	UFUNCTION(BlueprintCallable)
	bool Move(const FVector2D& LocationDist, const FVector2D& LocationSource, int Count, UInventoryComponent* DistInventory);

	UFUNCTION(BlueprintCallable)
	void Add(const FItem& Prototype, int Count = 1);

	UFUNCTION(BlueprintCallable)
	void Remove(const FItem& Prototype, int Count = 1);

	UFUNCTION(BlueprintPure)
	bool CheckCount(const FItem& Prototype, int Count = 1);

	UFUNCTION(BlueprintPure)
	TArray<FItemSlot> GetItems() const;

	UFUNCTION(BlueprintPure)
	float GetWeight() const;

	UFUNCTION(BlueprintPure)
	float GetCost() const;

protected:
	virtual void BeginPlay() override;	

private:
	FItemSlot* GetSlotByLocation(const FVector2D& Location);
	TArray<FItemSlot*> GetSlotsByLocation(const FVector2D& Location, const FVector2D& Size);
};
