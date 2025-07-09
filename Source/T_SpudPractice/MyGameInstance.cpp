// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "MySaveGame.h"
#include "SaveableInterface.h"
#include "GameFramework/AsyncActionHandleSaveGame.h"


void UMyGameInstance::SaveGame(bool bAsync) {
	
	bAsync = false;

	if (!SaveGameRef)
	{
		SaveGameRef = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	}

	if (!SaveGameRef)
		return;
	if (bAsync)
	{
		UAsyncActionHandleSaveGame* SaveAction = UAsyncActionHandleSaveGame::AsyncSaveGameToSlot(
			this,
			SaveGameRef,
			SaveGameSlotName,
			UserIndex
		);

		if (SaveAction)
		{
			CurrentAsyncSaveHandle = SaveAction;

			SaveAction->Completed.AddDynamic(this, &UMyGameInstance::OnAsyncSaveCompleted);
		}
	}
	else {

		bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameRef, SaveGameSlotName, UserIndex);
		OnAsyncSaveCompleted(SaveGameRef, bSuccess); // Call the same function for consistency

	}

}

void UMyGameInstance::OnAsyncSaveCompleted(USaveGame* SavedGame, bool bSuccess)
{
	CurrentAsyncSaveHandle = nullptr; // Clean up
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT(" Save completed successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" Save failed!"));
	}

	OnGameSaveCompleted.Broadcast(SaveGameRef);
}


void UMyGameInstance::RequestSave(bool bAsync)
{

	UWorld* World = GetWorld();
	//UGameplayStatics::GetAllActorsWithInterface()


	TArray<AActor*> SaveableActors;
	UGameplayStatics::GetAllActorsWithInterface(World, USaveableInterface::StaticClass(), SaveableActors);

	TArray<UObject*> SaveableObjects;
	for (AActor* Actor : SaveableActors)
	{
		if (Actor)
		{
			SaveableObjects.Add(Cast<UObject>(Actor));
		}
	}

	SaveGameRef->SaveAllData(SaveableObjects);

	SaveGame(bAsync);

}
void UMyGameInstance::RequestLoad(UObject* Requester)
{

	SaveGameRef->LoadDataForRequester(Requester);

}

void UMyGameInstance::CreateSaveGame() {

	SaveGameRef = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

}

void UMyGameInstance::LoadGame(bool bAsync) {

	if (!UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, UserIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("No save game found in slot '%s'."), *SaveGameSlotName);
		return;
	}

	if (bAsync)
	{
		UAsyncActionHandleSaveGame* LoadAction = UAsyncActionHandleSaveGame::AsyncLoadGameFromSlot(
			this, // World context object
			SaveGameSlotName,
			UserIndex
		);

		if (LoadAction)
		{
			LoadAction->Completed.AddDynamic(this, &UMyGameInstance::OnAsyncLoadCompleted);
		}
	}
	else
	{
		USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, UserIndex);
		SaveGameRef = Cast<UMySaveGame>(Loaded);

		bool bSuccess = (SaveGameRef != nullptr);
		OnAsyncLoadCompleted(Loaded, bSuccess); // Consistent handler
	}
}

void UMyGameInstance::OnAsyncLoadCompleted(USaveGame* LoadedSave, bool bSuccess)
{
	if (bSuccess)
	{
		SaveGameRef = Cast<UMySaveGame>(LoadedSave);
		UE_LOG(LogTemp, Log, TEXT("Load completed successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Load failed!"));
	}

	OnGameLoadCompleted.Broadcast(SaveGameRef);
}


