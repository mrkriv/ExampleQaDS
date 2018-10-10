#include "SaveFunctionLibrary.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Engine/Classes/Engine/World.h"

const FString SaveFileExt = TEXT(".sav");

FString USaveFunctionLibrary::GetFullPathToSave(const FString& saveName)
{
	return FPaths::ProjectSavedDir() / saveName + SaveFileExt;
}

bool USaveFunctionLibrary::Save(const UObject* WorldContextObject, const FString& saveName)
{
	TArray<uint8> data;

	//

	auto fullPath = GetFullPathToSave(saveName);
	FFileHelper::SaveArrayToFile(data, *fullPath);

	return true;
}

bool USaveFunctionLibrary::Load(const UObject* WorldContextObject, const FString& saveName)
{
	auto fullPath = GetFullPathToSave(saveName);
	TArray<uint8> data;

	if (!FFileHelper::LoadFileToArray(data, *fullPath))
		return false;

	//

	return true;
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