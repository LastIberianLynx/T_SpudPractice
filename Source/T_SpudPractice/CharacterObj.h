// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SaveableInterface.h"
#include "CharacterObj.generated.h"

/**
 * 
 */
UCLASS()
class T_SPUDPRACTICE_API UCharacterObj : public UObject, public ISaveableInterface
{
	GENERATED_BODY()
	public:

		virtual void SaveData_Implementation(UMySaveGame* SaveGameRef) override;
		virtual void LoadData_Implementation(UMySaveGame* SaveGameRef) override;

		UPROPERTY(EditAnywhere, BlueprintReadWrite/*, SaveGame*/)
			float CommandLevel = 10;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			FString CharacterName;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			int32 Level;


	
};
