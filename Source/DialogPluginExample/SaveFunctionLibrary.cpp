#include "SaveFunctionLibrary.h"
#include "DialogPluginExample.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Engine/Classes/Engine/World.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

#include "StoryInformationManager.h"
#include "QuestProcessor.h"

#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "BrainComponent.h"
#include "AIController.h"

const FString SaveFileExt = TEXT(".sav");

FString USaveFunctionLibrary::GetFullPathToSave(const FString& saveName)
{
	return FPaths::ProjectSavedDir() / saveName + SaveFileExt;
}

bool USaveFunctionLibrary::SaveGame(UObject* WorldContextObject, const FString& saveName)
{
	auto world = WorldContextObject->GetWorld();
	FSaveArchive save;
	save.Version = 01'00'00;
	save.Quest = UQuestProcessor::GetQuestProcessor(WorldContextObject)->SaveToBinary();
	save.StoryKeys = UStoryKeyManager::GetStoryKeyManager(WorldContextObject)->SaveToBinary();

	for (auto level : world->GetLevels())
	{
		UE_LOG(GameLog, Log, TEXT("Save level %s"), *level->GetName());

		for (auto actor : level->Actors)
		{
			if (CanSaveActor(actor))
			{
				SaveActor(save, actor);
			}
		}
	}

	TArray<uint8> data;
	auto ar = FMemoryWriter(data);
	ar << save;
	
	auto fullPath = GetFullPathToSave(saveName);
	FFileHelper::SaveArrayToFile(data, *fullPath);

	return true;
}

bool USaveFunctionLibrary::CanSaveActor(class AActor* actor)
{
	if (actor == NULL)
		return false;

	if (actor->IsActorBeingDestroyed())
		return false;

	if (actor->IsRootComponentStatic())
		return false;

	TArray<UClass*> classWhitelist;
	classWhitelist.Add(APawn::StaticClass());

	for (auto cl : classWhitelist)
	{
		if (actor->IsA(cl))
		{
			return true;
		}
	}

	return false;
}

void USaveFunctionLibrary::SaveActor(FSaveArchive& save, AActor* actor)
{
	FSaveActorArchive actArh;

	actArh.Class = actor->GetClass();
	actArh.Transform = actor->GetTransform();
	actArh.Name = actor->GetName();

	FMemoryWriter buffAr(actArh.Data, true);
	buffAr.ArIsSaveGame = true;

	FObjectAndNameAsStringProxyArchive wrapper(buffAr, false);
	actor->Serialize(wrapper);

	if (auto character = Cast<ACharacter>(actor))
	{
		FSavePawnArchive pawnArchive;

		if (character->Controller)
		{
			auto playerController = Cast<APlayerController>(character->Controller);

			pawnArchive.Controller = playerController ? FSavePawnControllerType::Player : FSavePawnControllerType::Default;
			pawnArchive.ControllerRotation = character->Controller->GetControlRotation();
		}
		else 
		{
			pawnArchive.Controller = FSavePawnControllerType::None;
		}

		for (auto comp : actor->GetComponents())
		{
			if (auto skmComponent = Cast<USkeletalMeshComponent>(comp))
			{
				//auto buff = FMemoryWriter(pawnArchive.SkeletalMeshDump, true);
				//auto wr = FObjectAndNameAsStringProxyArchive(buff, false);
				//skmComponent->Serialize(wr);
			}
			else if (auto movComponent = Cast<UMovementComponent>(comp))
			{
				pawnArchive.Velocity = movComponent->Velocity;
			}
			/*
			TArray<uint8> data;
			auto buff = FMemoryWriter(data, true);
			buffAr.ArIsSaveGame = true;
			auto wr = FObjectAndNameAsStringProxyArchive(buff, false);
			comp->Serialize(wr);

			if (data.Num() > 0)
			{
				FSaveComponentArchive compArh;
				compArh.Name = comp->GetFName();
				compArh.Data = data;

				actArh.Components.Add(compArh);
			}*/
		}

		actArh.PawnArchive.Add(pawnArchive);
	}


	save.Actors.Add(actArh);
	
	UE_LOG(GameLog, Log, TEXT("Save actor %s"), *actor->GetName());
}

bool USaveFunctionLibrary::LoadGame(UObject* WorldContextObject, const FString& saveName)
{
	auto fullPath = GetFullPathToSave(saveName);
	TArray<uint8> data;

	if (!FFileHelper::LoadFileToArray(data, *fullPath))
		return false;

	auto world = WorldContextObject->GetWorld();

	for (auto level : world->GetLevels())
	{	
		for (auto actor : level->Actors)
		{
			if (CanSaveActor(actor))
				actor->Destroy();
		}
	}
	
	FSaveArchive save;
	auto ar = FMemoryReader(data);
	ar << save;

	for (auto& actArh : save.Actors)
	{
		LoadActor(actArh, world);
	}

	UStoryKeyManager::GetStoryKeyManager(WorldContextObject)->LoadFromBinary(save.StoryKeys);
	UQuestProcessor::GetQuestProcessor(WorldContextObject)->LoadFromBinary(save.Quest);
		
	return true;
}

