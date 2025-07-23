// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveableInterface.h"
#include "TerritoryManager.generated.h"

class UTerritoryObj;

UCLASS()
class T_SPUDPRACTICE_API ATerritoryManager : public AActor, public ISaveableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerritoryManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		TArray<UTerritoryObj*> All_Territories;

};
