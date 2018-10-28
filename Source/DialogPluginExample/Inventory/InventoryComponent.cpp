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

		if ((Location.X >= rect.Min.X) && (Location.X <= rect.Max.X) && (Location.Y >= rect.Min.Y) && (Location.Y <= rect.Max.Y))
		{
			return &slot;
		}
	}

	return NULL;
}

TArray<FItemSlot*> UInventoryComponent::GetSlotsByLocation(const FVector2D& Location, const FVector2D& Size)
{
	TArray<FItemSlot*> result;

	for (auto& slot : Slots)
	{
		auto rect = FBox2D(slot.Location, slot.Location + slot.Item.Size);

		if ((Location.X >= rect.Min.X) && (Location.X <= rect.Max.X) && (Location.Y >= rect.Min.Y) && (Location.Y <= rect.Max.Y))
		{
			result.Add(&slot);
		}
	}

	return result;
}

FItemSlot UInventoryComponent::RemoveByLocation(const FVector2D& Location, int Count)
{
	if (Count <= 0)
		return FItemSlot();

	for (auto i = 0; i < Slots.Num(); i++)
	{
		auto slot = Slots[i];
		auto rect = FBox2D(slot.Location, slot.Location + slot.Item.Size);

		if ((Location.X >= rect.Min.X) && (Location.X <= rect.Max.X) && (Location.Y >= rect.Min.Y) && (Location.Y <= rect.Max.Y))
		{
			auto slotCopy = slot;
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

			slotCopy.Count -= slot.Count;

			OnChange.Broadcast(this);
			OnRemoveItem.Broadcast(this, slotCopy);

			return slotCopy;
		}
	}

	return FItemSlot();
}

void UInventoryComponent::Add(const FItem& Prototype, int Count)
{
	if (Count <= 0)
		return;

	auto maxItemLocation = FVector2D::ZeroVector;

	for (auto& slot : Slots)
	{
		if(slot.Item.Name == Prototype.Name)
		{
			slot.Count += Count;

			OnChange.Broadcast(this);
			OnAddItem.Broadcast(this, slot);
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

	OnChange.Broadcast(this);
	OnAddItem.Broadcast(this, newSlot);
}

bool UInventoryComponent::Move(const FVector2D& LocationDist, const FVector2D& LocationSource, int Count)
{
	return Move(LocationDist, LocationSource, Count, this);
}

bool UInventoryComponent::Move(const FVector2D& LocationDist, const FVector2D& LocationSource, int Count, UInventoryComponent* DistInventory)
{
	if (Count <= 0)
		return false;

	auto source = GetSlotByLocation(LocationSource);
	if (source == NULL)
		return false;

	auto dist = DistInventory->GetSlotByLocation(LocationDist);
	if (dist != NULL)
		dist->Location = LocationSource;

	source->Location = LocationDist;

	OnChange.Broadcast(this);
	if (DistInventory != this)
		DistInventory->OnChange.Broadcast(DistInventory);

	return true;
}

void UInventoryComponent::Remove(const FItem& Prototype, int Count)
{
	if (Count <= 0)
		return;

	FItemSlot slotForEvent;
	slotForEvent.Item = Prototype;
	slotForEvent.Count = Count;

	for (auto i = 0; i < Slots.Num(); i++)
	{
		auto slot = Slots[i];

		if (slot.Item.Name == Prototype.Name)
		{
			auto delta = FMath::Max(slot.Count - Count, 0);

			slot.Count -= delta;
			Count -= delta;

			if (slot.Count == 0)
			{
				Slots.RemoveAt(i--);
			}

			if (Count == 0)
			{
				slotForEvent.Location = slot.Location;
				break;
			}
		}
	}

	slotForEvent.Count -= Count;

	OnRemoveItem.Broadcast(this, slotForEvent);
	OnChange.Broadcast(this);
}

bool UInventoryComponent::CheckCount(const FItem& Prototype, int Count)
{
	if (Count <= 0)
		return true;

	auto realCount = 0;
	for (auto& slot : Slots)
	{
		if (slot.Item.Name == Prototype.Name)
		{
			Count -= slot.Count;

			if (Count <= 0)
				return true;
		}
	}

	return false;
}

TArray<FItemSlot> UInventoryComponent::GetItems() const
{ 
	return Slots; 
}

float UInventoryComponent::GetWeight() const
{
	auto result = 0.f;

	for (auto& slot : Slots)
	{
		result += slot.Item.Weight;
	}

	return result;
}

float UInventoryComponent::GetCost() const
{
	auto result = 0.f;

	for (auto& slot : Slots)
	{
		result += slot.Item.Cost;
	}

	return result;
}