#include "WayPointRenderingComponent.h"
#include "WayPoint.h"
#include "Engine/Canvas.h"
#include "SceneManagement.h"

static const FColor NavMeshRenderColor_OpenSet(255, 128, 0, 255);
static const FColor NavMeshRenderColor_ClosedSet(255, 196, 0, 255);
static const uint8 NavMeshRenderAlpha_Modifed = 255;
static const uint8 NavMeshRenderAlpha_NonModified = 64;

SIZE_T FWayPointSceneProxy::GetTypeHash() const
{
	static size_t UniquePointer;
	return reinterpret_cast<size_t>(&UniquePointer);
}

FWayPointSceneProxy::FWayPointSceneProxy(const UWayPointRenderingComponent* InComponent)
	: FDebugRenderSceneProxy(InComponent)
{
	if (InComponent == NULL)
		return;

	WayPointActor = Cast<AWayPoint>(InComponent->GetOwner());
	if (WayPointActor == NULL)
	{
		return;
	}

	GatherPathPoints();
	GatherPathStep();
}

void FWayPointSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
	for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
	{
		if (VisibilityMap & (1 << ViewIndex))
		{
			auto View = Views[ViewIndex];
			auto PDI = Collector.GetPDI(ViewIndex);

			if (WayPointActor != NULL)
			{
				auto ActorLocation = WayPointActor->GetActorLocation();

				for (auto next : WayPointActor->NextPoints)
				{
					if(next != NULL)
						PDI->DrawLine(next->GetActorLocation(), ActorLocation, FColorList::White, SDPG_World, 1.5);
				}

				//GetSphereMesh(ProjectedLocation, BoxExtent, 10, 7, ColoredMeshInstance, SDPG_World, false, ViewIndex, Collector);
			}
		}
	}
}

void FWayPointSceneProxy::GatherPathPoints()
{
	/*if (WayPointActor && WayPointActor->LastPath.IsValid())
	{
		for (int32 PointIndex = 0; PointIndex < WayPointActor->LastPath->GetPathPoints().Num(); PointIndex++)
		{
			PathPoints.Add(WayPointActor->LastPath->GetPathPoints()[PointIndex].Location);
			PathPointFlags.Add(FString::Printf(TEXT("%d-%d"), PointIndex, FNavMeshNodeFlags(WayPointActor->LastPath->GetPathPoints()[PointIndex].Flags).AreaFlags));
		}
	}*/
}

void FWayPointSceneProxy::GatherPathStep()
{
	OpenSetVerts.Reset();
	ClosedSetVerts.Reset();
	OpenSetIndices.Reset();
	ClosedSetIndices.Reset();
	BestNodeId = FSetElementId();
}

FPrimitiveViewRelevance FWayPointSceneProxy::GetViewRelevance(const FSceneView* View) const
{
	FPrimitiveViewRelevance Result;
	Result.bDrawRelevance = IsShown(View);
	Result.bDynamicRelevance = true;
	// ideally the TranslucencyRelevance should be filled out by the material, here we do it conservative
	Result.bSeparateTranslucencyRelevance = Result.bNormalTranslucencyRelevance = IsShown(View) && GIsEditor;
	return Result;
}

uint32 FWayPointSceneProxy::GetAllocatedSize(void) const
{
	return PathPoints.GetAllocatedSize()
		+ PathPointFlags.GetAllocatedSize()
		+ OpenSetVerts.GetAllocatedSize() + OpenSetIndices.GetAllocatedSize()
		+ ClosedSetVerts.GetAllocatedSize() + ClosedSetIndices.GetAllocatedSize();

}

#if WITH_RECAST && WITH_EDITOR
void FWayPointDebugDrawDelegateHelper::InitDelegateHelper(const FWayPointSceneProxy* InSceneProxy)
{
	Super::InitDelegateHelper(InSceneProxy);

	PathPoints.Reset();
	PathPoints.Append(InSceneProxy->PathPoints);
	PathPointFlags.Reset();
	PathPointFlags.Append(InSceneProxy->PathPointFlags);
	WayPointActor = InSceneProxy->WayPointActor;
}
#endif


UWayPointRenderingComponent::UWayPointRenderingComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FPrimitiveSceneProxy* UWayPointRenderingComponent::CreateSceneProxy()
{
	auto SceneProxy2 = new FWayPointSceneProxy(this);
#if WITH_RECAST && WITH_EDITOR
	WayPointDebugDrawDelegateHelper.InitDelegateHelper(SceneProxy2);
	WayPointDebugDrawDelegateHelper.ReregisterDebugDrawDelgate();
#endif
	return SceneProxy2;
}

FBoxSphereBounds UWayPointRenderingComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBox BoundingBox(ForceInit);

	auto wayPointActor = Cast<AWayPoint>(GetOwner());
	if (wayPointActor != NULL)
	{
		BoundingBox += wayPointActor->GetActorLocation();

		for (auto next : wayPointActor->NextPoints)
		{
			if (next != NULL)
				BoundingBox += next->GetActorLocation();
		}
	}

	return FBoxSphereBounds(BoundingBox);
}

#if WITH_RECAST && WITH_EDITOR
void UWayPointRenderingComponent::CreateRenderState_Concurrent(FRegisterComponentContext * ctx)
{
	Super::CreateRenderState_Concurrent(ctx);
	WayPointDebugDrawDelegateHelper.RegisterDebugDrawDelgate();
}

void UWayPointRenderingComponent::DestroyRenderState_Concurrent()
{
	WayPointDebugDrawDelegateHelper.UnregisterDebugDrawDelgate();
	Super::DestroyRenderState_Concurrent();
}

#endif