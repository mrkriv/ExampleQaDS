#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.h"
#include "ItemActor.generated.h"

UCLASS(Blueprintable, autoCollapseCategories = (Actor), hideCategories = (Input))
class DIALOGPLUGINEXAMPLE_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItem ItemPrototype;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Count;

	AItemActor();

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static AItemActor* SpawnItemActor(UObject* WorldContextObject, const FItem& Prototype, int ItemCount = 1);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
