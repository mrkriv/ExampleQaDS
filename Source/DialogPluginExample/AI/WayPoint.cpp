#include "WayPoint.h"
#include "WayPointRenderingComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "NavigationSystem.h"
#include "NavigationSystemTypes.h"
#include "Components/BillboardComponent.h"
#include "Engine/Texture2D.h"
#include "NavLinkCustomComponent.h"
#include "NavLinkRenderingComponent.h"
#include "NavAreas/NavArea_Default.h"
#include "AI/NavigationSystemHelpers.h"
#include "VisualLogger/VisualLogger.h"
#include "NavigationOctree.h"
#include "ObjectEditorUtils.h"

AWayPoint::AWayPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PositionComponent"));
	RootComponent = SceneComponent;
	
	bHidden = true;

#if WITH_EDITOR
	RenderingComponent = CreateDefaultSubobject<UWayPointRenderingComponent>(TEXT("RenderingComponent"));
	RenderingComponent->PostPhysicsComponentTick.bCanEverTick = false;

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
		SpriteComponent->RelativeScale3D = FVector(0.5f, 0.5f, 0.5f);
		SpriteComponent->bHiddenInGame = true;
		SpriteComponent->bVisible = true;
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
