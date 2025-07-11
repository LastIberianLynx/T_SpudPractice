// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyRegionActor.generated.h"

class AMyUnitActor;

UCLASS()
class T_SPUDPRACTICE_API AMyRegionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyRegionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FTransform LastTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		AMyUnitActor* CurrentUnit = nullptr;

	UPROPERTY(SaveGame)
		int32 SavedRegionIndex = -1;

};
