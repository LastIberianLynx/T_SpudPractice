// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class T_SPUDPRACTICE_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	void SaveAllData(TArray<UObject*> SaveableObjects);

	void LoadDataForRequester(UObject* Requester);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float SavedHP;
};
