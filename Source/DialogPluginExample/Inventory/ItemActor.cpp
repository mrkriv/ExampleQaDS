#include "ItemActor.h"


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

