// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveableInterface.h"
#include "MyUnitActor.generated.h"

UCLASS()
class T_SPUDPRACTICE_API AMyUnitActor : public AActor, public ISaveableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyUnitActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SaveData_Implementation(UMySaveGame* SaveGameRef) override;
	virtual void LoadData_Implementation(UMySaveGame* SaveGameRef) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HP = 100;

};
