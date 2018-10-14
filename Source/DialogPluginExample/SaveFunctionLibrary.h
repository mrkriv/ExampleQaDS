#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SaveFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct DIALOGPLUGINEXAMPLE_API FSaveDescriptor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime CreateDate;
};

UENUM(BlueprintType)
enum class FSavePawnControllerType : uint8
{
	None,
	Default,
	Player
};

USTRUCT()
struct DIALOGPLUGINEXAMPLE_API FSaveComponentArchive
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name;

	UPROPERTY()
	TArray<uint8> Data;

	friend FArchive& operator<<(FArchive& Ar, FSaveComponentArchive& A);
};

USTRUCT()
struct DIALOGPLUGINEXAMPLE_API FSavePawnArchive
{
	GENERATED_BODY()

	UPROPERTY()
	FSavePawnControllerType Controller;

	UPROPERTY()
	FRotator ControllerRotation;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	TArray<uint8> BrainDump;

	UPROPERTY()
	TArray<uint8> SkeletalMeshDump;

	friend FArchive& operator<<(FArchive& Ar, FSavePawnArchive& A);
};

USTRUCT()
struct DIALOGPLUGINEXAMPLE_API FSaveActorArchive
{
	GENERATED_BODY()

	UPROPERTY()
	UClass* Class;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FString Name;

	UPROPERTY()
	TArray<uint8> Data;

	UPROPERTY()
	TArray<FSavePawnArchive> PawnArchive;

	UPROPERTY()
	TArray<FSaveComponentArchive> Components;

	friend FArchive& operator<<(FArchive& Ar, FSaveActorArchive& A);
};

USTRUCT()
struct DIALOGPLUGINEXAMPLE_API FSaveArchive 
{
	GENERATED_BODY()

	UPROPERTY()
	int16 Version;

	UPROPERTY()
	TArray<FSaveActorArchive> Actors;

	UPROPERTY()
	TArray<uint8> Quest;

	UPROPERTY()
	TArray<uint8> StoryKeys;

	friend FArchive& operator<<(FArchive& Ar, FSaveArchive& A);
};

class AActor;

/**
 * Save world functions
 */
UCLASS()
class DIALOGPLUGINEXAMPLE_API USaveFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static bool CanSaveActor(AActor* actor);
	static void SaveActor(FSaveArchive& save, AActor* actor);
	static AActor* LoadActor(const FSaveActorArchive& actArh, UWorld* world);
public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static bool Save(const UObject* WorldContextObject, const FString& saveName);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static bool Load(const UObject* WorldContextObject, const FString& saveName);

	UFUNCTION(BlueprintPure)
	static TArray<FSaveDescriptor> GetSaveDescriptors();

	UFUNCTION(BlueprintPure)
	static FString GetFullPathToSave(const FString& saveName);

	UFUNCTION(BlueprintCallable)
	static bool RemoveSave(const FString& saveName);
};
