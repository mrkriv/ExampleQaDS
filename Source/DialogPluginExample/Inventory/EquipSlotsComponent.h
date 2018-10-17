#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipSlotsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIALOGPLUGINEXAMPLE_API UEquipSlotsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipSlotsComponent();

protected:
	virtual void BeginPlay() override;	
};
