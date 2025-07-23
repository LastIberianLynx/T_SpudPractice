// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveableInterface.h"
#include "UnitManager.generated.h"

class AMyUnitActor;
class AMyRegionActor;
class UCharacterObj;



UCLASS()
class T_SPUDPRACTICE_API AUnitManager : public AActor, public ISaveableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<AMyUnitActor*> All_UnitActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<AMyRegionActor*> All_RegionActors;


	UFUNCTION()
		void SpawnUnit(FTransform T, float HP);


	virtual void SaveData_Implementation(UMySaveGame* SaveGameRef) override;
	virtual void LoadData_Implementation(UMySaveGame* SaveGameRef) override;

	void SpawnRegions();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AMyRegionActor> RegionClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AMyUnitActor> UnitClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		bool bIsLoadingFromSave = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UCharacterObj*> FreeCharacters_Arr;

	UFUNCTION()
	AMyRegionActor* GetClosestRegion(const FVector& Location) const;




};
