#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Way.generated.h"

class UWayPoint;

UCLASS()
class DIALOGPLUGINEXAMPLE_API AWay : public AActor
{
	GENERATED_BODY()
	
public:	
	AWay();

protected:
	virtual void BeginPlay() override;
};
