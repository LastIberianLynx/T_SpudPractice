// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveableInterface.generated.h"

class UMySaveGame;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class T_SPUDPRACTICE_API ISaveableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//virtual void SaveData(UMySaveGame* SaveGameRef) = 0; doesnt work.

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save System")
		void SaveData(UMySaveGame* SaveGameRef);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save System")
		void LoadData(UMySaveGame* SaveGameRef);
};
