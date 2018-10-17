#include "InventoryComponent.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

FItemSlot* UInventoryComponent::GetSlotByLocation(const FVector2D& Location)
{
	for (auto& slot : Slots)
	{
		auto rect = FBox2D(slot.Location, slot.Location + slot.Item.Size);

		if (rect.IsInside(Location))
		{
			return &slot;
		}
	}

	return NULL;
}

int UInventoryComponent::RemoveByLocation(const FVector2D& Location, int Count)
{
	for (auto i = 0; i < Slots.Num(); i++)
	{
		auto slot = Slots[i];
		auto rect = FBox2D(slot.Location, slot.Location + slot.Item.Size);

		if (rect.IsInside(Location))
		{
			auto memoryCount = slot.Count;
			if (Count < 0)
			{
				slot.Count = 0;
			}
			else 
			{
				slot.Count = FMath::Max(slot.Count - Count, 0);
			}

			if (slot.Count <= 0)
			{
				Slots.RemoveAt(i);
			}

			return memoryCount - slot.Count;
		}
	}

	return 0;
}

void UInventoryComponent::Add(const FItem& Prototype, int Count)
{
	FVector2D maxItemLocation;

	for (auto& slot : Slots)
	{
		if(slot.Item.Name == Prototype.Name)
		{
			slot.Count += Count;
			return;
		}

		maxItemLocation.X = FMath::Max(maxItemLocation.X, slot.Location.X + slot.Item.Size.X);
		maxItemLocation.Y = FMath::Max(maxItemLocation.Y, slot.Location.Y + slot.Item.Size.Y);
	}

	FItemSlot newSlot;
	newSlot.Item = Prototype;
	newSlot.Count = Count;
	newSlot.Location = maxItemLocation;

	Slots.Add(newSlot);	
}