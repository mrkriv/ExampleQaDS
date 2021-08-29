#include "WayPoint.h"
#include "WayPointRenderingComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NavigationSystem.h"
#include "Components/BillboardComponent.h"
#include "Engine/Texture2D.h"
#include "NavLinkCustomComponent.h"
#include "AI/NavigationSystemHelpers.h"
#include "VisualLogger/VisualLogger.h"

AWayPoint::AWayPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PositionComponent"));
	RootComponent = SceneComponent;

	SetHidden(true);

#if WITH_EDITOR
	RenderingComponent = CreateDefaultSubobject<UWayPointRenderingComponent>(TEXT("RenderingComponent"));
	RenderingComponent->PrimaryComponentTick.bCanEverTick = false;

	SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	if (!IsRunningCommandlet() && (SpriteComponent != NULL))
	{
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture;
			FName ID_Decals;
			FText NAME_Decals;
			FConstructorStatics()
                : SpriteTexture(TEXT("/Engine/EditorResources/AI/S_NavLink"))
                , ID_Decals(TEXT("Navigation"))
                , NAME_Decals(NSLOCTEXT("SpriteCategory", "Navigation", "Navigation"))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;

		SpriteComponent->Sprite = ConstructorStatics.SpriteTexture.Get();
		SpriteComponent->SetRelativeScale3D( FVector(0.5f, 0.5f, 0.5f));
		SpriteComponent->bHiddenInGame = true;
		SpriteComponent->SetVisibility(true);
		SpriteComponent->SpriteInfo.Category = ConstructorStatics.ID_Decals;
		SpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Decals;
		SpriteComponent->SetupAttachment(RootComponent);
		SpriteComponent->SetAbsolute(false, false, true);
		SpriteComponent->bIsScreenSizeScaled = true;
	}
#endif
}

AWayPoint* AWayPoint::GetRandomNext()
{
	for (auto next : NextPoints)
	{
		return next;
	}

	return NULL;
}
