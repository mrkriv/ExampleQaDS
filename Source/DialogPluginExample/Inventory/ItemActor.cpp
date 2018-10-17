#include "ItemActor.h"
#include "Engine/World.h"

AItemActor* AItemActor::SpawnItemActor(UObject* WorldContextObject, const FItem& Prototype, int AddCount)
{
	if (AddCount < 1)
		return NULL;

	FActorSpawnParameters params;
	params.Name = Prototype.Name;

	auto world = WorldContextObject->GetWorld();
	auto actor = world->SpawnActor<AItemActor>();
	actor->ItemPrototype = Prototype;
	actor->Count = AddCount;

	return actor;
}

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

