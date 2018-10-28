#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemSlotType : uint8
{
	NotEquip,

	Weapon1,
	Weapon2,
	Weapon3,
	Weapon4,
	Device,

	Body,
	Head,
};

USTRUCT(BlueprintType)
struct DIALOGPLUGINEXAMPLE_API FItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemSlotType SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;
};
