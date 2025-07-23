// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveableInterface.h"
#include "CampaignMapManager.generated.h"


class UCharacterObj;

UCLASS()
class T_SPUDPRACTICE_API ACampaignMapManager : public AActor, public ISaveableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACampaignMapManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		TArray<UCharacterObj*> All_Characters;



};
