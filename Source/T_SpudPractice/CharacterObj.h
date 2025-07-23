// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SaveableInterface.h"
#include "CharacterObj.generated.h"

/**
 * 
 */

class AMyUnitActor;
class AUnitManager;
class UTerritoryObj;

UCLASS()
class T_SPUDPRACTICE_API UCharacterObj : public UObject, public ISaveableInterface
{
	GENERATED_BODY()
	public:

		virtual void SaveData_Implementation(UMySaveGame* SaveGameRef) override;
		virtual void LoadData_Implementation(UMySaveGame* SaveGameRef) override;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
			float CommandLevel = 10;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
			FString CharacterName;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
			int32 Level;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
			AMyUnitActor* Unit;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
			AUnitManager* UnitManager;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			UTerritoryObj* CurTerritory;


	
};
