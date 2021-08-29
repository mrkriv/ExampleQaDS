#pragma once

#include "CoreMinimal.h"
#include "DebugRenderSceneProxy.h"
#include "DynamicMeshBuilder.h"
#include "PrimitiveViewRelevance.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"
#include "WayPointRenderingComponent.generated.h"

class AWayPoint;
class UWayPointRenderingComponent;

class FWayPointSceneProxy final : public FDebugRenderSceneProxy
{
	friend class FWayPointDebugDrawDelegateHelper;

public:
	FWayPointSceneProxy(const UWayPointRenderingComponent* InComponent);
	~FWayPointSceneProxy() {}

	void GatherPathPoints();
	void GatherPathStep();

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, class FMeshElementCollector& Collector) const override;
	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;
	virtual uint32 GetMemoryFootprint(void) const override { return sizeof(*this) + GetAllocatedSize(); }

	uint32 GetAllocatedSize(void) const;
	SIZE_T GetTypeHash() const override;

	FORCEINLINE static bool LocationInView(const FVector& Location, const FSceneView* View)
	{
		return View->ViewFrustum.IntersectBox(Location, FVector::ZeroVector);
	}

private:
	FVector NavMeshDrawOffset;
	AWayPoint* WayPointActor;
	FDebugDrawDelegate DebugTextDrawingDelegate;
	FDelegateHandle DebugTextDrawingDelegateHandle;
	TArray<FVector> PathPoints;
	TArray<FString> PathPointFlags;

	TArray<FDynamicMeshVertex> OpenSetVerts;
	TArray<uint32> OpenSetIndices;
	TArray<FDynamicMeshVertex> ClosedSetVerts;
	TArray<uint32> ClosedSetIndices;
	FSetElementId BestNodeId;

	FVector ClosestWallLocation;
};

#if WITH_RECAST && WITH_EDITOR
class FWayPointDebugDrawDelegateHelper : public FDebugDrawDelegateHelper
{
	typedef FDebugDrawDelegateHelper Super;

public:
	virtual void InitDelegateHelper(const FDebugRenderSceneProxy* InSceneProxy) override
	{
		check(0);
	}

	void InitDelegateHelper(const FWayPointSceneProxy* InSceneProxy);

private:
	AWayPoint* WayPointActor;
	TArray<FVector> PathPoints;
	TArray<FString> PathPointFlags;
};
#endif //WITH_RECAST && WITH_EDITOR


UCLASS(hidecategories=Object)
class UWayPointRenderingComponent: public UPrimitiveComponent
{
	GENERATED_UCLASS_BODY()

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual FBoxSphereBounds CalcBounds(const FTransform &LocalToWorld) const override; 

#if WITH_RECAST && WITH_EDITOR
	virtual void CreateRenderState_Concurrent(class FRegisterComponentContext *) override;
	virtual void DestroyRenderState_Concurrent() override;

private:
	FWayPointDebugDrawDelegateHelper WayPointDebugDrawDelegateHelper;
#endif
};
