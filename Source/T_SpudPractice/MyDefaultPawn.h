// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "Kismet/GameplayStatics.h"
#include "SaveableInterface.h"
#include "MyDefaultPawn.generated.h"

/**
 * 
 */

class AUnitManager;
class AMyUnitActor;

UCLASS()
class T_SPUDPRACTICE_API AMyDefaultPawn : public ADefaultPawn, public ISaveableInterface
{
	GENERATED_BODY()
	public:
		UFUNCTION(BlueprintCallable)
			AMyUnitActor* SpawnUnit(FTransform T, float UnitHP);

		UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
			AUnitManager* UnitManager = nullptr;

		virtual void BeginPlay() override;

		UPROPERTY(EditAnywhere, Category = "Spawning")
			TSubclassOf<AMyUnitActor> UnitClassToSpawn;

	
};
