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

/**
 * Sage world functions
 */
UCLASS()
class DIALOGPLUGINEXAMPLE_API USaveFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
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
