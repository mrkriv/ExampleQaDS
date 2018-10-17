#include "EquipSlotsComponent.h"

UEquipSlotsComponent::UEquipSlotsComponent()
{
}


void UEquipSlotsComponent::BeginPlay()
{
	Super::BeginPlay();
}


FItemSlot UEquipSlotsComponent::Unequip(UInventoryComponent* Inventory, const FName& SlotName)
{
	return FItemSlot();
}

FEquipSlot UEquipSlotsComponent::Equip(UInventoryComponent* Inventory, const FName& SlotName, const FVector2D& InventorySlotLocation)
{
	return FEquipSlot();
}