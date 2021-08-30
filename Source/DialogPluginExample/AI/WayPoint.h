#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WayPoint.generated.h"

class UWayPointRenderingComponent;

UCLASS(Blueprintable, autoCollapseCategories=(Actor), hideCategories=(Input))
class DIALOGPLUGINEXAMPLE_API AWayPoint : public AActor
{
	GENERATED_UCLASS_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UBillboardComponent* SpriteComponent;

	UPROPERTY()
	UWayPointRenderingComponent* RenderingComponent;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AWayPoint*> NextPoints;

	UFUNCTION(BlueprintCallable)
	AWayPoint* GetRandomNext();

#if WITH_EDITORONLY_DATA
	UBillboardComponent* GetSpriteComponent() const { return SpriteComponent; }
	UWayPointRenderingComponent* GetRenderingComponent() const { return RenderingComponent; }
#endif
};