AActor* USaveFunctionLibrary::LoadActor(const FSaveActorArchive& actArh, UWorld* world)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Name = *actArh.Name;

	AActor* actor = NULL;
	if (actor == NULL)
	{
		actor = world->SpawnActor<AActor>(actArh.Class, actArh.Transform, SpawnParameters);
		UE_LOG(GameLog, Log, TEXT("Spawn actor %s"), *actArh.Name);
	}

	FMemoryReader buffAr (actArh.Data, true);
	FObjectAndNameAsStringProxyArchive wrapper(buffAr, false);
	
	actor->Serialize(wrapper);

	auto character = Cast<ACharacter>(actor);

	if (character != NULL && actArh.PawnArchive.Num() > 0)
	{
		auto pawnArchive = actArh.PawnArchive[0];

		if (pawnArchive.Controller == FSavePawnControllerType::Player)
		{
			world->GetFirstPlayerController()->Possess(character);
		}
		else if (pawnArchive.Controller == FSavePawnControllerType::Default)
		{
			character->SpawnDefaultController();
		}

		if (character->Controller != NULL)
		{
			character->Controller->SetControlRotation(pawnArchive.ControllerRotation);
		}

		for (auto comp : actor->GetComponents())
		{
			if (auto skmComponent = Cast<USkeletalMeshComponent>(comp))
			{
				//auto buff = FMemoryReader(pawnArchive.SkeletalMeshDump, true);
				//auto wr = FObjectAndNameAsStringProxyArchive(buff, false);
				//skmComponent->Serialize(wr);
			//	skmComponent->InitializeAnimScriptInstance(true);
			}
			else if (auto movComponent = Cast<UMovementComponent>(comp))
			{
				movComponent->Velocity = pawnArchive.Velocity;
			}

			/*for (auto& compArh : actArh.Components)
			{
				if (compArh.Name == comp->GetFName())
				{
					auto buff = FMemoryReader(compArh.Data, true);
					auto wr = FObjectAndNameAsStringProxyArchive(buff, false);
					comp->Serialize(wr);

					break;
				}
			}*/
		}
	}

	return actor;
}

TArray<FSaveDescriptor> USaveFunctionLibrary::GetSaveDescriptors()
{
	TArray<FSaveDescriptor> result;
	TArray<FString> Files;

	auto find_mask = FPaths::ProjectSavedDir() / "*" + SaveFileExt;
	IFileManager::Get().FindFiles(Files, *find_mask, true, false);

	for (auto file : Files)
	{
		FSaveDescriptor desc;
		desc.Name = FPaths::GetBaseFilename(file);
		desc.CreateDate = IFileManager::Get().GetTimeStamp(*(FPaths::ProjectSavedDir() / file));

		result.Add(desc);
	}

	return result;
}

bool USaveFunctionLibrary::RemoveSave(const FString& saveName)
{
	auto fullPath = GetFullPathToSave(saveName);

	return IFileManager::Get().Delete(*GetFullPathToSave(saveName));
}

FArchive& operator<<(FArchive& Ar, FSaveActorArchive& A)
{
	FName className;

	if (Ar.IsSaving())
	{
		className = A.Class->GetFName();
	}

	Ar << A.Name;
	Ar << className;
	Ar << A.Transform;
	Ar << A.Data;
	Ar << A.Components;
	Ar << A.PawnArchive;

	if (Ar.IsLoading())
	{
		A.Class = FindObject<UClass>(ANY_PACKAGE, *className.ToString());
	}

	return Ar;
}

FArchive& operator<<(FArchive& Ar, FSaveComponentArchive& A)
{
	Ar << A.Name;
	Ar << A.Data;

	return Ar;
}

FArchive& operator<<(FArchive& Ar, FSavePawnArchive& A)
{
	Ar << A.Controller;
	Ar << A.ControllerRotation;
	Ar << A.Velocity;
	Ar << A.BrainDump;
	Ar << A.SkeletalMeshDump;

	return Ar;
}

FArchive& operator<<(FArchive& Ar, FSaveArchive& A)
{
	Ar << A.Version;
	Ar << A.Actors;
	Ar << A.Quest;
	Ar << A.StoryKeys;

	return Ar;
}