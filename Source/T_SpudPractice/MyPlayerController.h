// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */

class AUnitManager;

UCLASS()
class T_SPUDPRACTICE_API AMyPlayerController : public APlayerController
{

		GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;

	// Function to handle Spawn Unit input
	void OnSpawnUnit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		AUnitManager* UnitManager = nullptr;
	
};
