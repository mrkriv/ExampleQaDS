#include "EquipSlotsComponent.h"

UEquipSlotsComponent::UEquipSlotsComponent()
{
}


void UEquipSlotsComponent::BeginPlay()
{
	Super::BeginPlay();
}

FEquipSlot* UEquipSlotsComponent::GetSlotByName(const FName& SlotName)
{
	for (auto& slot : Slots)
	{
		if (slot.SlotName == SlotName)
			return &slot;
	}

	return NULL;
}

void UEquipSlotsComponent::Unequip(UInventoryComponent* Inventory, const FName& SlotName)
{
	auto slot = GetSlotByName(SlotName);

	if (slot == NULL)
		return;

	Inventory->Add(slot->Item, slot->Count);

	slot->Count = 0;
	slot->Item = FItem();
}

bool UEquipSlotsComponent::Equip(UInventoryComponent* Inventory, const FName& SlotName, const FVector2D& InventorySlotLocation)
{
	auto slot = GetSlotByName(SlotName);

	if (slot == NULL)
		return false;

	if (slot->Item.SlotType != slot->Type)
		return false;

	if (!(slot->Item.Size <= slot->Size))
		return false;

	if (slot->Count > 0)
	{
		Unequip(Inventory, SlotName);
	}

	auto invSlot = Inventory->RemoveByLocation(InventorySlotLocation);	
	slot->Item = invSlot.Item;
	slot->Count = invSlot.Count;

	return true;
}