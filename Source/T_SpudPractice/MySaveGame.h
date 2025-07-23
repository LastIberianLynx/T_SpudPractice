// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
class AMyUnitActor;

USTRUCT()
struct FSerializedActorData
{
	GENERATED_BODY()

		//UPROPERTY()
		//TSubclassOf<AActor> ActorClass;
		//can be stored in the actor class.
		UPROPERTY()
		UClass* ActorClass;

		UPROPERTY()
		FString ActorPath;

		UPROPERTY()
		FString SavedName;

		UPROPERTY()
		FString ReferencePathName1;

		UPROPERTY()
		TArray<uint8> Data;
};

UCLASS()
class T_SPUDPRACTICE_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	void SaveAllData(TArray<UObject*> SaveableObjects);

	void LoadDataForRequester(UObject* Requester);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float SavedHP;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		UObject* Character;


	UPROPERTY( SaveGame)
		FSerializedActorData SavedUnitManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<AMyUnitActor*> SavedUnits;

	UPROPERTY(SaveGame)
		TArray<FSerializedActorData> SerializedUnitActors;

	UPROPERTY(SaveGame)
		TArray<FSerializedActorData> SerializedRegions;

	UPROPERTY()
		TArray<FSerializedActorData> SerializedCharacters;

	UPROPERTY()
		TArray<FSerializedActorData> SerializedFreeCharacters;
};
